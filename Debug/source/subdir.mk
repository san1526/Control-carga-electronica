################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/control_ADC.c \
../source/control_com.c \
../source/control_corriente.c \
../source/control_ventilador.c \
../source/main.c \
../source/timer.c 

OBJS += \
./source/control_ADC.o \
./source/control_com.o \
./source/control_corriente.o \
./source/control_ventilador.o \
./source/main.o \
./source/timer.o 

C_DEPS += \
./source/control_ADC.d \
./source/control_com.d \
./source/control_corriente.d \
./source/control_ventilador.d \
./source/main.d \
./source/timer.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL46Z256VLH4" -I../CMSIS -I../board -I../drivers -I../startup -I../utilities -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


