################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Basic Software/Slave4_Rsp_Rx_Tx.c" \

C_SRCS += \
../Sources/Basic\ Software/Slave4_Rsp_Rx_Tx.c \

OBJS += \
./Sources/Basic\ Software/Slave4_Rsp_Rx_Tx_c.obj \

OBJS_QUOTED += \
"./Sources/Basic Software/Slave4_Rsp_Rx_Tx_c.obj" \

C_DEPS += \
./Sources/Basic\ Software/Slave4_Rsp_Rx_Tx_c.d \

OBJS_OS_FORMAT += \
./Sources/Basic\ Software/Slave4_Rsp_Rx_Tx_c.obj \

C_DEPS_QUOTED += \
"./Sources/Basic Software/Slave4_Rsp_Rx_Tx_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Basic\ Software/Slave4_Rsp_Rx_Tx_c.obj: ../Sources/Basic\ Software/Slave4_Rsp_Rx_Tx.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Sources/Basic Software/Slave4_Rsp_Rx_Tx.args" -o "Sources/Basic Software/Slave4_Rsp_Rx_Tx_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/Basic\ Software/Slave4_Rsp_Rx_Tx_c.d: ../Sources/Basic\ Software/Slave4_Rsp_Rx_Tx.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


