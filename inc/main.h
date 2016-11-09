/*
 * main.h
 *
 *  Created on: 2016. 10. 7.
 *      Author: PangaeaSolution
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "mxconstants.h"

#define	_UseSoftTimer	1
#if	(1 == _UseSoftTimer)
	#define	_UseWatchDog	1
	#if	(1 == _UseWatchDog)
		#define	_UseIWDG		1
		#define	_UseDWDG		0
	#endif
#endif

#define	_UseBusMutex	1

#define	_UseAdcAcqTask	1

#define	_FmcBaseAddr	((uint32_t)0x60000000)

#define	_Led2Ctrl(x)	(x ? (HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET)) : HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET))
#define	_Led2Toggle()	(HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin))
#define	_LedBlueCtrl(x)	(x ? (HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET)) : HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET))
#define	_LedBlueToggle()	(HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin))

#define	_Led3Ctrl(x)	(x ? (HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET)) : HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET))
#define	_Led3Toggle()	(HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin))
#define	_LedRedCtrl(x)	(x ? (HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET)) : HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET))
#define	_LedRedToggle()	(HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin))

typedef struct {
		uint32_t bAdcAcqTaskError : 1;
		uint32_t bEncoderTaskError : 1;
}strSystemFlag;


extern SRAM_HandleTypeDef hsram1;
#if(1 == _UseBusMutex)
extern osMutexId osBusMutex;
#endif

extern uint16_t pusAdcValue[3];

void Error_Handler(void);

#endif /* MAIN_H_ */
