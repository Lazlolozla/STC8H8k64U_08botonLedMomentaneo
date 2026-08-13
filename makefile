# makefile para STC8H - Lectura de pulsador momentáneo con UART
CC = sdcc
CFLAGS = -mmcs51 --model-large
TARGET = 13botonLedMomentaneo
PORT = /dev/ttyUSB0

all: $(TARGET).ihx

$(TARGET).ihx: $(TARGET).c stc8h.h
	$(CC) $(CFLAGS) $(TARGET).c

flash: $(TARGET).ihx
	stcgal -P stc8g -p $(PORT) -b 2400 $(TARGET).ihx

flash-fast: $(TARGET).ihx
	stcgal -P stc8g -p $(PORT) -b 115200 $(TARGET).ihx

clean:
	rm -f *.ihx *.lk *.lst *.map *.mem *.rel *.rst *.sym *.asm

monitor:
	picocom -b 9600 $(PORT)

.PHONY: all flash flash-fast clean monitor
