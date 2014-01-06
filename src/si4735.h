/*
	SI4735 driver
*/

#ifndef __SI4735__
#define __SI4735__

#include <stdint.h>
#include <avr/io.h>
#include "delay.h"
#include "si4735_properties.h"

// ___ Setup _____________________________________________________________________________________

/*
	Select only one (comment out the rest).
*/

#define SI4735_SPI
// #define SI4735_3WIRE
// #define SI4735_2WIRE

/*
	Ports, bits, input pins and direction registers.
*/



// --- RST ---
#define SI4735_RSTPORT PORTB
#define SI4735_RSTDIR DDRB
#define SI4735_RSTBIT 0x02
// --- SEN ---
#define SI4735_SENPORT PORTB
#define SI4735_SENDIR DDRB
#define SI4735_SENBIT 0x04
// --- SCLK ---
#define SI4735_SCLKPORT PORTB
#define SI4735_SCLKDIR DDRB
#define SI4735_SCLKBIT 0x08
// --- SDIO ---
#define SI4735_SDIOPORT PORTB
#define SI4735_SDIODIR DDRB
#define SI4735_SDIOBIT 0x10
#define SI4735_SDIOPIN PINB



/*
	SCLK half period Delay in uS.

	Uncomment the line bellow
	to insert a delay routine
	in the 'sclk_pulse()' code.
*/

#define SI4735_SCLK_DELAY _1us_





// ___ Interface buffer ___________________________________________________________________________________________

// The communication interface's 16 bytes send/receive buffer.
uint8_t si4735_if_buffer[16];

// Status register bits, found at si4735_if_buffer[0] after every command resopnse read.
#define SI4735_CTS    ( si4735_if_buffer[0] & 0x80 )  // Clear To Send.
#define SI4735_ERR    ( si4735_if_buffer[0] & 0x40 )  // ERRor.
#define SI4735_RSQINT ( si4735_if_buffer[0] & 0x08 )  // Receive Signal Quality INTerrupt.
#define SI4735_RDSINT ( si4735_if_buffer[0] & 0x04 )  // Radio Data System INTerrupt.
#define SI4735_STCINT ( si4735_if_buffer[0] & 0x01 )  // Seek Tune Complete INTerrupt.





/*
	API
*/

void si4735_io_init( void );





/*
	POWER UP

	Moves the device from power down state to power up state.

	The function expects 2 arguments. The setup argument
	and the audio output argument.

	The setup argument is a single byte value composed of a high nibble
	of 4 setup bits and a low nibble of 3 possible function choices.
	Both the high nibble setup bits and the low nibble function codes
	are defined as macros, so that they can be ORed together.
    
	Note that while the setup bits can be combined together in any
	way necessary, the function codes cann't. So this is ok : s4735_power_up(CTSIN|XOSCEN|FM);
	while this is not : s4735_power_up(CTSIN|XOSCEN|FM|AM);

	The audio out argument defines whether the analog, the digital
	or both audio outputs will be active. Again macros have been defined
	for both choices which can be ORed together.

	Among the 3 function choices (AM / FM / QLID) the QLID is not a normal function. It's ment to query
	the compatibility of the internal software lib with a patch that might be necessary to apply every
	time the chip boots to upgrade it's functionality.
	When the power up command is executed with the QLID function, it will power up the device, dump
	software lib revision info into if_buffer and return the device back to power down mode.

	The power up command can be issued only when the device is in power down mode.
*/

// --- ARG1 ---
//	CTSIEN	0x80		Enable the Clear To Send interrupt (defined as a propery value in GPO_IEN property).
#define SI4735_GPO2OE		0x40			// Enable the GPO2 as General Purpose Output.
#define SI4735_PATCH		0x20			// Power up in firmware patch download mode.
#define SI4735_XOSCEN		0x10			// Enable the on board crystal oscilator.
#define SI4735_FM		0x00			// Power up in FM receive mode.
#define SI4735_AM		0x01			// Power up in AM receive mode.
#define SI4735_QLID		0x0f			// Get the library ID.
// --- ARG2 ---
#define SI4735_RDS_ONLY		0x00			// Disable all audio leaving RDS only running.
#define SI4735_ANALOG		0x05			// Enable analog audio output.
#define SI4735_DIGITAL		0xb0			// Enable digital audio output.
// --- RESPONSE ---
#define SI4735_PN		si4735_if_buffer[1]	// Part number's last 2 digits in HEX.
#define SI4735_FWMAJOR		si4735_if_buffer[2]	// Firmware major number in ASCII.
#define SI4735_FWMINOR		si4735_if_buffer[3]	// Firmware minor number in ASCII.
#define SI4735_LIBRARYID	si4735_if_buffer[7]	// Library ID in HEX.

