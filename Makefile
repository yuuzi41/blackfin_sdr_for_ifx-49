TARGET = bf_sdr_modulation
SRC = main.c
LIB = bfin_gcc.c bfin_pll.c bfin_spi0.c bfin_uart0.c bfin_gpio.c bfin_timer0.c sin_fr16.S bfin_twi_slave.c adau1361.c ifx-49.c

all:
	bfin-elf-gcc -O2 -mcpu=bf592-any -mfast-fp -Wl,-Map=$(TARGET).map,--cref -I. -Tbf592.ld -o $(TARGET).dxe $(SRC) $(LIB)
	bfin-elf-ldr -T BF592 -c $(TARGET).ldr $(TARGET).dxe --bmode spi

clean:
	rm *.map
	rm *.dxe
	rm *.ldr

