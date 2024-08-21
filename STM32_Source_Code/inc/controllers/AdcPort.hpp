#ifndef ADCPORT_HPP
#define ADCPORT_HPP

#include "Types.hpp"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_rcc_ex.h"
#include "stm32l4xx_hal_adc.h"

class AdcPort{
public:
    AdcPort(ADC_TypeDef* adc, uint32_t channel): adc_(adc), channel_(channel){};
    ~AdcPort() = default;

    void Init();
    MilliVolt GetVoltage();

private:
    ADC_HandleTypeDef hadc_;
    ADC_TypeDef* adc_;
    uint32_t channel_;
};


#endif