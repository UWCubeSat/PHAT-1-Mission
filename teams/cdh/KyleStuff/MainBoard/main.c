#include <msp430.h> 


/**
 * main.c
 */
int main(void){
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= (0x01|0x02);
	PM5CTL0 &= ~LOCKLPM5;

	//initUART();

	//9600 8N1
    UCA3CTLW0 |= UCSWRST;//has to be set to initialize board
	//sets DCO to 1mHz
	CSCTL1 |= DCOFSEL_0;
	//sets SMCLK to use DCO
	CSCTL2 |= SELS_3;
	//set ports for UART
	P6SEL1 &= ~(BIT0 | BIT1);
	P6SEL0 |= (BIT0 | BIT1);
	//set clock source to SMCLK
	UCA3CTLW0 |= UCSSEL_2;
	//set baud rate using table from data sheet
	UCA3BR0 = 104;
	UCA3BR1 = 0;
	UCA3MCTLW = UCBRS1;
	//enables interrupts
	UCA3IE |= (UCTXIE|UCRXIE);

	UCA3STATW |=  UCLISTEN; //Loopback mode

	//enable the USCI peripheral
	UCA3CTLW0 &= ~UCSWRST;

	UCA0TXBUF = 'A';

	/*while(1){
	    putChar(100);
	    if(UCA3STATW & UCBUSY){
	        P1OUT |= (0x01);
	    }
	    P1OUT &= ~(0x01);
	}*/

	return 0;
}
/*
void initUART(void){
    //9600 8N1
    UCA3CTLW0 |= UCSWRST;//has to be set to initialize board
    //sets DCO to 1mHz
    CSCTL1 |= DCOFSEL_0;
    //sets SMCLK to use DCO
    CSCTL2 |= SELS_3;
    //set ports for UART
    P6SEL1 &= ~(BIT0 | BIT1);
    P6SEL0 |= (BIT0 | BIT1);
    //set clock source to SMCLK
    UCA3CTLW0 |= UCSSEL_2;
    //set baud rate using table from data sheet
    UCA3BR0 = 104;
    UCA3BR1 = 0;
    UCA3MCTLW = UCBRS1;
    //enables interrupts
    UCA3IE |= (UCTXIE|UCRXIE);

    UCA3STATW |=  UCLISTEN; //Loopback mode

    //enable the USCI peripheral
    UCA3CTLW0 &= ~UCSWRST;



}

int getChar(void){
    int chr = -1;

    if(UCA3IFG & UCRXIFG){
        chr = UCRXBUF3;
    }
    return chr;
}

void putChar(int c){
    //wait for the buffer to be ready
    while(!(UCA3IFG & UCTXIFG));

    //transmit data
    UCA3TXBUF = (char)c;
}*/

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = EUSCI_A3_VECTOR             // eUSCI ISR
__interrupt void USCI_A3_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(EUSCI_A3_VECTOR))) USCI_A3_ISR (void)
#else
#error Compiler not supported!
#endif
{
   switch(__even_in_range(UCA3IV,18)) {
      case 0x00:
         //P1OUT &= ~(0x01|0x02);
         break; // Vector 0: No interrupts
      case 0x02:
         P1OUT ^= (0x01);
         UCA3IFG &= ~UCRXIFG;
         break; // Vector 2: UCRXIFG
      case 0x04:
          P1OUT ^= (0x02);
         __delay_cycles(1000000);
         P1OUT &= ~(0x02);
         break; // Vector 4: UCTXIFG
      default:
         break;
   }
}


