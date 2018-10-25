################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Freescale/KSDK_1.3.0/examples/K6xNetworkDemo/user_apps/lwip/KSDK_Network_Demo/network_demo_ucosiii/fsl_phy_driver.c \
C:/Freescale/KSDK_1.3.0/examples/K6xNetworkDemo/user_apps/lwip/KSDK_Network_Demo/network_demo_ucosiii/main.c 

OBJS += \
./source/fsl_phy_driver.o \
./source/main.o 

C_DEPS += \
./source/fsl_phy_driver.d \
./source/main.d 


# Each subdirectory must supply rules for building sources it contributes
source/fsl_phy_driver.o: C:/Freescale/KSDK_1.3.0/examples/K6xNetworkDemo/user_apps/lwip/KSDK_Network_Demo/network_demo_ucosiii/fsl_phy_driver.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -DFRDM_K64F -DFREEDOM -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../../.. -I../../../network -I../../../board -I../../../../../../../../../platform/osa/inc -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../platform/CMSIS/Include -I../../../../../../../../../platform/devices -I../../../../../../../../../platform/devices/MK64F12/include -I../../../../../../../../../platform/devices/MK64F12/startup -I../../../../../../../../../platform/hal/inc -I../../../../../../../../../platform/drivers/inc -I../../../../../../../../../platform/system/inc -I../../../../../../../../../middleware/tcpip/lwip -I../../../../../../../../../middleware/tcpip/lwip/port -I../../../../../../../../../middleware/tcpip/lwip/port/arch -I../../../../../../../../../middleware/tcpip/lwip/src -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/netif -I../../../../../../../../../middleware/tcpip/lwip/src/include/posix -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../rtos/uCOSIII/src/config -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Source -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU -I../../../../../../../../../rtos/uCOSIII/src/uC-LIB -I../../../../../../.. -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/main.o: C:/Freescale/KSDK_1.3.0/examples/K6xNetworkDemo/user_apps/lwip/KSDK_Network_Demo/network_demo_ucosiii/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -DFRDM_K64F -DFREEDOM -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../../.. -I../../../network -I../../../board -I../../../../../../../../../platform/osa/inc -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../platform/CMSIS/Include -I../../../../../../../../../platform/devices -I../../../../../../../../../platform/devices/MK64F12/include -I../../../../../../../../../platform/devices/MK64F12/startup -I../../../../../../../../../platform/hal/inc -I../../../../../../../../../platform/drivers/inc -I../../../../../../../../../platform/system/inc -I../../../../../../../../../middleware/tcpip/lwip -I../../../../../../../../../middleware/tcpip/lwip/port -I../../../../../../../../../middleware/tcpip/lwip/port/arch -I../../../../../../../../../middleware/tcpip/lwip/src -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv4/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6 -I../../../../../../../../../middleware/tcpip/lwip/src/include/ipv6/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/lwip -I../../../../../../../../../middleware/tcpip/lwip/src/include/netif -I../../../../../../../../../middleware/tcpip/lwip/src/include/posix -I../../../../../../../../../middleware/tcpip/lwip/src/include -I../../../../../../../../../platform/utilities/inc -I../../../../../../../../../rtos/uCOSIII/src/config -I../../../../../../../../../rtos/uCOSIII/src/uCOS-III/Source -I../../../../../../../../../rtos/uCOSIII/src/uC-CPU -I../../../../../../../../../rtos/uCOSIII/src/uC-LIB -I../../../../../../.. -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


