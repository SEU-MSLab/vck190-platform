################################################################################
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ELFSIZE += \
ADDA_eval_PS.elf.size \


# All Target
all: ADDA_eval_PS.elf secondary-outputs

# Tool invocations
ADDA_eval_PS.elf: $(OBJS) ../src/lscript.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM v8 gcc linker'
	aarch64-none-elf-gcc -mcpu=cortex-a72 -Wl,-T -Wl,../src/lscript.ld -LD:/FPGA/ADDA/VCK190_ADDA_eval/VCK190_ADDA_eval_vitis/design_eval/export/design_eval/sw/design_eval/standalone_domain/bsplib/lib -o "ADDA_eval_PS.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ADDA_eval_PS.elf.size: ADDA_eval_PS.elf
	@echo 'Invoking: ARM v8 Print Size'
	aarch64-none-elf-size ADDA_eval_PS.elf  |tee "ADDA_eval_PS.elf.size"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(S_UPPER_DEPS)$(C_DEPS)$(ELFSIZE) ADDA_eval_PS.elf
	-@echo ' '

secondary-outputs: $(ELFSIZE)

.PHONY: all clean dependents

-include ../makefile.targets

