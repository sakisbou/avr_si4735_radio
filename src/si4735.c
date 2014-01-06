/*
	SI4735 driver
*/

#include "si4735.h"



// ___ I/O _________________________________________________________________________________________________

/*
	SEN
*/

#define si4735_dis_sen() SI4735_SENPORT |= SI4735_SENBIT
#define si4735_en_sen() SI4735_SENPORT &= ~SI4735_SENBIT




/*
    SCLK
*/

inline void si4735_sclk_pulse( void )
{
	#ifdef SI4735_SCLK_DELAY
		delay(SI4735_SCLK_DELAY);
	#endif

	SI4735_SCLKPORT |= SI4735_SCLKBIT;

	#ifdef SI4735_SCLK_DELAY
		delay(SI4735_SCLK_DELAY);
	#endif

	SI4735_SCLKPORT &= ~SI4735_SCLKBIT;
}




/*
    SDIO
*/

#define si4735_set_sdio() SI4735_SDIOPORT |= SI4735_SDIOBIT
#define si4735_clr_sdio() SI4735_SDIOPORT &= ~SI4735_SDIOBIT
#define si4735_in_sdio() ( SI4735_SDIOPIN & SI4735_SDIOBIT )
#define si4735_set_sdio_out() SI4735_SDIODIR |= SI4735_SDIOBIT
#define si4735_set_sdio_in() { SI4735_SDIODIR &= ~SI4735_SDIOBIT; /* clear pull-up */ si4735_clr_sdio(); }




/*
    RST
*/

inline void si4735_rst( void )
{
	delay(_1ms_);
	SI4735_RSTPORT &= ~SI4735_RSTBIT;
	delay(_1ms_);
	SI4735_RSTPORT |= SI4735_RSTBIT;
	delay(_1ms_);
}




/*
    I/O Initialize
*/

void si4735_io_init( void )
{
	si4735_dis_sen();
	SI4735_SENDIR |= SI4735_SENBIT;

	SI4735_SCLKPORT &= ~SI4735_SCLKBIT;
	SI4735_SCLKDIR |= SI4735_SCLKBIT;

	si4735_set_sdio_in();

	SI4735_RSTDIR |= SI4735_RSTBIT;
	SI4735_RSTPORT |= SI4735_RSTBIT;
}




// ___ RECEIVER MODE ______________________________________________________________________________________________

uint8_t si4735_receiver_mode;




// ___ SPI ________________________________________________________________________________________________________

// Compile the 'SPI' part only if SI4735_SPI has been selected as the communications inteface.

#ifdef SI4735_SPI

/*
    Send a single byte over SPI
*/

void si4735_spi_send_byte( uint8_t byte )
{
	int8_t i;

	for( i = 7; i >= 0; i-- )
	{
		if( byte & ( 1 << i ) ) si4735_set_sdio();
		else si4735_clr_sdio();
		si4735_sclk_pulse();
	}
}



/*
    Receive a single byte over SPI
*/

uint8_t si4735_spi_receive_byte( void )
{
	uint8_t byte = 0;
	int8_t i;

	for( i = 7; i >= 0; i-- )
	{
		if(si4735_in_sdio()) byte |= ( 1 << i );
		si4735_sclk_pulse();
	}

	return byte;
}



/*
	The complete SPI send transaction, composed of
	1 SPI control byte, 1 command byte and 7 argument bytes.

	The command byte and the argument bytes should be placed
	in the 'if_buffer', with the command byte at index 0
	and the argument bytes at indices 1 through 7, prior
	to the function's call.

	Independent of the number of a command's arguments,
	all 7 bytes must be send, with those not containing
	arguments set to 0x00.
*/

void si4735_spi_send( void )
{
	uint8_t i;

	si4735_set_sdio_out();
	si4735_en_sen();
	si4735_spi_send_byte( 0x48 );
	for( i = 0; i < 8; i++ ) si4735_spi_send_byte( si4735_if_buffer[i] );
	si4735_dis_sen();
}



/*
	The complete SPI sort receive transaction, composed of
	1 control byte and 1 status byte.

	The status byte is placed at 'if_buffers's' index 0,
	after the function's return.
*/

void si4735_spi_sort_receive( void )
{
	si4735_set_sdio_out();
	si4735_en_sen();
	si4735_spi_send_byte( 0x80 );
	si4735_set_sdio_in();
	si4735_if_buffer[0] = si4735_spi_receive_byte();
	si4735_dis_sen();
}



/*
	The complete SPI long receive transaction, composed of
	1 control byte, 1 status byte and 15 response bytes.

	The status byte is placed at 'if_buffers's' index 0,
	and the response bytes at indices 1 through 15, after
	the function's return.

	All 15 response bytes must be read, indepedent of
	a command's real response bytes. The rest of the bytes
	contain random data and should be ignored.
*/

