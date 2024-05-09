/* 
 * File:   main_TQFP.c
 * Author: Dan Koellen AI6XG
 * 
 *decode 4 digit LED
 * write values on LCD
 * add offset to value for correct Frequency
 * also decode RIT, tune, WPM
 * send Dim values to dim ucontroller
 * this version is for v1.0 of the PCB
 * using a TQFP PIC16F887
 * New pin assignments with PCB layout and package change
 * from LCD_Tribander project that was a prototype with a PDIP
 * 
 * Pin configurations in the header files will also need to be changed
 * header files names appended with _TQFP and refactored to isolate from 
 * version used for PDIP prototype
 * 
 * Also eliminate commented alternatives
 *
 * Created on January 24, 2024 6:18 PM
 * version 0.0
 * * revised for TQFP on January 24, 2024
 * version 1.0
 * 
 * version 1.1 fix blank in RIT and WPM
 * Feb 2024
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    "configBits_tqfp.h"
#include    <xc.h>
#include    "SI_LCD_dk_TQFP.h"
#include    "Binary_to_BCD.h"
#include    "segment_to_LCD_TQFP.h"
#include    "band_select_TQFP.h"




//#define _XTAL_FREQ  4000000     //For 4 MHz Oscillator
#define _XTAL_FREQ  8000000     //For 8 MHz Oscillator

//(frequency offset)/100Hz
//e.g. for an offset of 5300Hz, use 53
//negative offset for corrected frequency less than initial frequency
//actual frequency - display frequency
int offset_40 = -1;             //offset for 40 meters
int offset_20 = -1;             //offset for 20 meters
int offset_15 = -2;             //offset for 15 meters
int offset  = 0;                //offset for selected band




//definitions for mSec timer
#define timer_reset_value 	15	//reset value for timer0



unsigned int 	timer0_events = 0;			//number of timer0 overflows
unsigned int 	previous_timer0_event =0xff;	//previous timer0_events


//dim level definitions
#define dim_set     RA6                 //input pin for setting LCD backlight level
#define dim_0       RA4                 //output dim level bit 0
#define dim_1       RA5                 //outpur dim level bit 1


unsigned char 	dim = 0;            	//dim level
unsigned char   previous_dim = 0;       //previous dim level
unsigned int 	state_dim = 0;			//debounce dim state holder





//declare functions

void freq_offset(unsigned char dgt_1, unsigned char dgt_2, unsigned char dgt_3,
        unsigned char dgt_4, int f_off);



/*
 * 
 */
