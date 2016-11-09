/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
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

//123123123 - pull request test asdfasdf
//testtesttest
#include "math.h"
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

#include "main.h"
#include "dac.h"
#include "digital.h"
#include "wheel_encoder.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;
DMA_HandleTypeDef hdma_adc3;

UART_HandleTypeDef huart4;
UART_HandleTypeDef xConsoleHandle;

UART_HandleTypeDef xBluetoothHandle;


#if	(1 == _UseIWDG)
IWDG_HandleTypeDef xIwdgHandle;
static __IO uint32_t ulLsiFreq = 0;
#else
static WWDG_HandleTypeDef xWdgHandle;
#endif

SRAM_HandleTypeDef hsram1;
osThreadId defaultTaskHandle;

uint16_t usAdc1Value;
uint16_t usAdc2Value;
uint16_t usAdc3Value;

uint16_t pusAdcValue[3];

#if	(1 == _UseBusMutex)
osMutexId osBusMutex;
#endif

strSystemFlag tSystermFlag;

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);

static void MX_FMC_Init(void);

static void MX_UART4_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);

void vBluetoothInit(void);


static void CPU_CACHE_Enable(void);

void StartDefaultTask(void const * argument);

#if	(1 == _UseAdcAcqTask)
void vAdcAcqTask(void const * argument);
osThreadId osAdcAcqTaskHandle;
uint32_t ulAdcAcqTaskCount = 0;
#endif

void vUserTask(void const * argument);
osThreadId osUserTaskHandle;
static uint32_t ulUserTaskCount = 0;

#if	(1 == _UseSoftTimer)
static void osTimerCallback(void const *argument);
#endif

int main(void)
{
	/* USER CODE BEGIN 1 */
//	CPU_CACHE_Enable();
	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();

	MX_FMC_Init();

	MX_UART4_Init();
	MX_USART3_UART_Init();
	MX_USART6_UART_Init();

	MX_DMA_Init();

	MX_ADC1_Init();

	vBluetoothInit();

	printf("Test by actionprog \n");

#if	(1 == _UseIWDG)
	xIwdgHandle.Instance = IWDG;
	xIwdgHandle.Init.Prescaler = IWDG_PRESCALER_32;
	xIwdgHandle.Init.Reload = (uint32_t)(1.5 / 0.0008);					//	1000 = 0.1s(Reload time) / 0.00001
	xIwdgHandle.Init.Window	= IWDG_WINDOW_DISABLE;

	HAL_IWDG_Init(&xIwdgHandle);
#endif
#if	(1 == _UseDWDG)
	if(__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET)
		__HAL_RCC_CLEAR_RESET_FLAGS();

	/*##-2- Configure the WWDG peripheral ######################################*/
	/* WWDG clock counter = (PCLK1 (216MHz)/4096)/8) = 6592 Hz (~158 us)
	     WWDG Window value = 120 means that the WWDG counter should be refreshed only
	     when the counter is below 120 (and greater than 64/0x40) otherwise a reset will
	     be generated.
	     WWDG Counter value = 127, WWDG timeout = ~158 us * 64 = 11 ms */
	xWdgHandle.Instance = WWDG;
	xWdgHandle.Init.Prescaler	= WWDG_PRESCALER_8;
//	xWdgHandle.Init.Window		= 120;
	xWdgHandle.Init.Window		= 80;
	xWdgHandle.Init.Counter		= 127;
	HAL_WWDG_Init(&xWdgHandle);
#endif

#if	(1 == _UseBusMutex)
	osMutexDef(osBusMutex);
	osBusMutex = osMutexCreate(osMutex(osBusMutex));
#endif

#if	(1 == _UseSoftTimer)
	osTimerDef(SysChkTimer, osTimerCallback);
	osTimerId osTimer = osTimerCreate(osTimer(SysChkTimer), osTimerPeriodic, NULL);

	osTimerStart(osTimer, 1000);
#endif

	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE + 256);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	/* Start scheduler */
	osKernelStart();

	while (1)
	{

	}
	/* USER CODE END 3 */

}

