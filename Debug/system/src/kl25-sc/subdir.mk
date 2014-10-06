################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/kl25-sc/adc16.c \
../system/src/kl25-sc/cmp.c \
../system/src/kl25-sc/i2c.c \
../system/src/kl25-sc/llwu.c \
../system/src/kl25-sc/lptmr.c \
../system/src/kl25-sc/mcg.c \
../system/src/kl25-sc/pmc.c \
../system/src/kl25-sc/rcm.c \
../system/src/kl25-sc/rtc.c \
../system/src/kl25-sc/smc.c \
../system/src/kl25-sc/uart.c 

OBJS += \
./system/src/kl25-sc/adc16.o \
./system/src/kl25-sc/cmp.o \
./system/src/kl25-sc/i2c.o \
./system/src/kl25-sc/llwu.o \
./system/src/kl25-sc/lptmr.o \
./system/src/kl25-sc/mcg.o \
./system/src/kl25-sc/pmc.o \
./system/src/kl25-sc/rcm.o \
./system/src/kl25-sc/rtc.o \
./system/src/kl25-sc/smc.o \
./system/src/kl25-sc/uart.o 

C_DEPS += \
./system/src/kl25-sc/adc16.d \
./system/src/kl25-sc/cmp.d \
./system/src/kl25-sc/i2c.d \
./system/src/kl25-sc/llwu.d \
./system/src/kl25-sc/lptmr.d \
./system/src/kl25-sc/mcg.d \
./system/src/kl25-sc/pmc.d \
./system/src/kl25-sc/rcm.d \
./system/src/kl25-sc/rtc.d \
./system/src/kl25-sc/smc.d \
./system/src/kl25-sc/uart.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/kl25-sc/%.o: ../system/src/kl25-sc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g3 -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_STDOUT -DMKL25Z4 -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/kl25-sc" -I../src/Ethernet -I../src/Ethernet/W5500 -I../src/Internet/DHCP -I../src/Internet/DNS -I../src/PlatformHandler -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


