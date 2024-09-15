################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/driver/r_ioport/r_ioport.c 

C_DEPS += \
./synergy/ssp/src/driver/r_ioport/r_ioport.d 

OBJS += \
./synergy/ssp/src/driver/r_ioport/r_ioport.o 

SREC += \
GPIOLed.srec 

MAP += \
GPIOLed.map 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/driver/r_ioport/%.o: ../synergy/ssp/src/driver/r_ioport/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_SYNERGY_ -I"D:/Documents/File_code/RENESAS/GPIOLed/synergy_cfg/ssp_cfg/bsp" -I"D:/Documents/File_code/RENESAS/GPIOLed/synergy_cfg/ssp_cfg/driver" -I"D:/Documents/File_code/RENESAS/GPIOLed/synergy/ssp/inc" -I"D:/Documents/File_code/RENESAS/GPIOLed/synergy/ssp/inc/bsp" -I"D:/Documents/File_code/RENESAS/GPIOLed/synergy/ssp/inc/bsp/cmsis/Include" -I"D:/Documents/File_code/RENESAS/GPIOLed/synergy/ssp/inc/driver/api" -I"D:/Documents/File_code/RENESAS/GPIOLed/synergy/ssp/inc/driver/instances" -I"D:/Documents/File_code/RENESAS/GPIOLed/src" -I"D:/Documents/File_code/RENESAS/GPIOLed/src/synergy_gen" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

