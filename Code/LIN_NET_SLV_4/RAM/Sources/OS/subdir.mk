################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/OS/Scheduler.c" \

C_SRCS += \
../Sources/OS/Scheduler.c \

OBJS += \
./Sources/OS/Scheduler_c.obj \

OBJS_QUOTED += \
"./Sources/OS/Scheduler_c.obj" \

C_DEPS += \
./Sources/OS/Scheduler_c.d \

OBJS_OS_FORMAT += \
./Sources/OS/Scheduler_c.obj \

C_DEPS_QUOTED += \
"./Sources/OS/Scheduler_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/OS/Scheduler_c.obj: ../Sources/OS/Scheduler.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/OS/Scheduler.args" -o "Sources/OS/Scheduler_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/OS/%.d: ../Sources/OS/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


