/*
 * tim.c
 *
 *  Created on: 2016. 10. 12.
 *      Author: PangaeaSolution
 */

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

#include "digital.h"
#include "wheel_encoder.h"

TIM_HandleTypeDef xTimer3Handle;

float pfEncoderFreq[4] = {0, 0, 0, 0};
uint32_t pulFrequency[4] = {0, 0, 0, 0};
uint32_t pulDiffCapture[4] = {0, 0, 0, 0};
static uint32_t pulCaptureValue[4][2];
static uint8_t ucCaptureIndex[4] = {0, 0, 0, 0};

#if	(0 == _UseWheelEncoderInputCapture)
static uint32_t ulWEncoderTick = 0;
#define	_EncoderFreq	(10000)

void vTimer3BaseInit(void)
{
	xTimer3Handle.Instance = TIM3;

	xTimer3Handle.Init.Period = 100 - 1;
	xTimer3Handle.Init.Prescaler = (uint32_t)((HAL_RCC_GetPCLK1Freq() * 2) / (_EncoderFreq * 100)) - 1;
	xTimer3Handle.Init.ClockDivision = 0;
	xTimer3Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	xTimer3Handle.Init.RepetitionCounter = 0;

	HAL_TIM_Base_Init(&xTimer3Handle);

	HAL_TIM_Base_Start_IT(&xTimer3Handle);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM3_CLK_ENABLE();

	HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	ulWEncoderTick++;
}

void vWheelEncoderGpioInit(void)
{
	GPIO_InitTypeDef xGpio;

	xGpio.Pin = FrontWheelEncr0_Pin | FrontWheelEncr1_Pin | RearWheelEncr0_Pin | RearWheelEncr1_Pin;
	xGpio.Mode = GPIO_MODE_IT_RISING;
	xGpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(FrontWheelEncr0_GPIO_Port, &xGpio);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 3, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void EXTI9_5_IRQHandler(void)
{
	uint8_t ucTemp = 0;
	/* EXTI line interrupt detected */
	if(__HAL_GPIO_EXTI_GET_IT(FrontWheelEncr0_Pin) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(FrontWheelEncr0_Pin);
		ucTemp = 0;
	}

	if(__HAL_GPIO_EXTI_GET_IT(FrontWheelEncr1_Pin) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(FrontWheelEncr1_Pin);
		ucTemp = 1;
	}

	if(__HAL_GPIO_EXTI_GET_IT(RearWheelEncr0_Pin) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(RearWheelEncr0_Pin);
		ucTemp = 2;
	}

	if(__HAL_GPIO_EXTI_GET_IT(RearWheelEncr1_Pin) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(RearWheelEncr1_Pin);
		ucTemp = 3;
	}

	if(0 == ucCaptureIndex[ucTemp])
	{
		pulCaptureValue[ucTemp][0] = ulWEncoderTick;
		ucCaptureIndex[ucTemp] = 1;
	}
	else if(1 == ucCaptureIndex[ucTemp])
	{
		pulCaptureValue[ucTemp][1] = ulWEncoderTick;
		ucCaptureIndex[ucTemp] = 0;

		pulDiffCapture[ucTemp] = pulCaptureValue[ucTemp][1] - pulCaptureValue[ucTemp][0];

		pfEncoderFreq[ucTemp] = (float)(2 * _EncoderFreq) / pulDiffCapture[ucTemp];
	}
}

#else

void vTimer3InputCaptureInit(void)
{
//	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_IC_InitTypeDef sConfigIC;

	//	Minimum frequency value to measure is TIM3 counter clock / CCR MAX = 216MHz / 65535
	xTimer3Handle.Instance = TIM3;
	xTimer3Handle.Init.Prescaler = 2;
	xTimer3Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	xTimer3Handle.Init.Period = 0xFFFF;
	xTimer3Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_IC_Init(&xTimer3Handle) != HAL_OK)
	{
		Error_Handler();
	}

//	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//	if (HAL_TIMEx_MasterConfigSynchronization(&xTimer3Handle, &sMasterConfig) != HAL_OK)
//	{
//		Error_Handler();
//	}

	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&xTimer3Handle, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_TIM_IC_ConfigChannel(&xTimer3Handle, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_TIM_IC_ConfigChannel(&xTimer3Handle, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_TIM_IC_ConfigChannel(&xTimer3Handle, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_IC_Start_IT(&xTimer3Handle, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&xTimer3Handle, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&xTimer3Handle, TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&xTimer3Handle, TIM_CHANNEL_4);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	uint8_t ucTemp = 0;

	if(HAL_TIM_ACTIVE_CHANNEL_1 == htim->Channel)
		ucTemp = 0;
	else if(HAL_TIM_ACTIVE_CHANNEL_2 == htim->Channel)
		ucTemp = 1;
	else if(HAL_TIM_ACTIVE_CHANNEL_3 == htim->Channel)
		ucTemp = 2;
	else if(HAL_TIM_ACTIVE_CHANNEL_4 == htim->Channel)
		ucTemp = 3;

	if(0 == ucCaptureIndex[ucTemp])
	{
		_LedBlueToggle();
		pulCaptureValue[ucTemp][0] = HAL_TIM_ReadCapturedValue(htim, (TIM_CHANNEL_1 + (ucTemp * 4)));
		ucCaptureIndex[ucTemp] = 1;
	}
	else if(1 == ucCaptureIndex[ucTemp])
	{
		_LedRedToggle();
		pulCaptureValue[ucTemp][1] = HAL_TIM_ReadCapturedValue(htim, (TIM_CHANNEL_1 + (ucTemp * 4)));
		ucCaptureIndex[ucTemp] = 0;

		if(pulCaptureValue[ucTemp][1] > pulCaptureValue[ucTemp][0])
			pulDiffCapture[ucTemp] = pulCaptureValue[ucTemp][1] - pulCaptureValue[ucTemp][0];
		else if(pulCaptureValue[ucTemp][1] < pulCaptureValue[ucTemp][0])
			pulDiffCapture[ucTemp] = ((0xFFFF - pulCaptureValue[ucTemp][0]) + pulCaptureValue[ucTemp][1]) + 1;

		pulFrequency[ucTemp] = (2 * HAL_RCC_GetPCLK1Freq()) / pulDiffCapture[ucTemp];
//		printf("%d %.6ld %.6ld\n", ucTemp, pulDiffCapture[ucTemp], pulFrequency[ucTemp]);

	}
}

#endif

