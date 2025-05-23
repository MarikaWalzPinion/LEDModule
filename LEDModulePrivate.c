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
LEDModule *activeLEDModule = NULL;

// Flags for DMA Sending
bool flagDMAStartSending = false;
bool flagPWMCallback = false;
bool flagStopDMA = false;

extern uint32_t HCLKFreq;

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


void LEDModule_SetColorData(LEDModule *self, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness) {
    red = ((uint16_t)red * brightness) / 255;
    green = ((uint16_t)green * brightness) / 255;
    blue = ((uint16_t)blue * brightness) / 255;

    uint32_t color = (blue << 16) | (red << 8) | green; 
    self->color = color;

    for (int bit = 23; bit >= 0; bit--) {
        self->LEDModule_pwmData[23 - bit] = (color & (1 << bit))
            ? ((self->dutyCycleHigh * HCLKFreq) / 100)
            : ((self->dutyCycleLow * HCLKFreq) / 100);
    }

    for (int i = 24; i < 54; i++) {
        self->LEDModule_pwmData[i] = 0;
    }
}

void LEDModule_SendDataToLED(LEDModule *self) {
    flagDMAStartSending = false;
    flagPWMCallback = false;
    flagStopDMA = false;
    HAL_TIM_PWM_Start_DMA(self->config.htim, self->config.tim_channel, (uint32_t *)self->LEDModule_pwmData, 54);
    flagDMAStartSending = true;
}
