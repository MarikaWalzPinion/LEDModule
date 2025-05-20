  ******************************************************************************
  * @file        LEDModule.h
  * @brief       This module can send data from a STM32 MCU to a WS2812 LED, using a Timer (with PWM and DMA)
  *
  * @date        05.05.2025
  * @author      @MarikaWalzPinion m.walz@pinion.eu
  * @copyright   Pinion GmbH <2025>
  ******************************************************************************

/* ========== General Information ========== */
// the default configuration for this module expects Timer17 and GPDMA1_Channel10 for a NUCLEO-U575ZI-Q MCU
// the LED_SetColor function asks for red, blue, green and brightness (0-255 range)


/* ========== MCU & CubeMX setup ========== */
// this Module requires one cable-connection from the WS2812 LED to the TIM17 Pinout on the MCU (Pin: PA_7 on NUCLEO-U575ZI-Q)
// in CubeMX the HCLK Clock Configuration is set to 72 MHz
// Timer: activate TIM17 (PWM Generation CH1): 
//        Parameter Settings: 'Counter Period': '89' + 'PWM Pulse': '0' 
//        NVIC Settings: 'global interrupt': 'enabled'
// DMA: go to GPDMA1
//      Mode: 'Channel 10': 'Standard Request Mode'
//      CH10: 'Request': 'TIM17_CH1'
//            'Direction': 'Memory to Peripheral'
//            'Source Data Setting': Enabled + Half Word 
//            'Destination Data Setting: Disabled + Half Word


/* ========== HAL usage & setup ========== */
// this Module uses 3 HAL functions
// HAL_TIM_PWM_Start_DMA                -> defined in "stm32u5xx_hal_tim.h"
// HAL_TIM_PWM_PulseFinishedCallback    -> defined in "stm32u5xx_hal_tim.h", this needs to be manually overwritten, will be autocalled
// HAL_TIM_PWM_Stop_DMA                 -> defined in "stm32u5xx_hal_tim.h"


/* ========== What to add to the main.c, to implement this module ========== */

// USER INCLUDE:
// #include "LEDModule.h"

// USER CODE: this HAL function has to be overwritten:
// void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
//   LEDModule *module = LEDModule_GetActiveInstance();
//   if (module && htim->Instance == TIM17) {
//       HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
//   }
// }

// MAIN FUNCTION:
// // Get default config (default is hardcoded: htim17 and GPDMA1_Channel10)
// LEDModule_Configuration config = LEDModule_GetDefaultConfiguration();

// // Create module instance with default class behavior
// LEDModule led = LEDModule_factory(NULL, config);

// // Set a color: red, blue, green, brightness
// led.class->LED_SetColor(&led, 255, 255, 255, 255);

// // Start sending the data via PWM+DMA
// led.class->LED_SendData(&led);
