#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
        PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                                // to activate previously configured port settings
        P1DIR |= BIT0;                          // Set P1.0 to output direction

        P1DIR |= BIT1;


        for(;;) {
            volatile unsigned int i;            // volatile to prevent optimization
            volatile unsigned int b;

            P1OUT ^= BIT1;
            P1OUT ^= BIT0;// Toggle P1.0 using exclusive-OR

            i = 10000;                          // SW Delay
            do i--;
            while(i != 0);

    //        P1OUT ^= 0x01;

    //        i = 20000;
    //        do i--;
    //        while (i!=0);

        }
	
	return 0;
}
