#include "controllers/EstimatorResultsLogger.hpp"
#include <sstream>

void EstimatorResultsLogger::LogResults(std::string algorithmName, std::string slope, std::string intercept, uint32_t time){
    std::ostringstream oss;

    oss << "===============================\n";
    oss << "Algorithm " << algorithmName << ":\n";
    oss << "Slope: " << slope << " Intercept: " << intercept;
    oss << "Time consumed: " << time << " ms\n";
    oss << "===============================\n\n\n";

    p_UartController_->SendData(oss.str());
}