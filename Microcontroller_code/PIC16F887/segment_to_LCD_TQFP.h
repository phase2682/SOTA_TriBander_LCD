/* 
 * File:   segment_to_LCD_TQFP.h
 * Author: Dan Koellen AI6XG
 *
 * Created on November 9, 2023, 3:42 PM
 *  revised for TQFP on January 24, 2024
 */

#include <xc.h> // include processor files - each processor file is guarded.

//definitions for segment and digits

#define D1          RD3         //Digit 1 input
#define D2          RD2         //Digit 2 input
#define D3          RD0         //Digit 3 input
#define D4          RD1         //Digit 4 input

#define seg_b       RC1         //segment b input
#define seg_e       RC6         //segment e input
#define seg_dp      RC7         //decimal point input
#define seg_d       RC5         //segment d input
#define seg_g       RC3         //segment g input
#define seg_a       RC0         //segment a input
#define seg_c       RC2         //segment c input
#define seg_f       RC4         //segment f input

/*
 Each definition is for segments of the seven segment LED that would be active
 * for each symbol.  The numerical value is the port value for the segments that
 * are active.  
 * Also determined by active low (common anode) or active high(common cathode).
 * For example for the following port definitions 
#define seg_b       RC1         //segment b input
#define seg_e       RC6         //segment e input
#define seg_dp      RC7         //decimal point input
#define seg_d       RC5         //segment d input
#define seg_g       RC3         //segment g input
#define seg_a       RC0         //segment a input
#define seg_c       RC2         //segment c input
#define seg_f       RC4         //segment f input
 * 
 * The character two on a seven segment would be segments a,b,d,e and g active
 * the PORTC value for the character two would be 0b01101011 or 0x6B active high 
 * and 0b10010100 or 0x94 active low
 */

//rev 1.0 PCB has fixed RC7 (dp) grounded (active) rather than high (inactive)
//rest of segments are active low, inactive high
#define zero    0x08            //0
#define one     0x79            //1
#define two     0x14            //2
#define three   0x50            //3
#define four    0x61            //4
#define five    0x42            //5
#define six     0x03            //6
#define seven   0x78            //7
#define eight   0x00            //8
#define nine    0x60            //9
#define blank   0x7F            //no segments active
#define r       0x37            //r
#define dash    0x77            //-
#define C       0x0E            //C
#define o       0x13            //o
#define t       0x07            //t
#define equal   0x76            //=



unsigned char   d_4 = 0;           //digit 4 data rightmost digit
unsigned char   d_4_old = 1;        //old digit 4 data

unsigned char   d_3 = 0;           //digit 3 data
unsigned char   d_3_old = 1;        //old digit 3 data

unsigned char   d_2 = 0;           //digit 2 data
unsigned char   d_2_old = 1;        //old digit 2 data

unsigned char   d_1 = 0;           //digit 1 data leftmost digit
unsigned char   d_1_old = 1;        //old digit 1 data

unsigned char   digit_changed = 0;  //flag for digit change


 
//declare functions

//updates display with character representative of c
extern void seg_write_lcd(unsigned char c);

//updates display with character representative of c_# (digits 1 -4)
extern void update_display(unsigned char pos_1, unsigned char pos_2,unsigned char pos_3,
        unsigned char pos_4, unsigned char c_1, unsigned char c_2, unsigned char c_3,
        unsigned char c_4);

//returns the value number representative of c
extern unsigned char seg_to_value(unsigned char c);

//updates display with character representative of c_# (digits 1 -4)
//input of value of digit of c_# (digits 1 -4), 0 - 9  inclusive
extern void update_display_value(unsigned char pos_1, unsigned char pos_2,unsigned char pos_3,
        unsigned char pos_4, unsigned char c_1, unsigned char c_2, unsigned char c_3,
        unsigned char c_4);


