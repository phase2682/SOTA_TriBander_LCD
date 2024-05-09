

/* 
 * File:   SI_LCD_dk_TQFP.h
 * Author: Dan Koellen 
 * Comments:
 * Revision history: 
 * * revised for TQFP on January 24, 2024
 */


#include <xc.h> // include processor files - each processor file is guarded.  


// define IOs for logic signals
#define SI          RD4         //Serial Data To LCD
#define SCL         RD5         //Serial Clock
#define RS          RD6         //Register Select 0=command 1=data
#define RST         RD7         //Active Low Reset

unsigned char two_line = 0;     //two line font flag

//CSB not used for only one display, tie to ground
//#define CSB         RD3         //Active Low Chip Select



//function declaration

/* write a character to the LCD */
extern void lcd_data (unsigned char d);

/* write a command to the LCD */
extern void lcd_command(unsigned char c);

/* intialize the LCD - call before anything else */
extern void lcd_initialize (void);

/* write a string of characters to the LCD */
extern void lcd_puts(const char * s);

/* Go to the specified position on line 1*/
extern void lcd_goto_one(unsigned char p);

/* Go to the specified position on line 2*/
extern void lcd_goto_two(unsigned char pos);

/* Clear and home the LCD */
extern void lcd_clear(void);


