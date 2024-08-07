#ifndef THEILSENESTIMATOR_H
#define THEILSENESTIMATOR_H

#include <array>
#include <algorithm>

#include "Point.hpp"

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

#endif