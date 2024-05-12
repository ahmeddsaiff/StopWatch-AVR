#ifndef DECLARATIONS_H_
#define DECLARATIONS_H_

#define u8 unsigned char

//three global variables of type unsigned char for time
u8 SEC = 0;
u8 MIN = 0;
u8 HR = 0;

u8 flag = 0;
u8 Buzzertick = 0;

//used this text replacement as the code will be replaced down in the while(1) in main once
//this is better than the function --->>>> due to no context switching
#define Display_7Segment() {\
	\
	/*enable & display first 7-seg*/ \
	PORTA = 0x01;\
	PORTC = (PORTC & 0xF0) | (SEC%10);\
	_delay_ms(2);\
	\
	/*enable & display second 7-seg*/ \
	PORTA = 0x02;\
	PORTC = (PORTC & 0xF0) | (SEC/10);\
	_delay_ms(2);\
	\
	/*enable & display third 7-seg*/ \
	PORTA = 0x04;\
	PORTC = (PORTC & 0xF0) | (MIN%10);\
	_delay_ms(2);\
	\
	/*enable & display fourth 7-seg*/ \
	PORTA = 0x08;\
	PORTC = (PORTC & 0xF0) | (MIN/10);\
	_delay_ms(2);\
	\
	/*enable & display fifth 7-seg*/ \
	PORTA = 0x10;\
	PORTC = (PORTC & 0xF0) | (HR%10);\
	_delay_ms(2);\
	\
	/*enable & display sixth 7-seg*/ \
	PORTA = 0x20;\
	PORTC = (PORTC & 0xF0) | (HR/10); \
	_delay_ms(2); \
}

//buzzer will make a sound every time you press on any button (RESET,PAUSE,RESUME)
#define BUZZERSOUND() {\
	PORTB |= (1<<PB5);\
}

#define BUZZEROFF() {\
	PORTB &= ~(1<<PB5);\
}

void TIMER1_Init(void);
void INT0_Init(void);
void INT1_Init(void);
void INT2_Init(void);



#endif /* DECLARATIONS_H_ */
