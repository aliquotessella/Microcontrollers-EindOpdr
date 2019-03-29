//***************************************************************************
//  File Name    : basicsound.c
//  Version      : 1.0
//  Description  : Basic AVR Sound with PWM
//  Author(s)    : RWB
//  Target(s)    : AVRJazz Mega168 Learning Board
//  Compiler     : AVR-GCC 4.3.0; avr-libc 1.6.2 (WinAVR 20080610)
//  IDE          : Atmel AVR Studio 4.14
//  Programmer   : AVRJazz Mega168 STK500 v2.0 Bootloader
//               : AVR Visual Studio 4.14, STK500 programmer
//  Last Updated : 02 Jan 2008
//***************************************************************************
/*
 * ThereminCopy.c
 *
 * Created: 20/03/2019 10:05:39
 * Author : Tim
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "audio.h"
#include "song.h"

int duration;
int distance;

void echo(void){
		
		//digitalWrite(trigPin, HIGH);
		PORTE = 0b00000100;
		
		//delayMicroseconds(1000);
		_delay_us(1000);
		
		//digitalWrite(trigPin, LOW);
		PORTE = 0b00000000;
		TCNT1 = 0;
}


ISR(INT4_vect)
{
	if (PINE & 0b00010000)
	{
		duration = 0;
		TCNT1 = 0;
		TCCR1B |= 0b10;	
	}else
	{
		TCCR1B = 0;
		duration = TCNT1;
		//todo get timer number
		distance = (duration/2) / 29.1;
		PORTB = duration/10;
		if (distance >= 200 || distance <= 0){
			//Serial.println("Out of range");
			PORTA = 0b11111111;
		}
		else {
			//Serial.print(distance);
			//Serial.println(" cm");
			PORTA = distance;
		}
	}
}

int main(void)
{
	int tune = 0;
	int oct = 0;
	int tonelen = 200;
	int lastOct = 0;
	DDRD = 0b00000100;
	DDRA = 0b11111111;
	DDRB = 0b11111111;
	
	EICRB = 0b00000001;
	EIMSK = 0b00010000;
	sei();
	
	DDRF = 0b00001000;			//set DDRD3 as output
	TCCR3B |= 0b00000001;	//set up timer
	while(1)
	{
		echo();
		if(PORTA < 0b11111111){
			tune = 1050 - (PORTA * 4);
		}
		if(tune > 100)
		{
			//playTune(tune, 100);
			oct = PORTA / 8;
			if(lastOct != oct){
				lastOct = oct;
				switch(oct)
				{
					case (0):
						playTune(261, tonelen);
						break;
					case (1):
						playTune(293,tonelen);
						break;
					case (2):
						playTune(329,tonelen);
						break;
					case (3):
						playTune(349,tonelen);
						break;
					case (4):
						playTune(391,tonelen);
						break;
					case (5):
						playTune(440,tonelen);
						break;
					case (6):
						playTune(493,tonelen);
						break;
					case (7):
						playTune(523,tonelen);
						break;
					case (8):
						playTune(587,tonelen);
						break;
					default:
						wait(tonelen);
						break;
				}
			}else{
				wait(100);
			}
		}else{
			wait(100);
		}
		//marioTheme();
	}
	
	return 0;	                  // Standard Return Code
}

