/* 
 * File:   band select TQFP.h
 * Author: Dan Koellen AI6XG
 *
 * Created on November 10, 2023, 5:25 PM
 * revised for TQFP on January 24, 2024
 */

#include <xc.h> // include processor files - each processor file is guarded.

//Definitions for band set
#define band_set	RA7                 //set input to advance band 
#define _15_mtr     RE0                 //15 meters band set output
#define _20_mtr     RE1                 //20 meters band set output

#define R12s        RA2                 //relays 1 and 2 set output
#define R12r        RA3                 //relays 1 and 2 reset output
#define R34s        RA0                 //relays 3 and 4 set output
#define R34r        RA1                 //relays 3 and 4 reset output
#define relay_pulse_dur 6               //relay pulse duration timer0_events output

unsigned char 	band = 0;            	//0->40 mtrs, 1->20 mtrs, 2->15 mtrs
unsigned char 	previous_band = 3;		//forces band <> previous_band at startup
unsigned int 	state_band = 0;			//debounce state holder

unsigned int   R12s_time = 0;          //time that R12 is set
unsigned int   R12r_time = 0;          //time that R12 is reset
unsigned int   R34s_time = 0;          //time that R34 is set
unsigned int   R34r_time = 0;          //time that R34 is reset

//declare functions

//returns 0 after duration # of timer0_events, otherwise 1
unsigned char 	DurationClear(unsigned int start, unsigned char duration,
        unsigned int reset, unsigned int events);
