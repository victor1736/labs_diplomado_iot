################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/fsl_component_generic_list.c 

C_DEPS += \
./component/lists/fsl_component_generic_list.d 

OBJS += \
./component/lists/fsl_component_generic_list.o 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c component/lists/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_K32L2B31VLH0A -DCPU_K32L2B31VLH0A_cm0plus -DFRDM_K32L2B -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"A:\Diplomado_IoT\NXP\Repositorios\labs_diplomado_iot\frdmk32l2bf_led_blinky\source" -I"A:\Diplomado_IoT\NXP\Repositorios\labs_diplomado_iot\frdmk32l2bf_led_blinky\utilities" -I"A:\Diplomado_IoT\NXP\Repositorios\labs_diplomado_iot\frdmk32l2bf_led_blinky\drivers" -I"A:\Diplomado_IoT\NXP\Repositorios\labs_diplomado_iot\frdmk32l2bf_led_blinky\device" -I"A:\Diplomado_IoT\NXP\Repositorios\labs_diplomado_iot\frdmk32l2bf_led_blinky\component\uart" -I"A:\Diplomado_IoT\NXP\Repositorios\labs_diplomado_iot\frdmk32l2bf_led_blinky\component\lists" -I"A:\Diplomado_IoT\NXP\Repositorios\labs_diplomado_iot\frdmk32l2bf_led_blinky\CMSIS" -I"A:\Diplomado_IoT\NXP\Repositorios\labs_diplomado_iot\frdmk32l2bf_led_blinky\board" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-lists

clean-component-2f-lists:
	-$(RM) ./component/lists/fsl_component_generic_list.d ./component/lists/fsl_component_generic_list.o

.PHONY: clean-component-2f-lists

