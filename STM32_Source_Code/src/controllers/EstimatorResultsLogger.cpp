#include "controllers/EstimatorResultsLogger.hpp"

void EstimatorResultsLogger::LogResults(std::string algorithmName, std::string slope, std::string intercept, uint32_t time){

   p_UartController_->SendData("===============================\r\n");
   p_UartController_->SendData("Algorithm " + algorithmName + ":\r\n");
   p_UartController_->SendData("Slope: " + slope + " Intercept: " + intercept + " \r\n");
   p_UartController_->SendData("Time consumed: " + std::to_string(time * 100) + " us\r\n");
   p_UartController_->SendData("===============================\r\n\n\n");
}