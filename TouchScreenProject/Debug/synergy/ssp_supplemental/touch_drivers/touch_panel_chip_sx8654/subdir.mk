################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp_supplemental/touch_drivers/touch_panel_chip_sx8654/sf_touch_panel_chip_sx8654.c 

C_DEPS += \
./synergy/ssp_supplemental/touch_drivers/touch_panel_chip_sx8654/sf_touch_panel_chip_sx8654.d 

OBJS += \
./synergy/ssp_supplemental/touch_drivers/touch_panel_chip_sx8654/sf_touch_panel_chip_sx8654.o 

SREC += \
TouchScreenProject.srec 

MAP += \
TouchScreenProject.map 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp_supplemental/touch_drivers/touch_panel_chip_sx8654/%.o: ../synergy/ssp_supplemental/touch_drivers/touch_panel_chip_sx8654/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_SYNERGY_ -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy_cfg/ssp_cfg/bsp" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy_cfg/ssp_cfg/driver" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp/inc" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp/inc/bsp" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp/inc/bsp/cmsis/Include" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp/inc/driver/api" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp/inc/driver/instances" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/src" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/src/synergy_gen" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy_cfg/ssp_cfg/framework" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp/inc/framework/api" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp/inc/framework/instances" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp/inc/framework/tes" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy_cfg/ssp_cfg/framework/el" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp/inc/framework/el" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp/src/framework/el/tx" -I"D:/Documents/File_code/RENESAS/TouchScreenProject/synergy/ssp_supplemental/inc/framework/instances" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

