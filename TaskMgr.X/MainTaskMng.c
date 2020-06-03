/*
 * Principal.c
 * Autor: Pedro Bertoleti
 * Data: Jan/2016
 * Description: 
 * This software is a task manager for 8-bit PIC microcontrollers.
 * WARNING: be careful when programming the tasks. Tasks must be as fast as possible (due to real-time features of this task manager)
 */

#include <xc.h>
#include "taskManager.h"

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config FOSC = XT 
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

//General defines
#define _XTAL_FREQ                 4000000      //clock frequency: 4MHz

#define MACHINE_CYCLE              (4/_XTAL_FREQ)   //In 8-bit PICs (at least, in 16F series ones), one instruction is executed only after 4 machine/clock cycles.
#define YES                        1
#define NO                         0
#define TIMER0_INIT_VALUE          131

/*
 *  Main function
 */
void main(void) 
{
	/*
     * example of input and output configuration. It can be changed according to what you need
     */
    TRISB = 0x00;            //configures all bits of PORTB as outputs
    PORTB = 0;               //sets all portb´s outputs to 0    
    TRISA = 0xFF;            //configures all bits of PORTA as inputs
    
    //init tasks and config Timer0
    TskMgrInit();   
	
    //init timer0 and set TskMgrtickCallback
    //ConfigTimer0(TskMgrtickCallback);
    
	//main loop 
    while(1)                      
    {
        //Verification: check if there´s a task to be executed
        if (TskMgrGetTickEvent()) 
        {
            TskMgrClearTickEvent();  
            TskMgrExecuteTask();	
        }
    }
}
