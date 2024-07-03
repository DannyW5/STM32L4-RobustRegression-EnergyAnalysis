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

  std::array<Point<int>, 5> points_int = {{
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 6},
    }};
  
  while (1)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

    timer.StartMeasurement();
    auto slope_intercept_TheilSen = CalculateTheilSenEstimator(points_int);
    auto time_TheilSen = timer.StopMeasurement();
    logger.LogResults("Theil-Sen", std::to_string(slope_intercept_TheilSen.first), std::to_string(slope_intercept_TheilSen.first), time_TheilSen);

    // timer.StartMeasurement();
    // auto slope_intercept_RANSAC = CalculateRansacEstimator(points_int, 10, 1);
    // auto time_RANSAC = timer.StopMeasurement();
    // logger.LogResults("RANSAC", std::to_string(slope_intercept_RANSAC.first), std::to_string(slope_intercept_RANSAC.first), time_RANSAC);

    // timer.StartMeasurement();
    // auto slope_intercept_Huber = CalculateHuberEstimator(points_int, 1);
    // auto time_Huber = timer.StopMeasurement();
    // logger.LogResults("Huber", std::to_string(slope_intercept_Huber.first), std::to_string(slope_intercept_Huber.first), time_Huber);

    timer.StartMeasurement();
    auto slope_intercept_OLS = CalculateOLSEstimator(points_int);
    auto time_OLS = timer.StopMeasurement();
    logger.LogResults("OLS", std::to_string(slope_intercept_OLS.first), std::to_string(slope_intercept_OLS.first), time_OLS);

    HAL_Delay(1000); 
  }
}

