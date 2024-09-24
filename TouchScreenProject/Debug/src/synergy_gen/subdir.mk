################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/synergy_gen/common_data.c \
../src/synergy_gen/hal_data.c \
../src/synergy_gen/main.c \
../src/synergy_gen/main_thread.c \
../src/synergy_gen/pin_data.c 

C_DEPS += \
./src/synergy_gen/common_data.d \
./src/synergy_gen/hal_data.d \
./src/synergy_gen/main.d \
./src/synergy_gen/main_thread.d \
./src/synergy_gen/pin_data.d 

OBJS += \
./src/synergy_gen/common_data.o \
./src/synergy_gen/hal_data.o \
./src/synergy_gen/main.o \
./src/synergy_gen/main_thread.o \
./src/synergy_gen/pin_data.o 

SREC += \
TouchScreenProject.srec 

MAP += \
TouchScreenProject.map 


# Each subdirectory must supply rules for building sources it contributes
src/synergy_gen/%.o: ../src/synergy_gen/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_SYNERGY_ -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy_cfg/ssp_cfg/bsp" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy_cfg/ssp_cfg/driver" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp/inc" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp/inc/bsp" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp/inc/bsp/cmsis/Include" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp/inc/driver/api" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp/inc/driver/instances" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/src" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/src/synergy_gen" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy_cfg/ssp_cfg/framework" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp/inc/framework/api" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp/inc/framework/instances" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp/inc/framework/tes" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy_cfg/ssp_cfg/framework/el" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp/inc/framework/el" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp/src/framework/el/tx" -I"D:/Documents/File_code/RENESAS/RENESAS_SSP_S7G2/TouchScreenProject/synergy/ssp_supplemental/inc/framework/instances" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

