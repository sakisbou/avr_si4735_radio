/*
	A simple delay loop using an assembly routine with known number of cycles per iterration.
	Macros are being defined to fixed delay values which can be used as arguments to the delay function.
	It is important that the system's clock frequency be set accordingly bellow.
*/

#ifndef __DELAY__
#define __DELAY__

#include <stdint.h>




/*
    Set the system's clock frequency in Hz.
*/

#define CLKFREQ 8000000

/*
    Pre-calculated delays based on system's clock frequency.
    Min delay in seconds = 6 / CLKFREQ
    Max delay in seconds = (2^32 * 6) / CLKFREQ
*/

#define   _1us_ ((0.000001f * CLKFREQ) / 6)
#define  _30us_ ((0.00003f * CLKFREQ) / 6)
#define _300us_ ((0.0003f * CLKFREQ) / 6)
#define   _1ms_ ((0.001f * CLKFREQ) / 6)
#define   _2ms_ ((0.002f * CLKFREQ) / 6)
#define  _10ms_ ((0.01f * CLKFREQ) / 6)
#define _100ms_ ((0.1f * CLKFREQ) / 6)
#define _110ms_ ((0.11f * CLKFREQ) / 6)
#define _120ms_ ((0.12f * CLKFREQ) / 6)
#define _200ms_ ((0.2f * CLKFREQ) / 6)
#define _300ms_ ((0.3f * CLKFREQ) / 6)


/*
	API
*/

void delay(uint32_t delay);

#endif

