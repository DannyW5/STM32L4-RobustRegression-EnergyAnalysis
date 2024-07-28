#ifndef OLSESTIMATOR_H
#define OLSESTIMATOR_H

#include <array>
#include <numeric>

#include "Point.hpp"

template <typename T, std::size_t N>
std::pair<float, float> CalculateOLSEstimator(const std::array<Point<T>, N>& points) {
    float sumX = std::accumulate(points.begin(), points.end(), 0.0f, [](float sum, const Point<T>& p) { return sum + p.x; });
    float sumY = std::accumulate(points.begin(), points.end(), 0.0f, [](float sum, const Point<T>& p) { return sum + p.y; });
    float sumXY = std::accumulate(points.begin(), points.end(), 0.0f, [](float sum, const Point<T>& p) { return sum + p.x * p.y; });
    float sumX2 = std::accumulate(points.begin(), points.end(), 0.0f, [](float sum, const Point<T>& p) { return sum + p.x * p.x; });

    float n = static_cast<float>(points.size());
    float slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    float intercept = (sumY - slope * sumX) / n;

    return {slope, intercept};
}


#endif