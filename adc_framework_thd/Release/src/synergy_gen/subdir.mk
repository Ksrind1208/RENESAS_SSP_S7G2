################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/synergy_gen/adc_framework_thd.c \
../src/synergy_gen/common_data.c \
../src/synergy_gen/hal_data.c \
../src/synergy_gen/main.c \
../src/synergy_gen/pin_data.c 

C_DEPS += \
./src/synergy_gen/adc_framework_thd.d \
./src/synergy_gen/common_data.d \
./src/synergy_gen/hal_data.d \
./src/synergy_gen/main.d \
./src/synergy_gen/pin_data.d 

OBJS += \
./src/synergy_gen/adc_framework_thd.o \
./src/synergy_gen/common_data.o \
./src/synergy_gen/hal_data.o \
./src/synergy_gen/main.o \
./src/synergy_gen/pin_data.o 

SREC += \
adc_framework_thd.srec 

MAP += \
adc_framework_thd.map 


# Each subdirectory must supply rules for building sources it contributes
src/synergy_gen/%.o: ../src/synergy_gen/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_SYNERGY_ -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/bsp" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/driver" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/bsp" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/bsp/cmsis/Include" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/driver/api" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/driver/instances" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/src" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/src/synergy_gen" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/framework" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/framework/api" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/framework/instances" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy_cfg/ssp_cfg/framework/el" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/inc/framework/el" -I"D:/Documents/File_code/RENESAS/adc_framework_thd/synergy/ssp/src/framework/el/tx" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

