/**
  * @brief Factory function to create a LEDModule_Class object. The following configuration is available. 
  * 
  * @param   self                     - object to be created
  * @param   configuration            - configuration based on the header file
  * 
  * @return  SUCCESS / FAIL 
  * 
  */

/* ============================== INCLUDES ============================== */
#include "LEDModulePrivate.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "stm32u5xx_ll_rcc.h"
#include "stm32u5xx_hal_tim.h"

/* ============================== VARIABLES ============================== */
// Global pointer for handling the HAL callback
static LEDModule *activeLEDModule = NULL;

// Flags for DMA Sending
bool flagDMAStartSending = false;
bool flagPWMCallback = false;
bool flagStopDMA = false;

static uint32_t HCLKFreq = 0;
extern TIM_HandleTypeDef htim17;

/* ============================== FUNCTIONS ============================== */
LEDModule *LEDModule_GetActiveInstance_Internal(void) {
    return activeLEDModule;
}

void LEDModule_LED_init(LEDModule *self) {
    activeLEDModule = self;
    self->dutyCycleHigh = 90;
    self->dutyCycleLow = 10;
    self->color = 0;
    memset(self->LEDModule_pwmData, 0, sizeof(self->LEDModule_pwmData));
}

void LEDModule_SetColor(LEDModule *self, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness) {
    red = ((uint16_t)red * brightness) / 255;
    green = ((uint16_t)green * brightness) / 255;
    blue = ((uint16_t)blue * brightness) / 255;

    uint32_t color = (blue << 16) | (red << 8) | green; 
    self->color = color;

    uint32_t HCLKFreq = GetHCLKFreqMhZ();
    setTimerCounterPeriod();

    for (int bit = 23; bit >= 0; bit--) {
        self->LEDModule_pwmData[23 - bit] = (color & (1 << bit))
            ? ((self->dutyCycleHigh * HCLKFreq) / 100)
            : ((self->dutyCycleLow * HCLKFreq) / 100);
    }

    for (int i = 24; i < 54; i++) {
        self->LEDModule_pwmData[i] = 0;
    }
}

void LEDModule_SendData(LEDModule *self) {
    flagDMAStartSending = false;
    flagPWMCallback = false;
    flagStopDMA = false;
    HAL_TIM_PWM_Start_DMA (self->config.htim, TIM_CHANNEL_1, (uint32_t *)self->LEDModule_pwmData, 54);
    flagDMAStartSending = true;
}

uint32_t GetHCLKFreqMhZ(void) {
    HCLKFreq = HAL_RCC_GetHCLKFreq() / 1000000;
    return HCLKFreq;
} 

void setTimerCounterPeriod (void) {
    HAL_TIM_PWM_DeInit(&htim17);
    htim17.Init.Period = HCLKFreq;

    if (HAL_TIM_Base_Init(&htim17) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&htim17) != HAL_OK) {
        Error_Handler();
    }
}
