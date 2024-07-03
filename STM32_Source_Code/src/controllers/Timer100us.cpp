#include "controllers/Timer100us.hpp"
#include "startup.h"

void Timer100us::Init(){
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    // /* TIM2 interrupt Init */
    // HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(TIM2_IRQn);

    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;

    timer_.Instance = TIM2;
    timer_.Init.Prescaler = (HAL_RCC_GetHCLKFreq() / 10000) - 1; // if Timer resolution will be to small change 10000 to 100000 -> Timer 10us then
    timer_.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer_.Init.Period = 4294967295; //maximum
    timer_.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    timer_.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
     if (HAL_TIM_Base_Init(&timer_) != HAL_OK)
     {
       Error_Handler();
     }
     sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
     if (HAL_TIM_ConfigClockSource(&timer_, &sClockSourceConfig) != HAL_OK)
     {
       Error_Handler();
     }
     sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
     sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
     if (HAL_TIMEx_MasterConfigSynchronization(&timer_, &sMasterConfig) != HAL_OK)
     {
       Error_Handler();
     }
}


void Timer100us::StartMeasurement(){
    __HAL_TIM_SET_COUNTER(&timer_, 0);
    HAL_TIM_Base_Start(&timer_);
}

uint32_t Timer100us::StopMeasurement(){
    HAL_TIM_Base_Stop(&timer_);
    return __HAL_TIM_GET_COUNTER(&timer_);
}


// extern "C"{
//     void TIM2_IRQHandler(void){

//     }

// }

