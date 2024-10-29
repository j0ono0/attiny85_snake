# Makefile for programming the ATtiny85
# I made this myself. It's my first Makefile so probably terrible!
# j0ono0 2024

# Some variables
TARGET = t85
BAUD  = 125kHz

F_CPU = 8000000UL 
CC = avr-gcc
OBJCOPY = avr-objcopy

CFLAGS = -std=c99 -Wall -g -Os -mmcu=attiny85 -DF_CPU=${F_CPU} -I.
FLAGS= -mmcu=attiny85 -DF_CPU=$(F_CPU) -Os -Wall

OBJDUMP = avr-objdump
AVRSIZE = avr-size

SRC = main.c

OBJS = main.o t85_i2c.o ssd1306.o t85_adc.o t85_pwm.o


## Compile and Program hardware
all : main.hex
	avrdude -c usbasp -p $(TARGET) -v -B $(BAUD) -U flash:w:main.hex:i

## Compile application

build : main.hex

main.hex : main.elf
	$(OBJCOPY) main.elf -O ihex main.hex

main.elf : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -Os -o main.elf


main.o : main.c t85_i2c.h
	$(CC) $(FLAGS) -c  main.c

t85_i2c.o : t85_i2c.c t85_i2c.h
	$(CC) $(FLAGS) -c t85_i2c.c

ssd1306.o : ssd1306.c ssd1306.h
	$(CC) $(FLAGS) -c ssd1306.c

t85_adc.o : t85_adc.c t85_adc.h
	$(CC) $(FLAGS) -c t85_adc.c

t85_pwm.o : t85_pwm.c t85_pwm.h
	$(CC) $(FLAGS) -c t85_pwm.c

## 	Utilities

.PHONY : clean check
# Check programmer and chip are wired up correctly
check :
	avrdude -c usbasp -p $(TARGET) -B $(BAUD)

clean :
	rm -fv *.elf *.hex *.o