
# Template for NUCLEO-U575ZI-Q with target STM32U575ZIT6Q

## Prerequisites
- The openOCD version `0.12.0` is required. Installation details are provided in the [docs](https://github.com/PinionGmbH/docs/blob/main/EmbeddedDevelopment.md#update-openocd). This is due to the fact, that for this MCU the config files are missing.
- Project is based on [GNU Toolchain 13.2](https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz)
- CMake should at least be of version 3.22. For more debug capabilities, it is recommended to use version 3.27.
- Install pre-commit hooks with the `Initialize commit hooks` task 

## How to run and build?
1. `cmake --preset <configuration_preset_name>` 
2. go to directory were build files were written to
3. `cmake -- build . --target all`
<br/>

## LED Module to light up a WS2812 LED with Timer (PWM & DMA)
  ******************************************************************************
  * @file        LEDModule.h
  * @brief       This module can send data from a STM32 MCU to a WS2812 LED, using a Timer (with PWM and DMA)
  *
  * @date        05.05.2025
  * @author      @MarikaWalzPinion m.walz@pinion.eu
  * @copyright   Pinion GmbH <2025>
  ******************************************************************************

## General Information
- the configuration for this module requires a Timer and a DMA Channel
- the LightUpLED function asks for red, blue, green and brightness (0-255 range)
- the standalone files for the Module can be found here: https://github.com/MarikaWalzPinion/LEDModule


## MCU & CubeMX setup
- this Module requires one cable-connection from the WS2812 LED to the Timer Pin on the MCU<br/>
- this Module reads the HCLK Clock Frequency and adapts the Timer Counter Period accordingly<br/>
  Warning: the HCLK Frequency should be between 48MHz - 148MHz<br/>
  (outside of that range the Timings get imprecise, which leads to visible malfunctions of the LED)<br/><br/>
- Timer: activate a Timer and set a Channel to PWM Generation (tested for 'TIM17' with 'PWM Generation CH1')<br/>
- DMA: activate a '2 Words Internal FIFO' Channel in 'Standard Request Mode'<br/>
  &emsp;configure the Channel like this:<br/>
&emsp;&emsp;&emsp;'Request': --select your Timer PWM Channel--<br/>
&emsp;&emsp;&emsp;'Direction': 'Memory to Peripheral'<br/>
&emsp;&emsp;&emsp;'Source Data Setting': Enabled + Half Word <br/>
&emsp;&emsp;&emsp;'Destination Data Setting': Disabled + Half Word


## HAL usage & setup
this Module uses HAL functions from 2 different Driver files
- stm32u5xx_hal_tim.h
- stm32u5xx_ll_rcc.h

## What to add to the main.c to implement this module

USER INCLUDE:<br/>
#include "LEDModule.h"

MAIN FUNCTION:<br/>
// Get configuration: pass the active Timer & DMA Channel<br/>
LEDModule_Configuration config = LEDModule_GetDefaultConfiguration(&htim17, &handle_GPDMA1_Channel10);

// Create module instance<br/>
LEDModule led = LEDModule_factory(NULL, config);

// Send a Color to the LED: pass red, blue, green and brightness<br/>
LightUpLED(&led, 0, 0, 0, 255);
