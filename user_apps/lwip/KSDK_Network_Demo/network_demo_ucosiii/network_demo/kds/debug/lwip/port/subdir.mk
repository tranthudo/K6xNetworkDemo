################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Freescale/KSDK_1.3.0/middleware/tcpip/lwip/port/ethernetif.c \
C:/Freescale/KSDK_1.3.0/platform/system/src/hwtimer/fsl_hwtimer_pit_irq.c \
C:/Freescale/KSDK_1.3.0/middleware/tcpip/lwip/port/lwip_fsl_irq.c \
C:/Freescale/KSDK_1.3.0/middleware/tcpip/lwip/port/sys_arch.c 

OBJS += \
./lwip/port/ethernetif.o \
./lwip/port/fsl_hwtimer_pit_irq.o \
./lwip/port/lwip_fsl_irq.o \
./lwip/port/sys_arch.o 

C_DEPS += \
./lwip/port/ethernetif.d \
./lwip/port/fsl_hwtimer_pit_irq.d \
./lwip/port/lwip_fsl_irq.d \
./lwip/port/sys_arch.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/port/ethernetif.o: C:/Freescale/KSDK_1.3.0/middleware/tcpip/lwip/port/ethernetif.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -DFRDM_K64F -DFREEDOM -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../../.. -I../../../network -I../../../board -I../../../../../../../../../platform/osa/inc -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../platform/CMSIS/Include -I../../../../../../../../../platform/devices -I../../../../../../../../../platform/devices/MK64F12/include -I../../../../../../../../../platform/devices/MK64F12/startup -I../../../../../../../../../platform/hal/inc -I../../../../../../../../../platform/drivers/inc -I../../../../../../../../../platform/system/inc -I../../../../../../../../../middleware/tcpip/lwip -I../../../../../../../../../middleware/tcpip/lwip/port -I../../../../../../../../../middleware/tcpip/lwip/port/arch -I../../../../../../../../../middleware/tcpip/lwip/src -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/netif -I../../../../../../../../../middleware/tcpip/lwip/src/include/posix -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../rtos/uCOSIII/src/config -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Source -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU -I../../../../../../../../../rtos/uCOSIII/src/uC-LIB -I../../../../../../.. -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lwip/port/fsl_hwtimer_pit_irq.o: C:/Freescale/KSDK_1.3.0/platform/system/src/hwtimer/fsl_hwtimer_pit_irq.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -DFRDM_K64F -DFREEDOM -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../../.. -I../../../network -I../../../board -I../../../../../../../../../platform/osa/inc -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../platform/CMSIS/Include -I../../../../../../../../../platform/devices -I../../../../../../../../../platform/devices/MK64F12/include -I../../../../../../../../../platform/devices/MK64F12/startup -I../../../../../../../../../platform/hal/inc -I../../../../../../../../../platform/drivers/inc -I../../../../../../../../../platform/system/inc -I../../../../../../../../../middleware/tcpip/lwip -I../../../../../../../../../middleware/tcpip/lwip/port -I../../../../../../../../../middleware/tcpip/lwip/port/arch -I../../../../../../../../../middleware/tcpip/lwip/src -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/netif -I../../../../../../../../../middleware/tcpip/lwip/src/include/posix -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../rtos/uCOSIII/src/config -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Source -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU -I../../../../../../../../../rtos/uCOSIII/src/uC-LIB -I../../../../../../.. -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lwip/port/lwip_fsl_irq.o: C:/Freescale/KSDK_1.3.0/middleware/tcpip/lwip/port/lwip_fsl_irq.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -DFRDM_K64F -DFREEDOM -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../../.. -I../../../network -I../../../board -I../../../../../../../../../platform/osa/inc -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../platform/CMSIS/Include -I../../../../../../../../../platform/devices -I../../../../../../../../../platform/devices/MK64F12/include -I../../../../../../../../../platform/devices/MK64F12/startup -I../../../../../../../../../platform/hal/inc -I../../../../../../../../../platform/drivers/inc -I../../../../../../../../../platform/system/inc -I../../../../../../../../../middleware/tcpip/lwip -I../../../../../../../../../middleware/tcpip/lwip/port -I../../../../../../../../../middleware/tcpip/lwip/port/arch -I../../../../../../../../../middleware/tcpip/lwip/src -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/netif -I../../../../../../../../../middleware/tcpip/lwip/src/include/posix -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../rtos/uCOSIII/src/config -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Source -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU -I../../../../../../../../../rtos/uCOSIII/src/uC-LIB -I../../../../../../.. -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lwip/port/sys_arch.o: C:/Freescale/KSDK_1.3.0/middleware/tcpip/lwip/port/sys_arch.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -DFRDM_K64F -DFREEDOM -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../../.. -I../../../network -I../../../board -I../../../../../../../../../platform/osa/inc -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../platform/CMSIS/Include -I../../../../../../../../../platform/devices -I../../../../../../../../../platform/devices/MK64F12/include -I../../../../../../../../../platform/devices/MK64F12/startup -I../../../../../../../../../platform/hal/inc -I../../../../../../../../../platform/drivers/inc -I../../../../../../../../../platform/system/inc -I../../../../../../../../../middleware/tcpip/lwip -I../../../../../../../../../middleware/tcpip/lwip/port -I../../../../../../../../../middleware/tcpip/lwip/port/arch -I../../../../../../../../../middleware/tcpip/lwip/src -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/netif -I../../../../../../../../../middleware/tcpip/lwip/src/include/posix -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../rtos/uCOSIII/src/config -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Source -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU -I../../../../../../../../../rtos/uCOSIII/src/uC-LIB -I../../../../../../.. -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


