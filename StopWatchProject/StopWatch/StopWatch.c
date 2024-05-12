#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Declarations.h"

int main(void) {
	//set PINS from 0 to 6 in portA as output pins
	//includes pin 6 for the buzzer
	DDRA |= 0x3F;

	//set PINS from 0 to 3 in portC as output pins
	DDRC |= 0x0F;

	DDRB |= (1<<PB5);

	//initialize buzzer pin (PA6) to zero
	PORTB &= ~(1 << PB5);

	//enable global interrupt bit
	SREG |= (1 << 7);

	TIMER1_Init();
	INT0_Init();
	INT1_Init();
	INT2_Init();

	while (1) {
		Display_7Segment();
		if(Buzzertick == 1){
			BUZZEROFF();
			flag = 0;
			Buzzertick = 0;
		}
	}
}

void TIMER1_Init(void) {
//	TCCR1A:
//	Non PWM mode : set FOC1A & set FOC1B
//	Normal comp mode : clear COM1A0 , COM1A1 , COM1B0 , COM1B1
//	MODE 4 ->> CTC : Clear WGM00 & WGM11
	TCCR1A |= (1 << FOC1A);

//	TCCR1B:
//	MODE 4 ->> CTC : set WGM12 & Clear WGM13
//	PRESCALAR = 1024 : set CS10 , CS12 & clear CS11
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);

	//initial value
	TCNT1 = 0;
	//COMPARE VALUE
	OCR1A = 976;

	//Output Compare A Match Interrupt Enable
	TIMSK |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
	SEC++;
	if (SEC == 60) {
		SEC = 0;
		MIN++;
	}
	if (MIN == 60) {
		MIN = 0;
		HR++;
	}
	if (HR == 99) {
		HR = 0;
	}
	if(flag ==  1){
		Buzzertick++;
	}
}

void INT0_Init(void) {
//set PINS 2 in PORTD as input pins (INT0)
	DDRD &= ~(1 << PD2);
//activate the internal PULL-UP resistor of pin 2 in PORTD
	PORTD |= (1 << PD2);

//	MCUCR:
//	falling Edge : set ISC01 and clear ISC00
	MCUCR |= (1 << ISC01);

//	GICR:
//	enable INT0 External Interrupt Request : set INT0
	GICR |= (1 << INT0);
}

//Reset the StopWatch
ISR(INT0_vect) {
	flag = 1;
	SEC = 0;
	MIN = 0;
	HR = 0;
	BUZZERSOUND();
}

void INT1_Init(void) {
//set PINS 3 in PORTD as input pins (INT1)
	DDRD &= ~(1 << PD3);

//	MCUCR:
//	falling Edge : set ISC10 & ISC11
	MCUCR |= (1 << ISC10) | (1 << ISC11);

//	GICR:
//	enable INT1 External Interrupt Request : set INT1
	GICR |= (1 << INT1);
}

//Pause the stopWatch
ISR(INT1_vect){
//	TCCR1B:
//	No clock : clear CS10 , CS11 & CS12
	TCCR1B &= ~(1 << CS10) & ~(1 << CS11) & ~(1 << CS12);
	flag = 1;
	BUZZERSOUND();
}

void INT2_Init(void){
//set PIN 2 in PORTB as as input pins (INT2)
	DDRB &= ~(1 << PB2);
//activate the internal PULL-UP resistor of pin 2 in PORTB
	PORTB |= (1 << PB2);

//	MCUCSR:
//	falling Edge : clear ISC2
	MCUCSR &= ~(1 << ISC2);

//	GICR:
//	enable INT2 External Interrupt Request : set INT2
	GICR |= (1 << INT2);
}

//Resume the stopWatch
ISR(INT2_vect){
//	TCCR1B:
//	set clock with PRESCALAR 1024 : set CS10 & CS12
	TCCR1B |= (1 << CS10) | (1 << CS12);
	flag = 1;
	BUZZERSOUND();
}



