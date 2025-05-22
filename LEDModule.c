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

static LEDModule_Class ledClass = {
    .LED_init = LEDModule_LED_init,
    .LED_SetColor = LEDModule_SetColor,
    .LED_SendData = LEDModule_SendData
};

/* ============================== PUBLIC FACTORY ============================== */

LEDModule LEDModule_factory(LEDModule_Class *classOverride, LEDModule_Configuration config) {
    LEDModule module;
    module.config = config;
    module.class = (classOverride != NULL) ? classOverride : &ledClass;
    module.class->LED_init(&module);
    return module;
}

LEDModule_Configuration LEDModule_GetDefaultConfiguration(void) {
    extern TIM_HandleTypeDef htim17;
    extern DMA_HandleTypeDef handle_GPDMA1_Channel10;

    return (LEDModule_Configuration){
        .htim = &htim17,
        .hdma = &handle_GPDMA1_Channel10
    };
}

/* ============================== PUBLIC GETTER ============================== */
LEDModule *LEDModule_GetActiveInstance(void) {
    return LEDModule_GetActiveInstance_Internal();
} 

/* ============================== PUBLIC HAL OVERWRITE ============================== */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  LEDModule *module = LEDModule_GetActiveInstance();
  if (module && htim->Instance == TIM17) {
      HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
  }
}