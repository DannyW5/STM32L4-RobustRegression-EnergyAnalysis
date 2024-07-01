#include "startup.h"
#include "main.h"

#include "controllers/UartController.hpp"
// #include "controllers/LPUartController.hpp"
#include "controllers/EstimatorResultsLogger.hpp"


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

    auto slope_intercept_TheilSen = CalculateTheilSenEstimator(points_int);
    logger.LogResults("Theils-Sen", std::to_string(slope_intercept_TheilSen.first), std::to_string(slope_intercept_TheilSen.first), 1000);

    auto slope_intercept_RANSAC = CalculateRansacEstimator(points_int, 10, 1);
    logger.LogResults("RANSAC", std::to_string(slope_intercept_RANSAC.first), std::to_string(slope_intercept_RANSAC.first), 1000);

    auto slope_intercept_Huber = CalculateHuberEstimator(points_int, 1);
    logger.LogResults("Huber", std::to_string(slope_intercept_Huber.first), std::to_string(slope_intercept_Huber.first), 1000);

    auto slope_intercept_OLS = CalculateOLSEstimator(points_int);
    logger.LogResults("OLS", std::to_string(slope_intercept_OLS.first), std::to_string(slope_intercept_OLS.first), 1000);

    HAL_Delay(500);
  }
}

