MCU = atmega168
PARTNO = m168
PROGRAMMER = usbasp
EFUSE = 0x01
HFUSE = 0xdf
LFUSE = 0xe2

CC = avr-gcc
GCC_FLAGS = -Wall -Os -mcall-prologues -mmcu=$(MCU)
AVRDUDE_FLAGS = -p $(PARTNO) -c $(PROGRAMMER)

PROGNAME = main
OBJ = main.o uc1701.o delay.o si4735.o chkb4.o

$(PROGNAME).hex:$(PROGNAME)
	avr-objcopy -O ihex $(PROGNAME) $(PROGNAME).hex
	avr-objdump -S $(PROGNAME) > $(PROGNAME).s

$(PROGNAME):$(OBJ)
	$(CC) $(GCC_FLAGS) -o $(PROGNAME) $(OBJ)



main.o:main.c
	$(CC) $(GCC_FLAGS) -c main.c

chkb4.o:chkb4.h chkb4.c
	$(CC) $(GCC_FLAGS) -c chkb4.c

delay.o:delay.h delay.c
	$(CC) $(GCC_FLAGS) -c delay.c

uc1701.o:uc1701.h uc1701.c uc1701_latin_charset.h
	$(CC) $(GCC_FLAGS) -c uc1701.c

si4735.o:si4735.h si4735_properties.h si4735.c
	$(CC) $(GCC_FLAGS) -c si4735.c



fuses:
	sudo avrdude $(AVRDUDE_FLAGS) -U efuse:w:$(EFUSE):m -U hfuse:w:$(HFUSE):m -U lfuse:w:$(LFUSE):m

prog:$(PROGNAME).hex
	sudo avrdude $(AVRDUDE_FLAGS) -U flash:w:$(PROGNAME).hex:i

imediate:
	sudo avrdude $(AVRDUDE_FLAGS) -t

clean:
	rm -rf $(PROGNAME) $(PROGNAME).hex *.s *.o *~
