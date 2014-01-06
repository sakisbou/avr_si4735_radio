#include "delay.h"




/*
   Delay loop. Consumes 6 cycles per itteration
   except for the last which consumes 5 cycles.
   Depending on compiler optimization, a small amount
   of cycles is consumed during initialization.
*/

void delay(uint32_t delay)
{
  asm volatile(
                "sbiw %A0, 1" "\n\t"
                "sbci %C0, 0" "\n\t"
                "sbci %D0, 0" "\n\t"
                "brne .-8"    "\n\t"
                : "=w" (delay)
                : "0" (delay)
              );
}

