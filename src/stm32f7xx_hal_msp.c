/**
 ******************************************************************************
 * File Name          : stm32f7xx_hal_msp.c
 * Description        : This file provides code for the MSP Initialization
 *                      and de-Initialization codes.
 ******************************************************************************
 *
 * COPYRIGHT(c) 2016 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;

extern void Error_Handler(void);
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{
	/* USER CODE BEGIN MspInit 0 */

	/* USER CODE END MspInit 0 */

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* System interrupt init*/
	/* MemoryManagement_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	/* BusFault_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	/* UsageFault_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	/* SVCall_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
	/* DebugMonitor_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
	/* PendSV_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

	/* USER CODE BEGIN MspInit 1 */

	/* USER CODE END MspInit 1 */
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if(hadc->Instance == ADC1)
	{
		/* USER CODE BEGIN ADC1_MspInit 0 */

		/* USER CODE END ADC1_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_ADC1_CLK_ENABLE();

		GPIO_InitStruct.Pin = ACCEL_AIN_Pin;
		GPIO_InitStruct.Pin = ACCEL_AIN_Pin | TORQ_AIN1_Pin | TORQ_AIN2_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ACCEL_AIN_GPIO_Port, &GPIO_InitStruct);

		/* Peripheral DMA init*/
		hdma_adc1.Instance = DMA2_Stream0;

		hdma_adc1.Init.Channel = DMA_CHANNEL_0;
		hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_adc1.Init.Mode = DMA_CIRCULAR;
		hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
		hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		hdma_adc1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
		hdma_adc1.Init.MemBurst = DMA_MBURST_SINGLE;
		hdma_adc1.Init.PeriphBurst = DMA_PBURST_SINGLE;
		if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
		{
			Error_Handler();
		}


		/**ADC1 GPIO Configuration
    PA3     ------> ADC1_IN3
		 */
//		GPIO_InitStruct.Pin = ACCEL_AIN_Pin;
//		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//		GPIO_InitStruct.Pull = GPIO_NOPULL;
//		HAL_GPIO_Init(ACCEL_AIN_GPIO_Port, &GPIO_InitStruct);
//
//		/* Peripheral DMA init*/
//		hdma_adc1.Instance = DMA2_Stream0;
//
//		hdma_adc1.Init.Channel = DMA_CHANNEL_0;
//		hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
//		hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
//		hdma_adc1.Init.MemInc = DMA_MINC_DISABLE;
//		hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//		hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//		hdma_adc1.Init.Mode = DMA_CIRCULAR;
//		hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
//		hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//		hdma_adc1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
//		hdma_adc1.Init.MemBurst = DMA_MBURST_SINGLE;
//		hdma_adc1.Init.PeriphBurst = DMA_PBURST_SINGLE;
//		if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
//		{
//			Error_Handler();
//		}

		__HAL_LINKDMA(hadc, DMA_Handle, hdma_adc1);

//		HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
//		HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

		/* Peripheral interrupt init */
//		HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
//		HAL_NVIC_EnableIRQ(ADC_IRQn);
		/* USER CODE BEGIN ADC1_MspInit 1 */

		/* USER CODE END ADC1_MspInit 1 */
	}
	else if(hadc->Instance == ADC2)
	{
		/* USER CODE BEGIN ADC2_MspInit 0 */

		/* USER CODE END ADC2_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_ADC2_CLK_ENABLE();

		/**ADC2 GPIO Configuration
    PA4     ------> ADC2_IN4
		 */
		GPIO_InitStruct.Pin = TORQ_AIN1_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(TORQ_AIN1_GPIO_Port, &GPIO_InitStruct);

		/* Peripheral DMA init*/

		hdma_adc2.Instance = DMA2_Stream2;

		hdma_adc2.Init.Channel = DMA_CHANNEL_1;
		hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc2.Init.MemInc = DMA_MINC_DISABLE;
		hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_adc2.Init.Mode = DMA_CIRCULAR;
		hdma_adc2.Init.Priority = DMA_PRIORITY_HIGH;
		hdma_adc2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		hdma_adc2.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
		hdma_adc2.Init.MemBurst = DMA_MBURST_SINGLE;
		hdma_adc2.Init.PeriphBurst = DMA_PBURST_SINGLE;
		if (HAL_DMA_Init(&hdma_adc2) != HAL_OK)
		{
			Error_Handler();
		}

		__HAL_LINKDMA(hadc, DMA_Handle, hdma_adc2);

		HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

		/* Peripheral interrupt init */
//		HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
//		HAL_NVIC_EnableIRQ(ADC_IRQn);
		/* USER CODE BEGIN ADC2_MspInit 1 */

		/* USER CODE END ADC2_MspInit 1 */
	}
	else if(hadc->Instance == ADC3)
	{
		/* USER CODE BEGIN ADC3_MspInit 0 */

		/* USER CODE END ADC3_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_ADC3_CLK_ENABLE();

		/**ADC3 GPIO Configuration
    PF7     ------> ADC3_IN5 
		 */
		GPIO_InitStruct.Pin = TORQ_AIN2_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(TORQ_AIN2_GPIO_Port, &GPIO_InitStruct);

		/* Peripheral DMA init*/

		hdma_adc3.Instance = DMA2_Stream1;

		hdma_adc3.Init.Channel = DMA_CHANNEL_2;
		hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc3.Init.MemInc = DMA_MINC_DISABLE;
		hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_adc3.Init.Mode = DMA_CIRCULAR;
		hdma_adc3.Init.Priority = DMA_PRIORITY_HIGH;
		hdma_adc3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		hdma_adc3.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
		hdma_adc3.Init.MemBurst = DMA_MBURST_SINGLE;
		hdma_adc3.Init.PeriphBurst = DMA_PBURST_SINGLE;
		if (HAL_DMA_Init(&hdma_adc3) != HAL_OK)
		{
			Error_Handler();
		}

		__HAL_LINKDMA(hadc, DMA_Handle, hdma_adc3);
		HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

		/* Peripheral interrupt init */
//		HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
//		HAL_NVIC_EnableIRQ(ADC_IRQn);
		/* USER CODE BEGIN ADC3_MspInit 1 */

		/* USER CODE END ADC3_MspInit 1 */
	}

}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

	if(hadc->Instance==ADC1)
	{
		/* USER CODE BEGIN ADC1_MspDeInit 0 */

		/* USER CODE END ADC1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_ADC1_CLK_DISABLE();

		/**ADC1 GPIO Configuration
    PA3     ------> ADC1_IN3 
		 */
		HAL_GPIO_DeInit(ACCEL_AIN_GPIO_Port, ACCEL_AIN_Pin);

		/* Peripheral DMA DeInit*/
		HAL_DMA_DeInit(hadc->DMA_Handle);

		/* Peripheral interrupt DeInit*/
		/* USER CODE BEGIN ADC1:ADC_IRQn disable */
		/**
		 * Uncomment the line below to disable the "ADC_IRQn" interrupt
		 * Be aware, disabling shared interrupt may affect other IPs
		 */
		/* HAL_NVIC_DisableIRQ(ADC_IRQn); */
		/* USER CODE END ADC1:ADC_IRQn disable */

		/* USER CODE BEGIN ADC1_MspDeInit 1 */

		/* USER CODE END ADC1_MspDeInit 1 */
	}
	else if(hadc->Instance==ADC2)
	{
		/* USER CODE BEGIN ADC2_MspDeInit 0 */

		/* USER CODE END ADC2_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_ADC2_CLK_DISABLE();

		/**ADC2 GPIO Configuration
    PA4     ------> ADC2_IN4 
		 */
		HAL_GPIO_DeInit(TORQ_AIN1_GPIO_Port, TORQ_AIN1_Pin);

		/* Peripheral DMA DeInit*/
		HAL_DMA_DeInit(hadc->DMA_Handle);

		/* Peripheral interrupt DeInit*/
		/* USER CODE BEGIN ADC2:ADC_IRQn disable */
		/**
		 * Uncomment the line below to disable the "ADC_IRQn" interrupt
		 * Be aware, disabling shared interrupt may affect other IPs
		 */
		/* HAL_NVIC_DisableIRQ(ADC_IRQn); */
		/* USER CODE END ADC2:ADC_IRQn disable */

		/* USER CODE BEGIN ADC2_MspDeInit 1 */

		/* USER CODE END ADC2_MspDeInit 1 */
	}
	else if(hadc->Instance==ADC3)
	{
		/* USER CODE BEGIN ADC3_MspDeInit 0 */

		/* USER CODE END ADC3_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_ADC3_CLK_DISABLE();

		/**ADC3 GPIO Configuration
    PF7     ------> ADC3_IN5 
		 */
		HAL_GPIO_DeInit(TORQ_AIN2_GPIO_Port, TORQ_AIN2_Pin);

		/* Peripheral DMA DeInit*/
		HAL_DMA_DeInit(hadc->DMA_Handle);

		/* Peripheral interrupt DeInit*/
		/* USER CODE BEGIN ADC3:ADC_IRQn disable */
		/**
		 * Uncomment the line below to disable the "ADC_IRQn" interrupt
		 * Be aware, disabling shared interrupt may affect other IPs
		 */
		/* HAL_NVIC_DisableIRQ(ADC_IRQn); */
		/* USER CODE END ADC3:ADC_IRQn disable */

		/* USER CODE BEGIN ADC3_MspDeInit 1 */

		/* USER CODE END ADC3_MspDeInit 1 */
	}

}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* htim_ic)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if(htim_ic->Instance == TIM3)
	{
		/* USER CODE BEGIN TIM3_MspInit 0 */

		/* USER CODE END TIM3_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_TIM3_CLK_ENABLE();

		/**TIM3 GPIO Configuration
    PC6     ------> TIM3_CH1
    PC7     ------> TIM3_CH2
    PC8     ------> TIM3_CH3
    PC9     ------> TIM3_CH4 
		 */
		GPIO_InitStruct.Pin = FrontWheelEncr0_Pin | FrontWheelEncr1_Pin | RearWheelEncr0_Pin | RearWheelEncr1_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/* Peripheral interrupt init */
		HAL_NVIC_SetPriority(TIM3_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
		/* USER CODE BEGIN TIM3_MspInit 1 */

		/* USER CODE END TIM3_MspInit 1 */
	}

}