void si4735_power_up( uint8_t setup, uint8_t audio_out );




/*
	GET REV

	Returns the part number, chip revision, firmware revision,
	patch revision and component revision numbers.
*/

// --- RESPONSE ---
/*
	The first 3 resonse bytes are already defined in 'si47nn_power_up()'.
  
	SI4735_PN								Part number's last 2 digits in HEX.
	SI4735_FWMAJOR								Firmware major number in ASCII.
	SI4735_FWMINOR								Firmware minor number in ASCII.
*/
#define SI4735_PATCHID	((si4735_if_buffer[4] << 8) | si4735_if_buffer[5])	// Patch ID in HEX.
#define SI4735_CMPMAJOR	si4735_if_buffer[6]					// Component major number in ASCII.
#define SI4735_CMPMINOR	si4735_if_buffer[7]					// Component minor number in ASCII.
#define SI4735_CHIPREV	si4735_if_buffer[8]					// Chip revision ASCII.

void si4735_get_rev( void );




/*
	POWER DOWN

	Moves the device from power up state to power down state.
*/

void si4735_power_down( void );




/*
	SET PROPERTY

	Sets a property.

	The command accepts 2 arguments. A 2 byte propery ID
	and a 2 byte value.
*/

void si4735_set_property( uint16_t property, uint16_t value );




/*
	GET PROPERTY

	Gets a property's value.

	The command accepts 1 argument, the 2 byte propery ID.
*/

// --- RESONSE ---
#define SI4735_PROPERTY_VALUE ((si4735_if_buffer[2] << 8) | si4735_if_buffer[3])

void si4735_get_property( uint16_t property );




/*
    GET INT STATUS

    Updates bits 6:0 of the status byte. This command should be called after any command
    that sets the STCINT, RDSINT, or RSQINT bits. When polling this command should be
    periodically called to monitor the STATUS byte, and when using interrupts,
    this command should be called after the interrupt is set to update the STATUS byte.
*/

void si4735_get_int_status( void );




/*
	FM / AM TUNE FREQ

	Tunes the FM / AM receiver to a specified frequency.
*/

// --- ARG 1 ---
#define SI4735_FAST	0x01	// Fast but invalidated tunning.
#define SI4735_FREEZE	0x02	// If set will cause the blend, hicut, and softmute to transition as a function
				// of the associated attack/release parameters rather than instantaneously
				// when tuning to alternate station. Valid only for FM.


void si4735_tune_freq( uint16_t freq, uint16_t antcap, uint8_t setup );




/*
	FM / AM SEEK START

	Begins searching for a valid frequency.
*/

// --- ARG 1 ---
// Seek direction.
#define SI4735_SEEKUP	0x08
#define SI4735_SEEKDOWN	0x00
// Band wrap around during seek.
#define SI4735_WRAP	0x04
#define SI4735_NOWRAP	0x00

void si4735_seek_start( uint8_t arg1, uint16_t antcap );




/*
	FM / AM TUNE STATUS

	Returns the status of FM_TUNE_FREQ or FM_SEEK_START commands.
	The commands returns the current frequency, RSSI, SNR,
	and the antenna tuning capacitance value (0-191).
	The command clears the STCINT interrupt bit when INTACK bit of ARG1 is set.
	The CTS bit (and optional interrupt) is set when it is safe to send
	the next command.

	NOTE: The command is not suitable for getting subsequent updates for the above
	magnitutes. It only captures a snapshot of their values after FM_TUNE_FREQ's or
	FM_SEEK_START's completion. Those snapshot values are returned, no matter
	how many times you call the command. For subsequent updates, use 'si47nn_rsq_status'.
*/

