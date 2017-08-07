/**
  ******************************************************************************
  * @file    I2C/SlaveReceiver/main.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Main program body.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/* Private variables ---------------------------------------------------------*/
uint8_t I2C1_Buf_Read[16];

/* Private define ------------------------------------------------------------*/
#define I2C1_OWN_ADDRESS7    0x82

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void I2C_Configuration(void);

/* Private function  ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    int i;

    RCC_Configuration();
    GPIO_Configuration();
    I2C_Configuration();

    i=0;
    /* Enable I2C1 */
    I2C_Enable(I2C1,ENABLE);

    /* Enable Acknowledge */
    I2C_Acknowledge_Enable(I2C1,ENABLE);

    /* Wait until the slave address matched */
    while(!I2C_StateDetect(I2C1, I2C_PROGRAMMINGMODE_SLAVE_RECEIVER_ADDSEND));

    for(i=0;i<16;i++)
    {
        /* Wait until the reception data register is not empty */
        while(!I2C_StateDetect(I2C1, I2C_PROGRAMMINGMODE_SLAVE_BYTE_RECEIVED));

        /* Read a data byte from I2C_DTR */
        I2C1_Buf_Read[i]=I2C_ReceiveData(I2C1);
    }
    /* Wait until the slave detected a stop condition on I2C bus */
    while(!I2C_StateDetect(I2C1, I2C_PROGRAMMINGMODE_SLAVE_STOP_DETECTED));

    /* Clear the STPDET bit */
    I2C_Enable(I2C1,ENABLE);

    while(1);
}

/**
  * @brief  Enable the peripheral clock.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    /* Enable GPIOB clock */
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOB,ENABLE);

    /* Enable I2C1 APB1 clock */
    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_I2C1,ENABLE);
}

/**
  * @brief  Cofigure the GPIO Ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    /* I2C1 GPIO ports*/
    GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  Cofigure the I2C1 interface.
  * @param  None
  * @retval None
  */
void I2C_Configuration(void)
{
    I2C_InitPara I2C_InitStructure;
    I2C_InitStructure.I2C_Protocol = I2C_PROTOCOL_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DUTYCYCLE_2;
    I2C_InitStructure.I2C_BitRate = 100000;
    I2C_InitStructure.I2C_AddressingMode = I2C_ADDRESSING_MODE_7BIT;
    I2C_InitStructure.I2C_DeviceAddress = I2C1_OWN_ADDRESS7;
    I2C_Init(I2C1, &I2C_InitStructure);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