void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef* htim_ic)
{

	if(htim_ic->Instance==TIM3)
	{
		/* USER CODE BEGIN TIM3_MspDeInit 0 */

		/* USER CODE END TIM3_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_TIM3_CLK_DISABLE();

		/**TIM3 GPIO Configuration
    PC6     ------> TIM3_CH1
    PC7     ------> TIM3_CH2
    PC8     ------> TIM3_CH3
    PC9     ------> TIM3_CH4 
		 */
		HAL_GPIO_DeInit(GPIOC, FrontWheelEncr0_Pin|FrontWheelEncr1_Pin|RearWheelEncr0_Pin|RearWheelEncr1_Pin);

		/* Peripheral interrupt DeInit*/
		HAL_NVIC_DisableIRQ(TIM3_IRQn);

	}
	/* USER CODE BEGIN TIM3_MspDeInit 1 */

	/* USER CODE END TIM3_MspDeInit 1 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(huart->Instance == UART4)
	{
		/* USER CODE BEGIN UART4_MspInit 0 */

		/* USER CODE END UART4_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_UART4_CLK_ENABLE();

		/**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
		 */
		GPIO_InitStruct.Pin			=	Console_TX_Pin | Console_RX_Pin;
		GPIO_InitStruct.Mode		=	GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull		=	GPIO_PULLUP;
		GPIO_InitStruct.Speed		=	GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate	=	GPIO_AF8_UART4;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/* Peripheral interrupt init */
		HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(UART4_IRQn);
		/* USER CODE BEGIN UART4_MspInit 1 */

		/* USER CODE END UART4_MspInit 1 */
	}
	else if(huart->Instance==USART3)
	{
		/* USER CODE BEGIN USART3_MspInit 0 */

		/* USER CODE END USART3_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_USART3_CLK_ENABLE();

		/**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
		 */
		GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

		/* USER CODE BEGIN USART3_MspInit 1 */

		/* USER CODE END USART3_MspInit 1 */
	}
	else if(huart->Instance == USART6)
	{
		/* USER CODE BEGIN USART6_MspInit 0 */

		/* USER CODE END USART6_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_USART6_CLK_ENABLE();

		/**USART6 GPIO Configuration
    PG9     ------> USART6_RX
    PG14     ------> USART6_TX 
		 */
		GPIO_InitStruct.Pin = Blue_RX_Pin | Blue_TX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		/* Peripheral interrupt init */
		HAL_NVIC_SetPriority(USART6_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
		/* USER CODE BEGIN USART6_MspInit 1 */

		/* USER CODE END USART6_MspInit 1 */
	}

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

	if(huart->Instance==UART4)
	{
		/* USER CODE BEGIN UART4_MspDeInit 0 */

		/* USER CODE END UART4_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_UART4_CLK_DISABLE();

		/**UART4 GPIO Configuration
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
		 */
		HAL_GPIO_DeInit(GPIOC, Console_TX_Pin|Console_RX_Pin);

		/* Peripheral interrupt DeInit*/
		HAL_NVIC_DisableIRQ(UART4_IRQn);

		/* USER CODE BEGIN UART4_MspDeInit 1 */

		/* USER CODE END UART4_MspDeInit 1 */
	}
	else if(huart->Instance==USART3)
	{
		/* USER CODE BEGIN USART3_MspDeInit 0 */

		/* USER CODE END USART3_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART3_CLK_DISABLE();

		/**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
		 */
		HAL_GPIO_DeInit(GPIOD, STLK_RX_Pin|STLK_TX_Pin);

		/* USER CODE BEGIN USART3_MspDeInit 1 */

		/* USER CODE END USART3_MspDeInit 1 */
	}
	else if(huart->Instance==USART6)
	{
		/* USER CODE BEGIN USART6_MspDeInit 0 */

		/* USER CODE END USART6_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART6_CLK_DISABLE();

		/**USART6 GPIO Configuration
    PG9     ------> USART6_RX
    PG14     ------> USART6_TX 
		 */
		HAL_GPIO_DeInit(GPIOG, Blue_RX_Pin|Blue_TX_Pin);

		/* Peripheral interrupt DeInit*/
		HAL_NVIC_DisableIRQ(USART6_IRQn);

		/* USER CODE BEGIN USART6_MspDeInit 1 */

		/* USER CODE END USART6_MspDeInit 1 */
	}

}

