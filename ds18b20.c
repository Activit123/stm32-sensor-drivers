#include "ds18b20.h"

/**
 * @brief  DS18B20 Delay in microseconds
 * @param  value: The delay value in microseconds.
 * @retval None
 * @note   This function introduces a delay in microseconds based on the input value.
 */
void delay_us(uint32_t value)
{
    uint32_t i;
    i = value * 50;  // Adjusted for typical STM32F4 clock frequency
    while (i--);
}

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
