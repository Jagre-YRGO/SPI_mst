
//typedef unsigned char uint8_t;
//static volatile uint8_t* const spcr = (volatile uint8_t*)(0x4C);
//#define SPCR *spcr

# define F_CPU 16000000UL


#include  <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
	/* Set MOSI, SCK and SS output, all others input */
	DDRB |= (1<< 5) | (1 << 3) | (1 << 2); //SCK as output and MOSI as output and SS as outpu - orginal
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	uint8_t data = 0x01;
	uint16_t d = 0;
	
    while (1) 
    {
		/* Start transmission */
	    PORTB &= ~_BV(PORTB2); //set ss low
		/*first byte Constant config bytes: Write to DACB, Output Gain = 1, Active mode operation */
		SPDR = 0xF0; 
		//SPDR = ((uint8_t)d >> 8) || 0x0F ;
		//data++;
		
		/* Wait for transmission complete */
        while(!(SPSR & (1<<SPIF)));

		//SPDR = data; //second byte
		SPDR = (uint8_t)d; //second byte
		data++;
		
		if (d == 0x0FFF )
			d = 0;
		else
			d++;

		/* Wait for transmission complete */
		while(!(SPSR & (1<<SPIF)));

		PORTB |= _BV(PORTB2); //set ss High
		
		//_delay_ms(100);
		
    }
}

