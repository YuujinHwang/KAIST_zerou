/*
 * dac.h
 *
 *  Created on: 2016. 10. 10.
 *      Author: PangaeaSolution
 */

#ifndef DAC_H_
#define DAC_H_

#include "main.h"

#define	_Dac1Addr		((uint32_t)0x00000020 + _FmcBaseAddr)
#define	_Dac2Addr		((uint32_t)0x00000024 + _FmcBaseAddr)
#define	_Dac3Addr		((uint32_t)0x00000028 + _FmcBaseAddr)
#define	_Dac4Addr		((uint32_t)0x0000002C + _FmcBaseAddr)
#define	_Dac5Addr		((uint32_t)0x00000030 + _FmcBaseAddr)
#define	_Dac6Addr		((uint32_t)0x00000034 + _FmcBaseAddr)

#define	_DacLSWHoldAddr	((uint32_t)0x00000000)
#define	_DacMSWHoldAddr	((uint32_t)0x00000001)
#define	_DacControlAddr	((uint32_t)0x00000003)

#define	_Dac1LswAddr	((uint32_t)(_Dac1Addr + _DacLSWHoldAddr))
#define	_Dac1MswAddr	((uint32_t)(_Dac1Addr + _DacMSWHoldAddr))
#define	_Dac1CtrlAddr	((uint32_t)(_Dac1Addr + _DacControlAddr))

#define	_Dac2LswAddr	((uint32_t)(_Dac2Addr + _DacLSWHoldAddr))
#define	_Dac2MswAddr	((uint32_t)(_Dac2Addr + _DacMSWHoldAddr))
#define	_Dac2CtrlAddr	((uint32_t)(_Dac2Addr + _DacControlAddr))

#define	_Dac3LswAddr	((uint32_t)(_Dac3Addr + _DacLSWHoldAddr))
#define	_Dac3MswAddr	((uint32_t)(_Dac3Addr + _DacMSWHoldAddr))
#define	_Dac3CtrlAddr	((uint32_t)(_Dac3Addr + _DacControlAddr))

#define	_Dac4LswAddr	((uint32_t)(_Dac4Addr + _DacLSWHoldAddr))
#define	_Dac4MswAddr	((uint32_t)(_Dac4Addr + _DacMSWHoldAddr))
#define	_Dac4CtrlAddr	((uint32_t)(_Dac4Addr + _DacControlAddr))

#define	_Dac5LswAddr	((uint32_t)(_Dac5Addr + _DacLSWHoldAddr))
#define	_Dac5MswAddr	((uint32_t)(_Dac5Addr + _DacMSWHoldAddr))
#define	_Dac5CtrlAddr	((uint32_t)(_Dac5Addr + _DacControlAddr))

#define	_Dac6LswAddr	((uint32_t)(_Dac6Addr + _DacLSWHoldAddr))
#define	_Dac6MswAddr	((uint32_t)(_Dac6Addr + _DacMSWHoldAddr))
#define	_Dac6CtrlAddr	((uint32_t)(_Dac6Addr + _DacControlAddr))

#define	_DacCtrlFastBit		((uint8_t)(1 << 0))
#define	_DacCtrlNormalBit	((uint8_t)(1 << 1))
#define	_DacCtrlLatchBit	((uint8_t)(1 << 2))

extern osThreadId osDacSetTaskHandle;

void vDacSetTask(void const * argument);
void vDacValueSet(uint8_t ucCh, uint16_t usValue);



#endif /* DAC_H_ */
