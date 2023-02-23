#include <pic32mx.h>
#include <stdint.h>

void TMR2_init()
{
    T2CON = 0x8070; 
    TMR2 = 0x0;
    PR2 = 20000; 

    IEC(0) |= 0x100; // Enable TMR2 interrupts
    IPC(2) |= 0x1c;  // Priority level 7
    enable_interrupt();
}

void BTN_init()
{
    // Set BTN1-4 as input
    TRISFSET = 0x1;     
    TRISDSET = 0xE0;    
}

void LED_init()
{
    // Set LED1-7 as output
    TRISECLR = 0xff;
}

uint8_t BTN_check()
{
    return ((PORTD >> 0x4) & 0xE)| ((PORTF >> 1) & 0x1);
}