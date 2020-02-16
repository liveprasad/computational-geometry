################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../DCEL.o \
../SimplePoly.o 

CPP_SRCS += \
../DCEL.cpp \
../Program.cpp \
../SimplePoly.cpp 

OBJS += \
./DCEL.o \
./Program.o \
./SimplePoly.o 

CPP_DEPS += \
./DCEL.d \
./Program.d \
./SimplePoly.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


