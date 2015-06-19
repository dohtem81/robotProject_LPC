NAME   = robot

CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-ld -v
AR      = arm-none-eabi-ar
AS      = arm-none-eabi-as
CP      = arm-none-eabi-objcopy
OD		= arm-none-eabi-objdump

CFLAGS  = -I./ -c  -fno-common -O0 -g
AFLAGS  = -ahls -mapcs-32 -o crt.o
IAFLAGS  = -ahls -mapcs-32 -o IAP.o
LFLAGS  =  -Map robot.map -T robot.cmd
CPFLAGS = -O binary
HEXFLAGS = -O ihex
ODFLAGS	= -x --syms

SRCF = ../source/
COMF = ../../Common/source/

all: test

clean:
	-rm crt.lst IAP.lst robot.o robot.out robot.hex robot.map robot.dmp robot.bin System.o crt.o timer.o pwm.o ENC28J60.o spi.o ethernet.o limNet.o tmp102.o i2c.o IAP.o
	
test: robot.out
	@ echo "...copying"
	$(CP) $(CPFLAGS) robot.out robot.bin
	$(OD) $(ODFLAGS) robot.out > robot.dmp
	@echo "...building hex"
	$(CP) $(HEXFLAGS) robot.out robot.hex

robot.out: IAP.o crt.o System.o timer.o ENC28J60.o spi.o ethernet.o limNet.o tmp102.o i2c.o pwm.o robot.cmd 
	@ echo "..linking"
	$(CC) -o robot.out crt.o System.o timer.o ENC28J60.o spi.o ethernet.o limNet.o tmp102.o i2c.o IAP.o pwm.o -nostartfiles -Trobot.cmd

	 
	 
crt.o: $(SRCF)crt.s
	@ echo ".assembling crt"
	$(AS) $(AFLAGS) $(SRCF)crt.s > crt.lst
	
IAP.o: $(COMF)IAP.s
	@ echo ".assembling IAP"
	$(AS) $(IAFLAGS) $(COMF)IAP.s > IAP.lst

System.o: $(SRCF)System.c

	 $(CC) $(CFLAGS) $(SRCF)System.c	 
	 
timer.o: $(COMF)timer.c

	 $(CC) $(CFLAGS) $(COMF)timer.c
	 
ENC28J60.o: $(COMF)ENC28J60.c

	 $(CC) $(CFLAGS) $(COMF)ENC28J60.c
	 
spi.o: $(COMF)spi.c

	 $(CC) $(CFLAGS) $(COMF)spi.c
	 
limNet.o: $(COMF)limNet.c

	 $(CC) $(CFLAGS) $(COMF)limNet.c
	 
ethernet.o: $(COMF)ethernet.c

	 $(CC) $(CFLAGS) $(COMF)ethernet.c
	 	 
tmp102.o: $(COMF)tmp102.c

	 $(CC) $(CFLAGS) $(COMF)tmp102.c
	 	 
i2c.o: $(COMF)i2c.c

	 $(CC) $(CFLAGS) $(COMF)i2c.c
	 
pwm.o: $(COMF)pwm.c

	 $(CC) $(CFLAGS) $(COMF)pwm.c
	 