#include <msp430.h> 
#include <stdint.h>

// TODO:  Support '#define USCI_MODULE  B2' or similar
#include "../sensors/magnetometer.h"


#define I2C_PORTSEL0        P7SEL0
#define I2C_PORTSEL1        P7SEL1
#define I2C_SDA_BIT         BIT0
#define I2C_SCL_BIT         BIT1

#define LED_PORT            P1OUT
#define LED_PORT_DIR        P1DIR
#define LED_PIN             BIT0

MagnetometerData *pReceivedMagData;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    // Configure GPIO for status LED
    LED_PORT &= ~LED_PIN;                               // Clear P1.0 output latch
    LED_PORT_DIR |= LED_PIN;                            // For LED

    // Configure I2C pins - Goal:  '01' for both 7.0 (SDA) and 7.1 (SCL)
    // NOTE:  P7DIR not necessary, direction set by eUSCI module
    I2C_PORTSEL1 &= ~(I2C_SDA_BIT | I2C_SCL_BIT);
    I2C_PORTSEL0 |= I2C_SDA_BIT | I2C_SCL_BIT;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    magInit();

    // Main loop - repeatedly write the buffer
    for (;;)
    {
        P1OUT ^= BIT0;
        pReceivedMagData = magReadXYZData(ConvertToTeslas);
        __delay_cycles(10000);  // Delay just to see LED flash - not necessary for communication timing

        //__bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/ interrupts

    }
}
