/*
 * digital.c
 *
 *  Created on: 2016. 10. 10.
 *      Author: PangaeaSolution
 */

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "digital.h"

osThreadId osEncoderAcqTaskHandle;
uint16_t pusAbsoluteEncoder[2];
uint8_t pucDigitalInput[4];

osThreadId osDigitalInOutTaskHandle;

uint32_t ulEncoderTaskCount = 0;

void vEncoderAcqTask(void const * argument)
{
	uint16_t usTemp;

	for(;;)
	{
		osDelay(1);

		ulEncoderTaskCount++;

#if	(1 == _UseBusMutex)
		if(osOK == osMutexWait(osBusMutex, 10))
		{
#endif
			usTemp = *(__IO uint8_t*)_AEncoder0Addr;
			pusAbsoluteEncoder[0] = (usTemp & 0x00FF);
			usTemp = *(__IO uint8_t*)_AEncoder1Addr;
			pusAbsoluteEncoder[0] |= ((usTemp & 0x0003) << 8);

			usTemp = *(__IO uint8_t*)_AEncoder2Addr;
			pusAbsoluteEncoder[1] = (usTemp & 0x00FF);
			usTemp = *(__IO uint8_t*)_AEncoder3Addr;
			pusAbsoluteEncoder[1] |= ((usTemp & 0x0003) << 8);

#if	(1 == _UseBusMutex)
			osMutexRelease(osBusMutex);
		}
#endif

//		printf("%x %x\n", pusAbsoluteEncoder[0], pusAbsoluteEncoder[1]);
//		printf("ADC1 - %d\n", pusAdcValue[0]);
//		printf("ADC2 - %d\n", pusAdcValue[1]);
//		printf("ADC3 - %d\n\n", pusAdcValue[2]);

	}
}

void vDigitalInOutTask(void const * argument)
{
	uint8_t ucTemp = 0;
	uint8_t ucLoop = 0;
	for(;;)
	{
		osDelay(1000);

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

		pucDigitalInput[0] = _IsKeyIn();
		pucDigitalInput[1] = _IsSpareDIn0();
		pucDigitalInput[2] = _IsSpareDIn1();
		pucDigitalInput[3] = _IsSpareDIn2();
//		printf("DIN - ");
//		for(ucLoop = 0; ucLoop < 4; ucLoop++)
//			printf("%d ", pucDigitalInput[ucLoop]);
//		printf(" \n");
	}
}
