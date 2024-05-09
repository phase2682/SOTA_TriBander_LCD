/* 
 * File:   SI_LCD.c
 * Author: Dan Koellen AI6XG
 * 
 * This code will interface with a serial LCD
 * Built for the NewHaven NHD-C0216CZ LCD
 * Uses  ST7032 controller internal to LCD
 *
 * Created on October 29, 2023, 1:23 PM
 * revised for TQFP on January 24, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include    "SI_LCD_dk_TQFP.h"

/*
 * 
 */

//designate your clock speed for __delay_ms())
//#define _XTAL_FREQ  4000000     //For 4 MHz Oscillator
#define _XTAL_FREQ  8000000     //For 8 MHz Oscillator

void lcd_data (unsigned char d)
{
    //CSB = 0;
    RS = 1;
    RST = 1;
    SCL = 0;
    SI = 0;
    
    //all 0 except RS RST
    
    for (int i =0; i < 8; i++)
    {
     
        if((d & 0x80) == 0x80)          //send MSB first
        {
            SI = 1;
           
        }
        else
        {
            SI = 0;
        }
        
 //               //clock SCL once
        SCL = 1;
       
        SCL = 0;
        
  //all 0 except RS RST
        SI = 0;
        RST = 1;
        RS = 1;
        
        d = (d << 1);
        
        
    }                   //end for
    
//    CSB = 1;

    //ALL 0 except RS RST
    RS = 1;
    RST = 1;
    SI = 0;
    SCL = 0;
}                       //End lcd_data function


//sends a command character
void lcd_command(unsigned char c)
{
 //   CSB = 0;            //Chip Select active low
    RS = 0;             //Register select command clear
   //all 0 except RST
    SI = 0;
    SCL = 0;
    RST = 1;
    for (int j = 0; j < 8; j++)
    {
       if ((c & 0x80) == 0x80)  //send MSB first
       {
           SI = 1;
       }
       else
       {
           SI = 0;
       }
       c = (c <<1);
        SCL = 0;          //clock SCL once
        SCL = 1;
        SCL = 0; 
 //all 0 except RST
        SI = 0;
        RST = 1;
       
    }               //end for
    
 //   CSB = 1;
  
 //ALL 0 except RS RST
    RS = 1;
    RST = 1;
    SI = 0;
    SCL = 0;
    
}                   //end lcd_command function


//initializes the LCD
void lcd_initialize (void)
{
    __delay_ms(40);
    RST = 0;           //reset LCD
    __delay_ms(2);
    RST = 1;           //exit reset
    __delay_ms(20);
    lcd_command(0x30);      //wake up
    __delay_ms(2);
    lcd_command(0x30);      //wake up
    lcd_command(0x30);      //wake up
    lcd_command(0x39);      //function set
    lcd_command(0x14);      //internal osc frequency
    lcd_command(0x56);      //power control
    lcd_command(0x6D);      //follower control
    lcd_command(0x70);      //contrast
    lcd_command(0x0C);      //display on
    lcd_command(0x06);      //entry mode
    lcd_command(0x01);      //clear
    __delay_ms(10);
        
}                       //end lcd_initialize


//sends a series of characters from a string
void lcd_puts(const char * s)
{
		while(*s)
		lcd_data(*s++);
}       // end of lcd_puts


//go to position p on line 1
//first position is p = 0
//verify starting address
void lcd_goto_one(unsigned char p)
{
	lcd_command(0x80+p);
}       //end of lcd_goto_one


//go to position pos on line 2
//first position is pos = 0
//verify starting address
void lcd_goto_two(unsigned char pos)
{
	lcd_command(0xC0+pos);
}       //end of lcd_goto_two


//clear and go to home
void lcd_clear(void)
{
	lcd_command(0x1);
	__delay_ms(2);
}       //end of lcd_clear



