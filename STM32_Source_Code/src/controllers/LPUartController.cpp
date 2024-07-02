#include "controllers/LPUartController.hpp"
#include "startup.h"

void LPUartController::Init(){
    // Enable clocks for pins and UART
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_LPUART1_CLK_ENABLE();

    GPIO_InitTypeDef gpioTx;
    gpioTx.Mode = GPIO_MODE_AF_PP;
    gpioTx.Pin = GPIO_PIN_1;
    gpioTx.Pull = GPIO_NOPULL;
    gpioTx.Speed = GPIO_SPEED_FREQ_HIGH;
    gpioTx.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOC, &gpioTx);

    GPIO_InitTypeDef gpioRx;
    gpioRx.Mode = GPIO_MODE_AF_PP;
    gpioRx.Pin = GPIO_PIN_0;
    gpioRx.Pull = GPIO_NOPULL;
    gpioRx.Speed = GPIO_SPEED_FREQ_HIGH;
    gpioRx.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOC, &gpioRx);

    uart_.Instance = LPUART1;
    uart_.Init.BaudRate = 115200;
    uart_.Init.WordLength = UART_WORDLENGTH_8B;
    uart_.Init.StopBits = UART_STOPBITS_1;
    uart_.Init.Parity = UART_PARITY_NONE;
    uart_.Init.Mode = UART_MODE_TX_RX;
    uart_.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart_.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    uart_.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&uart_) != HAL_OK)
    {
      Error_Handler();
    }
}

void LPUartController::SendData(const std::string& str){
   HAL_UART_Transmit(&uart_, reinterpret_cast<const uint8_t*>(str.c_str()), str.size(), HAL_MAX_DELAY);
}
