  /**
  ******************************************************************************
  * @file        LEDModule.h
  * @brief       This module can send data from a STM32 MCU to a WS2812 LED, using a Timer (with PWM and DMA)
  *
  * @date        05.05.2025
  * @author      @MarikaWalzPinion m.walz@pinion.eu
  * @copyright   Pinion GmbH <2025>
  ******************************************************************************
  */ 

#ifndef LEDMODULE_H
#define LEDMODULE_H 

#ifdef __cplusplus
  extern "C" {
#endif

/* ============================== INCLUDES ============================== */
#include "stm32u5xx_hal.h"   
#include "stm32u5xx_hal_tim.h"
#include "stm32u5xx_hal_dma.h" 

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* ============================== FORWARD ============================== */
typedef struct LEDModule LEDModule;
typedef struct LEDModule_Configuration LEDModule_Configuration;
typedef struct LEDModule_Class LEDModule_Class;

/* ============================== CONFIG ============================== */
struct LEDModule_Configuration {
    TIM_HandleTypeDef *htim; // default: htim17
    DMA_HandleTypeDef *hdma; // default: handle_GPDMA1_Channel10
};

/* ============================== CLASS ============================== */
struct LEDModule_Class {
    void (*LED_init)(LEDModule *);
    void (*LED_SetColor)(LEDModule *, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);
    void (*LED_SendData)(LEDModule *);
};

/* ============================== MAIN ============================== */
struct LEDModule {
    LEDModule_Configuration config;
    LEDModule_Class *class;
    uint8_t dutyCycleHigh; // default: 90
    uint8_t dutyCycleLow; //  default: 10
    uint32_t color; // 24bit data, that holds the RGB Value -> together with 30bits 'break' we get the length of the pwmData (just 24bits 'break' was too short)
    uint16_t LEDModule_pwmData[54]; // array of pulse width information, the length of the high for each duty cycle per bit
};

/* ============================== Public API ============================== */
LEDModule LEDModule_factory(LEDModule_Class *classOverride, LEDModule_Configuration config);
LEDModule_Configuration LEDModule_GetDefaultConfiguration(void);
LEDModule *LEDModule_GetActiveInstance(void); 

#endif // LEDMODULE_H

#ifdef __cpluscplus
}
#endif