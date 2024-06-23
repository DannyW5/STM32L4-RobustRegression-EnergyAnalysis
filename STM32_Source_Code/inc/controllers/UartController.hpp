#ifndef UARTCONTROLLER_H
#define UARTCONTROLLER_H

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_uart.h"
#include "stm32l476xx.h"


/**
 * Class which will be used to send data about result of execution estimators
 */
class UartController{
public:
    UartController() = default;
    ~UartController() = default;

    void Init();
    void SendData();

private:
    UART_HandleTypeDef huart_;
};

#endif