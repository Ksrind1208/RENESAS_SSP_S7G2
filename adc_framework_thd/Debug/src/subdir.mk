################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc_framework_thd_entry.c \
../src/hal_entry.c 

C_DEPS += \
./src/adc_framework_thd_entry.d \
./src/hal_entry.d 

OBJS += \
./src/adc_framework_thd_entry.o \
./src/hal_entry.o 

SREC += \
adc_framework_thd.srec 

MAP += \
adc_framework_thd.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_SYNERGY_ -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/bsp" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/driver" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/bsp" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/bsp/cmsis/Include" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/driver/api" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/driver/instances" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/src" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/src/synergy_gen" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/framework" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/framework/api" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/framework/instances" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/framework/el" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/framework/el" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/src/framework/el/tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