// --- ARG 1 ---
#define SI4735_CANCEL         0x02                                              // Cancels a running seek operation.
#define SI4735_INTACK         0x01                                              // Clears the STCINT bit in status register.
// --- RESPONSE ---
#define SI4735_BTLF          (si4735_if_buffer[1] & 0x80)                       // Band limit / wrap indicator.
#define SI4735_TUNE_VALID    (si4735_if_buffer[1] & 0x01)                       // Valid signal received indicator.
#define SI4735_FREQ         ((si4735_if_buffer[2] << 8) | si4735_if_buffer[3])  // Tuned frequency.
#define SI4735_READANTCAPFM   si4735_if_buffer[7]                               // FM Antenna tunning capacitor value.
#define SI4735_READANTCAPAM ((si4735_if_buffer[6] << 8) | si4735_if_buffer[7])  // AM Antenna tunning capacitor value.

void si4735_tune_status( uint8_t arg1 );




/*
	FM / AM RSQ STATUS

	Returns status information about the received signal quality.
*/

// --- ARG 1 ---
#define SI4735_INTACK      0x01  // Clears RSQINT, BLENDINT, SNRHINT, SNRLINT, RSSIHINT, RSSILINT interrupts.
// --- RESONSE ---
#define SI4735_BLENDINT   (si4735_if_buffer[1] & 0x80)   // Blend goes above or below the Blend threshold settings.
#define SI4735_SNRHINT    (si4735_if_buffer[1] & 0x08)   // Received SNR has exceeded above SNR high threshold.
#define SI4735_SNRLINT    (si4735_if_buffer[1] & 0x04)   // Received SNR has fallen below SNR low threshold.
#define SI4735_RSSIHINT   (si4735_if_buffer[1] & 0x02)   // RSSI has exceeded above RSSI high threshold.
#define SI4735_RSSILINT   (si4735_if_buffer[1] & 0x01)   // RSSI has fallen below RSSI low threshold.
#define SI4735_SMUTE      (si4735_if_buffer[2] & 0x08)   // Soft mute engaged indicator.
#define SI4735_AFCRL      (si4735_if_buffer[2] & 0x02)   // AFC railing indicator.
#define SI4735_VALID      (si4735_if_buffer[2] & 0x01)   // Valid channel indicator.
#define SI4735_FMST       (si4735_if_buffer[3] & 0x80)   // FM Stereo indicator.
#define SI4735_STBLEND    (si4735_if_buffer[3] & 0x7f)   // Stereo blend value: 0 - 100 mono to stereo.
#define SI4735_RSSI        si4735_if_buffer[4]           // The current receive signal strength (0–127 dBμV).
#define SI4735_SNR         si4735_if_buffer[5]           // The current SNR metric (0–127 dB).
#define SI4735_MULT        si4735_if_buffer[6]           // Contains the current multipath metric.
#define SI4735_FREQOFF     si4735_if_buffer[7]           // Signed frequency offset (kHz).

void si4735_rsq_status( uint8_t arg1 );




/*
	FM RDS STATUS

	Returns RDS information for current channel and reads an entry from the RDS FIFO.
*/

// --- ARG 1 ---
/*
	Determines if data should be removed from the RDS FIFO.
	0 = Data in BLOCKA, BLOCKB, BLOCKC, BLOCKD, and BLE contain the oldest data in the RDS FIFO.
	1 = Data in BLOCKA will contain the last valid block A data received for the current station.
	Data in BLOCKB will contain the last valid block B data received for the current station.
	Data in BLE will describe the bit errors for the data in BLOCKA and BLOCKB.
*/
#define SI4735_STATUSONLY    0x04

