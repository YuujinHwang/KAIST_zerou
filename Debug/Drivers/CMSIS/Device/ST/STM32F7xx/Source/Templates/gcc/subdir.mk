################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/gcc/startup_stm32f745xx.s 

OBJS += \
./Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/gcc/startup_stm32f745xx.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/gcc/%.o: ../Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/gcc/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/inc" -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/FreeRTOS/Source/ -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


