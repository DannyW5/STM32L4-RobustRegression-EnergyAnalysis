#ifndef THEILSENESTIMATOR_H
#define THEILSENESTIMATOR_H

#include <array>
#include <algorithm>
#include <unordered_set>
#include <random>
#include <cmath>
#include <numeric>
#include <utility>

#include "Point.hpp"

static constexpr uint8_t POINTS_MAX_SIZE = 210;

template <typename T, std::size_t N>
std::pair<float, float> CalculateTheilSenEstimator(const std::array<Point<T>, N> &points)
{
    if (N > POINTS_MAX_SIZE) {
        while(true); // Assertion
    }

    constexpr std::size_t num_slopes = N * (N - 1) / 2;
    std::array<float, num_slopes> slopes;
    size_t index = 0;

    for (size_t i = 0; i < points.size(); ++i)
    {
        for (size_t j = i + 1; j < points.size(); ++j)
        {
            if (points[i].x != points[j].x)
            {
                float slope = static_cast<float>(points[j].y - points[i].y) / (points[j].x - points[i].x);
                slopes[index++] = slope;
            }
        }
    }

    std::sort(slopes.begin(), slopes.begin() + index);
    float medianSlope = slopes[index / 2];

    std::array<float, N> intercepts;
    for (size_t i = 0; i < points.size(); ++i)
    {
        intercepts[i] = points[i].y - medianSlope * points[i].x;
    }

    std::sort(intercepts.begin(), intercepts.end());
    float medianIntercept = intercepts[intercepts.size() / 2];

    return {medianSlope, medianIntercept};
}

template <typename T, std::size_t N, std::size_t num_points>
std::array<Point<T>, num_points> GetRandomlyPoints(const std::array<Point<T>, N> &points){
    if ((num_points > N) || (num_points > POINTS_MAX_SIZE)) {
        while(true); // Assertion
    }

    std::array<Point<T>, num_points> selected_points;
    
    std::mt19937 gen(HAL_GetTick());
    std::uniform_int_distribution<> dis(0, N - 1);

    std::unordered_set<int> selected_indices;
    std::size_t count = 0;
    
    while (count < num_points) {
        std::size_t idx = dis(gen);
        
        if (selected_indices.find(idx) == selected_indices.end()) {
            selected_indices.insert(idx);
            selected_points[count] = points[idx];
            ++count;
        }
    }

    return selected_points;
}

template <typename T, std::size_t N, std::size_t num_points>
std::pair<float, float> CalculateRandomizedTheilSenEstimator(const std::array<Point<T>, N> &points)
{
    auto selected_points = GetRandomlyPoints<T, N, num_points>(points);
    return CalculateTheilSenEstimator(selected_points);
}

template <typename T, std::size_t N, std::size_t num_iterations, std::size_t num_points>
std::pair<float, float> CalculateRandomizedMeanTheilSenEstimator(const std::array<Point<T>, N> &points)
{
    std::array<std::pair<float, float>, num_iterations> calculation_results;
    for(auto i=0; i < num_iterations; i++){
        calculation_results[i] = CalculateRandomizedTheilSenEstimator<T, N, num_points>(points);
    }

    float sum_slope = 0.0f;
    float sum_intercept = 0.0f;

    for (std::size_t i = 0; i < num_iterations; ++i) {
        sum_slope += calculation_results[i].first;
        sum_intercept += calculation_results[i].second;
    }

    float mean_slope = sum_slope / num_iterations;
    float mean_intercept = sum_intercept / num_iterations;

    return std::make_pair(mean_slope, mean_intercept);
}

template <typename T, std::size_t N, std::size_t num_iterations, std::size_t num_points>
std::pair<float, float> CalculateRandomizedMedianTheilSenEstimator(const std::array<Point<T>, N> &points)
{
    std::array<std::pair<float, float>,  num_iterations> calculation_results;
    std::array<float, num_iterations> slopes;
    std::array<float, num_iterations> intercepts;
    for(auto i=0; i < num_iterations; i++){
        calculation_results[i] = CalculateRandomizedTheilSenEstimator<T, N, num_points>(points);
        slopes[i] = calculation_results[i].first;
        intercepts[i] = calculation_results[i].second;
    }

    std::sort(slopes.begin(), slopes.end());
    std::sort(intercepts.begin(), intercepts.end());

    float median_slope;
    float median_intercept;

    if (num_iterations % 2 == 0) {
        median_slope = (slopes[num_iterations / 2 - 1] + slopes[num_iterations / 2]) / 2.0f;
        median_intercept = (intercepts[num_iterations / 2 - 1] + intercepts[num_iterations / 2]) / 2.0f;
    } else {
        median_slope = slopes[num_iterations / 2];
        median_intercept= intercepts[num_iterations / 2];
    }

    return std::make_pair(median_slope, median_intercept);
}

