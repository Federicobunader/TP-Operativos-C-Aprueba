################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/conexion.c \
../src/config-reader.c \
../src/gameboy-v2.c \
../src/message-queue.c 

OBJS += \
./src/conexion.o \
./src/config-reader.o \
./src/gameboy-v2.o \
./src/message-queue.o 

C_DEPS += \
./src/conexion.d \
./src/config-reader.d \
./src/gameboy-v2.d \
./src/message-queue.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


