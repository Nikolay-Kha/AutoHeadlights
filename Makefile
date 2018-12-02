SDCC=/usr/local/bin/sdcc
FLASHER=/usr/local/bin/stm8flash

INCLUDEDIRS	= -I. $(addprefix -I, sdk/STM8S_StdPeriph_Driver/inc)
OBJDIR = build
FIRMWARE = build/firmware.ihx
SOURCES = $(wildcard *.c) $(addprefix sdk/STM8S_StdPeriph_Driver/src/stm8s_, gpio.c uart1.c adc1.c clk.c tim1.c)
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:%.c=%.rel))

PLATFORM = -lstm8 -mstm8
CFLAGS = --opt-code-size -DSTM8S003 -D__CSMC__ -DHSE_Value

.PHONY: all clean flash rebuild unlock

all: $(FIRMWARE)

clean:
	@rm -rf $(OBJDIR)
	
rebuild: clean all

flash: all
	$(FLASHER) -c stlinkv2 -p stm8s003?3 -w $(FIRMWARE)

unlock:
	$(FLASHER) -c stlinkv2 -p stm8s003?3 -u

$(FIRMWARE): $(OBJECTS)
	@echo "LD $@"
	@$(SDCC) -lstm8 -mstm8 $(INCLUDEDIRS) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.rel: %.c
	@echo "CC $<"
	@mkdir -p $(dir $@)
	@$(SDCC) $(PLATFORM) $(INCLUDEDIRS) $(CFLAGS) $(LDFLAGS) -c $< -o $@
