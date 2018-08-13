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

#define led_on		PORTD |= (1<<PORTD0);
#define led_off		PORTD &= ~(1<<PORTD0);

#define led_toggle	PIND |= (1<<PIND0);

#define _BV(bit) (1 << (bit))
//TCCR2 = _BV(COM20)|_BV(CTC2)|_BV(CS20);


#define bit_is_set(sfr,bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr,bit) \
(!(_SFR_BYTE(sfr) & _BV(bit)))

#define STOP_TIMER TCCR0 &= 0b11111000
#define START_TIMER TCCR0 |= 0b00000011 // prescaler = c/64

typedef volatile unsigned int WORD;

//�� ���
//
//-  ���  ��
//-  ���
//�� ���


unsigned char digit_to_7segval(unsigned char digit);
WORD Interrupts=0;;

ISR(TIMER0_OVF_vect)
{

	// at 11,0592mhz with a 'Clock/64' prescaler, 675 interrupts = 1 sec!
	
	if (Interrupts == 675)
	{
		Interrupts = 0;
	}
	
	Interrupts++;
}



int main(void)
{
//TCCR1B |= (1 << CS10)|(1 << CS12);
DDRD = 0xFF; //|= (1<<DDD0);  //PD0 pin2 out
//DDRD &= ~(1<<DDD1); //PD1 pin3 in

ConfigureDevice();

while (1)
	{
	//bit_is_clear(PINC, 4)
	
	//if (PIND & (1<<PIND1))	{		led_off;	}	else	{		led_on;	}
	
	//led_toggle;
	//led_on;
	//_delay_ms(500);
	//led_off;
	//_delay_ms(500);
	
	
	for (int i=0; i<=15; i++)
		{
			PORTD = ~digit_to_7segval(i);
			_delay_ms(300);
		}
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

unsigned char segs[]=
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