void vUserTask(void const * argument)
{
	/*
	 * ADC
	 * - pusAdcValue[3]
	 * - 필터 나 연산처리를 추가 하고 싶으면, main.h 의 _UseAdcAcqTask 를 '1' 로 수정하고,
	 * vAdcAcqTask 함수 내에서 추가 하면 됨.
	 *
	 * DAC
	 * - vDacValueSet(uint8_t ucCh, uint16_t usValue)  MAX 0xFFF(4095)
	 *
	 * Absolute Angle Encoder
	 * - pusAbsoluteEncoder[2] : Update in 1msec
	 * - Update 주기 바꾸고 싶으면, digital.c -> vEncoderAcqTask -> "osDelay(1)" 수정 (1msec 단위)
	 *
	 * Wheel Encoder (Max 10KHz)
	 * - pfEncoderFreq[4]
	 *
	 * Digital Out
	 * - digital.h 참조
	 *
	 * Digital In
	 * - digital.h 참조
	 */
	//shjung test

	uint16_t usDacTestValue = 0;
	uint8_t ucTemp = 0;

	float usSWA;						//	Steering Wheel Angle
	float usSAS;						//	Rear Steering Angle
	int usSPOS;
	uint usTPSraw;
	uint usIGN;

	float ucWT;
	float ucWB;
	float ucRadius;
	float ucRin;
	float ucRout;
	uint ucTPSoff;
	uint ucTPSmax;
	float ucTPS;


	float ucSteer_P;					//	Rear Steering P-gain
	float ucSteer_D;					//	Rear Steering D-gain
	float ucSteer_I;					//	Rear Steering I-gain
	float ucDrive_P;
	float ucDrive_D;
	float ucDrive_I;
	float ucPedalGain;
	float ucThrottle;					//	Throttle Pedal Gain

	float ucDriveMax=10;				//	Drive Motor Command Limit

	float ctrlSAS;
	float ctrlSWA;
	float ctrlDrive_L;
	float ctrlDrive_R;
	int ctrlDir_L;
	int ctrlDir_R;

	//	Initialization

	ucWT=1000;
	ucWB=1000;

	ucTPSoff=0;
	ucTPSmax=1024;

	ucSteer_P=0;
	ucSteer_D=0;
	ucSteer_I=0;

	ucDrive_P=0;
	ucDrive_D=0;
	ucDrive_I=0;

	ucPedalGain=0.001;

	ctrlSAS=0;
	ctrlSWA=0;
	ctrlDrive_L=0;
	ctrlDrive_R=0;
	ctrlDir_L=0;
	ctrlDir_R=0;

	for(;;)
	{
		osDelay(500);
		ulUserTaskCount++;

		//	TODO : User Code in here.

		//	Switch Configuration

		usIGN=_isKeyIn();

		if(_isSpareDin0()!=_isSpareDin1())
		{
			if(_isSpareDin0()!=0)
			{
				usSPOS=1;
			}
			else
			{
				usSPOS=-1;
			}
		}
		else
		{
			usSPOS=0;
		}

		usSWA=pusAbsoluteEncoder[0]*0.3515625-180;
		usSAS=pusAbsoluteEncoder[1]*0.3515625-180;

		usTPSraw=pusAdcValue[0];
		ucTPS=(usTPSraw-ucTPSoff)/(ucTPSmax-ucTPSoff);
		ucThrottle=ucPedalGain*ucTPS;

		//	Rear Wheel Steering

		//	Reaction Motor Control

		//	Drive Motor Control

		if(usSAS==0)
		{
			ucRadius=1000000000;
		}
		else
		{
			ucRadius=tan(usSAS*PI()/180-PI()/2)*ucWB;
		}


		if(absol(ucRadius>ucWT/2))
		{
			ucRin=ucRadius-ucWT/2;
			ucRout=ucRadius+ucWT/2;

			ctrlDrive_L=ucRin/(ucRin+ucRout)*ucThrottle;
			ctrlDrive_R=ucRout/(ucRin+ucRout)*ucThrottle;
		}
		else
		{
			if(ucRadius>0)
			{
				ucRin=-(ucWT/2-ucRadius);
				ucRout=ucWT/2+ucRadius;
			}
			if(ucRadius<0)
			{
				ucRin=ucWT/2-ucRadius;
				ucRout=-ucWT/2-ucRadius;
			}
			else
			{
				ucRin=ucWT/2;
				ucRout=-ucWT/2;
			}

			ctrlDrive_L=ucRin/ucWT*ucThrottle;
			ctrlDrive_R=ucRout/ucWT*ucThrottle;
		}



		if(usSPOS==1)
		{
			if(ctrlDrive_L>0)
			{
				ctrlDir_L=1;
			}
			else
			{
				ctrlDir_L=-1;
				ctrlDrive_L=-ctrlDrive_L;
			}

			if(ctrlDrive_R>0)
			{
				ctrlDir_R=1;
			}
			else
			{
				ctrlDir_R=-1;
				ctrlDrive_R=-ctrlDrive_R;
			}
		}
		if(usSPOS==-1)
		{
			if(ctrlDrive_L>0)
			{
				ctrlDir_L=-1;
			}
			else
			{
				ctrlDir_L=1;
				ctrlDrive_L=-ctrlDrive_L;
			}

			if(ctrlDrive_R>0)
			{
				ctrlDir_R=-1;
			}
			else
			{
				ctrlDir_R=1;
				ctrlDrive_R=-ctrlDrive_R;
			}
		}
		else
		{
			ctrlDir_L=0;
			ctrlDir_R=0;
		}


		//	Control Command Output

		if(ctrlDir_L=1)
		{
			_DriveMotor0Ctrl(1);
		}
		if(ctrlDir_L=1)
		{
			_DriveMotor0Ctrl(0);
		}
		else
		{
			_DriveMotor0Stop();
		}

		if(ctrlDir_R=1)
		{
			_DriveMotor1Ctrl(1);
		}
		if(ctrlDir_R=-1)
		{
			_DriveMotor1Ctrl(0);
		}
		else
		{
			_DriveMotor1Stop();
		}





		//	Absolute Encoder Example
		printf("A-Encoder - 0x%.4x, 0x%.4x\n", pusAbsoluteEncoder[0], pusAbsoluteEncoder[1]);

		//	ADC Example
		printf("ADC - %d, %d, %d\n", pusAdcValue[0], pusAdcValue[1], pusAdcValue[2]);

		//	Wheel Encoder Example
		printf("Wheel - %f %f %f %f\n", pfEncoderFreq[0], pfEncoderFreq[1], pfEncoderFreq[2], pfEncoderFreq[3]);

		//	DAC Example
		if(4000 > usDacTestValue)
		{
			usDacTestValue += 100;
			vDacValueSet(0, usDacTestValue);
			vDacValueSet(1, usDacTestValue);
			vDacValueSet(2, usDacTestValue);
			vDacValueSet(3, usDacTestValue);
			vDacValueSet(4, usDacTestValue);
			vDacValueSet(5, usDacTestValue);
		}
		else
		{
			usDacTestValue = 0;
			vDacValueSet(0, usDacTestValue);
			vDacValueSet(1, usDacTestValue);
			vDacValueSet(2, usDacTestValue);
			vDacValueSet(3, usDacTestValue);
			vDacValueSet(4, usDacTestValue);
			vDacValueSet(5, usDacTestValue);
		}
		printf("DAC Value - %d\n", usDacTestValue);

		//	Digital Output Example
		_SteerMotor0Run(ucTemp);
		_SteerMotor0Dir(ucTemp);
		_SteerMotor0Start(ucTemp);

		_SteerMotor1Run(ucTemp);
		_SteerMotor1Dir(ucTemp);
		_SteerMotor1Start(ucTemp);

		_DriveMotor0Ctrl(ucTemp);
		_DriveMotor1Ctrl(ucTemp);

		_RelayCtrl(ucTemp);
		_SpareDoutCtrl(ucTemp);

		_DriveMotor0Stop();
		_DriveMotor1Stop();

		ucTemp = ~ucTemp;

		//	Digital Input Example
		printf("Key In - %d\n", _IsKeyIn());
		printf("Din Spare 0 - %d\n", _IsSpareDIn0());
		printf("Din Spare 1 - %d\n", _IsSpareDIn1());
		printf("Din Spare 2 - %d\n", _IsSpareDIn2());

		printf("\n");
	}
}

