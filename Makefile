PREFIX=arm-none-eabi-
AS=$(PREFIX)as
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
OBJCOPY=$(PREFIX)objcopy
OBJDUMP=$(PREFIX)objdump
LIB_PATH=libs_iskra
LINK=$(LIB_PATH)/link.ld

AFLAGS=
CFLAGS=
LFLAGS=

_AFLAGS=-mthumb -mcpu=cortex-m4 -g -I$(LIB_PATH)/inc $(AFLAGS)
_CFLAGS=-c -mthumb -mcpu=cortex-m4 -O2 -g -I$(LIB_PATH)/inc $(CFLAGS)
_LFLAGS=-nostdlib --gc-sections -g $(LFLAGS)

LIBS_SRC=$(wildcard $(LIB_PATH)/src/*.c) $(wildcard $(LIB_PATH)/src/*.s)
LIBS_HDR=$(wildcard $(LIB_PATH)/inc/*.h) $(wildcard $(LIB_PATH)/inc/*.inc)
LIBS_OBJ=$(LIBS_SRC:$(LIB_PATH)/src/%=obj/libs/%.o)

SRC=$(wildcard src/*.c) $(wildcard src/*.s)
HDR=$(wildcard src/*.h) $(wildcard src/*.inc)
OBJ=$(SRC:src/%=obj/%.o)

OUTPUT=firmware

all: $(OUTPUT).bin

test:
	@echo $(LIBS_OBJ)
	@echo $(OBJ)

$(OUTPUT).bin: $(OUTPUT)
	$(OBJCOPY) -O binary $< $@

dump: $(OUTPUT)
	$(OBJDUMP) -D $< | less

st-link: $(OUTPUT).bin
	st-flash --connect-under-reset write $< 0x08000000

$(OUTPUT): $(LINK) $(OBJ) $(LIBS_OBJ)
	@mkdir -p $(@D)
	$(LD) $(_LFLAGS) -o $@ -T $^

obj/libs/%.c.o: $(LIB_PATH)/src/%.c $(LIBS_HDR) Makefile $(LIB_PATH)/Makefile
	@mkdir -p $(@D)
	$(CC) $(_CFLAGS) -o $@ $<

obj/libs/%.s.o: $(LIB_PATH)/src/%.s $(LIBS_HDR) Makefile $(LIB_PATH)/Makefile
	@mkdir -p $(@D)
	$(AS) $(_AFLAGS) -o $@ $<

obj/%.c.o: src/%.c $(HDR) $(LIBS_HDR) Makefile $(LIB_PATH)/Makefile
	@mkdir -p $(@D)
	$(CC) $(_CFLAGS) -o $@ $<

obj/%.s.o: src/%.s $(HDR) $(LIBS_HDR) Makefile $(LIB_PATH)/Makefile
	@mkdir -p $(@D)
	$(AS) $(_AFLAGS) -o $@ $<

clear:
	rm -r obj $(OUTPUT){,.bin} 2> /dev/null || false

.PHONY: all dump st-link clear test
