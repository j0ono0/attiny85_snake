# Makefile for programming the ATtiny85
# I made this myself. It's my first Makefile so probably terrible!
# j0ono0 2024

# Some variables
TARGET = t85
BAUD  = 125kHz

F_CPU = 1000000UL 
CC = avr-gcc
OBJCOPY = avr-objcopy

CFLAGS = -std=c99 -Wall -g -Os -mmcu=attiny85 -DF_CPU=${F_CPU} -I.
FLAGS= -mmcu=attiny85 -DF_CPU=$(F_CPU) -Os -Wall

OBJDUMP = avr-objdump
AVRSIZE = avr-size

SRC = main.c

OBJS = main.o i2c_t85.o ssd1306.o

## Compile application

all : 
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRC}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

build : build.hex

build.hex : main.elf
	$(OBJCOPY) main.elf -O ihex main.hex

main.elf : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -Os -o main.elf


main.o : main.c i2c_t85.h
	$(CC) $(FLAGS) -c  main.c

i2c_t85.o : i2c_t85.c i2c_t85.h
	$(CC) $(FLAGS) -c i2c_t85.c

ssd1306.o : ssd1306.c ssd1306.h
	$(CC) $(FLAGS) -c ssd1306.c

## Program hardware

flash : main.c
	avrdude -c usbasp -p $(TARGET) -v -B $(BAUD) -U flash:w:main.hex:i


## 	Utilities

.PHONY : clean check
# Check programmer and chip are wired up correctly
check :
	avrdude -c usbasp -p $(TARGET) -B $(BAUD)

clean :
	rm -fv *.elf *.hex *.o