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