/** System Clock Configuration
 */
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 216;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
	{
		Error_Handler();
	}

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_USART6
			|RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_CLK48;
	PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
	PeriphClkInitStruct.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
	PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
	PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{
	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = ENABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 3;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank = 2;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = 3;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

/* UART4 init function */
static void MX_UART4_Init(void)
{

	huart4.Instance = UART4;
	huart4.Init.BaudRate = 115200;
	huart4.Init.WordLength = UART_WORDLENGTH_8B;
	huart4.Init.StopBits = UART_STOPBITS_1;
	huart4.Init.Parity = UART_PARITY_NONE;
	huart4.Init.Mode = UART_MODE_TX_RX;
	huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart4) != HAL_OK)
	{
		Error_Handler();
	}

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{
	xConsoleHandle.Instance = USART3;
	xConsoleHandle.Init.BaudRate = 115200;
	xConsoleHandle.Init.WordLength = UART_WORDLENGTH_8B;
	xConsoleHandle.Init.StopBits = UART_STOPBITS_1;
	xConsoleHandle.Init.Parity = UART_PARITY_NONE;
	xConsoleHandle.Init.Mode = UART_MODE_TX_RX;
	xConsoleHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	xConsoleHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	xConsoleHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	xConsoleHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_MultiProcessor_Init(&xConsoleHandle, 0, UART_WAKEUPMETHOD_IDLELINE) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USART6 init function */
static void MX_USART6_UART_Init(void)
{
	xBluetoothHandle.Instance						=	USART6;
	xBluetoothHandle.Init.BaudRate					=	9600;
	xBluetoothHandle.Init.WordLength				=	UART_WORDLENGTH_8B;
	xBluetoothHandle.Init.StopBits					=	UART_STOPBITS_1;
	xBluetoothHandle.Init.Parity					=	UART_PARITY_NONE;
	xBluetoothHandle.Init.Mode						=	UART_MODE_TX_RX;
	xBluetoothHandle.Init.HwFlowCtl					=	UART_HWCONTROL_NONE;
	xBluetoothHandle.Init.OverSampling				=	UART_OVERSAMPLING_16;
	xBluetoothHandle.Init.OneBitSampling			=	UART_ONE_BIT_SAMPLE_DISABLE;
	xBluetoothHandle.AdvancedInit.AdvFeatureInit	=	UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&xBluetoothHandle) != HAL_OK)
	{
		Error_Handler();
	}
}

