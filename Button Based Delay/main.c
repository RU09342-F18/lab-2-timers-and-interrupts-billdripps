#include <msp430.h> 


/**
 * main.c
 *
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	BCSCTL3 = LFXT1S_2;//use internal crystal
	P1SEL = 0; //port 1 is i/o
	//P1DIR &= ~BIT0; //set p1.0 to output (to LED)
	P1DIR|= BIT0;
	//set up timer
	TACTL = TASSEL_2|ID_3|MC_2|TAIE; //sets up the timer to the 1 MHz clock with a divide by 8 in continuous mode

	//set up interrupt condition
	TACCR1 = 0x2000;
	TACCTL1 = CCIE; //capture.compare interrupt enabled.

	//enter low power mode 0 + enable global interrupt
	__bis_SR_register(GIE);

	while(1);


//	return 0;
}


//interrupt service routine
#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_vector(void)//
{
     //switch (TAIV) standard switch function
     switch(TA0IV)
     {
          case 2: //vector2 for ccifg1
          P1OUT &= ~BIT0;
          //p1.0 = 0;
          break;
          case 10: //vector A
          P1OUT |= BIT0;
          break;
          default:
          for(;;){} //false interrupt, disaster loop
          //endswitch
     }
}

*/
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    BCSCTL3 = LFXT1S_2;//use internal crystal
    P1SEL = 0; //port 1 is i/o
    P1DIR |= BIT0; //set p1.0 to output (to LED)
    P1DIR &= ~BIT3; //set p1.3 to input (from switch)
    P1IE |= BIT3; //enable interrupts on p1.3
    P1IES |= BIT3; //set interrupt to flag on a falling edge
    P1REN |= BIT3; //enable pull resistor (apparently unnecessary)

    //set up timer
    TACTL = TASSEL_1|ID_3|MC_1; //sets up the timer to the 32 kHz clock with a divide by 8 in up mode

    //initialize count up to value
    TACCR0 = 0x00FF; //default blinks fairly quickly

    __bis_SR_register(GIE); //enable interrupts

    while(1)
    {
        if (TA0R == TACCR0) // if the clock output matches the count to number, flip the led state
            P1OUT ^= BIT0;
    }

}

//interrupt service routine
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_vector(void)//
{
     //check button press and release triggers
     if(P1IES & BIT3) //falling edge trigger (button press)
     {
          P1IES &= ~BIT3; //set interrupt to flag on a rising edge
          TACTL = TASSEL_1|ID_3|MC_2|TACLR; //sets up the timer to the 32 kHz clock with a divide by 8 in continuous mode and clears
     }

     else // rising edge trigger (button release)
     {
         TACTL = MC_0; //halts clock
         P1IES |= BIT3; //set interrupt to flag on a falling edge
         TACCR0 = TA0R; //sets count to current clock count
         TACTL = TASSEL_1|ID_3|MC_1; //sets up the timer to the 32 kHz clock with a divide by 8 in up mode
     }
     P1IFG &= ~BIT3; //turn off the flag
}

