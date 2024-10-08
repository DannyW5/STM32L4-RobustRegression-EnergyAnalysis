#include "controllers/DacPort.hpp"
#include "startup.h"

void DacPort::Init()
{   
    __HAL_RCC_DAC1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    
    DAC_ChannelConfTypeDef sConfig;

    if (dac_ == DAC1)
    {
        hdac_.Instance = dac_;
    }
    else
    {
        Error_Handler();
    }

    if ((channel_ != DAC1_CHANNEL_1) && (channel_ != DAC1_CHANNEL_2))
    {
        Error_Handler();
    }

    if (HAL_DAC_Init(&hdac_) != HAL_OK)
    {
        Error_Handler();
    }

    sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
    sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
    if (HAL_DAC_ConfigChannel(&hdac_, &sConfig, channel_) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_DAC_Start(&hdac_, channel_);
}

MilliVolt DacPort::GetVoltage()
{
    return VDD_VALUE * HAL_DAC_GetValue(&hdac_, channel_) / 4096;
}

void DacPort::SetVoltage(MilliVolt voltage)
{
    if(voltage >= 0 && voltage <= VDD_VALUE){
        HAL_DAC_SetValue(&hdac_, channel_, DAC_ALIGN_12B_R, (static_cast<float>(voltage) / VDD_VALUE)*4096);
    }
}