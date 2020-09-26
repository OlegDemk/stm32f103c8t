################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/Humidity_and_Temperature_SI7021_sensor.c 

OBJS += \
./Core/Inc/Humidity_and_Temperature_SI7021_sensor.o 

C_DEPS += \
./Core/Inc/Humidity_and_Temperature_SI7021_sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/Humidity_and_Temperature_SI7021_sensor.o: ../Core/Inc/Humidity_and_Temperature_SI7021_sensor.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Inc/Humidity_and_Temperature_SI7021_sensor.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

