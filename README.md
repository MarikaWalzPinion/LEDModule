## LED Module
  ******************************************************************************
  * @file        LEDModule.h
  * @brief       This module can send data from a STM32 MCU to a WS2812 LED, using a Timer (with PWM and DMA)
  *
  * @date        05.05.2025
  * @author      @MarikaWalzPinion m.walz@pinion.eu
  * @copyright   Pinion GmbH <2025>
  ******************************************************************************

## General Information
- the default configuration for this module expects Timer17 and GPDMA1_Channel10 for a NUCLEO-U575ZI-Q MCU
- the LED_SetColor function asks for red, blue, green and brightness (0-255 range)
- the standalone files for the Module can be found here: https://github.com/MarikaWalzPinion/LEDModule


## MCU & CubeMX setup
- this Module requires one cable-connection from the WS2812 LED to the TIM17 Pinout on the MCU (Pin: PA_7 on NUCLEO-U575ZI-Q)<br/>
- this Module reads the HCLK Clock Frequency and adapts the Timer17 Counter Period accordingly<br/>
  Warning: the HCLK Frequency should be between 48MHz - 148MHz<br/>
  (outside of that range the Timings get imprecise, which leads to visible malfunctions of the LED)<br/>
- Timer: activate TIM17 (PWM Generation CH1):<br/>
- DMA:&ensp;go to GPDMA1<br/>
&emsp;&emsp;&emsp;Mode: 'Channel 10': 'Standard Request Mode'<br/>
&emsp;&emsp;&emsp;CH10: 'Request': 'TIM17_CH1'<br/>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;'Direction': 'Memory to Peripheral'<br/>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;'Source Data Setting': Enabled + Half Word <br/>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;'Destination Data Setting': Disabled + Half Word


## HAL usage & setup
this Module uses HAL functions from 2 different Driver files
- stm32u5xx_hal_tim.h
- stm32u5xx_ll_rcc.h

## What to add to the main.c to implement this module

USER INCLUDE:<br/>
#include "LEDModule.h"

MAIN FUNCTION:<br/>
// Get default config (default is hardcoded: htim17 and GPDMA1_Channel10)<br/>
LEDModule_Configuration config = LEDModule_GetDefaultConfiguration();

// Create module instance with default class behavior<br/>
LEDModule led = LEDModule_factory(NULL, config);

// Set a color: red, blue, green, brightness<br/>
led.class->LED_SetColor(&led, 255, 255, 255, 255);

// Start sending the data via PWM+DMA<br/>
led.class->LED_SendData(&led);
