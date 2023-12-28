
//typedef unsigned char uint8_t;
//static volatile uint8_t* const spcr = (volatile uint8_t*)(0x4C);
//#define SPCR *spcr

# define F_CPU 16000000UL


#include  <avr/io.h>
#include <util/delay.h>

uint8_t spi_write(uint8_t d){
	SPDR = d;				//write data
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
	
	return SPDR; //return received value
}



int main(void)
{
	
	/* Set MOSI, SCK and SS output, all others input */
	DDRB |= (1<< 5) | (1 << 3) | (1 << 2) | (1 << 0); //SCK as output and MOSI as output and SS as outpu - orginal
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	uint16_t d = 0;
	uint8_t d_lsb = 0;
	uint8_t d_msb = 0;
	
    while (1) 
    {
		/*Read AD*/
		//AD-read start bit high, config-bits high. All the rest low
		//Resulting in single-ended mode, CH1 active (p.15 in data sheet)
		//Leading zero inserted to achieve neat byte-boundary (p.17 in data sheet)
		PORTB &= ~_BV(PORTB0);	//set ss low
		d_msb = spi_write(0x78) & 0x03; 
		d_lsb = spi_write(0x00);
		PORTB |= _BV(PORTB0);	//set ss High
		
		/*Merge together and shift by two (according to MCP4812)*/
		d = ((d_msb << 8) | (d_lsb)) << 2;
		
		/*Write DA*/
		//All config bits high, resulting in Active mode, 1x gain (2.048V max out), DACB used
		//p.22 in data sheet
		PORTB &= ~_BV(PORTB2);	//set ss low
		(void)spi_write(((uint8_t)((d & 0x0F00) >> 8)) | 0xF0);
		(void)spi_write((uint8_t)(d & 0x00FF));
		PORTB |= _BV(PORTB2);	//set ss High
		
    }
}

