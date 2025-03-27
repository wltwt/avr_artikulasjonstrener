/*
* main.c
*
* Created: 7/5/2022 9:48:26 AM
* Author: Microchip Technology
*
* Info: Dette prosjektet demonstrerer KissFFT-biblioteket (et C-bibliotek for beregning av FFT) p� AVR128DB48. Det er basert p� et eksempel fra Microchip. 
* Testen gjennomf�res ved at det tas FFT av en sinus lagret i minne. Effekten av hver frekvenskomponent i FFT blir s� beregnet f�r dataen sendes over UART.
* Det er deretter mulig � visualisere dataen i Data Visualizer i MPLAB X.rosjektet er konfigurert til � bruke fixed-point beregninger p� 16 bit. 
* Se README og GitHub (https://github.com/microchip-pic-avr-examples/avr64ea48-digital-filters-studio/tree/master/fft) 
* for detaljert info om demonstrasjonen. 
*
* Adjusted for AVR128DB48: March, 2025
* Editor: Carl Richard Steen Fosse
*/ 

#include <xc.h>
#include "filter/kiss_fftr.h"
#include "filter/sine.h"
#include "peripherals/clock/clkctrl.h"
#include "peripherals/clock/timer.h"
#include "peripherals/usart/usart3.h"
#include "peripherals/adc/adc.h"
#include "peripherals/data_streamer/data_streamer.h"
#include "display/ssd1306.h"
#include <stdbool.h>
#include <avr/cpufunc.h>
#include <math.h>
#include <util/delay.h>


#define NFFT 256

kiss_fft_scalar cpx_in[NFFT]; 

int main(void)
    {
    clkctrl_init();
    VREF_init();
    ADC0_init();
	TCA0_init();
    USART3_Initialize();
    SSD1306_Init(SSD1306_ADDR);  // 0x3C
    
    //Configuring pin for time performance measurement
    PORTD.DIRSET = PIN4_bm;
    PORTC.DIRSET = PIN3_bm;
    
    volatile int out = (NFFT/2)+1;

    kiss_fftr_cfg cfg = kiss_fftr_alloc(NFFT, 0, NULL, NULL);  //kiss_fft_alloc( nfft ,is_inverse_fft ,0,0 );

    kiss_fft_cpx cpx_out[out];

    //for(int m =0;m<NFFT;m++){               // fill cpx_in with nfft data points from sinewave[]
    //    cpx_in[m] = sinewave[m];
    //};

    volatile int16_t watch_real=4;
    volatile int16_t watch_imag = 35;
    volatile uint32_t pwr;
    volatile uint16_t cnt=0;
    
    // enable interrupts
	sei();

    //PORTD.OUTSET = PIN4_bm;
    //kiss_fftr(cfg, cpx_in , cpx_out);
    //PORTD.OUTCLR = PIN4_bm;
    
    PORTC.OUTSET = PIN3_bm; // for timing ADC-sampling

    while(1) {        
        SSD1306_ClearScreen();

        if (bufferFull) {
            PORTC.OUTCLR = PIN3_bm;

            cli();
            bufferFull = false;
            sei(); 

            
            for (int i = 0; i < BUFFER_SIZE; i++) {
                cpx_in[i] = (kiss_fft_scalar)sampleBuffer[i];
            }
            
            // fft
            PORTD.OUTSET = PIN4_bm; // for timing
            kiss_fftr(cfg, cpx_in, cpx_out);
            PORTD.OUTCLR = PIN4_bm;
            
            PORTC.OUTSET = PIN3_bm; // for timing

        }

        int x_prev = 0;
        int y_prev = 63;
        
        
        for(int n=0;n<out;n++)
        {
            //putting cpx_out.r  into watchable variables
            cnt = n;
            watch_real = cpx_out[n].r;
            watch_imag = cpx_out[n].i;

            //pwr = sqrt(cpx_out[n].r * cpx_out[n].r + cpx_out[n].i * cpx_out[n].i);

            uint32_t pwr_raw = (uint32_t)cpx_out[n].r * cpx_out[n].r + (uint32_t)cpx_out[n].i * cpx_out[n].i;
            uint8_t amp = pwr_raw >> 1;

            // clamp y
            int y = 63 - (amp > 63 ? 63 : amp);
            
            SSD1306_DrawLine(x_prev, n, y_prev, y);

            x_prev = n;
            y_prev = y;
            
            //Writing four variables to USART, so they can be read by MPLAB DV:
            // 0. Start token
            // 1. watch_real (int16_t)
            // 2. watch_imag (int16_t)
            // 3. cnt (uint16_t))
            // 4. pwr (uint_32_t))
            // .. and the end token
            variableWrite_SendFrame(watch_real, watch_imag, cnt, amp);

        }

        SSD1306_UpdateScreen(SSD1306_ADDR);
        //_delay_ms(10);


    }
    kiss_fft_cleanup();                         // Tidy up after you.

}