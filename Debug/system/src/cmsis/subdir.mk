################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/system_MKL25Z4.c \
../system/src/cmsis/vectors_MKL25Z4.c 

OBJS += \
./system/src/cmsis/system_MKL25Z4.o \
./system/src/cmsis/vectors_MKL25Z4.o 

C_DEPS += \
./system/src/cmsis/system_MKL25Z4.d \
./system/src/cmsis/vectors_MKL25Z4.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/%.o: ../system/src/cmsis/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_STDOUT -DMKL25Z4 -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/kl25-sc" -I../src/Ethernet -I../src/Ethernet/W5500 -I../src/Internet/DHCP -I../src/Internet/DNS -I../src/PlatformHandler -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


