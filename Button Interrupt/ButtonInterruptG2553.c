/**
 * main.c buttonblink 2: electric boogaloo
 */
int main()
{
//configure
WDTCTL = WDTPW | WDTHOLD; //turns off watchdog
P1SEL &= ~(BIT0 | BIT3); //sets bits 0 3 as gpio
P1SEL2 &= ~(BIT0 | BIT3); //
P1DIR |= (BIT0); // sets 0 as outputs
P1DIR &= (~BIT3); //sets 3 as input
P1IE |= BIT3;//enable button interrupts
P1IES |= BIT3;//set interrupt to flag on falling edge

__bis_SR_register(GIE);

while(1)
{
}
return 0;
}
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_vector(void)//
{
    if (P1IES & BIT3) //checks for button press
    {
        P1OUT ^= BIT0; //flips LED
    }
    P1IFG &= ~BIT3; //turn off the flag
}

