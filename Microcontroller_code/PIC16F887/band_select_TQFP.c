/* 
 * File:   band select TQFP.c
 * Author: Dan Koellen AI6XG
 *
 * Created on November 7, 2023, 10:38 AM
 * * revised for TQFP on January 24, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include    "band_select_TQFP.h"




/*
 * 
 */



/**********************************************************************************************
		Function Name: 	unsigned char DurationClear(unsigned int start, unsigned char duration, 
 *                                                  unsigned int reset, unsigned int events)
		Return Value:	flag = 0
		Parameters:		start is when flag was set, duration is # of timer0_events flag to be set for,
 *                      reset is number of timer_events when reset or overflowed (usigned char-->255), 
 *                      events is timer_events count
		Description:	returns 0 after duration # of timer0_events, otherwise 1
	
	************************************************************************************************/
unsigned char DurationClear(unsigned int start, unsigned char duration, unsigned int reset, unsigned int events)
{
unsigned char flag = 1;
unsigned int time_now1 = events;						//current total of timer overflows
unsigned int delta = reset - duration;                 //region where overflow will occur during duration
if (start >= delta & time_now1 >= delta)
                                        ;				//do nothing until time_now1 is overflowed	
if (start >= delta & time_now1 < delta) 				//adapt for overflow reset
	{
	if ((time_now1 - start + reset)>=duration) flag = 0;	//clear flag at the end of duration
	}													//end if
if (start < delta)                                      //no adapting outside of overflow region
	{
	if (time_now1 - start >= duration) flag = 0;			//clear  flag
	}													//end if
return flag;
}														//end function





