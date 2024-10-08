#ifndef RANSACESTIMATOR_H
#define RANSACESTIMATOR_H

#include <array>
#include <algorithm>
#include <cmath>
#include <random>

#include "Point.hpp"

template <typename T>
float distanceFromLine(const Point<T>& point, float slope, float intercept) {
    return std::abs(slope * point.x - point.y + intercept) / std::sqrt(slope * slope + 1);
}

template <typename T, std::size_t N>
std::pair<float, float> CalculateRansacEstimator(const std::array<Point<T>, N>& points, std::size_t numIterations, float threshold) {
    std::mt19937 gen(HAL_GetTick());
    std::uniform_int_distribution<> dis(0, N - 1);

    float bestSlope = 0.0, bestIntercept = 0.0;
    std::size_t bestInliers = 0;

    for (std::size_t i = 0; i < numIterations; ++i) {
        std::size_t idx1 = dis(gen);
        std::size_t idx2 = dis(gen);
        while (idx2 == idx1) {
            idx2 = dis(gen);
        }

        const auto& p1 = points[idx1];
        const auto& p2 = points[idx2];

        if (p1.x == p2.x) continue;

        float slope = static_cast<float>(p2.y - p1.y) / (p2.x - p1.x);
        float intercept = p1.y - slope * p1.x;

        std::size_t inliers = std::count_if(points.begin(), points.end(), [&](const Point<T>& point) {
            return distanceFromLine(point, slope, intercept) < threshold;
        });

        if (inliers > bestInliers) {
            bestInliers = inliers;
            bestSlope = slope;
            bestIntercept = intercept;
        }
    }

    return {bestSlope, bestIntercept};
}

#endif