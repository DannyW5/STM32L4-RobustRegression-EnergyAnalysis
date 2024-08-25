#ifndef THEILSENESTIMATOR_H
#define THEILSENESTIMATOR_H

#include <array>
#include <algorithm>
#include <unordered_set>
#include <random>

#include "Point.hpp"

static constexpr uint8_t POINTS_MAX_SIZE = 200;

template <typename T, std::size_t N>
std::pair<float, float> CalculateTheilSenEstimator(const std::array<Point<T>, N> &points)
{
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

template <typename T, std::size_t N, std::size_t num_iterations>
std::pair<float, float> CalculateRandomizedMeanTheilSenEstimator(const std::array<Point<T>, N> &points, const uint8_t num_points)
{
    std::array<std::pair<float, float>, num_iterations> calculation_results;
    for(auto i=0; i < num_iterations; i++){
        calculation_results[i] = CalculateRandomizedTheilSenEstimator(points, num_points);
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

template <typename T, std::size_t N, std::size_t num_iterations>
std::pair<float, float> CalculateRandomizedMedianTheilSenEstimator(const std::array<Point<T>, N> &points, const uint8_t num_points)
{
    std::array<std::pair<float, float>,  num_iterations> calculation_results;
    std::array<float, num_iterations> slopes;
    std::array<float, num_iterations> intercepts;
    for(auto i=0; i < num_iterations; i++){
        calculation_results[i] = CalculateRandomizedTheilSenEstimator(points, num_points);
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

// template <typename T, std::size_t N>
// std::pair<float, float> CalculateWeightedMeanTheilSenEstimator(const std::array<Point<T>, N> &points, const uint8_t num_points)
// {

// }

// template <typename T, std::size_t N>
// std::pair<float, float> CalculateExtremeAndCentralPointsTheilSenEstimator(const std::array<Point<T>, N> &points, const uint8_t num_points)
// {

// }

#endif