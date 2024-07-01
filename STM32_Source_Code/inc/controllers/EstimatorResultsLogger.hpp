#ifndef ESTIMATORSRESTULTLOGGER_HPP
#define ESTIMATORSRESTULTLOGGER_HPP

#include "controllers/IUartController.hpp"
#include <memory>

class EstimatorResultsLogger{
public:
    EstimatorResultsLogger(std::unique_ptr<IUartController> p_UartController): p_UartController_(std::move(p_UartController)) {};
    ~EstimatorResultsLogger() = default;

    void LogResults(std::string algorithmName, std::string slope, std::string intercept, uint32_t time);

private:
    std::unique_ptr<IUartController> p_UartController_;
};

#endif