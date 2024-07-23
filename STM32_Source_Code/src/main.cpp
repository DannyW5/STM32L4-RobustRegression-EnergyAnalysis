#include "startup.h"
#include "main.h"

#include "controllers/UartController.hpp"
#include "controllers/LPUartController.hpp"
#include "controllers/EstimatorResultsLogger.hpp"
#include "controllers/Timer100us.hpp"

#include "algorithms/TheilSenEstimator.hpp"
#include "algorithms/RansacEstimator.hpp"
#include "algorithms/HuberEstimator.hpp"
#include "algorithms/OLSEstimator.hpp"

#include "data/DataLength1000.hpp"

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

  while (1)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

    // timer.StartMeasurement();
    // auto slope_intercept_TheilSen = CalculateTheilSenEstimator(points_cut);
    // auto time_TheilSen = timer.StopMeasurement();
    // logger.LogResults("Theil-Sen", std::to_string(slope_intercept_TheilSen.first), std::to_string(slope_intercept_TheilSen.second), time_TheilSen);

    timer.StartMeasurement();
    auto slope_intercept_RANSAC = CalculateRansacEstimator(points, 100, 0.1);
    auto time_RANSAC = timer.StopMeasurement();
    logger.LogResults("RANSAC", std::to_string(slope_intercept_RANSAC.first), std::to_string(slope_intercept_RANSAC.second), time_RANSAC);

    timer.StartMeasurement();
    auto slope_intercept_Huber = CalculateHuberEstimator(points, 1.35);
    auto time_Huber = timer.StopMeasurement();
    logger.LogResults("Huber", std::to_string(slope_intercept_Huber.first), std::to_string(slope_intercept_Huber.second), time_Huber);

    timer.StartMeasurement();
    auto slope_intercept_OLS = CalculateOLSEstimator(points);
    auto time_OLS = timer.StopMeasurement();
    logger.LogResults("OLS", std::to_string(slope_intercept_OLS.first), std::to_string(slope_intercept_OLS.second), 1);

    HAL_Delay(1000); 
  }
}

