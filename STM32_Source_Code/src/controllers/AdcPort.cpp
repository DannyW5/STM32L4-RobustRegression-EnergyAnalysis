#include "controllers/AdcPort.hpp"
#include "startup.h"

void AdcPort::Init()
{   
    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    RCC_PeriphClkInit.AdcClockSelection    = RCC_ADCCLKSOURCE_SYSCLK;
    if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK){
        Error_Handler();
    }

    GPIO_InitTypeDef PinADC;
    PinADC.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    PinADC.Pin = GPIO_PIN_0;
    PinADC.Pull = GPIO_NOPULL;
    PinADC.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &PinADC);

    ADC_MultiModeTypeDef multimode;
    ADC_ChannelConfTypeDef sConfig;

    if (adc_ == ADC1)
    {
        hadc_.Instance = adc_;
    }
    else
    {
        Error_Handler();
    }
    hadc_.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc_.Init.Resolution = ADC_RESOLUTION_12B;
    hadc_.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc_.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc_.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc_.Init.LowPowerAutoWait = DISABLE;
    hadc_.Init.ContinuousConvMode = ENABLE;
    hadc_.Init.NbrOfConversion = 1;
    hadc_.Init.DiscontinuousConvMode = DISABLE;
    hadc_.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc_.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc_.Init.DMAContinuousRequests = DISABLE;
    hadc_.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    hadc_.Init.OversamplingMode = DISABLE;
    if (HAL_ADC_Init(&hadc_) != HAL_OK)
    {
        Error_Handler();
    }
  
    multimode.Mode = ADC_MODE_INDEPENDENT;
    if (HAL_ADCEx_MultiModeConfigChannel(&hadc_, &multimode) != HAL_OK)
    {
        Error_Handler();
    }

    if ((channel_ != ADC_CHANNEL_0) && (channel_ != ADC_CHANNEL_1) &&
        (channel_ != ADC_CHANNEL_2) && (channel_ != ADC_CHANNEL_3) &&
        (channel_ != ADC_CHANNEL_4) && (channel_ != ADC_CHANNEL_5))
    {
        Error_Handler();
    }

    sConfig.Channel = channel_;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if (HAL_ADC_ConfigChannel(&hadc_, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_ADCEx_Calibration_Start(&hadc_, sConfig.SingleDiff) != HAL_OK)
    {
        Error_Handler();
    }

    // HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
    HAL_ADC_Start(&hadc_);
}

MilliVolt AdcPort::GetVoltage()
{
    if (HAL_ADC_PollForConversion(&hadc_, HAL_MAX_DELAY) == HAL_OK)
    {
        return VDD_VALUE * HAL_ADC_GetValue(&hadc_) / 4096;
    }
    else
    {
        return 0;
    }
}