/** 
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) 
{
	/* DMA controller clock enable */
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA2_Stream0_IRQn interrupt configuration */
//	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
//	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
//	/* DMA2_Stream1_IRQn interrupt configuration */
//	HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 0);
//	HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
//	/* DMA2_Stream2_IRQn interrupt configuration */
//	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
//	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}
/* FMC initialization function */
static void MX_FMC_Init(void)
{
	FMC_NORSRAM_TimingTypeDef Timing;

	/** Perform the SRAM1 memory initialization sequence
	 */
	hsram1.Instance					= FMC_NORSRAM_DEVICE;
	hsram1.Extended					= FMC_NORSRAM_EXTENDED_DEVICE;
	/* hsram1.Init */
	hsram1.Init.NSBank				= FMC_NORSRAM_BANK1;
	hsram1.Init.DataAddressMux		= FMC_DATA_ADDRESS_MUX_DISABLE;
	hsram1.Init.MemoryType			= FMC_MEMORY_TYPE_SRAM;
	hsram1.Init.MemoryDataWidth		= FMC_NORSRAM_MEM_BUS_WIDTH_8;
	hsram1.Init.BurstAccessMode		= FMC_BURST_ACCESS_MODE_DISABLE;
	hsram1.Init.WaitSignalPolarity	= FMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram1.Init.WaitSignalActive	= FMC_WAIT_TIMING_BEFORE_WS;
	hsram1.Init.WriteOperation		= FMC_WRITE_OPERATION_ENABLE;
	hsram1.Init.WaitSignal			= FMC_WAIT_SIGNAL_DISABLE;
	hsram1.Init.ExtendedMode		= FMC_EXTENDED_MODE_DISABLE;
	hsram1.Init.AsynchronousWait	= FMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram1.Init.WriteBurst			= FMC_WRITE_BURST_DISABLE;
	hsram1.Init.ContinuousClock		= FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
	hsram1.Init.WriteFifo			= FMC_WRITE_FIFO_ENABLE;
	hsram1.Init.PageSize			= FMC_PAGE_SIZE_NONE;
	/* Timing */
	Timing.AddressSetupTime			= 15;
	Timing.AddressHoldTime			= 15;
	Timing.DataSetupTime			= 255;
	Timing.BusTurnAroundDuration	= 15;
	Timing.CLKDivision				= 16;
	Timing.DataLatency				= 17;
	Timing.AccessMode				= FMC_ACCESS_MODE_A;
	/* ExtTiming */

	if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
	{
		Error_Handler();
	}

}