#define SI4735_MTFIFO        0x02                              // Clear RDS Receive FIFO.
#define SI4735_INTACK        0x01                              // Clears RDSINT.
// --- RESONSE ---
#define SI4735_RDSNEWBLOCKB (si4735_if_buffer[1] & 0x20)       // Valid Block B data has been received.
#define SI4735_RDSNEWBLOCKA (si4735_if_buffer[1] & 0x10)       // Valid Block A data has been received.
#define SI4735_RDSSYNCFOUND (si4735_if_buffer[1] & 0x04)       // Found RDS synchronization.
#define SI4735_RDSSYNCLOST  (si4735_if_buffer[1] & 0x02)       // Lost RDS synchronization.
#define SI4735_RDSRECV      (si4735_if_buffer[1] & 0x01)       // FIFO filled to minimum number of groups set by RDSFIFOCNT.
#define SI4735_GRPLOST      (si4735_if_buffer[2] & 0x04)       // One or more RDS groups discarded due to FIFO overrun.
#define SI4735_RDSSYNC      (si4735_if_buffer[2] & 0x01)       // RDS currently synchronized.
#define SI4735_RDSFIFOUSED   si4735_if_buffer[3]               // Number of groups remaining in the RDS FIFO (0 if empty).
#define SI4735_RDSBLOCKA   ((si4735_if_buffer[4] << 8) | si4735_if_buffer[5])    // RDS Block A.
#define SI4735_RDSBLOCKB   ((si4735_if_buffer[6] << 8) | si4735_if_buffer[7])    // RDS Block A.
#define SI4735_RDSBLOCKC   ((si4735_if_buffer[8] << 8) | si4735_if_buffer[9])    // RDS Block A.
#define SI4735_RDSBLOCKD   ((si4735_if_buffer[10] << 8) | si4735_if_buffer[11])  // RDS Block A.
/*
	RDS block error correction indicators:
	0 = No errors.
	1 = 1–2 bit errors detected and corrected.
	2 = 3–5 bit errors detected and corrected.
	3 = Uncorrectable.
*/
#define SI4735_BLEA         (si4735_if_buffer[12] >> 6)                   // RDS Block A Corrected Errors.
#define SI4735_BLEB        ((si4735_if_buffer[12] >> 4) & 0x03)           // RDS Block B Corrected Errors.
#define SI4735_BLEC        ((si4735_if_buffer[12] >> 2) & 0x03)           // RDS Block C Corrected Errors.
#define SI4735_BLED         (si4735_if_buffer[12] & 0x03)                 // RDS Block D Corrected Errors.

void si4735_fm_rds_status( uint8_t arg1 );




/*
	FM / AM AGC STATUS

	The command returns whether the AGC is enabled or disabled
	and it returns the LNA Gain index.
*/

// --- RESPONSE ---
#define SI4735_RFAGCDIS_STATUS      (si4735_if_buffer[1] & 0x01) // This bit indicates whether the RF AGC is disabled.
#define SI4735_FM_LNA_GAIN_INDEX    (si4735_if_buffer[2] & 0x1f) // These bits returns the value of the LNA GAIN index.
#define SI4735_AM_LNA_GAIN_INDEX     si4735_if_buffer[2]         // 0 = no attenuation, 26(FM) or 255(AM) = max attenuation.

void si4735_agc_status( void );




/*
	FM / AM AGC OVERRIDE

	Overrides AGC setting by disabling the AGC and forcing the LNA to have a set gain.
*/

// --- ARG 1 ---
#define SI4735_RFAGCDIS 0x01	// Disables RF LNA AGC.
#define SI4735_RFAGCEN  0x00	// Enables RF LNA AGC.

void si4735_agc_override( uint8_t rfagcdis, uint8_t lnagain );




/*
	GPIO_CTL

	Enables output for GPO1, 2, and 3. GPO1, 2, and 3 can be configured
	for output (Hi-Z or active drive) by setting the GPO1OEN, GPO2OEN, and GPO3OEN bit.
	The state (high or low) of GPO1, 2, and 3 is set with the GPIO_SET command.
	To avoid excessive current consumption due to oscillation, GPO pins should not be left
	in a high impedance state.
*/

// --- ARG 1 ---
#define SI4735_GPO3OEN 0x08	// Set GPO3 as output.
#define SI4735_GPO2OEN 0x04	// Set GPO2 as output.
#define SI4735_GPO1OEN 0x02	// Set GPO1 as output.

void si4735_gpio_ctl( uint8_t arg1 );




/*
	GPIO SET

	Sets the output level (high or low) for GPO1, 2, and 3.
	GPO1, 2, and 3 can be configured for output by setting
	the GPO1OEN, GPO2OEN, and GPO3OEN bit in the GPIO_CTL command.
	To avoid excessive current consumption due to oscillation,
	GPO pins should not be left in a high impedance state.
*/

// --- ARG 1 ---
// Set - reset individual GPIO pins.
#define SI4735_SET_GPO3 0x08
#define SI4735_CLR_GPO3 0x00
#define SI4735_SET_GPO2 0x04
#define SI4735_CLR_GPO2 0x00
#define SI4735_SET_GPO1 0x02
#define SI4735_CLR_GPO1 0x00

void si4735_gpio_set( uint8_t arg1 );




/*
	initialise
*/

void si4735_init( void );



#endif
