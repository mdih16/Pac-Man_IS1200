#include <pic32mx.h>
#include <stdint.h>

// Initialize the timer
void TMR2_init()
{
    T2CON = 0x8070; 
    TMR2 = 0x0;
    PR2 = 20000; 

    IEC(0) |= 0x100; // Enable TMR2 interrupts
    IPC(2) |= 0x1c;  // Priority level 7
    enable_interrupt();
}

// Initizalize the buttons
void BTN_init()
{
    // Set BTN1-4 as input
    TRISFSET = 0x1;     
    TRISDSET = 0xE0;    
}

// Initizalize the LEDs
void LED_init()
{
    // Set LED1-7 as output
    TRISECLR = 0xff;
}

// Initizalize the switches
void SW_init()
{
	IEC(0) |= 0x80;
	IPC(1) |= 0x1f000000; 
    IFS(0) &= ~0x80;

	IEC(0) |= 0x800;
	IPC(2) |= 0x1f000000; 
    IFS(0) &= ~0x800;

	IEC(0) |= 0x8000;
	IPC(3) |= 0x1f000000; 
    IFS(0) &= ~0x8000;
}

// Disable switches
void SW_disable()
{
    IEC(0) &= ~0x80;
    IEC(0) &= ~0x800;
    IEC(0) &= ~0x8000;
}

// Return which buttons are pressed
uint8_t BTN_check()
{
    return ((PORTD >> 0x4) & 0xE)| ((PORTF >> 1) & 0x1);
}