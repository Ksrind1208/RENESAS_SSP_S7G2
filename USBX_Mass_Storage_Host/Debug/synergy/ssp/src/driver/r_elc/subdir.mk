################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/driver/r_elc/r_elc.c 

C_DEPS += \
./synergy/ssp/src/driver/r_elc/r_elc.d 

OBJS += \
./synergy/ssp/src/driver/r_elc/r_elc.o 

SREC += \
USBX_Mass_Storage_Host.srec 

MAP += \
USBX_Mass_Storage_Host.map 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/driver/r_elc/%.o: ../synergy/ssp/src/driver/r_elc/%.c
	$(file > $@.in,-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_SYNERGY_ -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy_cfg/ssp_cfg/bsp" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy_cfg/ssp_cfg/driver" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy/ssp/inc" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy/ssp/inc/bsp" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy/ssp/inc/bsp/cmsis/Include" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy/ssp/inc/driver/api" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy/ssp/inc/driver/instances" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/src" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/src/synergy_gen" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy_cfg/ssp_cfg/framework/el" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy/ssp/inc/framework/el" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy/ssp/src/framework/el/tx" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy/ssp/src/framework/el/ux" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy/ssp/src/framework/el/tx/tx_src" -I"D:/Documents/File_code/RENESAS/USBX_Mass_Storage_Host/synergy_cfg/ssp_cfg/framework" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

