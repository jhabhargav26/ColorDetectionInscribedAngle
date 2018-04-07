################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/InscribedAngle.cpp \
../src/arucoDetector.cpp 

OBJS += \
./src/InscribedAngle.o \
./src/arucoDetector.o 

CPP_DEPS += \
./src/InscribedAngle.d \
./src/arucoDetector.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I/usr/local/include/aruco -I/home/ros/apriltags -I/usr/local/include/opencv2 -I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


