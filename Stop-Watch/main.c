/*
 * main.c
 *
 *  Created on: Jan 28, 2024
 *      Author: AbdElRahman Khalifa
 */

#include<avr/io.h>
#include<avr/interrupt.h>
#include"StandardTypes.h"

#define INIT(NAME) NAME##_init()

int sec = 0, min = 0, hour = 0; /* global variables for time each one controll two seven segmen */

void Timer1_CTC_Init(void);
void INT0_init(void);
void INT1_init(void);
void INT2_init(void);
void Delay_ms(int);

int main(void){

	DDRC|=(0x0f);			/* configure first 4 pins as output for decoder */

	DDRA|=(0x3f);			/* configure first 6 pins as output for controlling 6-7-segment */

	SREG |=(1<<7);			/* enable global interrupt */

	Timer1_CTC_Init();

	DDRD&=~((1<<PD2) | (1<<PD3));	/* configure INT0 and INT1 as input */
	DDRB&=~(1<<PB2);				/* configure INT2 as input */

	PORTD |=(1<<PD2);      			/* configure PD2 as internal pull-up resistor */
	PORTB |=(1<<PB2);				/* configure PB2 as internal pull-up resistor */

	INIT(INT0);
	INIT(INT1);
	INIT(INT2);


	while(1){
		PORTA=(PORTA&~(0x3f))|(1<<PA0); 		/* enable first segment only */
		PORTC=(PORTC&~(0x0f))|(sec%10); 		/* display first digit of seconds */
		Delay_ms(4);

		PORTA=(PORTA&~(0x3f))|(1<<PA1); 		/* enable second segment only */
		PORTC=(PORTC&~(0x0f))|((sec/10)%10); 	/* display second digit of seconds */
		Delay_ms(4);

		PORTA=(PORTA&~(0x3f))|(1<<PA2); 		/* enable third segment only */
		PORTC=(PORTC&~(0x0f))|(min%10); 		/* display third digit of seconds */
		Delay_ms(4);

		PORTA=(PORTA&~(0x3f))|(1<<PA3); 		/* enable 4th segment only */
		PORTC=(PORTC&~(0x0f))|((min/10)%10); 	/* display 4th digit of seconds */
		Delay_ms(4);

		PORTA=(PORTA&~(0x3f))|(1<<PA4); 		/* enable 5th segment only */
		PORTC=(PORTC&~(0x0f))|(hour%10); 		/* display 5th digit of seconds */
		Delay_ms(4);

		PORTA=(PORTA&~(0x3f))|(1<<PA5); 		/* enable 6th segment only */
		PORTC=(PORTC&~(0x0f))|((hour/10)%10); 	/* display 6th digit of seconds */
		Delay_ms(4);
	}

}

void Timer1_CTC_Init(void){
	TCCR1A=(1<<FOC1A);				/* non_PWM For A unit */
	TCCR1B=(1<<WGM12)|(1<<CS12);	/* CTC mode, prescaler 256*/
	TIMSK=(1<<OCIE1A);				/* enable interrupt for CTC in A unit */
	OCR1A=3906; 					/* 1MHz/256 -> 1 sec */
}

void INT0_init(void){
	/* configure interrupt with falling edge */
	MCUCR|=(1<<ISC01);
	MCUCR&=~(1<<ISC00);

	GICR|=(1<<INT0); /* enable interrupt for INT0 */
}

void INT1_init(void){
	/* configure interrupt with rising edge */
	MCUCR|=(1<<ISC11)|(1<<ISC10);

	GICR|=(1<<INT1); /* enable interrupt for INT0 */
}

void INT2_init(void){
	/* configure interrupt with falling edge */
	MCUCSR&=~(1<<ISC2);

	GICR|=(1<<INT2); /* enable interrupt for INT0 */
}

void Delay_ms(int time){
	int i;
	TCCR0=(1<<FOC0)|(1<<WGM01)|(1<<CS01)|(1<<CS00); /* non PWM, CTC, 64 prescaler */
	OCR0=16;										/* ((10^6)*(2*10^-3)) / 64  = 32 */
	for(i=0;i<time;i++){							/* loop every iteration takes 1ms */
		TCNT0=0;									/* reset counter */
		while(!(TIFR&(1<<OCF0)));					/* polling */
		TIFR|=(1<<OCF0);							/* clear the flag */
	}

	TCCR0=0;										/* stop timer0 */
}

ISR(TIMER1_COMPA_vect){
	/* algorithm for calculating time */
	sec++;
	if(sec==60){
		sec=0;
		min++;
	}
	if(min==60){
		min=0;
		hour++;
	}
}

ISR(INT0_vect){
	sec=min=hour=0;		/* reset stop watch */
}

ISR(INT1_vect){
	TCCR1B&=~(1<<CS12); /* pause the timer by removeing the clock */
}

ISR(INT2_vect){
	TCCR1B|=(1<<CS12); 	/* resumed the timer by return the clock */
}

