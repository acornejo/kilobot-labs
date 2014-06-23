TARGETS = test-speaker test-listener orbit-star orbit-planet movetolight gradient

all: $(TARGETS)
.PHONY: $(TARGETS)

test-speaker: build/test-speaker.elf build/test-speaker.hex build/test-speaker.lss
test-listener: build/test-listener.elf build/test-listener.hex build/test-listener.lss
orbit-star: build/orbit-star.elf build/orbit-star.hex build/orbit-star.lss
orbit-planet: build/orbit-planet.elf build/orbit-planet.hex build/orbit-planet.lss
movetolight: build/movetolight.elf build/movetolight.hex build/movetolight.lss
gradient: build/gradient.elf build/gradient.hex build/gradient.lss

CC = avr-gcc
AVROC = avr-objcopy
AVROD = avr-objdump
AVRUP = avrdude

PFLAGS = -P /dev/ttyACM0 -c avrisp2
PFLAGS2 = -P usb -c avrispmkII
CFLAGS = -mmcu=atmega328p -Wall -gdwarf-2 -O3 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Ikilolib
CFLAGS += -DF_CPU=8000000

FLASH = -R .eeprom -R .fuse -R .lock -R .signature
EEPROM = -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0  

define MERGE 
	@cat $(1) | grep -v ":00000001FF" > $(3)
	@cat $(2) >> $(3)
endef

KILOLIB = kilolib/build/kilolib.a

$(KILOLIB):
	make -C kilolib build/kilolib.a

%.lss: %.elf
	$(AVROD) -d -S $< > $@

%.hex: %.elf
	$(AVROC) -O ihex $(FLASH) $< $@

%.eep: %.elf
	$(AVROC) -O ihex $(EEPROM) $< $@

%.bin: %.elf
	$(AVROC) -O binary $(FLASH) $< $@ 

build:
	mkdir -p $@

build/%.elf: %.c $(KILOLIB)  | build
	$(CC) $(CFLAGS) -o $@ $< $(KILOLIB)

clean:
	rm -fR build
	rm -f $(KILOLIB)
