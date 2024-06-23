#include "startup.h"
#include "main.h"

#include "controllers/UartController.hpp"

int main(void)
{
  PlatformStartup();

  UartController uartController = UartController();
  
  while (1)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_Delay(500);
  }
}

