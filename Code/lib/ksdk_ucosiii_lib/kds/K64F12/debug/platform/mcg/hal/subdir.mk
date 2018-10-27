################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
G:/Work/OSrc/K6xNetworkDemo/Code/platform/hal/src/mcg/fsl_mcg_hal.c \
G:/Work/OSrc/K6xNetworkDemo/Code/platform/hal/src/mcg/fsl_mcg_hal_modes.c 

OBJS += \
./platform/mcg/hal/fsl_mcg_hal.o \
./platform/mcg/hal/fsl_mcg_hal_modes.o 

C_DEPS += \
./platform/mcg/hal/fsl_mcg_hal.d \
./platform/mcg/hal/fsl_mcg_hal_modes.d 


# Each subdirectory must supply rules for building sources it contributes
platform/mcg/hal/fsl_mcg_hal.o: G:/Work/OSrc/K6xNetworkDemo/Code/platform/hal/src/mcg/fsl_mcg_hal.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -I../../../../../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../../../../../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../../../../../rtos -I../../../../../rtos/uCOSIII/src/config -I../../../../../rtos/uCOSIII/src/uCOS-III/Source -I../../../../../rtos/uCOSIII/src/uC-LIB -I../../../../../rtos/uCOSIII/src/uC-CPU -I../../../../../platform/CMSIS/Include -I../../../../../platform/devices -I../../../../../platform/devices/MK64F12/include -I../../../../../platform/devices/MK64F12/startup -I../../../../../platform/utilities/inc -I../../../../../platform/hal/inc -I../../../../../platform/drivers/inc -I../../../../../platform/system/inc -I../../../../../platform/osa/inc -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

platform/mcg/hal/fsl_mcg_hal_modes.o: G:/Work/OSrc/K6xNetworkDemo/Code/platform/hal/src/mcg/fsl_mcg_hal_modes.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -I../../../../../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../../../../../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../../../../../rtos -I../../../../../rtos/uCOSIII/src/config -I../../../../../rtos/uCOSIII/src/uCOS-III/Source -I../../../../../rtos/uCOSIII/src/uC-LIB -I../../../../../rtos/uCOSIII/src/uC-CPU -I../../../../../platform/CMSIS/Include -I../../../../../platform/devices -I../../../../../platform/devices/MK64F12/include -I../../../../../platform/devices/MK64F12/startup -I../../../../../platform/utilities/inc -I../../../../../platform/hal/inc -I../../../../../platform/drivers/inc -I../../../../../platform/system/inc -I../../../../../platform/osa/inc -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


