#include "../clock/clkctrl.h"

void clkctrl_init(void)
{
	//ccp_write_io((void*)&CLKCTRL.MCLKCTRLB, 0x00); //Set Main clock to 20 MHz
    /* Setter OSCHF-klokka til 24 MHz */
	ccp_write_io ((uint8_t *) & CLKCTRL.OSCHFCTRLA, CLKCTRL_FRQSEL_24M_gc 
                                                   | CLKCTRL_AUTOTUNE_bm);
}