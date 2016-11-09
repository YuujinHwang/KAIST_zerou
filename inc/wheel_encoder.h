/*
 * tim.h
 *
 *  Created on: 2016. 10. 12.
 *      Author: PangaeaSolution
 */

#ifndef WHEEL_ENCODER_H_
#define WHEEL_ENCODER_H_

#include "main.h"

#define	_UseWheelEncoderInputCapture	0

extern TIM_HandleTypeDef xTimer3Handle;

extern float pfEncoderFreq[4];
extern uint32_t pulFrequency[4];
extern uint32_t pulDiffCapture[4];

#if	(1 == _UseWheelEncoderInputCapture)
void vTimer3InputCaptureInit(void);
#else
void vTimer3BaseInit(void);
void vWheelEncoderGpioInit(void);
#endif




#endif /* WHEEL_ENCODER_H_ */
