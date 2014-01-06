/*
    UltraChip uc1701x driver.
*/

#include "uc1701.h"
#include "uc1701_latin_charset.h"



// ___ I/O _________________________________________________________________________________________________

/*
    CS
*/

#define uc1701_en_cs() ( UC1701_CS_PORT &= ~UC1701_CS_BIT )
#define uc1701_dis_cs() ( UC1701_CS_PORT |= UC1701_CS_BIT )




/*
    CD
*/

#define uc1701_cd_command() ( UC1701_CD_PORT &= ~UC1701_CD_BIT )
#define uc1701_cd_data() ( UC1701_CD_PORT |= UC1701_CD_BIT )




/*
    SCLK
*/

inline void uc1701_sclk_pulse(void)
{
  #ifdef UC1701_SCLK_DELAY
    delay(UC1701_SCLK_DELAY);
  #endif
  UC1701_SCLK_PORT &= ~UC1701_SCLK_BIT;
  #ifdef UC1701_SCLK_DELAY
    delay(UC1701_SCLK_DELAY);
  #endif
  UC1701_SCLK_PORT |= UC1701_SCLK_BIT;
}




/*
    SI
*/

#define uc1701_set_sda(void) ( UC1701_SDA_PORT |= UC1701_SDA_BIT )
#define uc1701_clr_sda(void) ( UC1701_SDA_PORT &= ~UC1701_SDA_BIT )




/*
    BL
*/

#define uc1701_bl_off(void) ( UC1701_BL_PORT &= ~UC1701_BL_BIT )
#define uc1701_bl_on(void) ( UC1701_BL_PORT |= UC1701_BL_BIT )




/*
    PWR
*/

inline void uc1701_pwr_off(void)
{
  UC1701_PWR_PORT  &= ~UC1701_PWR_BIT;
  UC1701_BL_PORT   &= ~UC1701_BL_BIT;
  UC1701_CS_PORT  &= ~UC1701_CS_BIT;
  UC1701_CD_PORT   &= ~UC1701_CD_BIT;
  UC1701_SCLK_PORT &= ~UC1701_SCLK_BIT;
  UC1701_SDA_PORT   &= ~UC1701_SDA_BIT;
}

inline void uc1701_pwr_on(void)
{
  UC1701_PWR_PORT  |= UC1701_PWR_BIT;
  UC1701_CS_PORT   |= UC1701_CS_BIT;
  UC1701_SCLK_PORT |= UC1701_SCLK_BIT;
}




/*
    IO Initialize
*/

void uc1701_io_init(void)
{
  uc1701_pwr_off();
  UC1701_PWR_DIR |= UC1701_PWR_BIT;
  UC1701_BL_DIR |= UC1701_BL_BIT;
  UC1701_CS_DIR |= UC1701_CS_BIT;
  UC1701_CD_DIR |= UC1701_CD_BIT;
  UC1701_SCLK_DIR |= UC1701_SCLK_BIT;
  UC1701_SDA_DIR |= UC1701_SDA_BIT;
}




// ___ Display _______________________________________________________________________________________________________

/*
   Low level, command or data write.

   'data' is the data byte send to the display.
   'cd' is the display's cotrol/data select value.
*/

#define UC1701_DATA 0x01
#define UC1701_COMMAND 0x00

void uc1701_write(uint8_t data, uint8_t cd)
{
  // Set register select pin
  if(cd == 1) uc1701_cd_data(); else uc1701_cd_command();
  // Enable chip select pin
  uc1701_en_cs();
  // Send a byte
  int8_t i;
  for(i = 7; i >= 0; i--)
  {
    if(data & (1 << i)) uc1701_set_sda(); else uc1701_clr_sda();
    uc1701_sclk_pulse();
  }
  // Disable chip select pin
  uc1701_dis_cs();
}


// ___ Display Commands ___


/*
    Set DDRAM column address.
*/

