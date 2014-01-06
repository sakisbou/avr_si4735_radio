/*
	The keyboard is using 4 dedicated I/O lines to implement 12 keys.
	The I/O lines can be configured indepentently from any available port.

	Before the keyboard can be used the chkb4_init() function has to be called.

	The keyboard system is using 3 16-bit registers for keeping key information.
	Each bit of a register corresponds to a key. 1 means pressed, 0 means not pressed.
	Only 12 bits (0 through 11) are being used.

	'keys_current_sample' is holding the keys status aquired by the last run of the chkb4_update() function.
	'keys_old_sample' is holding the keys status aquired by the prevous to the last run of chkb4_update().
	'keys_flags' is holding a flag for each key. The key's flag will be set when the key is pressed
	(i.e. a transition from 0 to 1 in the 'keys_current_sample' register). It will not be set when
	the key is released (i.e. transition from 1 to 0). The flag will be reset only via either the chkb4_key_pressed()
	or the chkb4_any_key_pressed() functions call.

	The chkb4_update() function must be called periodically (usually via a timer interrupt) with a period
	of no less than 20ms (a safe value), to facillate key debouncing.
	Prior to scanning, the previous 'keys_current_sample' value is saved into 'keys_old_sample'.
	The function scans the I/O lines using the following pattern:
	The first I/O line is set as output. The other 3 lines are set as inputs and are getting sampled indepentently.
	If a key is found to be pressed, the corresponding 'keys_current_sample' register's bit will be set to 1.
	The procedure is repeated for all I/O lines (i.e. the next line is set as output, etc).
	Next, a XOR operation between 'keys_current_sample' and 'keys_old_sample' will reveal if there was any key
	state changed (i.e pressed or released), by indicating 1 for a change and 0 for no change.
	If change is detected, a second XOR operation between the first XOR result (key changes) and 'keys_old_sample',
	will reveal which keys were pressed (not released). This information is ORed into the 'key_flags' variable.

	The idea behind the 'key_flags' register, is to have a mechanism similar to interrupt flags.
	If a key is pressed, a flag is raised until the key is serviced.

	The chkb4_key_pressed() function checks if a specific key has it's flag set. It also resets that key's flag.
	The argument is the key's name as a macro, defined bellow in the 'key names' section.

	The chkb4_any_key_pressed() function checks if any key has it's flag set. It also resets all the key flags.
*/

#ifndef __CHKB4__
#define __CHKB4__

#include <stdint.h>
#include <avr/io.h>




/*
	Setup
*/

// Keyboard lines port, direction, pin registers and bit position (0 lsbit, 7 msbit)

// line 0
#define CHKB4_LINE_0_PORT PORTD
#define CHKB4_LINE_0_PDIR DDRD
#define CHKB4_LINE_0_PIN PIND
#define CHKB4_LINE_0_BIT 0

// line 1
#define CHKB4_LINE_1_PORT PORTD
#define CHKB4_LINE_1_PDIR DDRD
#define CHKB4_LINE_1_PIN PIND
#define CHKB4_LINE_1_BIT 1

// line 2
#define CHKB4_LINE_2_PORT PORTD
#define CHKB4_LINE_2_PDIR DDRD
#define CHKB4_LINE_2_PIN PIND
#define CHKB4_LINE_2_BIT 2

// line 3
#define CHKB4_LINE_3_PORT PORTD
#define CHKB4_LINE_3_PDIR DDRD
#define CHKB4_LINE_3_PIN PIND
#define CHKB4_LINE_3_BIT 3

//	Key names and bit positions in chkb4 registers

#define KEY_00 0
#define KEY_01 1
#define KEY_02 2
#define KEY_03 3
#define KEY_04 4
#define KEY_05 5
#define KEY_06 6
#define KEY_07 7
#define KEY_08 8
#define KEY_09 9
#define KEY_10 10
#define KEY_11 11




/*
	API
*/

void chkb4_init( void );
void chkb4_update( void );
uint8_t chkb4_key_pressed( uint8_t key );
uint8_t chkb4_any_key_pressed( void );

#endif