/** Configure pins as 
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
     PC1   ------> ETH_MDC
     PA1   ------> ETH_REF_CLK
     PA2   ------> ETH_MDIO
     PA7   ------> ETH_CRS_DV
     PC4   ------> ETH_RXD0
     PC5   ------> ETH_RXD1
     PB13   ------> ETH_TXD1
     PA8   ------> USB_OTG_FS_SOF
     PA10   ------> USB_OTG_FS_ID
     PA11   ------> USB_OTG_FS_DM
     PA12   ------> USB_OTG_FS_DP
     PG11   ------> ETH_TX_EN
     PG13   ------> ETH_TXD0
 */
static void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/*Configure GPIO pins : SpareDin1_Pin SpareDin2_Pin SpareDin0_Pin */
	GPIO_InitStruct.Pin = SpareDin1_Pin|SpareDin2_Pin|SpareDin0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : User_Blue_Button_Pin */
	GPIO_InitStruct.Pin = User_Blue_Button_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(User_Blue_Button_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
	GPIO_InitStruct.Pin = RMII_MDC_Pin|RMII_RXD0_Pin|RMII_RXD1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PA1 RMII_MDIO_Pin PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_1|RMII_MDIO_Pin|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : BLUE_EN_Pin */
	GPIO_InitStruct.Pin = BLUE_EN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BLUE_EN_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : SteerMotor0_Dir_Pin SteerMotor0_Run_Pin SteerMotor0_Start_Pin DriveMotor1_F_Pin
                           DriveMotor1_B_Pin RelayCtrl_Pin LD3_Pin SpareDout_Pin 
                           SteerMotor1_Dir_Pin SteerMotor1_Run_Pin SteerMotor1_Start_Pin LD2_Pin 
                           DriveMotor0_F_Pin DriveMotor0_B_Pin */
	GPIO_InitStruct.Pin = SteerMotor0_Dir_Pin|SteerMotor0_Run_Pin|SteerMotor0_Start_Pin|DriveMotor1_F_Pin
			|DriveMotor1_B_Pin|RelayCtrl_Pin|LD3_Pin|SpareDout_Pin
			|SteerMotor1_Dir_Pin|SteerMotor1_Run_Pin|SteerMotor1_Start_Pin|LD2_Pin
			|DriveMotor0_F_Pin|DriveMotor0_B_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : RMII_TXD1_Pin */
	GPIO_InitStruct.Pin = RMII_TXD1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(RMII_TXD1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : USB_PowerSwitchOn_Pin */
	GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : USB_OverCurrent_Pin */
	GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
	GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : RMII_TX_EN_Pin RMII_TXD0_Pin */
	GPIO_InitStruct.Pin = RMII_TX_EN_Pin|RMII_TXD0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pin : KeyIn_Pin */
	GPIO_InitStruct.Pin = KeyIn_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(KeyIn_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BLUE_EN_GPIO_Port, BLUE_EN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, SteerMotor0_Dir_Pin|SteerMotor0_Run_Pin|SteerMotor0_Start_Pin|DriveMotor1_F_Pin
			|DriveMotor1_B_Pin|RelayCtrl_Pin|LD3_Pin|SpareDout_Pin
			|SteerMotor1_Dir_Pin|SteerMotor1_Run_Pin|SteerMotor1_Start_Pin|LD2_Pin
			|DriveMotor0_F_Pin|DriveMotor0_B_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

}


static void osTimerCallback(void const *argument)
{
	(void) argument;

#if	(1 == _UseAdcAcqTask)
	static uint32_t ulAdcAcqTaskCnt = 0;
	if(ulAdcAcqTaskCnt == ulAdcAcqTaskCount)
	{
		HAL_NVIC_SystemReset();
	}
	else
		ulAdcAcqTaskCnt = ulAdcAcqTaskCount;
#endif

	static uint32_t ulEncoderTaskCnt = 0;
	if(ulEncoderTaskCnt == ulEncoderTaskCount)
	{
		HAL_NVIC_SystemReset();
	}
	else
		ulEncoderTaskCnt = ulEncoderTaskCount;

	static uint32_t ulUserTaskCnt = 0;
	if(ulUserTaskCnt == ulUserTaskCount)
		HAL_NVIC_SystemReset();
	else
		ulUserTaskCnt = ulUserTaskCount;

#if	(1 == _UseIWDG)
		HAL_IWDG_Refresh(&xIwdgHandle);
#endif
#if	(1 == _UseDWDG)
		HAL_WWDG_Refresh(&xWdgHandle);
#endif

	_LedRedToggle();
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
#if	(1 == _UseWheelEncoderInputCapture)
	vTimer3InputCaptureInit();
#else
	vTimer3BaseInit();
	vWheelEncoderGpioInit();
#endif

	if(HAL_OK != HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&pusAdcValue[0], 3))
		printf("DMA Error\n");

	osThreadDef(EncoderTask, vEncoderAcqTask, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE + 128);
	osEncoderAcqTaskHandle = osThreadCreate(osThread(EncoderTask), NULL);

	osThreadDef(UserTask, vUserTask, osPriorityRealtime, 0, 512);
	osUserTaskHandle = osThreadCreate(osThread(UserTask), NULL);

#if	(1 == _UseAdcAcqTask)
	osThreadDef(AdcAcqTask, vAdcAcqTask, osPriorityHigh, 0, configMINIMAL_STACK_SIZE + 128);
	osAdcAcqTaskHandle = osThreadCreate(osThread(AdcAcqTask), NULL);
#endif

//	osThreadDef(DacSetTask, vDacSetTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE + 128);
//	osDacSetTaskHandle = osThreadCreate(osThread(DacSetTask), NULL);

//	osThreadDef(DigiTask, vDigitalInOutTask, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE);
//	osDigitalInOutTaskHandle = osThreadCreate(osThread(DigiTask), NULL);

	for(;;)
	{
		osThreadTerminate(defaultTaskHandle);
	}
	/* USER CODE END 5 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while(1)
	{
	}
	/* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */

}

#endif

PUTCHAR_PROTOTYPE
{
	if('\n' == ch)
	{
		xConsoleHandle.Instance->TDR = '\r';
		xBluetoothHandle.Instance->TDR = '\r';
		while(__HAL_UART_GET_FLAG(&xBluetoothHandle, UART_FLAG_TC) == RESET);
	}

	xConsoleHandle.Instance->TDR = ch;
	xBluetoothHandle.Instance->TDR = ch;
	while(__HAL_UART_GET_FLAG(&xBluetoothHandle, UART_FLAG_TC) == RESET);

//	if('\n' == ch)
//	{
//		HAL_UART_Transmit(&xConsoleHandle, (uint8_t *)"\r", 1, 0xFFFF);
//		HAL_UART_Transmit(&xBluetoothHandle, (uint8_t *)"\r", 1, 0xFFFF);
//	}
//
//	HAL_UART_Transmit(&xConsoleHandle, (uint8_t *)&ch, 1, 0xFFFF);
//	HAL_UART_Transmit(&xBluetoothHandle, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
	if(ADC1 == AdcHandle->Instance)
	{
//		_LedRedToggle();
	}
	else if(ADC2 == AdcHandle->Instance)
	{
//		_LedRedToggle();
	}
	else if(ADC3 == AdcHandle->Instance)
	{
//		_LedRedToggle();
	}
}

#if	(1 == _UseAdcAcqTask)
void vAdcAcqTask(void const * argument)
{
	static uint8_t ucLoopCnt = 0;
	uint32_t ulAdc0Temp = 0;
	uint32_t ulAdc1Temp = 0;
	uint32_t ulAdc2Temp = 0;
	for(;;)
	{
		osDelay(10);
		ulAdcAcqTaskCount++;

		if(10 > ucLoopCnt)
		{
			ucLoopCnt++;
			ulAdc0Temp += pusAdcValue[0];
			ulAdc1Temp += pusAdcValue[1];
			ulAdc2Temp += pusAdcValue[2];
		}

		if(10 == ucLoopCnt)
		{
			ucLoopCnt = 0;
			usAdc1Value = (uint16_t)(ulAdc0Temp / 10);
			usAdc2Value = (uint16_t)(ulAdc1Temp / 10);
			usAdc3Value = (uint16_t)(ulAdc2Temp / 10);
		}

//		printf("Cnt - %ld\n", ulAdcAcqTaskCount);
//		printf("ADC1 - %d\n", pusAdcValue[0]);
//		printf("ADC2 - %d\n", pusAdcValue[1]);
//		printf("ADC3 - %d\n\n", pusAdcValue[2]);
	}
}
#endif

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

void vBluetoothInit(void)
{
	uint16_t usTemp;
	uint8_t *pucPtr;

	__HAL_UART_ENABLE_IT(&xBluetoothHandle, UART_IT_RXNE);
	HAL_GPIO_WritePin(BLUE_EN_GPIO_Port, BLUE_EN_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(BLUE_EN_GPIO_Port, BLUE_EN_Pin, GPIO_PIN_SET);
	HAL_Delay(500);

	HAL_UART_Transmit(&xBluetoothHandle, "AT+BAUD8\r\n", 10, 100);
	HAL_Delay(100);
	xBluetoothHandle.Init.BaudRate = 115200;
	HAL_UART_Init(&xBluetoothHandle);
	HAL_Delay(100);

	HAL_UART_Transmit(&xBluetoothHandle, "AT\r\n", 4, 100);
	HAL_Delay(200);
}

#if	(1 == _UseDWDG)
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
  /* WWDG Peripheral clock enable */
  __HAL_RCC_WWDG_CLK_ENABLE();
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
