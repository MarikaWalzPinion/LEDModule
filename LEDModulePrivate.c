/**
  * @brief Factory function to create a LEDModule_Class object. The following configuration is available. 
  * 
  * @param   self                     - object to be created
  * @param   configuration            - configuration based on the header file
  * 
  * @return  SUCCESS / FAIL 
  * 
  */

/* ============================ INCLUDES =========================== */
#include "LEDModulePrivate.h"
#include <string.h>

// Global pointer for handling the HAL callback (single active module for now)
static LEDModule *activeLEDModule = NULL;

/* ========================== FUNCTIONS ============================ */

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

    for (int bit = 23; bit >= 0; bit--) {
        self->LEDModule_pwmData[23 - bit] = (color & (1 << bit))
            ? ((self->dutyCycleHigh * 89) / 100)
            : ((self->dutyCycleLow * 89) / 100);
    }

    for (int i = 24; i < 74; i++) {
        self->LEDModule_pwmData[i] = 0;
    }
}

void LEDModule_SendData(LEDModule *self) {
    HAL_TIM_PWM_Start_DMA (self->config.htim, TIM_CHANNEL_1, (uint32_t *)self->LEDModule_pwmData, 74);
}
