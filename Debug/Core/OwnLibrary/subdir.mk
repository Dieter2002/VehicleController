################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/OwnLibrary/VL6180X.c 

C_DEPS += \
./Core/OwnLibrary/VL6180X.d 

OBJS += \
./Core/OwnLibrary/VL6180X.o 


# Each subdirectory must supply rules for building sources it contributes
Core/OwnLibrary/%.o Core/OwnLibrary/%.su Core/OwnLibrary/%.cyclo: ../Core/OwnLibrary/%.c Core/OwnLibrary/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H533xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-OwnLibrary

clean-Core-2f-OwnLibrary:
	-$(RM) ./Core/OwnLibrary/VL6180X.cyclo ./Core/OwnLibrary/VL6180X.d ./Core/OwnLibrary/VL6180X.o ./Core/OwnLibrary/VL6180X.su

.PHONY: clean-Core-2f-OwnLibrary

