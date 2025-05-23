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
#include "LEDModule.h"
#include "LEDModulePrivate.h"
#include "main.h"

static LEDModule_Class ledClass = {
    .LED_init = LEDModule_LED_init,
    .LED_SetColorData = LEDModule_SetColorData,
    .LED_SendDataToLED = LEDModule_SendDataToLED
};

uint32_t HCLKFreq = 0;

/* ============================== PUBLIC FACTORY ============================== */

LEDModule LEDModule_factory(LEDModule_Class *classOverride, LEDModule_Configuration config) {
    LEDModule module;
    module.config = config;
    module.class = (classOverride != NULL) ? classOverride : &ledClass;
    module.class->LED_init(&module);
    return module;
}

LEDModule_Configuration LEDModule_GetDefaultConfiguration(TIM_HandleTypeDef *htim, DMA_HandleTypeDef *hdma) {
    HCLKFreq = GetHCLKFreqMhZ();
    setTimerCounterPeriod(htim);

    return (LEDModule_Configuration){
        .htim = htim,
        .hdma = hdma
    };
}

/* ============================== PUBLIC FUNCTION ============================== */
void LightUpLED(LEDModule *self, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness) {
    LEDModule_SetColorData(self, red, blue, green, brightness);
    HAL_TIM_PWM_DeInit(self->config.htim);
    HAL_TIM_PWM_Init(self->config.htim);
    LEDModule_SendDataToLED(self);
}


/* ============================== PUBLIC GETTER ============================== */
LEDModule *LEDModule_GetActiveInstance(void) {
    return LEDModule_GetActiveInstance_Internal();
} 

/* ============================== PUBLIC HAL OVERWRITE ============================== */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  LEDModule *module = LEDModule_GetActiveInstance();
  if (module && htim == module->config.htim) {
    HAL_TIM_PWM_Stop_DMA(htim, module->config.tim_channel);
  }
}

/* ============================== TIMER & CLOCK ============================== */
uint32_t GetHCLKFreqMhZ(void) {
    return HAL_RCC_GetHCLKFreq() / 1000000;
} 

void setTimerCounterPeriod (TIM_HandleTypeDef *htim) {
    HAL_TIM_PWM_DeInit(htim);
    htim->Init.Period = HCLKFreq;

    if (HAL_TIM_Base_Init(htim) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(htim) != HAL_OK) {
        Error_Handler();
    }
}
