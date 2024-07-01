#ifndef UARTCONTROLLER_HPP
#define UARTCONTROLLER_HPP

#include "IUartController.hpp"

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_uart.h"
#include "stm32l476xx.h"

/**
 * Class which will be used to send data about result of execution estimators
 */
class UartController: public IUartController{
public:
    UartController() = default;
    ~UartController() override = default;

    void Init() override;
    void SendData(const std::string& str) override;

private:
    UART_HandleTypeDef uart_;
};

#endif