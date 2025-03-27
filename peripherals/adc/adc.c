#include "../adc/adc.h"
#include <util/delay.h>
#include <stdbool.h>

volatile uint16_t sampleBuffer[BUFFER_SIZE]; // make adc buffer
volatile uint16_t bufferIndex = 0; // for incrementing buffer index
volatile bool bufferFull = false; // flag for signaling full buffer

 /**
 * @brief Initializes the ADC on PIN D1 and with a prescaler of CLK_PER/12.
 */
void ADC0_init(void)
{
	/* Disable digital input buffer */
    PORTD.PIN1CTRL &= ~PORT_ISC_gm;
    PORTD.PIN1CTRL |= PORT_ISC_INPUT_DISABLE_gc;
    /* Disable pull-up resistor */
    PORTD.PIN1CTRL &= ~PORT_PULLUPEN_bm;
    
    ADC0.CTRLC = ADC_PRESC_DIV12_gc;    /* CLK_PER divided by 12 */
    ADC0.CTRLA = ADC_ENABLE_bm | ADC_RESSEL_10BIT_gc; /* ADC Enable: enabled */
    

	/* Select ADC channel */
    ADC0.MUXPOS = ADC_MUXPOS_AIN1_gc; //PD1
    
    ADC0.INTCTRL = ADC_RESRDY_bm;
}

 /**
 * @brief Does a sample on the configured analog pin.
 */
uint16_t ADC0_read(void)
{
    ADC0.COMMAND = ADC_STCONV_bm;

    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm))
    {
        ;
    }
    
    ADC0.INTFLAGS = ADC_RESRDY_bm;
    return ADC0.RES;
}


/**
* @brief Initializes the VREF peripheral with the desired reference voltages 
* for the DAC and the ADC.
*/
void VREF_init(void)
{
    /* Voltage reference at VDD */
	 
    VREF.DAC0REF |= VREF_REFSEL_VDD_gc;
	VREF.ADC0REF |= VREF_REFSEL_VDD_gc;
      
	  
	VREF.DAC0REF |= VREF_ALWAYSON_bm;
	VREF.ADC0REF |= VREF_ALWAYSON_bm;
	  
	/* Wait VREF start-up time */
	_delay_us(VREF_STARTUP_MICROS);
}