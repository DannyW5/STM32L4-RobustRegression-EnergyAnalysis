#include "startup.h"
#include "main.h"

#include "controllers/UartController.hpp"
#include "controllers/LPUartController.hpp"
#include "controllers/EstimatorResultsLogger.hpp"
#include "controllers/Timer100us.hpp"
#include "controllers/DacPort.hpp"
#include "controllers/AdcPort.hpp"


#include "algorithms/TheilSenEstimator.hpp"
#include "algorithms/RansacEstimator.hpp"
#include "algorithms/HuberEstimator.hpp"
#include "algorithms/OLSEstimator.hpp"

#include "data/DataLength200.hpp"

int main(void)
{
  PlatformStartup();

  static UartController uartController = UartController();
  // static LPUartController lpUartController = LPUartController();

  uartController.Init();
  // lpUartController.Init();

  static EstimatorResultsLogger logger = EstimatorResultsLogger(std::make_unique<UartController>(uartController));
  // static EstimatorResultsLogger logger = EstimatorResultsLogger(std::make_unique<LPUartController>(lpUartController));

  static Timer100us timer = Timer100us();
  timer.Init();
  HAL_Delay(100); 


  while (1)
  {
    timer.StartMeasurement();
    auto slope_intercept_TheilSen = CalculateTheilSenEstimator(points);
    auto time_TheilSen = timer.StopMeasurement();
    logger.LogResults("Theil-Sen", std::to_string(slope_intercept_TheilSen.first), std::to_string(slope_intercept_TheilSen.second), time_TheilSen);
    HAL_Delay(100); 

    // timer.StartMeasurement();
    // auto slope_intercept_RTheilSen = CalculateRandomizedTheilSenEstimator<float, 1000, 200>(points);
    // auto time_RTheilSen = timer.StopMeasurement();
    // logger.LogResults("Randomized Theil-Sen", std::to_string(slope_intercept_RTheilSen.first), std::to_string(slope_intercept_RTheilSen.second), time_RTheilSen);

    // timer.StartMeasurement();
    // auto slope_intercept_RMeanTheilSen = CalculateRandomizedMeanTheilSenEstimator<float, 1000, 100, 200>(points);
    // auto time_RMeanTheilSen = timer.StopMeasurement();
    // logger.LogResults("Randomized Mean Theil-Sen", std::to_string(slope_intercept_RMeanTheilSen.first), std::to_string(slope_intercept_RMeanTheilSen.second), time_RMeanTheilSen);

    // timer.StartMeasurement();
    // auto slope_intercept_RMedianTheilSen = CalculateRandomizedMedianTheilSenEstimator<float, 1000, 100, 200>(points);
    // auto time_RMedianTheilSen = timer.StopMeasurement();
    // logger.LogResults("Randomized Median Theil-Sen", std::to_string(slope_intercept_RMedianTheilSen.first), std::to_string(slope_intercept_RMedianTheilSen.second), time_RMedianTheilSen);

    // timer.StartMeasurement();
    // auto slope_intercept_ExtremeAndCentralPointTheilSen = CalculateExtremeAndCentralPointsTheilSenEstimator<float, 1000, 190>(points);
    // auto time_ExtremeAndCentralPointTheilSen= timer.StopMeasurement();
    // logger.LogResults("Extreme and Central Point Theil-Sen", std::to_string(slope_intercept_ExtremeAndCentralPointTheilSen.first), std::to_string(slope_intercept_ExtremeAndCentralPointTheilSen.second), time_ExtremeAndCentralPointTheilSen);

    // timer.StartMeasurement();
    // auto slope_intercept_AWTheilSen = CalculateAdaptiveTheilSenEstimator<float, 1000, 180>(points);
    // auto time_AWTheilSen = timer.StopMeasurement();
    // logger.LogResults("Adaptive Theil-Sen", std::to_string(slope_intercept_AWTheilSen.first), std::to_string(slope_intercept_AWTheilSen.second), time_AWTheilSen);

    timer.StartMeasurement();
    auto slope_intercept_RANSAC = CalculateRansacEstimator(points, 100, 0.1f);
    auto time_RANSAC = timer.StopMeasurement();
    logger.LogResults("RANSAC", std::to_string(slope_intercept_RANSAC.first), std::to_string(slope_intercept_RANSAC.second), time_RANSAC);
    HAL_Delay(100); 

    timer.StartMeasurement();
    auto slope_intercept_Huber = CalculateHuberEstimator(points, 1.35f);
    auto time_Huber = timer.StopMeasurement();
    logger.LogResults("Huber", std::to_string(slope_intercept_Huber.first), std::to_string(slope_intercept_Huber.second), time_Huber);
    HAL_Delay(100); 

    timer.StartMeasurement();
    auto slope_intercept_OLS = CalculateOLSEstimator(points);
    auto time_OLS = timer.StopMeasurement();
    logger.LogResults("OLS", std::to_string(slope_intercept_OLS.first), std::to_string(slope_intercept_OLS.second), time_OLS);
    HAL_Delay(100); 
  }
}

