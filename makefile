MCU = atmega328p
DEF_CPU = __AVR_ATmega168__
F_CPU = 16000000L
UPLOAD_RATE = 19200
ARDUINOSOURCES = .arduino
AVRDUDE = ./.arduino/avrdude
AVRDUDE_CONF = ./.arduino/avrdude.conf
TARGET = itplazeretcher
ARDUINO_PORT = /dev/tty.usbserial-A600e1kC
PROJ_SRC =
PROJ_PDESRC =  itplazeretcher.pde motorControl.pde
CXXSRC = \
    $(PROJ_CXXSRC) \
    $(ARDUINOSOURCES)/HardwareSerial.cpp \
    $(ARDUINOSOURCES)/WMath.cpp \
    $(ARDUINOSOURCES)/Print.cpp \
    $(ARDUINOSOURCES)/Arduino_main.cpp \
    $(CORE_CXXSRC)

SRC = \
    $(PROJ_SRC) \
    $(ARDUINOSOURCES)/pins_arduino.c \
    $(ARDUINOSOURCES)/wiring.c \
    $(ARDUINOSOURCES)/wiring_analog.c \
    $(ARDUINOSOURCES)/wiring_digital.c \
    $(ARDUINOSOURCES)/wiring_pulse.c \
    $(ARDUINOSOURCES)/wiring_serial.c \
    $(ARDUINOSOURCES)/wiring_shift.c \
    $(ARDUINOSOURCES)/WInterrupts.c \
    $(CORE_SRC)
	
PDESRC = \
	$(PROJ_PDESRC)

FORMAT = ihex

CFLAGS = -gstabs -DF_CPU=$(F_CPU) -D$(DEF_CPU) -I $(ARDUINOSOURCES) -Os -std=gnu99
CXXFLAGS = -gstabs -DF_CPU=$(F_CPU) -D$(DEF_CPU) -I $(ARDUINOSOURCES) -Os -fno-exceptions
LDFLAGS = -lm -Xlinker --allow-multiple-definition

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex:i
AVRDUDE_FLAGS = -F -p $(MCU) -P $(ARDUINO_PORT) -cstk500 -b $(UPLOAD_RATE) -C $(AVRDUDE_CONF)

# Program settings
MACPACK=/usr/local/CrossPack-AVR/bin
CC = $(MACPACK)/avr-gcc
CXX = $(MACPACK)/avr-g++
OBJCOPY = $(MACPACK)/avr-objcopy

OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(PDESRC:.pde=.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)


# Default target.
all: build

build: elf hex

elf: $(TARGET).elf
hex: $(TARGET).hex

# Program the device.  
upload: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)


.SUFFIXES: .elf .hex .pde

# Link: create ELF output file from object files.
$(TARGET).elf: $(OBJ)
	$(CC) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

# Compile: create object files from C++ source files.
.cpp.o:
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 

# Compile: create object files from C++ source files.
.pde.o:
	$(CXX) -c -x c++ $(ALL_CXXFLAGS) $< -o $@ 

# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Target: clean project.
clean:
	-rm -f $(TARGET).hex $(TARGET).elf $(TARGET).map $(OBJ)

.PHONY:	all build elf hex clean
