################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/RC522/rc522_config.c \
../user/RC522/rc522_function.c 

OBJS += \
./user/RC522/rc522_config.o \
./user/RC522/rc522_function.o 

C_DEPS += \
./user/RC522/rc522_config.d \
./user/RC522/rc522_function.d 


# Each subdirectory must supply rules for building sources it contributes
user/RC522/%.o: ../user/RC522/%.c user/RC522/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Miss/Desktop/IcCard/user/RC522" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