void main(void) {

PORTA = PORTB = PORTC= 0;		//Clear Ports A,B and C
PORTD = PORTE= 0;               //Clear Ports D and E
TRISA = 0b11000000;             //All PortA outputs except RA7 RA6
TRISB = 0b00000000; 			//All PortB outputs
TRISC = 0xFF; 					//All PortC inputs
TRISD = 0b00001111;             //All PortD outputs except RD0 RD1 RD2 RD3
TRISE = 0b00000000; 			//All PortE outputs

CM1CON0 = 0;                    //Comparator Off
CM2CON0 = 0;                    //Comparator Off

CCP1CON = 0;                    //ECCP off
CCP2CON = 0;                    //ECCP off


ANSEL = 0b00000000;				//All digital 
ANSELH = 0b00000000;            //ADC disabled

//Interrupt setup
GIE = 1;						//global interrupt enable
INTF = 0;						//clear the external interrupt flag
INTE = 0;						//disable0/enable1 external interrupt on RB0
IOCB = 0;                       //disable port B IOC

//Internal Oscillator setup
//OSCCONbits.IRCF = 0B110;         //4 MHz Internal Oscillator 
OSCCONbits.IRCF = 0B111;       //8 MHz Internal Oscillator 

//setup Timer0
OPTION_REGbits.PSA = 0;			//prescaler for T0
OPTION_REGbits.PS = 0b000;      //prescaler set to 1:2
//OPTION_REGbits.PS = 0b001;      //prescaler set to 1:4
//OPTION_REGbits.PS = 0b010;      //prescaler set to 1:8
T0CS = 0;						//Fosc/4 timer
T0IE = 1;                       //T0 interrupt enabled 



if(band_set) two_line = 1;      //set two line flag if band set is pressed
                                //at power on

RST = 1;                    //RST must high before going into initialization
                            //must stay high after initialization
lcd_initialize ();
__delay_ms(50);

//reset relays to band = 0 --> 40 meters
 R12r = 1;                   //reset relays 1 and 2
 R34r = 1;                   //reset relays 3 and 4
 __delay_ms(3);              //3 mSec pulse
  R12r = 0;                   //reset relays 1 and 2
  R34r = 0;                   //reset relays 3 and 4
 

//Initial two line Screen for 3 seconds
lcd_command(0x80);
lcd_puts(" SOTA TriBander ");

lcd_goto_two(0);
lcd_puts(" Hacked by AI6XG ");

__delay_ms(3000);

//second screen, one line - double height font
lcd_clear();
lcd_command(0x35);          //single line double font
lcd_puts(" www.AI6XG.com");
__delay_ms(3000);
lcd_clear();

//switch to two line mode if requested at power up
if (two_line) lcd_command(0x39);          //two line single font


while(1==1){
    
    //millisecond tasks -> pre-scaler 1:8
    //500 uSec tasks -> pre-scaler 1:4
    //250 uSec tasks -> pre-scaler 1:2
    if(timer0_events != previous_timer0_event)	// every tmr0 interrupt        
        {
        
	previous_timer0_event = timer0_events;
    
    //de-bounce  band_set -> increment band for each press
        state_band = ((state_band << 1) | (!band_set) | 0xE000);	//must be clear after 12 loops
		if (state_band == 0xF000)           //true if band_set set
			{
			band++;							//increment band to next band
			if (band > 2) band = 0;         //go back to initial band
            
            }
        
       //de-bounce  dim_set -> increment LCD back light for each press
        state_dim = ((state_dim << 1) | (!dim_set) | 0xE000);	//must be clear after 12 loops
		if (state_dim == 0xF000)           //true if dim_set set
			{
			dim++;							//increment dim to next dim level
			if (dim > 5) dim = 0;         //go back initial dim level
            
            }

            
//set the band at resistor selection and set or reset relays
    if (band != previous_band)              //test if the band has changed
    {
        if (band == 0) 					//40 meters
        {
            _15_mtr = 0;                //band set to 15 meter set resistor
            _20_mtr = 0;                //band set to 20 meter set resistor

            R12r = 1;                   //reset relays 1 and 2
            R34r = 1;                   //reset relays 3 and 4
            R12r_time = R34r_time = timer0_events;
                                        //time relay pulse starts

        }				//end 40 meters if

        if (band == 1)					//20 meters	
            {

            _15_mtr = 0;                //band set to 15 meter set resistor
            _20_mtr = 1;                //band set to 20 meter set resistor

            R12r = 1;                   //reset relays 1 and 2
            R34s = 1;                   //set relays 3 and 4
            R12r_time = R34s_time = timer0_events;
                                        //time relay pulse starts

            }           //end 20 meters if

        if (band  == 2)					//15 meters
            {

            _15_mtr = 1;                //band set to 15 meter set resistor
            _20_mtr = 0;                //band set to 20 meter set resistor

            R12s = 1;                   //set relays 1 and 2
     
            R12s_time = timer0_events;   //time relay pulse starts


            }			//end 15 meters if
        previous_band = band;
        }            //end previous_band if
        
        
 //set dim level and output to LCD backlight controller
    if (dim != previous_dim)        //test if dim level has changed
        {
         if (dim == 0)                  //LCD backlight off
         {
             dim_0 = 0;
             dim_1 = 0;
         }               //end 0% if
         
         if (dim == 1 || dim == 5)       //LCD backlight 25%
         {
             dim_0 = 1;
             dim_1 = 0;
         }                //end 25% if
         
         if (dim == 2 || dim == 4)       //LCD backlight 50%
         {
             dim_0 = 0;
             dim_1 = 1;
         }               //end 50% if
         
         if (dim == 3)                  //LCD backlight 100%
         {
             dim_0 = 1;
             dim_1 = 1;
         }               //end 100% if
         previous_dim = dim;
        }               //end previous_dim if
        
       }        //end if timer0_events
        
        
 //clear relay pulse after relay_pulse_dur duration
 R12r &= DurationClear(R12r_time, relay_pulse_dur, 0xffff, timer0_events ) ;
 R34r &= DurationClear(R34r_time, relay_pulse_dur, 0xffff, timer0_events ) ;
 R12s &= DurationClear(R12s_time, relay_pulse_dur, 0xffff, timer0_events ) ;
 R34s &= DurationClear(R34s_time, relay_pulse_dur, 0xffff, timer0_events ) ;
 

 
 
 

 //Decode segments by digit position
        //segment decode for rightmost digit 4
        if(D4)                          //Digit 4 active high
        {        
            
            d_4 = PORTC;
            
            if(d_4 != d_4_old)          //did the digit change?
                
            {            
            d_4_old = d_4;
            
             digit_changed = 1;           //set digit change flag
            
            }       //end d_4 if
        }           //end D4 if
        
        //segment decode for digit 3  
        if(D3)                          //Digit 3 active high    
        {        
            
            d_3 = PORTC;
            
            if(d_3 != d_3_old)          //did the digit change?
                
            {            
            d_3_old = d_3;
           
            digit_changed = 1;           //set digit change flag            

            
            }       //end d_3 if
        }           //end D3 if
        
        //segment decode for digit 2 
        if(D2)                          //Digit 2 active high
        {        
            
            d_2 = PORTC;
            
            if(d_2 != d_2_old)          //did the digit change?
                
            {            
            d_2_old = d_2;

              digit_changed = 1;           //set digit change flag          
             
            }       //end d_2 if
        }           //end D2 if
        
        //segment decode for leftmost digit 1
        //special characters on this digit
        if(D1)                              //Digit 1 active high
        {        
            
            d_1 = PORTC;
            
            if(d_1 != d_1_old)              //did the digit change?
                
            {            
            d_1_old = d_1;
            
             digit_changed = 1;           //set digit change flag
             
            }           //end d_1 if
        }               //end D1 if
            
        
 //When a digit changes, the display needs to be update according to D1 mode           
            if (digit_changed)                  
            {
                digit_changed = 0;             //clear digit_changed flag
            if (d_1 == t)                   //special character t
            {
                 
                lcd_goto_one(0);
                lcd_puts("Tune Up Quickly!");             //tuning mode

            }
            
            else if (d_1 == C)                        //special character C
            {
                lcd_goto_one(7);
                lcd_puts("WPM     ");                   //display keyer speed
                update_display(3,4,5,6,blank,d_3,d_4,blank);
                lcd_goto_one(0);
                lcd_puts("    ");                        //clear tens and ones MHz
            }
            
            else if (d_1 == r)                         //special character r
            {
               lcd_goto_one(0);
               lcd_puts("    ");                        //clear tens and ones MHz
               
               lcd_goto_one(8);
               lcd_puts(" kHz RIT");
               lcd_goto_one(6);
               lcd_puts(".");
               update_display(3,4,5,7,d_2,blank,d_3,d_4);    //display RIT offset
               
            }
            
            else if (d_1 == equal)                           //special character =
            {
                lcd_goto_one(8);
                lcd_puts("Tx = Rx ");                       //RIT returns to 0.0
                
                lcd_goto_one(0);
                lcd_puts("        ");                       //clear tens and ones MHz
            }
            
            else if (d_1 == dash && d_3 == dash)         //special character -
            {
                lcd_goto_one(0);
                lcd_puts("msg1dot msg2dash");
            }
            
            else {
                if(d_1 == blank) d_1 = zero;                 //blank becomes a zero
                
                lcd_goto_one(6);
                lcd_puts(".");
                lcd_goto_one(8);
                lcd_puts(" MHz       ");                     //clear special characters
                
                  //display tens and ones MHz
                lcd_goto_one(0);
                switch (band)
                        {
                case 0x00:                      //40 meters
                    lcd_puts(" 7.");            //tens and ones MHz;
                    offset = offset_40;         //set offset for 40 meter band
                    break;
                    
                case 0x01:                      //20 meters
                    lcd_puts("14.");
                    offset = offset_20;         //set offset for 20 meter band
                    break;
                    
                case 0x02:                      //15 meters
                   lcd_puts("21.");
                   offset = offset_15;          //set offset for 15 meter band
                    break;
                    
                 default :
                    lcd_puts("  .");
                    offset = 0;
                    break;
                           
                        
                        }       //end switch

                 freq_offset(d_1,d_2,d_3,d_4,offset);           //apply offset, generate BCD
                                                                //bcd1000, bcd100,bcdtens, bcdunits
                  
                update_display_value(3,4,5,7, bcd1000, bcd100,      //update the display
                        bcdtens, bcdunits);                         // with offset applied
                
              
            
                }    //end else
            
            }       //end digit_changed if
        
        
        
        

       
    
    
    }           //end while(1=1)



}