void si4735_spi_long_receive( void )
{
	uint8_t i;

	si4735_set_sdio_out();
	si4735_en_sen();
	si4735_spi_send_byte( 0xC0 );
	si4735_set_sdio_in();
	for( i = 0; i < 16; i++ ) si4735_if_buffer[i] = si4735_spi_receive_byte();
	si4735_dis_sen();
}



/*
    Define the abstracted communication function names for SPI.
*/

#define si4735_if_send() si4735_spi_send()
#define si4735_if_sort_receive() si4735_spi_sort_receive()
#define si4735_if_long_receive() si4735_spi_long_receive()




#endif





// ___ Commands ___________________________________________________________________________________________________

/*
	Interface command transaction
*/

void si4735_send_command( uint8_t if_buffer_size )
{
	uint8_t i;
	for( i = if_buffer_size; i < 8; i++ ) si4735_if_buffer[i] = 0x00;
	si4735_if_send();
	delay( _300us_ );
}




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

void si4735_power_up( uint8_t setup, uint8_t audio_out )
{
	si4735_receiver_mode = setup & 0x0f;	// make a note of the receiver's status
	si4735_if_buffer[0] = 0x01;
	si4735_if_buffer[1] = setup;
	// RDS_ONLY is not supported in AM.
	if( si4735_receiver_mode == SI4735_AM && setup == SI4735_RDS_ONLY ) si4735_if_buffer[2] = SI4735_ANALOG;
	si4735_if_buffer[2] = audio_out;
	si4735_send_command(3);
	delay( _300ms_ );
	if((setup & 0x0f) == SI4735_QLID) si4735_if_long_receive();
	else si4735_if_sort_receive();
}




/*
	GET REV

	Returns the part number, chip revision, firmware revision,
	patch revision and component revision numbers.
*/

void si4735_get_rev( void )
{
	si4735_if_buffer[0] = 0x10;
	si4735_send_command(1);
	si4735_if_long_receive();
}




/*
	POWER DOWN

	Moves the device from power up state to power down state.
*/

void si4735_power_down( void )
{
	si4735_if_buffer[0] = 0x11;
	si4735_send_command(1);
	si4735_if_sort_receive();
}




/*
	SET PROPERTY

	Sets a property.

	The command accepts 2 arguments. A 2 byte propery ID
	and a 2 byte value.
*/

void si4735_set_property( uint16_t property, uint16_t value )
{
	si4735_if_buffer[0] = 0x12;
	si4735_if_buffer[1] = 0x00;
	si4735_if_buffer[2] = property >> 8;
	si4735_if_buffer[3] = property & 0xff;
	si4735_if_buffer[4] = value >> 8;
	si4735_if_buffer[5] = value & 0xff;
	si4735_send_command(6);
	delay( _10ms_ );
	si4735_if_sort_receive();
}




/*
	GET PROPERTY

	Gets a property's value.

	The command accepts 1 argument, the 2 byte propery ID.
*/

void si4735_get_property( uint16_t property )
{
	si4735_if_buffer[0] = 0x13;
	si4735_if_buffer[1] = 0x00;
	si4735_if_buffer[2] = property >> 8;
	si4735_if_buffer[3] = property & 0xff;
	si4735_send_command(4);
	si4735_if_sort_receive();
}




/*
    GET INT STATUS

    Updates bits 6:0 of the status byte. This command should be called after any command
    that sets the STCINT, RDSINT, or RSQINT bits. When polling this command should be
    periodically called to monitor the STATUS byte, and when using interrupts,
    this command should be called after the interrupt is set to update the STATUS byte.
*/

void si4735_get_int_status( void )
{
	si4735_if_buffer[0] = 0x14;
	si4735_send_command(1);
	si4735_if_sort_receive();
}




/*
	FM / AM TUNE FREQ

	Tunes the FM / AM receiver to a specified frequency.
*/

void si4735_tune_freq( uint16_t freq, uint16_t antcap, uint8_t setup )
{
	si4735_if_buffer[2] = freq >> 8;
	si4735_if_buffer[3] = freq & 0x00ff;

	switch( si4735_receiver_mode )
	{
		case SI4735_FM :	si4735_if_buffer[0] = 0x20;
					si4735_if_buffer[1] = setup;
					si4735_if_buffer[4] = antcap & 0x00ff;
					si4735_send_command(5);
					delay( _120ms_ );
					break;

		case SI4735_AM :	si4735_if_buffer[0] = 0x40;
					si4735_if_buffer[1] = setup & 0x01;  // FREEZE is not supported in AM.
					si4735_if_buffer[4] = antcap >> 8;
					si4735_if_buffer[5] = antcap & 0x00ff;
					si4735_send_command(6);
					delay( _200ms_ );
					break;
	}

	si4735_if_sort_receive();
}




