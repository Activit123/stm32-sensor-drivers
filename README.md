# DS18B20 Temperature Sensor Library

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Getting Started](#getting-started)
- [Installation](#installation)
- [Usage](#usage)
- [Documentation](#documentation)
  - [Header File (ds18b20.h)](#header-file-ds18b20h)
  - [Implementation File (ds18b20.c)](#implementation-file-ds18b20c)
    - [DS18B20_Rst](#ds18b20_rst)
    - [DS18B20_Check](#ds18b20_check)
    - [DS18B20_Read_Bit](#ds18b20_read_bit)
    - [DS18B20_Read_Byte](#ds18b20_read_byte)
    - [DS18B20_Write_Byte](#ds18b20_write_byte)
    - [DS18B20_Start](#ds18b20_start)
    - [DS18B20_Init](#ds18b20_init)
    - [DS18B20_Get_Temp](#ds18b20_get_temp)


## Introduction

The DS18B20 Temperature Sensor Library provides functions for interfacing with the DS18B20 digital temperature sensor on STM32 microcontrollers.

## Features

- **Initialization:** Initialize DS18B20 sensor for communication.
- **Temperature Reading:** Read temperature from DS18B20 sensor in Celsius.
- **Conversion Start:** Start temperature conversion on DS18B20 sensor.
- **Presence Check:** Check the presence of DS18B20 sensor on the bus.
- **Bus Reset:** Reset DS18B20 sensor for communication.
- **Byte Read/Write:** Read and write bytes to/from DS18B20 sensor.

## Getting Started

To integrate the DS18B20 Temperature Sensor Library into your project, follow these steps:

1. Clone the repository: `git clone https://github.com/your-username/your-repo.git`
2. Copy the `ds18b20.h` header file into your project's include directory.
3. Implement the functions from `ds18b20.c` in your project.
4. Include the `ds18b20.h` header file in your source code.

## Installation

No additional installation steps are required. Ensure that the `ds18b20.h` header file is included in your project.

## Usage

Example usage of the library in your application code:

```c
#include "ds18b20.h"

int main() {
    // Initialize DS18B20 sensor
    DS18B20_Init();

    // Read temperature from DS18B20 sensor
    short temperature = DS18B20_Get_Temp();

    // Your application logic here

    return 0;
}
```
## Documentation
## Header File(ds18b20.h)
```h
#ifndef __DS18B20_H
#define __DS18B20_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/** @defgroup GPIO_pins_define GPIO pins define
  * @{
  */

/* DS18B20 IO operation functions *****************************************************/

/**
 * @brief  Macro to clear the DS18B20 bus by pulling the data line low.
 * @note   This macro is used to initiate a communication sequence with the DS18B20 sensor by pulling the data line low.
 */
#define CLR_DS18B20()     HAL_GPIO_WritePin(OneWrie_GPIO_Port, OneWrie_Pin, GPIO_PIN_RESET)

/**
 * @brief  Macro to set the DS18B20 bus by pulling the data line high.
 * @note   This macro is used to release the DS18B20 bus after communication by pulling the data line high.
 */
#define SET_DS18B20()     HAL_GPIO_WritePin(OneWrie_GPIO_Port, OneWrie_Pin, GPIO_PIN_SET)

/**
 * @brief  Macro to read the DS18B20 data line state (input).
 * @retval GPIO_PIN_RESET if the data line is low, GPIO_PIN_SET if the data line is high.
 * @note   This macro is used to check the state of the DS18B20 data line during communication.
 */

#define DS18B20_DQ_IN     HAL_GPIO_ReadPin(OneWrie_GPIO_Port, OneWrie_Pin)

/* Exported functions --------------------------------------------------------*/

/** @addtogroup DS18B20_Exported_Functions
  * @{
  */

/**
 * @brief  Initialize DS18B20 sensor.
 * @retval Status: 0 if initialization succeeds, 1 otherwise.
 * @note   This function initializes the GPIO pins and performs a presence check to ensure the DS18B20 sensor is on the bus.
 */
uint8_t DS18B20_Init(void);

/**
 * @brief  Read the temperature from DS18B20 sensor.
 * @retval Temperature in Celsius as a short integer.
 * @note   This function triggers a temperature conversion on the DS18B20 sensor and reads the temperature value.
 */
short DS18B20_Get_Temp(void);

/**
 * @brief  Start temperature conversion on DS18B20 sensor.
 * @note   This function initiates a temperature conversion on the DS18B20 sensor.
 */
void DS18B20_Start(void);

/**
 * @brief  Write a byte to DS18B20 sensor.
 * @param  dat: The byte to be written.
 * @note   This function sends a byte of data to the DS18B20 sensor during communication.
 */
void DS18B20_Write_Byte(uint8_t dat);

/**
 * @brief  Read a byte from DS18B20 sensor.
 * @retval The byte read from the sensor.
 * @note   This function reads a byte of data from the DS18B20 sensor during communication.
 */
uint8_t DS18B20_Read_Byte(void);

/**
 * @brief  Read a bit from DS18B20 sensor.
 * @retval The bit read from the sensor.
 * @note   This function reads a single bit of data from the DS18B20 sensor during communication.
 */
uint8_t DS18B20_Read_Bit(void);

/**
 * @brief  Check the presence of DS18B20 sensor on the bus.
 * @retval Status: 0 if sensor is present, 1 otherwise.
 * @note   This function performs a presence check to verify if the DS18B20 sensor is connected to the bus.
 */
uint8_t DS18B20_Check(void);

/**
 * @brief  Reset DS18B20 sensor.
 * @note   This function performs a reset operation on the DS18B20 sensor during communication.
 */
void DS18B20_Rst(void);

#endif

```
## Implementation File (ds18b20.c)
### DS18B20_Rst
```c
/**
 * @brief  Reset DS18B20 sensor
 * @param  None
 * @retval None
 * @note   This function performs the reset sequence for DS18B20 communication.
 */
void DS18B20_Rst(void)
{
    CLR_DS18B20();    // Pull DQ low
    delay_us(750);    // Delay for 750us
    SET_DS18B20();    // Release DQ
    delay_us(15);     // Delay for 15us
}
```
### DS18B20_Check
```c
/**
 * @brief  Check the presence of DS18B20 sensor
 * @param  None
 * @retval 1: No sensor present, 0: Sensor present
 * @note   This function checks the presence of DS18B20 sensor on the bus.
 */
uint8_t DS18B20_Check(void)
{
    uint8_t retry = 0;

    while (DS18B20_DQ_IN && retry < 200)
    {
        retry++;
        delay_us(1);
    }

    if (retry >= 200)
        return 1;
    else
        retry = 0;

    while (!DS18B20_DQ_IN && retry < 240)
    {
        retry++;
        delay_us(1);
    }

    if (retry >= 240)
        return 1;

    return 0;
}
```
### DS18B20_Read_Bit
```c
/**
 * @brief  Read a bit from DS18B20 sensor
 * @param  None
 * @retval 1: Bit is high, 0: Bit is low
 * @note   This function reads a single bit of data from the DS18B20 sensor during communication.
 */
uint8_t DS18B20_Read_Bit(void)
{
    uint8_t data;

    CLR_DS18B20();   // Pull DQ low
    delay_us(2);
    SET_DS18B20();   // Release DQ
    delay_us(12);

    if (DS18B20_DQ_IN)
        data = 1;
    else
        data = 0;

    delay_us(50);

    return data;
}
```
### DS18B20_Read_Byte
```c
/**
 * @brief  Read one byte from DS18B20 sensor
 * @param  None
 * @retval The byte read from the sensor
 * @note   This function reads one byte of data from the DS18B20 sensor during communication.
 */
uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i, j, dat;
    dat = 0;

    for (i = 1; i <= 8; i++)
    {
        j = DS18B20_Read_Bit();
        dat = (j << 7) | (dat >> 1);
    }

    return dat;
}
```
### DS18B20_Write_Byte
```c
/**
 * @brief  Write one byte to DS18B20 sensor
 * @param  dat: The byte to be written
 * @retval None
 * @note   This function writes one byte of data to the DS18B20 sensor during communication.
 */
void DS18B20_Write_Byte(uint8_t dat)
{
    uint8_t j;
    uint8_t testb;

    for (j = 1; j <= 8; j++)
    {
        testb = dat & 0x01;
        dat = dat >> 1;

        if (testb)
        {
            CLR_DS18B20(); // Write 1
            delay_us(2);
            SET_DS18B20();
            delay_us(60);
        }
        else
        {
            CLR_DS18B20(); // Write 0
            delay_us(60);
            SET_DS18B20();
            delay_us(2);
        }
    }
}
```
### DS18B20_Start
```c
/**
 * @brief  Start temperature conversion on DS18B20 sensor
 * @param  None
 * @retval None
 * @note   This function initiates a temperature conversion on the DS18B20 sensor.
 */
void DS18B20_Start(void)
{
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc); // Skip ROM command
    DS18B20_Write_Byte(0x44); // Start temperature conversion command
}
```
### DS18B20_Init
```c
/**
 * @brief  Initialize DS18B20 sensor
 * @param  None
 * @retval Status: 0 if initialization succeeds, 1 otherwise
 * @note   This function initializes the DS18B20 sensor by setting the DQ line high and performing a presence check.
 */
uint8_t DS18B20_Init(void)
{
    SET_DS18B20();
    DS18B20_Rst();
    return DS18B20_Check();
}
```
### DS18B20_Get_Temp
```c
/**
 * @brief  Read temperature from DS18B20 sensor
 * @param  None
 * @retval Temperature (-55 to 125 degrees Celsius)
 * @note   This function triggers a temperature conversion, reads the temperature, and converts it to Celsius.
 */
short DS18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL, TH;
    short tem;

    DS18B20_Start(); // Start temperature conversion
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc); // Skip ROM command
    DS18B20_Write_Byte(0xbe); // Read Scratchpad command
    TL = DS18B20_Read_Byte(); // LSB
    TH = DS18B20_Read_Byte(); // MSB

    if (TH > 7)
    {
        TH = ~TH;
        TL = ~TL;
        temp = 0;
    }
    else
        temp = 1;

    tem = TH;
    tem <<= 8;
    tem += TL;
    tem = (float)tem * 0.625f;

    if (temp)
        return tem;
    else
        return -tem;
}
```
