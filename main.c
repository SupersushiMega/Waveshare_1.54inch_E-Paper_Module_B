#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/delay.h>

#define CS_ON PORTD |= (1<<PD0)
#define CS_OFF PORTD &= ~(1<<PD0)

#define DS_ON PORTD |= (1<<PD1)
#define DS_OFF PORTD &= ~(1<<PD1)

#define RESET_ON PORTD |= (1<<PD2)
#define RESET_OFF PORTD &= ~(1<<PD2)

#define BUSY !(PIND & (1<<PD3))

void Init();
void SPI_Com (uint8_t data);
void SPI_Data (uint8_t data);

void SPI_Com (uint8_t data)
{/* Start transmission */
	//_delay_ms(10);
	PORTD &= ~(1<<PD0);
	PORTD &= ~(1<<PD1);
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
}

void SPI_Data (uint8_t data)
{/* Start transmission */
	PORTD &= ~(1<<PD1);
	PORTD |= (1<<PD0);
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
}

void Wait4Idle ()
{
	while(BUSY);
}

void Init()
{
	PORTD |= (1<<PD2);
	_delay_ms(2000);
	PORTD &= ~(1<<PD2);
	_delay_ms(2000);
	PORTD |= (1<<PD2);
	_delay_ms(2000);
	
	SPI_Com(0x01);
	SPI_Data(0x07);
	SPI_Data(0x00);
	SPI_Data(0x0A);
	SPI_Data(0x00);
	
	SPI_Com(0x06);
	SPI_Data(0x07);
	SPI_Data(0x07);
	SPI_Data(0x07);
	
	SPI_Com(0x04); //Power ON
	
	SPI_Com(0x71);
	
	SPI_Com(0x00);
	SPI_Data(0xCF);
	
	SPI_Com(0x50);
	SPI_Data(0x57);
	
	SPI_Com(0x61);
	SPI_Data(0x08);
	SPI_Data(0x00);
	SPI_Data(0x08);
	
	SPI_Com(0x82);
	SPI_Data(0x17);
}

const uint8_t Width = 154;
const uint8_t Height = 154;



int main(void)
{
	// Set MOSI, SCK as Output
    DDRB |= (1<<PB3) | (1<<PB5) | (1<<PB2) | (1<<PB1);
	DDRB &= ~(1<<PB4);//Input (Miso)
	PORTB |= (1<<PB1);
	PORTB &= ~(1<<PB2);
	//PORTD |= (1<<PD0);//epaper-BUSY; activate pullup
    // Enable SPI, Set as Master
    // Prescaler: Fosc/16, Enable Interrupts
    //The MOSI, SCK pins are as per ATMega8
    // Enable SPI as master, set clock rate fck/2
    //Init SPI		CLK/2
	//==================================================================
	SPCR = (1<<SPE) | (1<<MSTR);
	SPSR |= (1<<SPI2X);
	//==================================================================
 
    // Enable Global Interrupts
    sei();
	
	DDRD |= (1<<PD0) | (1<<PD1) | (1<<PD2);	//Pin definition CS DS Reset
	PORTD &= ~(1<<PD1);
	PORTD &= ~(1<<PD0);	//set CS to low
	PORTD |= (1<<PD2);	//set Reset to high
	
	DDRC &= ~(1<<PC1);	//Pin definition Input Button
	PORTC |= (1<<PC1);	//activate Pull up
	
	DDRC &= ~(1<<PD3);	//Pin definition Busy Input
	PORTD |= (1<<PD3);	//activate Pull up
	
	//uint8_t Image[154][154] = {0};
	uint64_t x = 0;
	uint8_t y = 0;
	
	
	//~ PORTD &= ~(1<<PD2);
	//~ while(1);
	Init();
	while(1)
	{
		x++;
		y++;
		SPI_Com(0x10);
		SPI_Data(x);
		SPI_Data(y);
		SPI_Data(0x00);
		SPI_Data(0x0F);
		
		SPI_Com(0x13);
		SPI_Data(0x11);
		SPI_Data(0x00);
		SPI_Data(0x00);
		
		SPI_Com(0x12);
	} //end while
}//end of main






