################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/board/s7g2_sk/bsp_init.c \
../synergy/board/s7g2_sk/bsp_leds.c \
../synergy/board/s7g2_sk/bsp_qspi.c 

C_DEPS += \
./synergy/board/s7g2_sk/bsp_init.d \
./synergy/board/s7g2_sk/bsp_leds.d \
./synergy/board/s7g2_sk/bsp_qspi.d 

OBJS += \
./synergy/board/s7g2_sk/bsp_init.o \
./synergy/board/s7g2_sk/bsp_leds.o \
./synergy/board/s7g2_sk/bsp_qspi.o 

SREC += \
adc_framework_thd.srec 

MAP += \
adc_framework_thd.map 


# Each subdirectory must supply rules for building sources it contributes
synergy/board/s7g2_sk/%.o: ../synergy/board/s7g2_sk/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_SYNERGY_ -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/bsp" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/driver" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/bsp" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/bsp/cmsis/Include" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/driver/api" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/driver/instances" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/src" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/src/synergy_gen" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/framework" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/framework/api" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/framework/instances" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/framework/el" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/framework/el" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/src/framework/el/tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

