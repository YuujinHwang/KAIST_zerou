################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c 

OBJS += \
./Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.o 

C_DEPS += \
./Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/%.o: ../Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -DSTM32F746xx -DUSE_HAL_DRIVER -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/inc" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Drivers/CMSIS/Include" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Drivers/STM32F7xx_HAL_Driver/Inc"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