void uc1701_set_column_address(uint8_t address)
{
  uc1701_write(address & 0x0f, UC1701_COMMAND);
  uc1701_write( 0x10 | (address >> 4), UC1701_COMMAND);
}




/*
    Set power control.
*/

#define UC1701_BOOSTER_ON 0x04
#define UC1701_REGULATOR_ON 0x02
#define UC1701_FOLLOWER_ON 0x01

void uc1701_set_power_control(uint8_t power_control_bits)
{
  uc1701_write(0x28 | power_control_bits, UC1701_COMMAND);
}




/*
    Set scroll line.

    'scroll_line' must be supplied a 6 bit value.
    The 2 most significant bits must be '0'.
*/

void uc1701_set_scroll_line(uint8_t scroll_line)
{
  uc1701_write(0x40 | scroll_line, UC1701_COMMAND);
}




/*
    Set page address.

    'page_address' must be supplied a 4 bit value.
    The 4 most siginificant bits must be '0'.
*/

void uc1701_set_page_address(uint8_t page_address)
{
  uc1701_write(0xb0 | page_address, UC1701_COMMAND);
}




/*
    Set VLCD resistor ratio

    'vlcd_resistor_ratio' must be supplied a 3 bit value.
    The 5 most siginificant bits must be '0'.
*/

void uc1701_set_vlcd_resistor_ratio(uint8_t vlcd_resistor_ratio)
{
  uc1701_write(0x20 | vlcd_resistor_ratio, UC1701_COMMAND);
}




/*
    Set contrast

    'contrast' must be supplied 6 bit value.
    The 2 most significant bits must be '0'.
*/

void uc1701_set_contrast(uint8_t contrast)
{
  uc1701_write(0x81, UC1701_COMMAND);
  uc1701_write(contrast, UC1701_COMMAND);
}




/*
    Set all pixels on

    Use the defintions bellow to either:
    - set all display's pixels on independent of DDRAM contents, or
    - set the dsplay back to it's normal operation. 
*/

#define UC1701_ALL_PIXELS_ON 0x01
#define UC1701_ALL_PIXELS_NORMAL 0x00

void uc1701_set_all_pixels(uint8_t all_pixels_status)
{
  uc1701_write(0xa4 | all_pixels_status, UC1701_COMMAND);
}




/*
    Set inverse display

    Use the definitions bellow to either:
    - invert all display pixels, or
    - set all display pixels back to normal.
*/

#define UC1701_DISPLAY_INVERSE 0x01
#define UC1701_DISPLAY_NORMAL 0x00

void uc1701_set_inverse_display(uint8_t inverse)
{
  uc1701_write(0xa6 | inverse, UC1701_COMMAND);
}




/*
    Set display enable

    Use the definitions bellow to either:
    - get the display out of sleep, or
    - put the display to sleep.
*/

#define UC1701_DISPLAY_ENABLE 0x01
#define UC1701_DISPLAY_DISABLE 0x00

void uc1701_set_display_enable(uint8_t sleep)
{
  uc1701_write(0xae | sleep, UC1701_COMMAND);
}




/*
    Set SEG direction

    Use the definitions bellow to either:
    - mirror the the display horizontally, or
    - set the display back to normal.
*/

#define UC1701_MIRROR_X 0x01
#define UC1701_NORMAL_X 0x00

void uc1701_set_seg_dir(uint8_t mirror_x)
{
  uc1701_write(0xa0 | mirror_x, UC1701_COMMAND);
}




/*
    Set COM direction

    Use the definitions bellow to either:
    - mirror the the display vertically, or
    - set the display back to normal.
*/

#define UC1701_MIROR_Y 0x08
#define UC1701_NORMAL_Y 0x00

void uc1701_set_com_dir(uint8_t mirror_y)
{
  uc1701_write(0xc0 | mirror_y, UC1701_COMMAND);
}




/*
    System reset.
*/

void uc1701_system_reset(void)
{
  uc1701_write(0xe2, UC1701_COMMAND);
}




