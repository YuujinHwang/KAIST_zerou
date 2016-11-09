/*
 * dac.c
 *
 *  Created on: 2016. 10. 10.
 *      Author: PangaeaSolution
 */
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

#include "main.h"
#include "dac.h"

osThreadId osDacSetTaskHandle;

void vDacSetTask(void const * argument)
{
	uint16_t pusTemp[6] = {0, 0, 0, 0, 0, 0};
	uint16_t usTemp = 0;

	for(;;)
	{
		osDelay(10);

		if(osOK == osMutexWait(osBusMutex, 10))
		{
			*(__IO uint8_t *)_Dac1MswAddr = (uint8_t)(pusTemp[0] >> 8);
			*(__IO uint8_t *)_Dac1LswAddr = (uint8_t)(pusTemp[0] >> 0);
			*(__IO uint8_t *)_Dac1CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;

			*(__IO uint8_t *)_Dac2MswAddr = (uint8_t)(pusTemp[0] >> 8);
			*(__IO uint8_t *)_Dac2LswAddr = (uint8_t)(pusTemp[0] >> 0);
			*(__IO uint8_t *)_Dac2CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;

			*(__IO uint8_t *)_Dac3MswAddr = (uint8_t)(pusTemp[0] >> 8);
			*(__IO uint8_t *)_Dac3LswAddr = (uint8_t)(pusTemp[0] >> 0);
			*(__IO uint8_t *)_Dac3CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;

			*(__IO uint8_t *)_Dac4MswAddr = (uint8_t)(pusTemp[0] >> 8);
			*(__IO uint8_t *)_Dac4LswAddr = (uint8_t)(pusTemp[0] >> 0);
			*(__IO uint8_t *)_Dac4CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;

			*(__IO uint8_t *)_Dac5MswAddr = (uint8_t)(pusTemp[0] >> 8);
			*(__IO uint8_t *)_Dac5LswAddr = (uint8_t)(pusTemp[0] >> 0);
			*(__IO uint8_t *)_Dac5CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;

			*(__IO uint8_t *)_Dac6MswAddr = (uint8_t)(pusTemp[0] >> 8);
			*(__IO uint8_t *)_Dac6LswAddr = (uint8_t)(pusTemp[0] >> 0);
			*(__IO uint8_t *)_Dac6CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;

			osMutexRelease(osBusMutex);
		}

		if(pusTemp[0] < 4080)
			pusTemp[0] += 10;
		else
			pusTemp[0] = 0;
	}
}

void vDacValueSet(uint8_t ucCh, uint16_t usValue)
{
	if(0xFFF < usValue)
		printf("DAC Value Error (0 ~ FFF) - %.4x\n", usValue);
	else
	{
#if	(1 == _UseBusMutex)
		if(osOK == osMutexWait(osBusMutex, 10))
		{
#endif
			switch (ucCh)
			{
				case	0 :
					*(__IO uint8_t *)_Dac1MswAddr = (uint8_t)(usValue >> 8);
					*(__IO uint8_t *)_Dac1LswAddr = (uint8_t)(usValue >> 0);
					*(__IO uint8_t *)_Dac1CtrlAddr = _DacCtrlFastBit |_DacCtrlLatchBit;
					break;

				case	1 :
					*(__IO uint8_t *)_Dac2MswAddr = (uint8_t)(usValue >> 8);
					*(__IO uint8_t *)_Dac2LswAddr = (uint8_t)(usValue);
					*(__IO uint8_t *)_Dac2CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;
					break;

				case	2 :
					*(__IO uint8_t *)_Dac3MswAddr = (uint8_t)(usValue >> 8);
					*(__IO uint8_t *)_Dac3LswAddr = (uint8_t)(usValue);
					*(__IO uint8_t *)_Dac3CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;
					break;

				case	3 :
					*(__IO uint8_t *)_Dac4MswAddr = (uint8_t)(usValue >> 8);
					*(__IO uint8_t *)_Dac4LswAddr = (uint8_t)(usValue);
					*(__IO uint8_t *)_Dac4CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;
					break;

				case	4 :
					*(__IO uint8_t *)_Dac5MswAddr = (uint8_t)(usValue >> 8);
					*(__IO uint8_t *)_Dac5LswAddr = (uint8_t)(usValue);
					*(__IO uint8_t *)_Dac5CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;
					break;

				case	5 :
					*(__IO uint8_t *)_Dac6MswAddr = (uint8_t)(usValue >> 8);
					*(__IO uint8_t *)_Dac6LswAddr = (uint8_t)(usValue);
					*(__IO uint8_t *)_Dac6CtrlAddr = _DacCtrlFastBit | _DacCtrlLatchBit;
					break;
			}
#if	(1 == _UseBusMutex)
			osMutexRelease(osBusMutex);
		}
#endif
	}
}
