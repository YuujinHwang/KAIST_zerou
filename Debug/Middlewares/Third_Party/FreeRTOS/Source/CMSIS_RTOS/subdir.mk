################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -DSTM32F746xx -DUSE_HAL_DRIVER -I"D:/Dropbox/work/Eclipse/SystemWorkBench/KAIST_AUTO/inc" -I"D:/Dropbox/work/Eclipse/SystemWorkBench/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"D:/Dropbox/work/Eclipse/SystemWorkBench/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source" -I"D:/Dropbox/work/Eclipse/SystemWorkBench/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source/include" -I"D:/Dropbox/work/Eclipse/SystemWorkBench/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"D:/Dropbox/work/Eclipse/SystemWorkBench/KAIST_AUTO/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"D:/Dropbox/work/Eclipse/SystemWorkBench/KAIST_AUTO/Drivers/CMSIS/Include" -I"D:/Dropbox/work/Eclipse/SystemWorkBench/KAIST_AUTO/Drivers/STM32F7xx_HAL_Driver/Inc"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


