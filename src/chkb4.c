#include "chkb4.h"




/*
	keyboard I/O control macros
*/

// line 0
#define chkb4_line_0_set_out() CHKB4_LINE_0_PDIR |= 1 << CHKB4_LINE_0_BIT
#define chkb4_line_0_set_in() CHKB4_LINE_0_PDIR &= ~( 1 << CHKB4_LINE_0_BIT )
#define chkb4_line_0_set_hi() CHKB4_LINE_0_PORT |= 1 << CHKB4_LINE_0_BIT
#define chkb4_line_0_set_low() CHKB4_LINE_0_PORT &= ~( 1 << CHKB4_LINE_0_BIT )
#define chkb4_line_0_read() ( ( ~CHKB4_LINE_0_PIN & ( 1 << CHKB4_LINE_0_BIT ) ) >> CHKB4_LINE_0_BIT )

// line 1
#define chkb4_line_1_set_out() CHKB4_LINE_1_PDIR |= 1 << CHKB4_LINE_1_BIT
#define chkb4_line_1_set_in() CHKB4_LINE_1_PDIR &= ~( 1 << CHKB4_LINE_1_BIT )
#define chkb4_line_1_set_hi() CHKB4_LINE_1_PORT |= 1 << CHKB4_LINE_1_BIT
#define chkb4_line_1_set_low() CHKB4_LINE_1_PORT &= ~( 1 << CHKB4_LINE_1_BIT )
#define chkb4_line_1_read() ( ( ~CHKB4_LINE_1_PIN & ( 1 << CHKB4_LINE_1_BIT ) ) >> CHKB4_LINE_1_BIT )

// line 2
#define chkb4_line_2_set_out() CHKB4_LINE_2_PDIR |= 1 << CHKB4_LINE_2_BIT
#define chkb4_line_2_set_in() CHKB4_LINE_2_PDIR &= ~( 1 << CHKB4_LINE_2_BIT )
#define chkb4_line_2_set_hi() CHKB4_LINE_2_PORT |= 1 << CHKB4_LINE_2_BIT
#define chkb4_line_2_set_low() CHKB4_LINE_2_PORT &= ~( 1 << CHKB4_LINE_2_BIT )
#define chkb4_line_2_read() ( ( ~CHKB4_LINE_2_PIN & ( 1 << CHKB4_LINE_2_BIT ) ) >> CHKB4_LINE_2_BIT )

// line 3
#define chkb4_line_3_set_out() CHKB4_LINE_3_PDIR |= 1 << CHKB4_LINE_3_BIT
#define chkb4_line_3_set_in() CHKB4_LINE_3_PDIR &= ~( 1 << CHKB4_LINE_3_BIT )
#define chkb4_line_3_set_hi() CHKB4_LINE_3_PORT |= 1 << CHKB4_LINE_3_BIT
#define chkb4_line_3_set_low() CHKB4_LINE_3_PORT &= ~( 1 << CHKB4_LINE_3_BIT )
#define chkb4_line_3_read() ( ( ~CHKB4_LINE_3_PIN & ( 1 << CHKB4_LINE_3_BIT ) ) >> CHKB4_LINE_3_BIT )




/*
	Globals
*/

uint16_t keys_old_sample, keys_current_sample, key_flags;




/*
	keyboard I/O initialise
*/

void chkb4_init( void )
{
	// set all lines to input with pull-ups
	chkb4_line_0_set_in();
	chkb4_line_0_set_hi();
	chkb4_line_1_set_in();
	chkb4_line_1_set_hi();
	chkb4_line_2_set_in();
	chkb4_line_2_set_hi();
	chkb4_line_3_set_in();
	chkb4_line_3_set_hi();
}




/*
	keyborad update
*/

void chkb4_update( void )
{
	// preserve the current sample
	keys_old_sample = keys_current_sample;

	// set LINE 0 as collumn driver
	chkb4_line_0_set_out();
	chkb4_line_0_set_low();
	// read lines 1-3
	keys_current_sample &= ~( 1 << KEY_00 );
	keys_current_sample |= chkb4_line_1_read() << KEY_00;
	keys_current_sample &= ~( 1 << KEY_01 );
	keys_current_sample |= chkb4_line_2_read() << KEY_01;
	keys_current_sample &= ~( 1 << KEY_02 );
	keys_current_sample |= chkb4_line_3_read() << KEY_02;
	// set line 0 as input with pull-up
	chkb4_line_0_set_hi();
	chkb4_line_0_set_in();


	// set LINE 1 as collumn driver
	chkb4_line_1_set_out();
	chkb4_line_1_set_low();
	// read lines 0,2,3
	keys_current_sample &= ~( 1 << KEY_03 );
	keys_current_sample |= chkb4_line_0_read() << KEY_03;
	keys_current_sample &= ~( 1 << KEY_04 );
	keys_current_sample |= chkb4_line_2_read() << KEY_04;
	keys_current_sample &= ~( 1 << KEY_05 );
	keys_current_sample |= chkb4_line_3_read() << KEY_05;
	// set line 1 as input with pull-up
	chkb4_line_1_set_hi();
	chkb4_line_1_set_in();
	

	// set LINE 2 as collumn driver
	chkb4_line_2_set_out();
	chkb4_line_2_set_low();
	// read lines 0,1,3
	keys_current_sample &= ~( 1 << KEY_06 );
	keys_current_sample |= chkb4_line_0_read() << KEY_06;
	keys_current_sample &= ~( 1 << KEY_07 );
	keys_current_sample |= chkb4_line_1_read() << KEY_07;
	keys_current_sample &= ~( 1 << KEY_08 );
	keys_current_sample |= chkb4_line_3_read() << KEY_08;
	// set line 2 as input with pull-up
	chkb4_line_2_set_hi();
	chkb4_line_2_set_in();
	

	// set LINE 3 as collumn driver
	chkb4_line_3_set_out();
	chkb4_line_3_set_low();
	// read lines 0-2
	keys_current_sample &= ~( 1 << KEY_09 );
	keys_current_sample |= chkb4_line_0_read() << KEY_09;
	keys_current_sample &= ~( 1 << KEY_10 );
	keys_current_sample |= chkb4_line_1_read() << KEY_10;
	keys_current_sample &= ~( 1 << KEY_11 );
	keys_current_sample |= chkb4_line_2_read() << KEY_11;
	// set line 3 as input with pull-up
	chkb4_line_3_set_hi();
	chkb4_line_3_set_in();


	// filter the keys pressed or released since the last keyboard update.
	uint16_t keys_changed = keys_current_sample ^ keys_old_sample;

	// further filter to get the keys that were pressed only (not released),
	// and OR them into the flags register.
	if( keys_changed ) key_flags |= keys_changed ^ keys_old_sample;	
}




/*
	key pressed check
*/

uint8_t chkb4_key_pressed( uint8_t key )
{
	// get the state of a single key
	if( key_flags & ( 1 << key ) )
	{
		// reset it's flag
		key_flags &= ~( 1 << key );

		return 1;
	}

	return 0;
}




/*
	any key pressed check
*/

uint8_t chkb4_any_key_pressed( void )
{
	if( key_flags ) { key_flags = 0; return 1; }

	return 0;
}

