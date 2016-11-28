################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../alnutils.c \
../blutils.c \
../dummy.c \
../encutils.c \
../epipred3.0.c \
../fsautils.c \
../lstutils.c \
../nnutils.c \
../peputils.c \
../strutils.c \
../sysutils.c \
../xyzutils.c 

OBJS += \
./alnutils.o \
./blutils.o \
./dummy.o \
./encutils.o \
./epipred3.0.o \
./fsautils.o \
./lstutils.o \
./nnutils.o \
./peputils.o \
./strutils.o \
./sysutils.o \
./xyzutils.o 

C_DEPS += \
./alnutils.d \
./blutils.d \
./dummy.d \
./encutils.d \
./epipred3.0.d \
./fsautils.d \
./lstutils.d \
./nnutils.d \
./peputils.d \
./strutils.d \
./sysutils.d \
./xyzutils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


