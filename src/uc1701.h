/*
    UltraChip uc1701x driver.
*/

#ifndef __UC1701__
#define __UC1701__

#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "delay.h"

// ___ Setup _____________________________________________________________________________________

/*
    Set the contrast
*/
#define UC1701_CONTRAST 0x08


/*
    Ports, direction registers, and bits.
*/

// Power (PWR) pin
#define UC1701_PWR_PORT PORTC
#define UC1701_PWR_DIR DDRC
#define UC1701_PWR_BIT 0x20
// Back Light (BL) pin
#define UC1701_BL_PORT PORTC
#define UC1701_BL_DIR DDRC
#define UC1701_BL_BIT 0x10
// Chip Select (CS) pin
#define UC1701_CS_PORT PORTC
#define UC1701_CS_DIR DDRC
#define UC1701_CS_BIT 0x01
// Register Select (CD) pin
#define UC1701_CD_PORT PORTC
#define UC1701_CD_DIR DDRC
#define UC1701_CD_BIT 0x02
// Serial Clock (SCLK) pin
#define UC1701_SCLK_PORT PORTC
#define UC1701_SCLK_DIR DDRC
#define UC1701_SCLK_BIT 0x04
// Serial Input (SDA) pin
#define UC1701_SDA_PORT PORTC
#define UC1701_SDA_DIR DDRC
#define UC1701_SDA_BIT 0x08




/*
    SCLK half period Delay in uS.

    Uncomment the line bellow
    to insert a delay routine
    in the 'sclk_pulse()' code.
*/

// #define UC1701_SCLK_DELAY _1us_




/*
   Comment out the functions you don't need to preserve program memory space.
*/

#define UC1701_CURSOR_MOVE
#define UC1701_PRINT_STR_P
#define UC1701_PRINT_STR
#define UC1701_PRINT_DEC_U8
#define UC1701_PRINT_DEC_S8  // depends on LCD_PRINT_DEC_U8.
#define UC1701_PRINT_DEC_U16
#define UC1701_PRINT_HEX_U8
#define UC1701_PRINT_HEX_U16




/*
 LCD static messages.

 These constants are placed in program flash memory. Had they been declared like this :
 char local[] = "local   "; or used directly like this :
 lcd_print("local   "); they would have ended up in SRAM, seriously shrinking the space
 available for global variables and PC stack.

 Specialy declared pointers and special functions have to be used to access them.
 The pointers are i.e. of type 'PGM_P' and can be used as normal in pointer arithmetic.
 But they cann't be de-refferenced directly. They can be de-refferenced only through
 special functions like 'strcpy_P(char *, PGM_P)'or 'bytebuff = pgm_read_byte_near(PGM_P)'.
*/

//const char hello[] PROGMEM = "  Hello  ";




// ___ Character sets ___





/*
    IO Initialize
*/

void uc1701_io_init(void);




/*
    Sets the LCD's data address, effectively moving the cursor
    to a new position, based on display 'line' and 'column' values.
*/

#ifdef UC1701_CURSOR_MOVE
void uc1701_cursor_move(uint8_t line, uint8_t column);
#endif




/*
    CLS.
*/

void uc1701_cls(void);




/*
   LCD power up.
*/

void uc1701_power_up(void);




/*
    LCD power down.
*/

void uc1701_power_down(void);




/*
    Print a single character.
*/

void uc1701_print_symbol(char symbol);




/*
    Print a program flash stored string at the cursor's current position.
*/

#ifdef UC1701_PRINT_STR_P
void uc1701_print_str_P(PGM_P text);
#endif




/*
    Print a SRAM stored string at the cursor's current position.
*/

#ifdef UC1701_PRINT_STR
void uc1701_print_str(char *text);
#endif




/*
    Print an unsigned 8 bit decimal number at the cursor's current position.
*/

#ifdef UC1701_PRINT_DEC_U8
void uc1701_print_dec_u8(uint8_t value);
#endif




/*
    Print a signed 8 bit decimal number at the cursor's current position.
*/

#ifdef UC1701_PRINT_DEC_S8
void uc1701_print_dec_s8(int8_t value);
#endif




/*
    Print a 16 bit decimal number at the cursor's current position.
*/

#ifdef UC1701_PRINT_DEC_U16
void uc1701_print_dec_u16(uint16_t value);
#endif




/*
    Print a 8 bit hexadecimal number at the cursor's current position.
*/

#ifdef UC1701_PRINT_HEX_U8
void uc1701_print_hex_u8(uint8_t value);
#endif




/*
    Print a 16 bit hexadecimal number at the cursor's current position.
*/

#ifdef UC1701_PRINT_HEX_U16
void uc1701_print_hex_u16(uint16_t value);
#endif




#endif

