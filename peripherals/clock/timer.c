#include "timer.h"
#include "clkctrl.h"
#include "../adc/adc.h"

volatile uint16_t currentSample = 0;

/**
 * @brief Initializes the TCA0 timer with a periodic interrupt based on the 
 * desired SAMPLING_PERIOD. 
 */
void TCA0_init(void) {
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SINGLE_CLKSEL_DIV1_gc; 
    
	TCA0.SINGLE.PERBUF = (uint16_t)(F_CPU*((float)SAMPLING_PERIOD/(1000000*1)))-PERIOD_ERROR_COMPENSATION;
}


 /**
 * @brief ISR for sampling and ADC into buffer and notifying when buffer is full
 */
ISR(TCA0_OVF_vect) {
    if (!bufferFull) {
        uint16_t adcValue = ADC0_read();
        sampleBuffer[bufferIndex] = adcValue;
        bufferIndex++;

        if (bufferIndex >= BUFFER_SIZE) {
                bufferFull = true;
                bufferIndex = 0;
        }
    }
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

