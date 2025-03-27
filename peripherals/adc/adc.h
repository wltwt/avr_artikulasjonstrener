/* 
 * File:   adc.h
 * Author: tln
 *
 * Created on 26. mars 2025, 14:55
 */



#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
    
#define VREF_STARTUP_MICROS (50)
#define BUFFER_SIZE 256

extern volatile uint16_t sampleBuffer[BUFFER_SIZE];
extern volatile uint16_t bufferIndex;
extern volatile bool bufferFull;


void ADC0_init(void);
uint16_t ADC0_read(void);
void VREF_init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

