/*
 * moistureSensorTest7SegVoltiSimul.c
 *
 * Created: 14/11/2023 04:23:34
 * Author : Towa
 */ 

#define F_CPU 16E6

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include <time.h>

uint16_t adc_result;
uint16_t Vref = 500;
uint8_t percentage;
uint16_t voltage;



void Pin_init()
{
	//D0-D7, B0, B2-B5
	// Set D0 to D7 as output pins
	DDRD = 0xFF;  // 0xFF in binary is 11111111
	DDRB |= (1 << DDB0) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5); //setting several Bits individually 
}

void ADC_init()
{
	// Set ADC reference to AVCC
	ADMUX = (1<<REFS0);

	// Enable the ADC and set the prescaler to 128 (16MHz/128 = 125 KHz)
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t ADC_read(uint8_t ch)
{
	// Select ADC channel ch must be 0-7
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;

	// Start single conversion
	ADCSRA |= (1<<ADSC);

	// Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	// Clear ADIF by writing one to it
	ADCSRA|=(1<<ADIF);

	return (ADC);
}

uint8_t ADC_result_percentage(uint16_t res)
{
	percentage = res * 1023 / 100;
	return (percentage);
}

uint16_t ADC_result_voltage(uint16_t res) //changed into uint16_t ohne Komma weil einfacher mit Pins!!!
{
	uint16_t voltage = res * Vref / 1023.0;
	return (voltage);
}

void Perc_bcd_Output(uint8_t perc) //TODO Fehler in Berechnungen for some reason!
{
	if (perc == 100)
	{
		//00000001
		//00000000
		//00000000	
	}
	//first convert the percentage to single BCD digits
	//bigger number:
	uint8_t ten = perc / 10;
	// Manipulate bits for Port D0 to D3
	PORTD = (PORTD & 0xF0) | (ten & 0x0F);
	
	//smaller number:
	uint8_t one = perc % 10;
	
	// Manipulate bits for Port B2 to B5
	PORTB = (PORTB & 0xC3) | ((one << 2) & 0x3C);
}

void Volt_bcd_Output(uint16_t volt)
{

}


int main(void)
{
    while (1) 
    {
			//test output
			PORTD = 0b10101010;
			PORTB = 0b00101000; //seems to work, except LED
			PORTB |= (1<<0);
			
			// Initialize the ADC
			ADC_init();

			// Read the ADC value from channel 0
			adc_result = ADC_read(0); //result in 10bit between 0 and 1023 with 1023 as the reference voltage
			
			ADC_result_percentage(adc_result);
			//Perc_bcd_Output(percentage);
			
			
			
			ADC_result_voltage(adc_result);
			//Volt_bcd_Output(voltage);
			 // You can add a delay here if needed
			 // This is just an example, and you might need to adjust the delay based on your requirements
			 _delay_ms(1000);  // Delay for 1000 milliseconds (1 second)
			 
			 			
    }
	return 0;
}

