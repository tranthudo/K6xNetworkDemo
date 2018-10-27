################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
G:/Work/OSrc/K6xNetworkDemo/Code/platform/drivers/src/cmt/fsl_cmt_common.c \
G:/Work/OSrc/K6xNetworkDemo/Code/platform/drivers/src/cmt/fsl_cmt_driver.c 

OBJS += \
./platform/cmt/drivers/fsl_cmt_common.o \
./platform/cmt/drivers/fsl_cmt_driver.o 

C_DEPS += \
./platform/cmt/drivers/fsl_cmt_common.d \
./platform/cmt/drivers/fsl_cmt_driver.d 


# Each subdirectory must supply rules for building sources it contributes
platform/cmt/drivers/fsl_cmt_common.o: G:/Work/OSrc/K6xNetworkDemo/Code/platform/drivers/src/cmt/fsl_cmt_common.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -I../../../../../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../../../../../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../../../../../rtos -I../../../../../rtos/uCOSIII/src/config -I../../../../../rtos/uCOSIII/src/uCOS-III/Source -I../../../../../rtos/uCOSIII/src/uC-LIB -I../../../../../rtos/uCOSIII/src/uC-CPU -I../../../../../platform/CMSIS/Include -I../../../../../platform/devices -I../../../../../platform/devices/MK64F12/include -I../../../../../platform/devices/MK64F12/startup -I../../../../../platform/utilities/inc -I../../../../../platform/hal/inc -I../../../../../platform/drivers/inc -I../../../../../platform/system/inc -I../../../../../platform/osa/inc -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

platform/cmt/drivers/fsl_cmt_driver.o: G:/Work/OSrc/K6xNetworkDemo/Code/platform/drivers/src/cmt/fsl_cmt_driver.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -I../../../../../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../../../../../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../../../../../rtos -I../../../../../rtos/uCOSIII/src/config -I../../../../../rtos/uCOSIII/src/uCOS-III/Source -I../../../../../rtos/uCOSIII/src/uC-LIB -I../../../../../rtos/uCOSIII/src/uC-CPU -I../../../../../platform/CMSIS/Include -I../../../../../platform/devices -I../../../../../platform/devices/MK64F12/include -I../../../../../platform/devices/MK64F12/startup -I../../../../../platform/utilities/inc -I../../../../../platform/hal/inc -I../../../../../platform/drivers/inc -I../../../../../platform/system/inc -I../../../../../platform/osa/inc -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