/*
    Set LCD bias ratio.

    Use the definitions bellow to set the LCD's bias ratio depending
    on mux's duty cycle as described in the following table:

    Duty  BR=0  BR=1
    1/65  1/9   1/7
    1/49  1/8   1/6
    1/33  1/6   1/5
    1/55  1/8   1/6
*/

#define UC1710_BR_SET 0x01
#define UC1710_BR_CLR 0x00

void uc1701_set_lcd_bias_ratio(uint8_t bias_ratio)
{
  uc1701_write(0xa2 | bias_ratio, UC1701_COMMAND);
}




/*
    Set cursor update mode
*/

void uc1701_set_cursor_update_mode(void)
{
  uc1701_write(0xe0, UC1701_COMMAND);
}




/*
    Reset cursor update mode
*/

void uc1701_reset_cursor_update_mode(void)
{
  uc1701_write(0xee, UC1701_COMMAND);
}




/*
    Set advanved program control 0.

    Use the definitions bellow to:
    - set the temperature compensation curve to either -0.11%/C or -0.05%/C.
    - set / reset the column address wrap around function.
    - set / reset the page address wrap around function.
*/

#define UC1701_TC_011 0x80
#define UC1701_TC_005 0x00
#define UC1701_COLUMN_WRAP 0x02
#define UC1701_COLUMN_NO_WRAP 0x00
#define UC1701_PAGE_WRAP 0x01
#define UC1701_PAGE_NO_WRAP 0x00

void uc1701_set_adv_program_control_0(uint8_t control_bits)
{
  uc1701_write(0xfa, UC1701_COMMAND);
  uc1701_write(0x10 | control_bits, UC1701_COMMAND);
}




// ___ Main ___

/*
    Sets the LCD's data address, effectively moving the cursor
    to a new position, based on display 'line' and 'column' values.
*/

#ifdef UC1701_CURSOR_MOVE
void uc1701_cursor_move(uint8_t line, uint8_t column)
{
  uc1701_set_column_address(column * 6);
  uc1701_set_page_address(line);
}
#endif




/*
    CLS.
*/

void uc1701_cls(void)
{
  uint8_t line, column;
  for(line = 0; line < 8; line++)
  {
    uc1701_cursor_move(line, 0);
    for(column = 0; column < 102; column++)
      uc1701_write(0x00, UC1701_DATA);
  }
}




/*
   LCD power up.
*/

void uc1701_power_up(void)
{
  // Power on.
  uc1701_pwr_on();
  // Wait 200ms for the power to settle and the LCD to startup.
  delay(_200ms_);
  // Set normal Y (not mirrored).
  uc1701_set_com_dir(UC1701_NORMAL_Y);
  // Set LCD's bias ratio to 1/9 (duty is 1/65 for eadogs102).
  uc1701_set_lcd_bias_ratio(UC1710_BR_CLR);
  // Turn on booster, regulator and follower.
  uc1701_set_power_control( UC1701_BOOSTER_ON | UC1701_REGULATOR_ON | UC1701_FOLLOWER_ON );
  // Set vlcd resistor ratio.
  uc1701_set_vlcd_resistor_ratio(0x07);
  // Set contrast.
  uc1701_set_contrast(UC1701_CONTRAST);
  // Set temperature compensation curve, column address wrap on and page address wrap on.
  uc1701_set_adv_program_control_0( UC1701_TC_011 | UC1701_COLUMN_NO_WRAP | UC1701_PAGE_NO_WRAP );
  // Enable the display.
  uc1701_set_display_enable(UC1701_DISPLAY_ENABLE);
  // Clear the screen from random data after power up.
  uc1701_cls();
}




/*
    LCD power down.
*/

void uc1701_power_down(void)
{
  // Necessary sequence for proper power down.
  uc1701_set_display_enable(UC1701_DISPLAY_DISABLE);
  uc1701_set_all_pixels(UC1701_ALL_PIXELS_ON);
  uc1701_system_reset();
  delay(_10ms_);
  uc1701_pwr_off();
}




