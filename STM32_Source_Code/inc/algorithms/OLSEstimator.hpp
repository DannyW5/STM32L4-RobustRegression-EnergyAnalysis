#ifndef OLSESTIMATOR_H
#define OLSESTIMATOR_H

#include <array>
#include <numeric>

#include "Point.hpp"

template <typename T, std::size_t N>
std::pair<double, double> CalculateOLSEstimator(const std::array<Point<T>, N>& points) {
    double sumX = std::accumulate(points.begin(), points.end(), 0.0, [](double sum, const Point<T>& p) { return sum + p.x; });
    double sumY = std::accumulate(points.begin(), points.end(), 0.0, [](double sum, const Point<T>& p) { return sum + p.y; });
    double sumXY = std::accumulate(points.begin(), points.end(), 0.0, [](double sum, const Point<T>& p) { return sum + p.x * p.y; });
    double sumX2 = std::accumulate(points.begin(), points.end(), 0.0, [](double sum, const Point<T>& p) { return sum + p.x * p.x; });

    double n = static_cast<double>(points.size());
    double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;

    return {slope, intercept};
}


#endif