################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/AD6688.c \
../src/AD9173.c \
../src/DFX.c \
../src/DMA_support.c \
../src/DPD.c \
../src/JESD.c \
../src/SPI_support.c \
../src/clock.c \
../src/main.c \
../src/mycache.c \
../src/platform.c \
../src/sys_intr.c \
../src/tcp_transmission.c \
../src/timer_intr.c 

OBJS += \
./src/AD6688.o \
./src/AD9173.o \
./src/DFX.o \
./src/DMA_support.o \
./src/DPD.o \
./src/JESD.o \
./src/SPI_support.o \
./src/clock.o \
./src/main.o \
./src/mycache.o \
./src/platform.o \
./src/sys_intr.o \
./src/tcp_transmission.o \
./src/timer_intr.o 

C_DEPS += \
./src/AD6688.d \
./src/AD9173.d \
./src/DFX.d \
./src/DMA_support.d \
./src/DPD.d \
./src/JESD.d \
./src/SPI_support.d \
./src/clock.d \
./src/main.d \
./src/mycache.d \
./src/platform.d \
./src/sys_intr.d \
./src/tcp_transmission.d \
./src/timer_intr.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v8 gcc compiler'
	aarch64-none-elf-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a72 -ID:/FPGA/ADDA/VCK190_ADDA_eval/VCK190_ADDA_eval_vitis/design_eval/export/design_eval/sw/design_eval/standalone_domain/bspinclude/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '




