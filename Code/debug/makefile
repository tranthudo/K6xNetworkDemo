################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include rtos/uCOSIII/src/uCOS-III/Source/subdir.mk
-include rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU/subdir.mk
-include rtos/uCOSIII/src/uC-LIB/subdir.mk
-include rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU/subdir.mk
-include rtos/uCOSIII/src/uC-CPU/subdir.mk
-include rtos/uCOSIII/src/config/subdir.mk
-include platform/utilities/src/subdir.mk
-include platform/composite/src/sdcard/subdir.mk
-include network/subdir.mk
-include lwip/src/netif/ppp/subdir.mk
-include lwip/src/netif/subdir.mk
-include lwip/src/core/snmp/subdir.mk
-include lwip/src/core/ipv4/subdir.mk
-include lwip/src/core/subdir.mk
-include lwip/src/api/subdir.mk
-include lwip/port/subdir.mk
-include fatfs/fsl_sd_disk/src/subdir.mk
-include fatfs/subdir.mk
-include board/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: network_demo_ucosiii_frdmk64f.elf secondary-outputs

# Tool invocations
network_demo_ucosiii_frdmk64f.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -T ../platform/devices/MK64F12/linker/gcc/MK64FN1M0xxx12_flash.ld -Xlinker --gc-sections -Wl,-Map,"network_demo_ucosiii_frdmk64f.map" -v -Xlinker -z  -Xlinker muldefs   --specs=nano.specs          -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mapcs  -Xlinker -static  -Xlinker --defsym=__stack_size__=0x4000  -Xlinker --defsym=__heap_size__=0x10000 -o "network_demo_ucosiii_frdmk64f.elf" -Xlinker --start-group $(OBJS) $(USER_OBJS) $(LIBS) -Xlinker --end-group
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(S_UPPER_DEPS) network_demo_ucosiii_frdmk64f.elf
	-@echo ' '

post-build:
	-arm-none-eabi-size --format=sysv network_demo_ucosiii_frdmk64f.elf  & arm-none-eabi-size --format=berkeley network_demo_ucosiii_frdmk64f.elf
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
