#ifndef HUBERESTIMATOR_H
#define HUBERESTIMATOR_H

#include <array>

#include "Point.hpp"

template <typename T, std::size_t N>
std::pair<double, double> CalculateHuberEstimator(const std::array<Point<T>, N>& points, double delta) {
    double slope = 0.0, intercept = 0.0;

    for (size_t iter = 0; iter < 100; ++iter) {
        double sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0, sumW = 0.0;

        for (const auto& point : points) {
            double pred = slope * point.x + intercept;
            double residual = point.y - pred;
            double weight = (std::abs(residual) <= delta) ? 1.0 : delta / std::abs(residual);

            sumW += weight;
            sumX += weight * point.x;
            sumY += weight * point.y;
            sumXY += weight * point.x * point.y;
            sumX2 += weight * point.x * point.x;
        }

        slope = (sumW * sumXY - sumX * sumY) / (sumW * sumX2 - sumX * sumX);
        intercept = (sumY - slope * sumX) / sumW;
    }

    return {slope, intercept};
}

#endif