/*
    Print a single character.
*/

void uc1701_print_symbol(char symbol)
{
  uint8_t i;
  // get the symbol's starting address based on the charset's starting address and the symbol's ascii code
  PGM_P symbol_address = latin_charset + ( (symbol - 32) * 5 );
  // send the symbol's 5 bitmap bytes to the display
  for(i = 0; i < 5; i++ )
    uc1701_write(pgm_read_byte_near(symbol_address++), UC1701_DATA);
  // add a blank separator column next to the symbol
  uc1701_write(0x00, UC1701_DATA);
}




/*
    Print a program flash stored string at the cursor's current position.
*/

#ifdef UC1701_PRINT_STR_P
void uc1701_print_str_P(PGM_P text)
{
  uint8_t buff;
  while( (buff = pgm_read_byte_near(text++)) != '\0' ) uc1701_print_symbol(buff);
}
#endif




/*
    Print a SRAM stored string at the cursor's current position.
*/

#ifdef UC1701_PRINT_STR
void uc1701_print_str(char *text)
{
  while(*text != '\0') uc1701_print_symbol(*text++);
}
#endif




/*
    Print an unsigned 8 bit decimal number at the cursor's current position.
*/

#ifdef UC1701_PRINT_DEC_U8
void uc1701_print_dec_u8(uint8_t value)
{
  uint8_t i;
  i = 0; while(value >= 100) { value -= 100; i++; } uc1701_print_symbol('0' + i);
  i = 0; while(value >= 10) { value -= 10; i++; } uc1701_print_symbol('0' + i);
  uc1701_print_symbol('0' + value);
}
#endif




/*
    Print a signed 8 bit decimal number at the cursor's current position.
*/

#ifdef UC1701_PRINT_DEC_S8
void uc1701_print_dec_s8(int8_t value)
{
  if(value < 0)
  { value = -value; uc1701_print_symbol('-'); }
  else
    uc1701_print_symbol('+');

  uc1701_print_dec_u8((uint8_t) value);
}
#endif




/*
    Print a 16 bit decimal number at the cursor's current position.
*/

#ifdef UC1701_PRINT_DEC_U16
void uc1701_print_dec_u16(uint16_t value)
{
  uint16_t i;
  i = 0; while(value >= 10000) { value -= 10000; i++; } uc1701_print_symbol('0' + i);
  i = 0; while(value >= 1000) { value -= 1000; i++; } uc1701_print_symbol('0' + i);
  i = 0; while(value >= 100) { value -= 100; i++; } uc1701_print_symbol('0' + i);
  i = 0; while(value >= 10) { value -= 10; i++; } uc1701_print_symbol('0' + i);
  uc1701_print_symbol('0' + value);
}
#endif




/*
    Print a 8 bit hexadecimal number at the cursor's current position.
*/

#ifdef UC1701_PRINT_HEX_U8
void uc1701_print_hex_u8(uint8_t value)
{
  uint8_t val;
  val = '0' + (value >> 4); if(val > '9') val += 7; uc1701_print_symbol(val);
  val = '0' + (value & 0x0f); if(val > '9') val += 7; uc1701_print_symbol(val);
}
#endif




/*
    Print a 16 bit hexadecimal number at the cursor's current position.
*/

#ifdef UC1701_PRINT_HEX_U16
void uc1701_print_hex_u16(uint16_t value)
{
  uint8_t val;
  val = '0' + ((value >> 12) & 0x000f); if(val > '9') val += 7; uc1701_print_symbol(val);
  val = '0' + ((value >> 8) & 0x000f); if(val > '9') val += 7; uc1701_print_symbol(val);
  val = '0' + ((value >> 4) & 0x000f); if(val > '9') val += 7; uc1701_print_symbol(val);
  val = '0' + (value & 0x000f); if(val > '9') val += 7; uc1701_print_symbol(val);
}
#endif

