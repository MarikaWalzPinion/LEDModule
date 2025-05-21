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


## MCU & CubeMX setup
- this Module requires one cable-connection from the WS2812 LED to the TIM17 Pinout on the MCU (Pin: PA_7 on NUCLEO-U575ZI-Q)
- in CubeMX the HCLK Clock Configuration is set to 72 MHz
- Timer: activate TIM17 (PWM Generation CH1):<br/>
&emsp;&emsp;&emsp;Parameter Settings: 'Counter Period': '89' + 'PWM Pulse': '0' <br/>
&emsp;&emsp;&emsp;NVIC Settings: 'global interrupt': 'enabled'<br/>
- DMA:&ensp;go to GPDMA1<br/>
&emsp;&emsp;&emsp;Mode: 'Channel 10': 'Standard Request Mode'<br/>
&emsp;&emsp;&emsp;CH10: 'Request': 'TIM17_CH1'<br/>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;'Direction': 'Memory to Peripheral'<br/>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;'Source Data Setting': Enabled + Half Word <br/>
&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;'Destination Data Setting': Disabled + Half Word


## HAL usage & setup
this Module uses 3 HAL functions
- HAL_TIM_PWM_Start_DMA&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;-> defined in "stm32u5xx_hal_tim.h"
- HAL_TIM_PWM_PulseFinishedCallback&emsp;-> defined in "stm32u5xx_hal_tim.h", but manually overwritten in LEDModule.c
- HAL_TIM_PWM_Stop_DMA&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;-> defined in "stm32u5xx_hal_tim.h"


## What to add to the main.c, to implement this module

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
