#include <msp430g2553.h>

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;               // Disable the Watchdog timer
    BCSCTL3 = LFXT1S_2;//use internal crystal
    P1DIR |= BIT0 | BIT6;

    //timer setup
    TA0CTL = TASSEL_1 | MC_1; //sets up the timer to the 32kHz clock in Continuous mode
    TA1CTL = TASSEL_1 | MC_1; //sets up the timer to the 32kHz clock in Continuous mode

    //sets PWM modes and enable interrupts
    TA0CCTL0 = CCIE; //enable interrupts
    TA1CCTL0 = CCIE; //enable interrupts

    //sets counts for red and greed LEDs
    TA0CCR0 = 0x0F0F;
    TA1CCR0 = 0xF0F0;

    __bis_SR_register(GIE);       // global interrupts enabled
    while(1);
}


// Timer 0 A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_VECTOR( void )
{
    P1OUT ^= BIT0; //flip green LED



}
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_VECTOR( void )
{
    P1OUT ^= BIT6; //flip red LED



}
