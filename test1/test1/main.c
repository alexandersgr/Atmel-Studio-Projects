/*
 * test1.c
 *
 * Created: 11.08.2018 18:11:17
 * Author : Dean
 */ 


#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include "../../00-libfiles/uart.c"
#include "../../00-libfiles/i2c.c"

#include "lcd_hd44780_avr.c"


#define led_on		PORTD |= (1<<PORTD0);
#define led_off		PORTD &= ~(1<<PORTD0);

#define led_toggle	PINB |= (1<<PINB0);

#define _BV(bit) (1 << (bit))  //bit value
//TCCR2 = _BV(COM20)|_BV(CTC2)|_BV(CS20);


#define bit_is_set(sfr,bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr,bit) (!(_SFR_BYTE(sfr) & _BV(bit)))

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= -_BV(bit))  //clear bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))   //set bit

#define STOP_TIMER TCCR0 &= 0b11111000
#define START_TIMER TCCR0 |= 0b00000011 // prescaler = c/64

typedef volatile unsigned int WORD;

//кр зел
//
//-  бел  кл
//-  син
//че жел


unsigned char digit_to_7segval(unsigned char digit);
void ConfigureDevice(void);

WORD Interrupts=0;
unsigned int step =0;
unsigned char N1 = 0;
unsigned char N2 = 0;

ISR(TIMER0_OVF_vect)
{

	// at 11,0592mhz with a 'Clock/64' prescaler, 675 interrupts = 1 sec!
	
	
	
	if (Interrupts == 1)
	{
		if (step==2)
		{
			step = 0;
		}
		
		Interrupts = 0;
		/////////
		
		switch(step)
		{
			case 0:
			PORTB |= (1<<PORTB0);
			PORTB &= ~(1<<PORTB1);
			//PORTD = ~digit_to_7segval(N2);
			break;
			
			case 1:
			PORTB |= (1<<PORTB1);
			PORTB &= ~(1<<PORTB0);
			//PORTD = ~digit_to_7segval(N1);
			break;
		}
		
		
		step++;
		
		////////
	}
	
	Interrupts++;
}

#define MCP4725_ADDR 0x60

unsigned char lookup = 0; //varaible for navigating through the tables

int sintab2[8] =
{
	0001, 0500, 1000, 1500, 2000, 3000, 3500, 4095
};

int main(void)
{
	
	UART_Init(9600);
	
//TCCR1B |= (1 << CS10)|(1 << CS12);
///////////////DDRD = 0xFF; //|= (1<<DDD0);  //PD0 pin2 out
//DDRD &= ~(1<<DDD1); //PD1 pin3 in

//////////////////DDRB |= ((1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3));

ConfigureDevice();

I2C_Init();



UART_Printf("start\n\r");
	
	I2C_Start();
		I2C_Write(0xc0);
		I2C_Write(0x4);
		I2C_Write(0x62);
		I2C_Stop();
		UART_Printf("test ");
		
		
		
		   //Initialize LCD module
		   LCDInit(LS_BLINK|LS_ULINE);

		   //Clear the screen
		   LCDClear();

		   //Simple string printing
		   LCDWriteString("Congrats ");

		   //A string on line 2
		   LCDWriteStringXY(0,1,"Loading ");

while (1)
	{
	//bit_is_clear(PINC, 4)
	
	//if (PIND & (1<<PIND1))	{		led_off;	}	else	{		led_on;	}
	
	//led_toggle;
	//led_on;
	//_delay_ms(500);
	//led_off;
	//_delay_ms(500);
	
	//UART_Printf("Welcome to AVR Serial Programming by ExploreEmbedded\n\r");
	
	/*
	(signed/unsigned) char - 1 byte
	(signed/unsigned) short - 2 bytes
	(signed/unsigned) int - 2 bytes
	(signed/unsigned) long - 4 bytes
	(signed/unsigned) long long - 8 bytes
	float - 4 bytes (floating point)
	double - alias to float
	*/
	
	
	
	I2C_Start();
	I2C_Write(0xc0);
	I2C_Write(0x4);
	I2C_Write(lookup);
	I2C_Stop();
	
	char string[8];

	itoa(lookup, string, 10);
	
	UART_Printf(string);
	UART_Printf("\n\r");
	
	if (lookup==255)
	{
		lookup=0;
	} 
	else
	{
		lookup++;
	}
		
	
	
	/*for (int i = 0; i < 255; i++)
	{
		I2C_Start();
		I2C_Write(i);
		I2C_Write(64);
		I2C_Write(sintab2[7] >> 4);
		I2C_Write((sintab2[7] & 15) << 4);
		I2C_Stop();
		//UART_Printf("test ");
		_delay_ms(30);
	}*/
	
			if (N1==15)	{N1=0; N2++;}
			if (N2==15)	{N2=0;}
			_delay_ms(100);
			N1++;
	
	}
}

	/*Configure SwitchPort as an input port*/
//	SwitchPortDDR = 0x00;
	/*Enable SwitchPort pull-up resistors*/
//	SwitchPort = 0xFF;

void ConfigureDevice(void)
{
	cli(); // mask interruptions

	_SFR_BYTE(DDRD)=0b11111111;
	_SFR_BYTE(DDRC)=0b00001111;
	_SFR_BYTE(DDRB)=0b00000000;

	TCNT0 = 0x00;
	START_TIMER;
	TIMSK |= _BV(TOIE0);

	sei(); // unmask interruptions
}


/*

unsigned char digit_to_7segval(unsigned char digit)
{
	unsigned char segval = '0';
	
	if(digit == 0)
	segval = 0x3F;
	else if(digit == 1)
	segval = 0x06;
	else if (digit == 2)
	segval = 0x5B;
	else if (digit == 3)
	segval = 0x4F;
	else if (digit == 4)
	segval = 0x66;
	else if (digit == 5)
	segval = 0x6D;
	else if (digit == 6)
	segval = 0x7D;
	
	else if (digit == 7)
	segval = 0x07;
	else if (digit == 8)
	segval = 0x7F;
	else if (digit == 9)
	segval = 0x6F;
	else if (digit == 10) //a
	segval = 0x77;
	else if (digit == 11) //b
	segval = 0x7C;
	else if (digit == 12) //c
	segval = 0x39;
	else if (digit == 13) //d
	segval = 0x5E;
	else if (digit == 14) //e
	segval = 0x79;
	else if (digit == 15) //f
	segval = 0x71;
	
	return segval;
}

*/

/*unsigned char segs[]=
{
	0b00111111,
	0b00000110,
	0b01011011,
	0b01001111,
	0b01100110,
	0b01101101,
	0b01111101,
	0b00000111,
	0b01111111,
	0b01101111,
	0x77,
	0x7C,
	0x39,
	0x5E,
	0x79,
	0x71
};
*/