template <typename T, std::size_t N, std::size_t num_points>
std::pair<float, float> CalculateExtremeAndCentralPointsTheilSenEstimator(const std::array<Point<T>, N> &points)
{
    if ((num_points > N) || (num_points > POINTS_MAX_SIZE)) {
        while(true); // Assertion
    }

    std::array<Point<T>, num_points> selected_points;

    if (N == num_points) {
        return CalculateTheilSenEstimator(points);
    }

    std::size_t first_count = num_points / 4;   
    std::size_t middle_count = num_points / 2;  
    std::size_t last_count = num_points - first_count - middle_count; 

    std::array<Point<T>, N> sorted_points = points;
    std::sort(sorted_points.begin(), sorted_points.end());

    std::copy_n(sorted_points.begin(), first_count, selected_points.begin());
    std::copy_n(sorted_points.end() - last_count, last_count, selected_points.begin() + first_count);
    std::size_t central_start = (N / 2) - (middle_count / 2);
    std::copy_n(sorted_points.begin() + central_start, middle_count, selected_points.begin() + first_count + last_count);

    return CalculateTheilSenEstimator(selected_points);
}

template <std::size_t N>
float calculateStandardDeviation(const std::array<float, N>& values, double mean) {
    float sum = 0.0f;
    for (const auto& value : values) {
        sum += (value - mean) * (value - mean);
    }
    return std::sqrt(sum / N);
}

template <std::size_t N>
float calculateMedian(std::array<float, N>& values) {
    std::sort(values.begin(), values.end());
    if (N % 2 == 0) {
        return (values[N / 2 - 1] + values[N / 2]) / 2.0;
    } else {
        return values[N / 2];
    }
}

double calculateDistance(const std::pair<float, float>& est1, const std::pair<float, float>& est2) {
    float slope_diff = est1.first - est2.first;
    float intercept_diff = est1.second - est2.second;
    return std::sqrt(slope_diff * slope_diff + intercept_diff * intercept_diff);
}


template <typename T, std::size_t N, std::size_t num_points>
std::pair<float, float> CalculateAdaptiveTheilSenEstimator(const std::array<Point<T>, N> &points){
    static std::size_t MAX_ITER = 100;
    std::array<Point<T>, num_points> selected_points;

    if ((num_points > N) || (num_points > POINTS_MAX_SIZE)) {
        while(true); // Assertion
    }

    std::array<Point<T>, N> sorted_points = points;
    std::sort(sorted_points.begin(), sorted_points.end());

    std::array<T, N> x_values;
    std::array<T, N> y_values;
    for (std::size_t i = 0; i < N; ++i) {
        x_values[i] = sorted_points[i].x;
        y_values[i] = sorted_points[i].y;
    }
    double mean_x = std::accumulate(x_values.begin(), x_values.end(), 0.0) / N;
    double mean_y = std::accumulate(y_values.begin(), y_values.end(), 0.0) / N;
    double stddev_x = calculateStandardDeviation(x_values, mean_x);
    double stddev_y = calculateStandardDeviation(y_values, mean_y);

    std::random_device rd;
    std::mt19937 gen(HAL_GetTick());
    std::uniform_int_distribution<std::size_t> dist(0, num_points - 1);

    std::array<Point<T>, num_points> data_to_use;
    for (std::size_t i = 0; i < num_points; ++i) {
        data_to_use[i] = sorted_points[dist(gen)];
    }
    std::pair<float, float> previous_estimate = CalculateTheilSenEstimator(data_to_use);
    std::pair<float, float> current_estimate = previous_estimate;
    double tolerance = 1e-9;

    for (int iter = 0; iter < MAX_ITER; ++iter) {
        std::array<Point<T>, num_points> new_data_to_use;
        std::size_t new_count = 0;

        float slope = current_estimate.first;
        float intercept = current_estimate.second;

        for (std::size_t i = 0; i < num_points && new_count < num_points; ++i) {
            auto x = sorted_points[i].x;
            auto y = sorted_points[i].y;
            double predicted_y = slope * x + intercept;
            if (std::fabs(y - predicted_y) < 2*stddev_y){
                new_data_to_use[new_count++] = sorted_points[i];
            }
        }

        while (new_count < num_points) {
            new_data_to_use[new_count++] = sorted_points[dist(gen)];
        }

        current_estimate = CalculateTheilSenEstimator(new_data_to_use);

        auto distance = calculateDistance(current_estimate, previous_estimate);
        if (distance < tolerance) {
            break;
        }

        previous_estimate = current_estimate;
    }

    return current_estimate;
}

#endif