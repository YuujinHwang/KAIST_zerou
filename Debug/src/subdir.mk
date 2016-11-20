################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/dac.c \
../src/digital.c \
../src/freertos.c \
../src/main.c \
../src/stm32f7xx_hal_msp.c \
../src/stm32f7xx_it.c \
../src/syscalls.c \
../src/wheel_encoder.c 

OBJS += \
./src/dac.o \
./src/digital.o \
./src/freertos.o \
./src/main.o \
./src/stm32f7xx_hal_msp.o \
./src/stm32f7xx_it.o \
./src/syscalls.o \
./src/wheel_encoder.o 

C_DEPS += \
./src/dac.d \
./src/digital.d \
./src/freertos.d \
./src/main.d \
./src/stm32f7xx_hal_msp.d \
./src/stm32f7xx_it.d \
./src/syscalls.d \
./src/wheel_encoder.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -DSTM32F746xx -DUSE_HAL_DRIVER -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/inc" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Drivers/CMSIS/Include" -I"C:/Users/ACL/Documents/GitHub/KAIST_AUTO/Drivers/STM32F7xx_HAL_Driver/Inc"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


