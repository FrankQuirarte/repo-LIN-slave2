################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Application/LED_App.c" \
"../Sources/Application/SlaveApp.c" \

C_SRCS += \
../Sources/Application/LED_App.c \
../Sources/Application/SlaveApp.c \

OBJS += \
./Sources/Application/LED_App_c.obj \
./Sources/Application/SlaveApp_c.obj \

OBJS_QUOTED += \
"./Sources/Application/LED_App_c.obj" \
"./Sources/Application/SlaveApp_c.obj" \

C_DEPS += \
./Sources/Application/LED_App_c.d \
./Sources/Application/SlaveApp_c.d \

OBJS_OS_FORMAT += \
./Sources/Application/LED_App_c.obj \
./Sources/Application/SlaveApp_c.obj \

C_DEPS_QUOTED += \
"./Sources/Application/LED_App_c.d" \
"./Sources/Application/SlaveApp_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Application/LED_App_c.obj: ../Sources/Application/LED_App.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Application/LED_App.args" -o "Sources/Application/LED_App_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Application/%.d: ../Sources/Application/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Application/SlaveApp_c.obj: ../Sources/Application/SlaveApp.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Application/SlaveApp.args" -o "Sources/Application/SlaveApp_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


