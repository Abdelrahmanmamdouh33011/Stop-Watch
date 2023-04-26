#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
// (display)  macro for display all 7 segment
#define display for (counter = 0; counter < 7; counter++)\
					{\
					PORTA = (1 << counter);\
					PORTC = number[counter];\
					_delay_ms(2);\
					}
// This array of counter to give the PORTC what should it display
int count[6] = {0};
// This array of number That connected with PORTC,each element is connected to one bit in PORTC
int number[6] = {0};
// set interrupt 0 with a falling edge
void set_int0();
// set interrupt 1 with a raising edge
void set_int1();
// set interrupt 2 with a falling edge
void set_int2();
// set timer 1 with ctc mode and prescaler 1024
void set_timer1();
// function to initialize the variables to making easy to work & Reduce main code
void initialization();


int main(void) {

	// this counter for macro to display
	int counter = 0;
	// calling init function
	initialization();

	for(;;)
	{



		for (count[5] = 0; count[5] < 10; count[5]++) // sixth loop for Hours
		{
			number[5] = count[5];
			for (count[4] = 0; count[4] < 10; count[4]++) // fifth loop for Hours
			{
				number[4] = count[4];
				for (count[3] = 0; count[3] < 6; count[3]++) // fourth loop for Hours tens
				{
					number[3] = count[3];
					for (count[2] = 0; count[2] < 10; count[2]++) // third loop for Hours Units
					{
						number[2] = count[2];
						for (count[1] = 0; count[1] < 6; count[1]++) // second loop for MIN tens
						{
							number[1] = count[1];
							for (count[0] = 0; count[0] < 10;) // first loop for MIN Units
							{
								number[0] = count[0];
								display
							}
						}
					}
				}
			}
		}
		//Program explanation
		/*
		 In These six loops , each loop control one of the 6 seven segment,

         for example , The last loop for first digit in the stop watch 00:00:0x

         The second loop for the second digit in the stop watch 00:00:y0
         And so it is in the rest.

         *The method of work
         In the normal case and starting from 0 ,

         All conditions is true and there we are in the first loop ,

         The program will be stuck in the first loop Until Interrupt happened and
         Counter[0] incremented in the Timer1

         therefore the program will be stuck in the first loop for 10 Seconds .
         for Counter[0] = 10

         Program will exit from the first loop to the second loop to increment 1
         and return stuck in the first loop
         And so it is in the rest.

		 */




	}

	return 0;
}



void initialization()
{
	DDRC |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
	DDRA |= 0X3F;
	DDRD &= ~(1 << 2) & ~(1 << 3);
	DDRB &= ~(1 << 2);
	PORTC = 0;
	PORTD |= (1<<2);
	PORTB |= (1<<2);
	set_timer1();
	set_int0();
	set_int1();
	set_int2();
}

void set_timer1()
{

	 // Set the prescaler value to 1024
	    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
	    // Set the mode of operation to CTC
	    TCCR1A &= ~(1 << WGM10);
	    TCCR1B |= (1 << WGM12) | (0 << WGM11);

	    // Set the compare match value to 800
	    OCR1A = 800; //  ~= 1 sec
	    // Enable Timer1 compare match interrupt
	    TIMSK |= (1 << OCIE1A);

	    // Enable global interrupts
	    sei();
}

// set interrupt 0 with a falling edge
void set_int0()
{
    MCUCSR |= (1<<ISC01);
    GICR |= (1<<INT0);
}

// set interrupt 1 with a raising edge
void set_int1()
{
	MCUCR |= (1<<ISC11) | (1<<ISC10);
	GICR |= (1<<INT1);
}

// set interrupt 2 with a falling edge
void set_int2()
{
	MCUCSR &= ~(1<<ISC2);
	GICR |= (1<<INT2);
}


ISR(TIMER1_COMPA_vect)
{
	count[0]++; // increment every 1 second
}

ISR(INT0_vect)
{
	// If INT0 happened ... reset to 0
    int k;
    for(k=0; k<6; k++)
    {
    	number[k] = 0;
    	count[k]  = 0;
    }
}

ISR(INT1_vect)
{
	// if INT1 happened we will stop the timer
	TIMSK &= ~(1 << OCIE1A);
}

ISR(INT2_vect)
{
	//If INT2 happened The timer will resume
	TIMSK |= (1 << OCIE1A);
}










