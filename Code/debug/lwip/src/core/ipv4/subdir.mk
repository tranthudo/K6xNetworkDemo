################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/core/ipv4/autoip.c \
../lwip/src/core/ipv4/icmp.c \
../lwip/src/core/ipv4/igmp.c \
../lwip/src/core/ipv4/inet.c \
../lwip/src/core/ipv4/inet_chksum.c \
../lwip/src/core/ipv4/ip.c \
../lwip/src/core/ipv4/ip_addr.c \
../lwip/src/core/ipv4/ip_frag.c 

OBJS += \
./lwip/src/core/ipv4/autoip.o \
./lwip/src/core/ipv4/icmp.o \
./lwip/src/core/ipv4/igmp.o \
./lwip/src/core/ipv4/inet.o \
./lwip/src/core/ipv4/inet_chksum.o \
./lwip/src/core/ipv4/ip.o \
./lwip/src/core/ipv4/ip_addr.o \
./lwip/src/core/ipv4/ip_frag.o 

C_DEPS += \
./lwip/src/core/ipv4/autoip.d \
./lwip/src/core/ipv4/icmp.d \
./lwip/src/core/ipv4/igmp.d \
./lwip/src/core/ipv4/inet.d \
./lwip/src/core/ipv4/inet_chksum.d \
./lwip/src/core/ipv4/ip.d \
./lwip/src/core/ipv4/ip_addr.d \
./lwip/src/core/ipv4/ip_frag.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/core/ipv4/%.o: ../lwip/src/core/ipv4/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -DFRDM_K64F -DFREEDOM -I../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../src -I../network -I../board -I../utilities -I../startup -I../platform/osa/inc -I../platform/utilities/inc -I../platform/CMSIS/Include -I../platform/devices -I../platform/devices/MK64F12/include -I../platform/devices/MK64F12/startup -I../platform/hal/inc -I../platform/drivers/inc -I../platform/system/inc -I../lwip -I../lwip/port -I../lwip/port/arch -I../lwip/src -I../lwip/src/include -I../lwip/src/include/ipv4 -I../lwip/src/include/ipv4/lwip -I../lwip/src/include/ipv6 -I../lwip/src/include/ipv6/lwip -I../lwip/src/include/lwip -I../lwip/src/include/netif -I../lwip/src/include/posix -I../lwip/src/include -I../platform/utilities/inc -I../rtos/uCOSIII/src/config -I../rtos/uCOSIII/src/uCOS-III/Source -I../rtos/uCOSIII/src/uC-CPU -I../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../rtos/uCOSIII/src/uC-LIB -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


