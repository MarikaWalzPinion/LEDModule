  /**
  ******************************************************************************
  * @file        LEDModulePrivate.h
  * @brief       This module can send data from a STM32 MCU to a WS2812 LED, using a Timer (with PWM and DMA)
  *
  * @date        05.05.2025
  * @author      @MarikaWalzPinion m.walz@pinion.eu
  * @copyright   Pinion GmbH <2025>
  ******************************************************************************
  */ 

#ifndef LEDMODULEPRIVATE_H
#define LEDMODULEPRIVATE_H 

#ifdef __cplusplus
  extern "C" {
#endif


/* ============================== INCLUDES ============================== */
#include <string.h>
#include <stdint.h>
#include "LEDModule.h"

/* ============================== FUNCTIONS ============================== */

/**
  * @brief init function for the LED Object
  * 
  * @param   LEDModule (self)            - value used by function
  * @param   LEDModule (self)            - variable used as input and changed by function
  * 
  * @return  void
  * 
  */
void LEDModule_LED_init(LEDModule *self);


/**
  * @brief getter Function for HCLK Clock Frequency - will be aligned with Timer Counter Period for synchronized DutyCycles
  * 
  * @param   void                 - value used by function
  * @param   HCLKFreq             - variable used as input and changed by function
  * 
  * @return  void
  * 
  */
uint32_t GetHCLKFreqMhZ(void);


/**
  * @brief synchronizes Timer Counter Period with HCLK Clock
  * 
  * @param   void                 - value used by function
  * @param   HCLKFreq             - variable used as input and changed by function
  * 
  * @return  void
  * 
  */
 void setTimerCounterPeriod (void);


/**
  * @brief turns user input into 74bit long PWM-Data, that can be send to the LED
  * 
  * @param   user input: red, green, blue, brightness     - value used by function
  * @param   LEDModule (self), pwmData                    - variable used as input and changed by function
  * 
  * @return  void
  * 
  */
void LEDModule_SetColor(LEDModule *self, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);


/**
  * @brief sends the PWM Data with DMA to the WS2812 LED
  * 
  * @param   LEDModule (self)             - value used by function
  * @param   LEDModule (self)             - variable used as input and changed by function
  * 
  * @return  void
  * 
  */
void LEDModule_SendData(LEDModule *self);


/**
  * @brief getter Function for activeInstance
  * 
  * @param   LEDModule (self)             - value used by function
  * @param   LEDModule (self)             - variable used as input and changed by function
  * 
  * @return  void
  * 
  */
LEDModule *LEDModule_GetActiveInstance_Internal(void);


// This block is obligatory here as this is only included by `MyModule.c`
#else // LEDMODULEPRIVATE_H
  #error "Double included LEDMODULEPRIVATE_H"
#endif // LEDMODULEPRIVATE_H

#ifdef __cpluscplus
}
#endif
