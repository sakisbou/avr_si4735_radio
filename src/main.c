/*
    to set the mega168 fuses type the following in a terminal,
    while in the source dir: (sudo) make fuses
*/



//___ INCLUDES __________________________________________________________________________________

//  Necessary for standard integer sizes
#include <stdint.h>

//  All AVR I/O names
#include <avr/io.h>

//  Needed for retreiving strings stored in program flash memory.
//#include <avr/pgmspace.h>

#include <avr/interrupt.h>
// #include <avr/eeprom.h>

#include "delay.h"
#include "uc1701.h"
#include "chkb4.h"
#include "si4735.h"

//___ GLOBALS _______________________________________________________________________________

#define FM 0
#define MW 1
#define SW 2
#define OFF 3

uint8_t band;
uint16_t freq[3];
uint8_t step[3];
uint16_t top_limit[3];
uint16_t bottom_limit[3];
uint16_t antcap[3];

//___ FUNCTIONS ______________________________________________________________________________

/*

	Measure

*/

void measure(void)
{
  si4735_rsq_status(0);
  uc1701_cursor_move(0, 6);
  uc1701_print_dec_s8(-(SI4735_FREQOFF));
  uc1701_cursor_move(1, 0);
  uc1701_print_dec_u8(SI4735_RSSI);      
  uc1701_cursor_move(1, 4);
  uc1701_print_dec_u8(SI4735_SNR);
  if(SI4735_FMST)
  {
    uc1701_print_str(" S");      
    uc1701_print_dec_u8(SI4735_STBLEND);
  }
  else
  {
    uc1701_print_str("      ");      
  }

  si4735_agc_status();
  uc1701_cursor_move(2, 13); 
  uc1701_print_dec_u8(SI4735_AM_LNA_GAIN_INDEX);

  si4735_tune_status(SI4735_INTACK);
  uc1701_cursor_move(0, 0);
  uc1701_print_dec_u16(SI4735_FREQ);
  uc1701_cursor_move(2, 0);
  switch(band)
  {
    case MW :
    case SW : uc1701_print_dec_u16(SI4735_READANTCAPAM); break;
    case FM : uc1701_print_str("     "); break;
  }
}




/*

	Power up FM

*/

void power_up_fm(void)
{
  si4735_power_up(SI4735_XOSCEN|SI4735_FM, SI4735_ANALOG);
  si4735_set_property(SI4735_FM_SEEK_BAND_BOTTOM, bottom_limit[band]);
  si4735_set_property(SI4735_FM_SEEK_BAND_TOP, top_limit[band]);
  si4735_set_property(SI4735_FM_DEEMPHASIS, SI4735_EUR_50us);
  si4735_tune_freq(freq[band], antcap[band], 0);
  measure();
}




/*

	Power up AM

*/

void power_up_am(void)
{
  si4735_power_up(SI4735_XOSCEN|SI4735_AM, SI4735_ANALOG);
  si4735_set_property(SI4735_AM_SEEK_BAND_BOTTOM, bottom_limit[band]);
  si4735_set_property(SI4735_AM_SEEK_BAND_TOP, top_limit[band]);
  si4735_set_property(SI4735_AM_CHANNEL_FILTER, SI4735_BW4KHZ);
  si4735_tune_freq(freq[band], antcap[band], 0);
  measure();
}




#define UP 1
#define DOWN -1

/*

	Step

*/

void channel_step(int dir)
{
  freq[band] += (step[band] * dir);
  if(freq[band] < bottom_limit[band]) freq[band] = top_limit[band];
  if(freq[band] > top_limit[band]) freq[band] = bottom_limit[band];
  si4735_tune_freq(freq[band], antcap[band], 0);
  measure();
}




/*

	Scan

*/

void scan(int dir)
{
  do
  {
    channel_step(dir);
  }
  while(!SI4735_TUNE_VALID && !chkb4_any_key_pressed() );
}




/*
	timer 0 overflow interrupt
*/

ISR(TIMER0_OVF_vect)
{
	chkb4_update();
}


/*

	Init

*/

void init(void)
{

  TCCR0B = 0x05; // start with 10bit prescaler
  TIMSK0 = 0x01; // enable overflow interrupt
  chkb4_init();

  uc1701_io_init();
  si4735_init();

  bottom_limit[MW] = 522; top_limit[MW] = 1620; step[MW] = 9; antcap[MW] = 0; freq[MW] = bottom_limit[MW];
  bottom_limit[SW] = 2300; top_limit[SW] = 23000; step[SW] = 5; antcap[SW] = 0; freq[SW] = bottom_limit[SW];
  bottom_limit[FM] = 8750; top_limit[FM] = 10800; step[FM] = 5; antcap[FM] = 0; freq[FM] = bottom_limit[FM];

  band = OFF;
  sei();
}



/*

	Main

*/

int main(void)
{
  init();

/*
// Graphic LCD testing begin
  uc1701_power_up();
  uint8_t i=0;
  uc1701_cls();
  uc1701_cursor_move(0, 0);
  for(i=48; i<58; i++) uc1701_print_symbol(i);
  uc1701_cursor_move(1, 1);
  for(i=48; i<58; i++) uc1701_print_symbol(i);
//  uc1701_cursor_move(6, 2);
//  for(i=48; i<58; i++) uc1701_print_symbol(i);
//  return 0;
// Graphic LCD testing end


	// keyboard test begin
	for(;;)
	{
		uint18_t key2count = 0;
//		uc1701_cursor_move( 7, 0 );
//		uc1701_print_hex_u16( key_flags );
		delay( _100ms_ );

		if( chkb4_key_pressed( KEY_09 ) )
		{
			keycount++;
			uc1701_cursor_move( 3, 0 );
			uc1701_print_dec_u8( keycount );
		}
	}
	// keyboard test end
*/



  for(;;)
    {
	if( chkb4_key_pressed( KEY_06 ) ) { band = OFF; si4735_power_down(); uc1701_power_down(); }
	if( chkb4_key_pressed( KEY_09 ) )
	  {
		if( band == OFF ) uc1701_power_up();
		band = FM;
	 	si4735_power_down();
		power_up_fm();
	  }

	if( chkb4_key_pressed( KEY_10 ) )
	  {
		if( band == OFF ) uc1701_power_up();
		band = MW;
	 	si4735_power_down();
		power_up_am();
	  }

	if( chkb4_key_pressed( KEY_11 ) )
	  {
		if( band == OFF ) uc1701_power_up();
		band = SW;
	 	si4735_power_down();
		power_up_am();
	  }


	if( chkb4_key_pressed( KEY_04 ) ) channel_step(UP);
	if( chkb4_key_pressed( KEY_01 ) ) channel_step(DOWN);


	if( chkb4_key_pressed( KEY_05 ) ) scan(UP);
	if( chkb4_key_pressed( KEY_02 ) ) scan(DOWN);
    }

  return 0;
}
