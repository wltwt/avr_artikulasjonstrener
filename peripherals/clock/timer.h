/* 
 * File:   timer.h
 * Author: tln
 *
 * Created on 26. mars 2025, 15:47
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

//#include <avr/io.h>
#include <avr/interrupt.h>    
#include <math.h>    

#define SAMPLING_PERIOD 31 //us 
#define PERIOD_ERROR_COMPENSATION (float) 0.129*pow(SAMPLING_PERIOD,-0.32)*SAMPLING_PERIOD
    
void TCA0_init(void);

extern volatile uint16_t currentSample;

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

