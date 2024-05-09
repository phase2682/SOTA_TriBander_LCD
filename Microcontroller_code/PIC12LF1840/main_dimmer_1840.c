/* 
 * File:   main_dimmer_1840.c
 * Author: Dan Koellen AI6XG
 *
 * Created on December 21, 2023, 11:01 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "configBits.h"

#define LCD_on  RA2                     //output to LCD SSR
#define Dim_1   RA5                     //bit 1 dim level input
#define Dim_0   RA4                     //bit 0 dim level input

unsigned char 	dim = 0;            	//dim level

unsigned char   a_25 = 0;               //25% dim level element
unsigned char   a_50 = 0;               //50% dim level element



//declare function
__bit backlight_dim(unsigned char dim);




void main(void) {
    
    
    
   //options, no WPU, timer or prescaler
    OPTION_REG = 0b00000000;
    
    //Interrupts disabled
    INTCON = 0b00000000;
    
    //Internal Oscillator setup
   OSCCON = 0b01101000;         //4 MHz  
    
    //Disable Reference Clock Module
   CLKRCONbits.CLKREN = 0;
   
   //Disable DAC
   DACCON0bits.DACEN = 0;
   
   //Disable SR Latch
   SRCON0bits.SRLEN = 0;
   
   //Disable Comparator
   CM1CON0bits.C1ON = 0;
   
   //Disable Timer1 and Timer2
   T1CONbits.TMR1ON = 0;
   T2CONbits.TMR2ON = 0;
   
   //Disable Modulation
   MDCONbits.MDEN = 0;
   
   //Disable MSSP and EUSART
   SSP1CON1bits.SSPEN = 0;
   RCSTAbits.SPEN = 0;
   
   //Disable Capacitive Sensing
   CPSCON0bits.CPSON = 0;
   
    //PORTA setup
    TRISA = 0b00110000;     //PORTA all outputs except RA4 RA5
    PORTA = 0;              //PORTA cleared
    ANSELA = 0b00000000;    //all digital
    
    
    while (1 == 1)
    {
        dim = (Dim_1 & 0b1)<<1 | (Dim_0 & 0b1);
        //dim = 0;
        
       LCD_on = backlight_dim(dim); 
        
        
        
    }           //end while()
}       //end main()

//control the LCD backlight brightness


/**********************************************************************************************
        Function Name:backlight dim
 *  	
        Return Value:	state for LCD backlight control IO
        Parameters:		dim level 0 - 5 is 0%, 33%, 50%, 100%, 50%, 33%
 *                       
 *                      
        Description:	returns state for LCD_on IO depending on dim level
 *                      0 - 5 is 0%, 25%, 50%, 100%, 50%, 25%
	
 ************************************************************************************************/
__bit backlight_dim(unsigned char dim) 

{

           static __bit LCD;
            

            
        //LCD backlight off
            if (dim == 0)
            {
                LCD = 0;
            }
        
        //LCD backlight 33%
            else if (dim == 1)
            {                
                a_25++;
                
                if (a_25 < 45) LCD = 1;
                else if ((a_25 >= 45) && (a_25 < 180)) LCD = 0;
                else 
                {
                    a_25 = 0;
                    LCD = 0;
                }
                
                
            }
        
        //LCD backlight 50%
            else if (dim == 2)
            {               
                a_50++;
                
                if (a_50 < 90) LCD = 1;
                else if ((a_50 >= 90) && (a_50 < 180)) LCD = 0;
                else 
                {
                    a_50 = 0;
                    LCD = 0;
                }
                
            }
        
                
        //LCD backlight full on
            else if (dim == 3)
            {
                LCD = 1;
            }
        
            else
            {
                 LCD = 0;
            }
            return LCD;
            
            } //end of function