/*
	FM / AM SEEK START

	Begins searching for a valid frequency.
*/

void si4735_seek_start( uint8_t arg1, uint16_t antcap )
{
	si4735_if_buffer[1] = arg1;
	si4735_if_buffer[2] = 0x00;
	si4735_if_buffer[3] = 0x00;
	switch( si4735_receiver_mode )
	{
		case SI4735_FM :	si4735_if_buffer[0] = 0x21;
					si4735_if_buffer[4] = 0x00;
					si4735_if_buffer[5] = 0x00;
					break;

		case SI4735_AM :	si4735_if_buffer[0] = 0x41;
					si4735_if_buffer[4] = antcap >> 8;
					si4735_if_buffer[5] = antcap & 0x00ff;
					break;
	}

	si4735_send_command(6);
	si4735_if_sort_receive();
}




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

void si4735_tune_status( uint8_t arg1 )
{
	switch( si4735_receiver_mode )
	{
		case SI4735_FM : si4735_if_buffer[0] = 0x22; break;
		case SI4735_AM : si4735_if_buffer[0] = 0x42; break;
	}
	si4735_if_buffer[1] = arg1;
	si4735_send_command(2);
	si4735_if_long_receive();
}




/*
	FM / AM RSQ STATUS

	Returns status information about the received signal quality.
*/

void si4735_rsq_status( uint8_t arg1 )
{
	switch( si4735_receiver_mode )
	{
		case SI4735_FM : si4735_if_buffer[0] = 0x23; break;
		case SI4735_AM : si4735_if_buffer[0] = 0x43; break;
	}
	si4735_if_buffer[1] = arg1;
	si4735_send_command(2);
	si4735_if_long_receive();
}




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

void si4735_fm_rds_status( uint8_t arg1 )
{
	si4735_if_buffer[0] = 0x24;
	si4735_if_buffer[1] = arg1;
	si4735_send_command(2);
	si4735_if_long_receive();
}




/*
	FM / AM AGC STATUS

	The command returns whether the AGC is enabled or disabled
	and it returns the LNA Gain index.
*/

void si4735_agc_status( void )
{
	switch( si4735_receiver_mode )
	{
		case SI4735_FM : si4735_if_buffer[0] = 0x27; break;
		case SI4735_AM : si4735_if_buffer[0] = 0x47; break;
	}
	si4735_send_command(1);
	si4735_if_long_receive();
}




/*
	FM / AM AGC OVERRIDE

	Overrides AGC setting by disabling the AGC and forcing the LNA to have a set gain.
*/

void si4735_agc_override( uint8_t rfagcdis, uint8_t lnagain )
{
	switch( si4735_receiver_mode )
	{
		case SI4735_FM : si4735_if_buffer[0] = 0x28; break;
		case SI4735_AM : si4735_if_buffer[0] = 0x48; break;
	}
	si4735_if_buffer[1] = rfagcdis;
	si4735_if_buffer[2] = lnagain;
	si4735_send_command(3);
	si4735_if_sort_receive();
}




/*
	GPIO_CTL

	Enables output for GPO1, 2, and 3. GPO1, 2, and 3 can be configured
	for output (Hi-Z or active drive) by setting the GPO1OEN, GPO2OEN, and GPO3OEN bit.
	The state (high or low) of GPO1, 2, and 3 is set with the GPIO_SET command.
	To avoid excessive current consumption due to oscillation, GPO pins should not be left
	in a high impedance state.
*/

void si4735_gpio_ctl( uint8_t arg1 )
{
	si4735_if_buffer[0] = 0x80;
	si4735_if_buffer[1] = arg1;
	si4735_send_command(2);
	si4735_if_sort_receive();
}




/*
	GPIO SET

	Sets the output level (high or low) for GPO1, 2, and 3.
	GPO1, 2, and 3 can be configured for output by setting
	the GPO1OEN, GPO2OEN, and GPO3OEN bit in the GPIO_CTL command.
	To avoid excessive current consumption due to oscillation,
	GPO pins should not be left in a high impedance state.
*/

void si4735_gpio_set( uint8_t arg1 )
{
	si4735_if_buffer[0] = 0x81;
	si4735_if_buffer[1] = arg1;
	si4735_send_command(2);
	si4735_if_sort_receive();
}




/*
	initialise
*/

void si4735_init( void )
{
	si4735_io_init();
	si4735_rst();
}




