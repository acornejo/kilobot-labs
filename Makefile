TARGETS = test-speaker test-listener orbit-star orbit-planet movetolight gradient sync blinkleds simple-movement nonblocked-movement test-speaker-mod test-listener-mod transmit-receive-randmotion

all: $(TARGETS)
.PHONY: $(TARGETS)

test-speaker: build/test-speaker.elf build/test-speaker.hex build/test-speaker.lss
test-listener: build/test-listener.elf build/test-listener.hex build/test-listener.lss
test-speaker-mod: build/test-speaker-mod.elf build/test-speaker-mod.hex build/test-speaker-mod.lss
test-listener-mod: build/test-listener-mod.elf build/test-listener-mod.hex build/test-listener-mod.lss
orbit-star: build/orbit-star.elf build/orbit-star.hex build/orbit-star.lss
orbit-planet: build/orbit-planet.elf build/orbit-planet.hex build/orbit-planet.lss
movetolight: build/movetolight.elf build/movetolight.hex build/movetolight.lss
gradient: build/gradient.elf build/gradient.hex build/gradient.lss
sync: build/sync.elf build/sync.hex build/sync.lss
blinkleds: build/blinkleds.elf build/blinkleds.hex build/blinkleds.lss
simple-movement: build/simple-movement.elf build/simple-movement.hex build/simple-movement.lss
nonblocked-movement: build/nonblocked-movement.elf build/nonblocked-movement.hex build/nonblocked-movement.lss
transmit-receive-randmotion: build/transmit-receive-randmotion.elf build/transmit-receive-randmotion.hex build/transmit-receive-randmotion.lss

CC = avr-gcc -std=c99
AVROC = avr-objcopy
AVROD = avr-objdump
AVRUP = avrdude

CFLAGS = -mmcu=atmega328p -Wall -gdwarf-2 -O3 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Ikilolib
CFLAGS += -DF_CPU=8000000

FLASH = -R .eeprom -R .fuse -R .lock -R .signature
EEPROM = -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0  

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

define LABS_TEMPLATE
<div class="container">
$$body$$
</div>
endef
export LABS_TEMPLATE

docs:
	@echo "$$LABS_TEMPLATE" > labs.template
	pandoc --from markdown+fenced_code_blocks --to html --template labs.template README.md --output labs.html
	@rm -f labs.template

clean:
	rm -fR build
	rm -f $(KILOLIB)
