#ifndef LPUARTCONTROLLER_HPP
#define LPUARTCONTROLLER_HPP

#include "IUartController.hpp"

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_uart.h"
#include "stm32l476xx.h"

class LPUartController: public IUartController{
public:
    LPUartController() = default;
    ~LPUartController() override = default;

    void Init() override;
    void SendData(const std::string& str) override;

private:
    UART_HandleTypeDef uart_;
};

#endif