void __interrupt()
isr(void)
{
		
	if (T0IF)
		{
			timer0_events++;					//increment number of T0 overflow
			TMR0 = timer_reset_value;			//reload starting value in T0
			T0IF = 0;							//clear interrupt flag
		}										//end T0IF if
}



/**********************************************************************************************
		Function Name: 	freq_offset( unsigned char dgt_1, unsigned char dgt_2, unsigned char dgt_3,
        unsigned char dgt_4, int f_off)
		Return Value:	BCD values
		Parameters:		character representative of dgt_# (digits 1 -4), dgt_1 leftmost
 *                      
 *                      f_off is (frequency offset)/100Hz
		Description:	calculates frequency display with offset applied
 *                      output is BCD value for each digit
 *                      
	
	************************************************************************************************/


void freq_offset(unsigned char dgt_1, unsigned char dgt_2, unsigned char dgt_3,
        unsigned char dgt_4, int f_off)

{
    //retrieve actual value of each digit
    unsigned char v_1 = seg_to_value(dgt_1);
    unsigned char v_2 = seg_to_value(dgt_2);
    unsigned char v_3 = seg_to_value(dgt_3);
    unsigned char v_4 = seg_to_value(dgt_4);
    
    
    
    //calculate corrected frequency f_cor
    int f_cor = v_1 * 1000 + v_2 * 100 + v_3 * 10 + v_4 + f_off;
    
    binary_to_BCDconvert(f_cor);        //generate BCD
                                        //bcd1000, bcd100, bcdtens, bcdunits
    
    
}               //end function
    
  
 

    





