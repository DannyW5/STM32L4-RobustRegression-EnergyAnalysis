#ifndef DACPORT_HPP
#define DACPORT_HPP

#include <cstdint>
#include "Types.hpp"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_dac.h"
#include "stm32l4xx_hal_dac_ex.h"


class DacPort{
public:
    DacPort(DAC_TypeDef* dac, uint8_t channel): dac_(dac), channel_(channel){};
    ~DacPort() = default;

    void Init();
    MilliVolt GetVoltage();
    void SetVoltage(MilliVolt voltage);

private:
    DAC_HandleTypeDef hdac_;
    DAC_TypeDef* dac_;
    uint8_t channel_;
};


#endif