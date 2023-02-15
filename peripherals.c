#include <pic32mx.h>

void TMR2_init()
{
    T2CON = 0x8070; 
    TMR2 = 0x0;
    PR2 = 9765; 

    IEC(0) |= 0x100; // Enable TMR2 to cause an interrupt
    IPC(2) |= 0x1c;  // Priority level 7
    //enable_interrupt();
}