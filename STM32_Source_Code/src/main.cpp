#include "startup.h"
#include "main.h"

#include "controllers/UartController.hpp"

#include "algorithms/TheilSenEstimator.hpp"
#include "algorithms/RansacEstimator.hpp"
#include "algorithms/HuberEstimator.hpp"
#include "algorithms/OLSEstimator.hpp"


int main(void)
{
  PlatformStartup();

  UartController uartController = UartController();

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
    (void) slope_intercept_TheilSen;

    HAL_Delay(500);
  }
}

