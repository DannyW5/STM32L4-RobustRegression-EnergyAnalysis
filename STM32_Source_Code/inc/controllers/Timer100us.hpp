#ifndef TIMER10US_HPP
#define TIMER10US_HPP

#include "stm32l476xx.h"
#include "stm32l4xx.h"
#include "stm32l4xx_hal_tim.h"
#include "stm32l4xx_hal_tim_ex.h"


class Timer100us{
public:
    Timer100us() = default;
    ~Timer100us() = default;

    void Init();
    void StartMeasurement();
    uint32_t StopMeasurement();

private:
    TIM_HandleTypeDef timer_;
};

#endif
