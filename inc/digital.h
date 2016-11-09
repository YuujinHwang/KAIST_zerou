/*
 * digital.h
 *
 *  Created on: 2016. 10. 10.
 *      Author: PangaeaSolution
 */

#ifndef DIGITAL_H_
#define DIGITAL_H_

#include "main.h"

#define	_AEncoder0Addr	((uint32_t)0x00000000 + _FmcBaseAddr)
#define	_AEncoder1Addr	((uint32_t)0x00000040 + _FmcBaseAddr)
#define	_AEncoder2Addr	((uint32_t)0x00000080 + _FmcBaseAddr)
#define	_AEncoder3Addr	((uint32_t)0x000000C0 + _FmcBaseAddr)

#define	_SteerMotor0Run(x)	(x ? (SteerMotor0_Run_GPIO_Port->BSRR = SteerMotor0_Run_Pin) : (SteerMotor0_Run_GPIO_Port->BSRR = ((uint32_t)SteerMotor0_Run_Pin << 16)))
#define	_SteerMotor0Dir(x)	(x ? (SteerMotor0_Dir_GPIO_Port->BSRR = SteerMotor0_Dir_Pin) : (SteerMotor0_Dir_GPIO_Port->BSRR = ((uint32_t)SteerMotor0_Dir_Pin << 16)))
#define	_SteerMotor0Start(x)	(x ? (SteerMotor0_Start_GPIO_Port->BSRR = SteerMotor0_Start_Pin) : (SteerMotor0_Start_GPIO_Port->BSRR = ((uint32_t)SteerMotor0_Start_Pin << 16)))

#define	_SteerMotor1Run(x)	(x ? (SteerMotor1_Run_GPIO_Port->BSRR = SteerMotor1_Run_Pin) : (SteerMotor1_Run_GPIO_Port->BSRR = ((uint32_t)SteerMotor1_Run_Pin << 16)))
#define	_SteerMotor1Dir(x)	(x ? (SteerMotor1_Dir_GPIO_Port->BSRR = SteerMotor1_Dir_Pin) : (SteerMotor1_Dir_GPIO_Port->BSRR = ((uint32_t)SteerMotor1_Dir_Pin << 16)))
#define	_SteerMotor1Start(x)	(x ? (SteerMotor1_Start_GPIO_Port->BSRR = SteerMotor1_Start_Pin) : (SteerMotor1_Start_GPIO_Port->BSRR = ((uint32_t)SteerMotor1_Start_Pin << 16)))

#define	_DriveMotor0Forward()	((DriveMotor0_F_GPIO_Port->BSRR = (((uint32_t)DriveMotor0_B_Pin << 16)) | (uint32_t)DriveMotor0_F_Pin))
#define	_DriveMotor0Backward()	((DriveMotor0_F_GPIO_Port->BSRR = (((uint32_t)DriveMotor0_F_Pin << 16)) | (uint32_t)DriveMotor0_B_Pin))
#define	_DriveMotor0Ctrl(x)		(x ? (_DriveMotor0Forward()) : (_DriveMotor0Backward()))

#define	_DriveMotor0Stop()		((DriveMotor0_F_GPIO_Port->BSRR = ((uint32_t)DriveMotor0_F_Pin) | ((uint32_t)DriveMotor0_B_Pin)))

#define	_DriveMotor1Forward()	((DriveMotor1_F_GPIO_Port->BSRR = (((uint32_t)DriveMotor1_B_Pin << 16)) | (uint32_t)DriveMotor1_F_Pin))
#define	_DriveMotor1Backward()	((DriveMotor1_F_GPIO_Port->BSRR = (((uint32_t)DriveMotor1_F_Pin << 16)) | (uint32_t)DriveMotor1_B_Pin))
#define	_DriveMotor1Ctrl(x)		(x ? (_DriveMotor1Forward()) : (_DriveMotor1Backward()))

#define	_DriveMotor1Stop()		((DriveMotor1_F_GPIO_Port->BSRR = ((uint32_t)DriveMotor1_F_Pin) | ((uint32_t)DriveMotor1_B_Pin)))

#define	_RelayCtrl(x)		(x ? (RelayCtrl_GPIO_Port->BSRR = RelayCtrl_Pin) : (RelayCtrl_GPIO_Port->BSRR = ((uint32_t)RelayCtrl_Pin << 16)))
#define	_SpareDoutCtrl(x)	(x ? (SpareDout_GPIO_Port->BSRR = SpareDout_Pin) : (SpareDout_GPIO_Port->BSRR = ((uint32_t)SpareDout_Pin << 16)))

#define	_IsKeyIn()			((KeyIn_GPIO_Port->IDR & KeyIn_Pin) ? (GPIO_PIN_SET) : (GPIO_PIN_RESET))
#define	_IsSpareDIn0()		((SpareDin0_GPIO_Port->IDR & SpareDin0_Pin) ? (GPIO_PIN_SET) : (GPIO_PIN_RESET))
#define	_IsSpareDIn1()		((SpareDin1_GPIO_Port->IDR & SpareDin1_Pin) ? (GPIO_PIN_SET) : (GPIO_PIN_RESET))
#define	_IsSpareDIn2()		((SpareDin2_GPIO_Port->IDR & SpareDin2_Pin) ? (GPIO_PIN_SET) : (GPIO_PIN_RESET))

extern osThreadId osEncoderAcqTaskHandle;
extern osThreadId osDigitalInOutTaskHandle;
extern uint32_t ulEncoderTaskCount;

extern uint16_t pusAbsoluteEncoder[2];
extern uint8_t pucDigitalInput[4];

void vEncoderAcqTask(void const * argument);
void vDigitalInOutTask(void const * argument);



#endif /* DIGITAL_H_ */