static uint32_t FMC_Initialized = 0;

static void HAL_FMC_MspInit(void){
	/* USER CODE BEGIN FMC_MspInit 0 */

	/* USER CODE END FMC_MspInit 0 */
	GPIO_InitTypeDef GPIO_InitStruct;
	if (FMC_Initialized) {
		return;
	}
	FMC_Initialized = 1;
	/* Peripheral clock enable */
	__HAL_RCC_FMC_CLK_ENABLE();

	/** FMC GPIO Configuration
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PF12   ------> FMC_A6
  PF13   ------> FMC_A7
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PD4   ------> FMC_NOE
  PD5   ------> FMC_NWE
  PD7   ------> FMC_NE1
	 */
	GPIO_InitStruct.Pin			= GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
									GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13;
	GPIO_InitStruct.Mode		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate	= GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin			= GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
	GPIO_InitStruct.Mode		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate	= GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin			= GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1
									| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
	GPIO_InitStruct.Mode		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate	= GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* USER CODE BEGIN FMC_MspInit 1 */

	/* USER CODE END FMC_MspInit 1 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram){
	/* USER CODE BEGIN SRAM_MspInit 0 */

	/* USER CODE END SRAM_MspInit 0 */
	HAL_FMC_MspInit();
	/* USER CODE BEGIN SRAM_MspInit 1 */

	/* USER CODE END SRAM_MspInit 1 */
}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void){
	/* USER CODE BEGIN FMC_MspDeInit 0 */

	/* USER CODE END FMC_MspDeInit 0 */
	if (FMC_DeInitialized) {
		return;
	}
	FMC_DeInitialized = 1;
	/* Peripheral clock enable */
	__HAL_RCC_FMC_CLK_DISABLE();

	/** FMC GPIO Configuration
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PF12   ------> FMC_A6
  PF13   ------> FMC_A7
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PD4   ------> FMC_NOE
  PD5   ------> FMC_NWE
  PD7   ------> FMC_NE1
	 */
	HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
			|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13);

	HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);

	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1
			|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7);

	/* USER CODE BEGIN FMC_MspDeInit 1 */

	/* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* hsram){
	/* USER CODE BEGIN SRAM_MspDeInit 0 */

	/* USER CODE END SRAM_MspDeInit 0 */
	HAL_FMC_MspDeInit();
	/* USER CODE BEGIN SRAM_MspDeInit 1 */

	/* USER CODE END SRAM_MspDeInit 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
