#ifndef THEILSENESTIMATOR_H
#define THEILSENESTIMATOR_H

#include <array>
#include <algorithm>
#include <unordered_set>
#include <random>

#include "Point.hpp"

static constexpr uint8_t POINTS_MAX_SIZE = 200;

template <typename T, std::size_t N>
std::pair<float, float> CalculateTheilSenEstimator(const std::array<Point<T>, N>& points){
    constexpr std::size_t num_slopes = N * (N - 1) / 2;
    std::array<float, num_slopes> slopes;
    size_t index = 0;

    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            if (points[i].x != points[j].x) {
                float slope = static_cast<float>(points[j].y - points[i].y) / (points[j].x - points[i].x);
                slopes[index++] = slope;
            }
        }
    }

    std::sort(slopes.begin(), slopes.begin() + index);
    float medianSlope = slopes[index / 2];

    std::array<float, N> intercepts;
    for (size_t i = 0; i < points.size(); ++i) {
        intercepts[i] = points[i].y - medianSlope * points[i].x;
    }
    
    std::sort(intercepts.begin(), intercepts.end());
    float medianIntercept = intercepts[intercepts.size() / 2];

    return {medianSlope, medianIntercept};
}

template <typename T, std::size_t N>
std::pair<float, float> CalculateRandomizedTheilSenEstimator(const std::array<Point<T>, N>& points, uint8_t num_points){
    if ((num_points > N) || (num_points > POINTS_MAX_SIZE)) {
        while(true); // Assertion
    }

    std::array<Point<T>, 200> selected_points;
    
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

    return CalculateTheilSenEstimator(selected_points);
}

#endif