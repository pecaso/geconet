################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
/home/jackiez/20160219/geco-sctp-cplus/src/auth.cc \
/home/jackiez/20160219/geco-sctp-cplus/src/dispatch_layer.cc \
/home/jackiez/20160219/geco-sctp-cplus/src/gecotimer.cc \
/home/jackiez/20160219/geco-sctp-cplus/src/globals.cc \
/home/jackiez/20160219/geco-sctp-cplus/src/transport_layer.cc 

CPP_SRCS += \
/home/jackiez/20160219/geco-sctp-cplus/src/geco-malloc.cpp 

CC_DEPS += \
./src/auth.d \
./src/dispatch_layer.d \
./src/gecotimer.d \
./src/globals.d \
./src/transport_layer.d 

OBJS += \
./src/auth.o \
./src/dispatch_layer.o \
./src/geco-malloc.o \
./src/gecotimer.o \
./src/globals.o \
./src/transport_layer.o 

CPP_DEPS += \
./src/geco-malloc.d 


# Each subdirectory must supply rules for building sources it contributes
src/auth.o: /home/jackiez/20160219/geco-sctp-cplus/src/auth.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/jackiez/20160219/geco-sctp-cplus/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/dispatch_layer.o: /home/jackiez/20160219/geco-sctp-cplus/src/dispatch_layer.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/jackiez/20160219/geco-sctp-cplus/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/geco-malloc.o: /home/jackiez/20160219/geco-sctp-cplus/src/geco-malloc.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/jackiez/20160219/geco-sctp-cplus/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gecotimer.o: /home/jackiez/20160219/geco-sctp-cplus/src/gecotimer.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/jackiez/20160219/geco-sctp-cplus/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/globals.o: /home/jackiez/20160219/geco-sctp-cplus/src/globals.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/jackiez/20160219/geco-sctp-cplus/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/transport_layer.o: /home/jackiez/20160219/geco-sctp-cplus/src/transport_layer.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/jackiez/20160219/geco-sctp-cplus/include" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

