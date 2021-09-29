#include <msp430.h>

void delay(int n){
    volatile int i;
    for(i = 0; i<n; i++ );
}

void uart_init(){
    P3SEL |= BIT3+BIT4;                       // P3.3,4 = USCI_A0 TXD/RXD
    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 109;                            // 1MHz 9600
    UCA0BR1 = 0;                              // 1MHz 9600
    UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

void led_init(){
    P1REN |= 0x02;                           // Enable resistor on P1.1
    P2REN |= 0x02;                           // Enable resistor on P2.1
    P1OUT  = 0x02;                           // Set resistor to pull-up
    P2OUT  = 0x02;                           // Set resistor to pull-up
    P1DIR  = 0x01;                           // Set P1.0 as output
    P4DIR  = 0x80;                           // Set P4.7 as output
//   P4OUT  = 0x80;                           // Set P4.7 high
}

void UARTSendInt(int Txnum){
    while (!(UCA0IFG&UCTXIFG));             // Wait for TX buffer to be ready for new data
    UCA0TXBUF = Txnum;                      // Write the character at the location specified py the pointer
 }

void main( void ){
WDTCTL = WDTPW | WDTHOLD;                   // Stop watchdog timer
led_init();                                 // Initialize led settings
uart_init();                                // Initialize uart settings
while( 1 ){
    if( !(P1IN & 0x02) ){                   // If button 1 is pressed
        P4OUT ^= 0x80;
        UARTSendInt(1);
        delay(30000);
    }
    if(!(P2IN & 0x02)){                     // If button 2 is pressed
        P1OUT ^= 0x01;
        UARTSendInt(2);
        delay(30000);
    }
  }
}

// int main(void){
//   WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

//   P3SEL |= BIT3+BIT4;                       // P3.3,4 = USCI_A0 TXD/RXD
//   UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
//   UCA0CTL1 |= UCSSEL_2;                     // SMCLK
//   UCA0BR0 = 9;                               // 1MHz 115200 (see User's Guide)
//   UCA0BR1 = 0;                              // 1MHz 115200
//   UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
//   UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//   UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

//   __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
//   __no_operation();                         // For debugger
// }

// // Echo back RXed character, confirm TX buffer is ready first
// #if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
// #pragma vector=USCI_A0_VECTOR
// __interrupt void USCI_A0_ISR(void)
// #elif defined(__GNUC__)
// void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
// #else
// #error Compiler not supported!
// #endif
// {
//   switch(__even_in_range(UCA0IV,4))
//   {
//   case 0:break;                             // Vector 0 - no interrupt
//   case 2:                                   // Vector 2 - RXIFG
//     while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
//     UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
//     break;
//   case 4:break;                             // Vector 4 - TXIFG
//   default: break;
//   }
// }