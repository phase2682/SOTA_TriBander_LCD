/*Convert segments to font values or numeric values
 Write to serial input LCD at specific positions
 * * revised for TQFP on January 24, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include    "segment_to_LCD_TQFP.h"
#include    "SI_LCD_dk_TQFP.h"



/**********************************************************************************************
		Function Name: 	seg_write_lcd(unsigned char c)
		Return Value:	none
		Parameters:		segment character representative of c
 *                      
		Description:	updates display with character representative of c
 *                      writes on line one only for double font height
 *                      position the cursor at the write position prior to running this function
	
	************************************************************************************************/

//writes the character represented by the segments c to the LCD
//position the cursor at the write position prior to running this function
void seg_write_lcd(unsigned char c)
{
switch (c)
            {
                case zero:                      //0
                    lcd_data(0x30);
                    break;
                    
                case one:                      //1
                    lcd_data(0x31);
                    break;
                    
                case two:                      //2
                    lcd_data(0x32);
                    break;
                    
                case three:                      //3
                    lcd_data(0x33);
                    break;
                    
                case four:                      //4
                    lcd_data(0x34);
                    break;
                    
                case five:                      //5
                    lcd_data(0x35);
                    break;
                
                case six:                      //6
                    lcd_data(0x36);
                    break;
                    
                case seven:
                    lcd_data(0x37);             //7
                    break;
                    
                case eight:                      //8
                    lcd_data(0x38);
                    break;
                    
                case nine:
                    lcd_data(0x39);             //9
                    break;
                    
                case blank:                      //blank
                    lcd_data(0x20);
                    break;
                    
                case dash:                      //-
                    lcd_data(0x2d);
                    break;
                    
                case equal:                      //=
                    lcd_data(0x3d);
                    break;
                    
                default :
                    lcd_data(0x21);
                    break;
                           
                        
            }       //end switch
}




/**********************************************************************************************
		Function Name: 	update_display(unsigned char pos_1, unsigned char pos_2,unsigned char pos_3,
        unsigned char pos_4, unsigned char c_1, unsigned char c_2, unsigned char c_3,
        unsigned char c_4)
		Return Value:	none
		Parameters:		segment representative of c_# (digits 1 -4)
 *                      LCD position for each character pos_#
		Description:	updates display with character representative of c_# (digits 1 -4)
 *                      writes on line one only for double font height
	
	************************************************************************************************/


void update_display(unsigned char pos_1, unsigned char pos_2,unsigned char pos_3,
        unsigned char pos_4, unsigned char c_1, unsigned char c_2, unsigned char c_3,
        unsigned char c_4)
{
    lcd_goto_one(pos_1);
    seg_write_lcd(c_1);
    
    lcd_goto_one(pos_2);
    seg_write_lcd(c_2);
    
    lcd_goto_one(pos_3);
    seg_write_lcd(c_3);
    
    lcd_goto_one(pos_4);
    seg_write_lcd(c_4);
    
}                               //end function


/**********************************************************************************************
		Function Name: 	seg_to_value(unsigned char c)
		Return Value:	unsigned char representing value of c, 
 *                      between 0 - 9 inclusive
		Parameters:		font representative of c
 *                      
		Description:	returns the value number representative of c
 *                      
 * 
	
	************************************************************************************************/


unsigned char seg_to_value(unsigned char c)
{
switch (c)
            {
                case zero:                      //0
                    return 0;
                    break;
                    
                case one:                      //1
                     return 1;
                    break;
                    
                case two:                      //2
                    return 2;
                    break;
                    
                case three:                      //3
                     return 3;
                    break;
                    
                case four:                      //4
                     return 4;
                    break;
                    
                case five:                      //5
                     return 5;
                    break;
                
                case six:                      //6
                     return 6;
                    break;
                    
                case seven:
                     return 7;                  //7
                    break;
                    
                case eight:                      //8
                     return 8;
                    break;
                    
                case nine:
                    return 9;                   //9
                    break;
                    
                                    
                default :
                    return 0;
                    break;
                           
                        
            }       //end switch
}                   //end function


  
 /**********************************************************************************************
		Function Name: 	update_display_value(unsigned char pos_1, unsigned char pos_2,unsigned char pos_3,
        unsigned char pos_4, unsigned char c_1, unsigned char c_2, unsigned char c_3,
        unsigned char c_4)
		Return Value:	none
		Parameters:		value of digit of c_# (digits 1 -4), 0 - 9  inclusive
 *                      LCD position for each character pos_#
		Description:	updates display with character representative of c_# (digits 1 -4)
 *                      writes on line one only for double font height
	
	************************************************************************************************/


void update_display_value(unsigned char pos_1, unsigned char pos_2,unsigned char pos_3,
        unsigned char pos_4, unsigned char c_1, unsigned char c_2, unsigned char c_3,
        unsigned char c_4)
{
    lcd_goto_one(pos_1);
    lcd_data(c_1 + 0x30);
    
    lcd_goto_one(pos_2);
    lcd_data(c_2 + 0x30);
    
    lcd_goto_one(pos_3);
    lcd_data(c_3 + 0x30);
    
    lcd_goto_one(pos_4);
    lcd_data(c_4 + 0x30);
    
}                               //end function   



