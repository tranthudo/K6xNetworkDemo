################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/api/api_lib.c \
../lwip/src/api/api_msg.c \
../lwip/src/api/err.c \
../lwip/src/api/netbuf.c \
../lwip/src/api/netdb.c \
../lwip/src/api/netifapi.c \
../lwip/src/api/sockets.c \
../lwip/src/api/tcpip.c 

OBJS += \
./lwip/src/api/api_lib.o \
./lwip/src/api/api_msg.o \
./lwip/src/api/err.o \
./lwip/src/api/netbuf.o \
./lwip/src/api/netdb.o \
./lwip/src/api/netifapi.o \
./lwip/src/api/sockets.o \
./lwip/src/api/tcpip.o 

C_DEPS += \
./lwip/src/api/api_lib.d \
./lwip/src/api/api_msg.d \
./lwip/src/api/err.d \
./lwip/src/api/netbuf.d \
./lwip/src/api/netdb.d \
./lwip/src/api/netifapi.d \
./lwip/src/api/sockets.d \
./lwip/src/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/api/%.o: ../lwip/src/api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DFSL_RTOS_UCOSIII -DFRDM_K64F -DFREEDOM -I../rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../fatfs -I../fatfs/fsl_sd_disk -I../platform/composite/inc -I../src -I../network -I../board -I../utilities -I../startup -I../platform/osa/inc -I../platform/utilities/inc -I../platform/CMSIS/Include -I../platform/devices -I../platform/devices/MK64F12/include -I../platform/devices/MK64F12/startup -I../platform/hal/inc -I../platform/drivers/inc -I../platform/system/inc -I../platform/composite/inc -I../lwip -I../lwip/port -I../lwip/port/arch -I../lwip/src -I../lwip/src/include -I../lwip/src/include/ipv4 -I../lwip/src/include/ipv4/lwip -I../lwip/src/include/ipv6 -I../lwip/src/include/ipv6/lwip -I../lwip/src/include/lwip -I../lwip/src/include/netif -I../lwip/src/include/posix -I../lwip/src/include -I../platform/utilities/inc -I../rtos/uCOSIII/src/config -I../rtos/uCOSIII/src/uCOS-III/Source -I../rtos/uCOSIII/src/uC-CPU -I../rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU -I../rtos/uCOSIII/src/uC-LIB -std=gnu99 -fno-common  -ffreestanding  -fno-builtin  -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


