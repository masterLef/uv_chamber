#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>   // ========== Auth einai gia thn itoa()
#include "LCD.h"
#include "KEYPAD.h"

int main (void)
{
	//================= LCD BUS AND CONTROL PINS========================
	DDRA = OUTPUT; // BUS
    DDRB = 0x07; // CONTROL PINS RS RW E  of the LCD 
	 
	 //====================== KEYPAD ===================================
	
						 
	unsigned char character=0;
	//====================================================
	 char input[] = {0, 0, 0, 0, 0};
	 char output[] = {0, 0, 0, 0, 0};
	
	 DDRD = 0xFF;
	 PORTD = 0x00;

	//=================================================================================================================
	
	LCD_init();
	
	LCD_string2x16("< HELLO WRE! >");
	_delay_ms(1000);
	LCD_clear_screen();
	
	LCD_string2x16("Dwse 1o ari8mo: ");
	character = wait_until_key_is_pressed(stoixeio);
	while((character == '*') || (character == '#') || (character == 'A') || (character == 'B') || (character == 'C') || (character == 'D'))
	{
		LCD_string_xy("ARI8MO RE...", 2, 0);
		_delay_ms(1000);
		LCD_string_xy("            ", 2, 0);
		character = wait_until_key_is_pressed(stoixeio);
	}
	input[0] = character;
	LCD_clear_screen();
	LCD_string2x16("Edwses ari8mo:");
	LCD_data(character);
	_delay_ms(1000);
	
	LCD_clear_screen();
	LCD_string2x16("Dwse 2o ari8mo: ");	
	character = wait_until_key_is_pressed(stoixeio);
	while((character == '*') || (character == '#') || (character == 'A') || (character == 'B') || (character == 'C') || (character == 'D'))
	{
		LCD_string_xy("ME DOULEUEIS RE?", 2, 0);
		_delay_ms(1000);   
		LCD_string_xy("                ", 2, 0);
		character = wait_until_key_is_pressed(stoixeio);
	}
	input[1] = character;
	LCD_clear_screen();
	LCD_string2x16("Edwses ari8mo:");
	LCD_data(character);
	_delay_ms(1000);
	
	LCD_clear_screen();
	LCD_string_xy("min <<< >>> sec ", 1, 0);
	LCD_string_xy(" A           B  ", 2, 0);
	character = wait_until_key_is_pressed(stoixeio);
	while(!((character == 'A') || (character == 'B')))
	{
		LCD_string_xy("                ", 2, 0);
		LCD_string_xy("PIWMENOS EISAI?", 2, 0);
		_delay_ms(700);   
		LCD_string_xy(" A           B  ", 2, 0);
		character = wait_until_key_is_pressed(stoixeio);
	}
	//======================================================================
	char min_sec = 0;
	
	if( character == 'A')
	{	min_sec = 1;
		LCD_string_xy("Epelexes MIN:", 2, 0);
	}
	if( character == 'B')
	{
		min_sec = 0 ;
		LCD_string_xy("Epelexes SEC:", 2, 0);
	}
	LCD_string_xy(input, 2, 13);
	_delay_ms(2000);
	LCD_clear_screen();
	
	//======================================================================
							
	LCD_string_xy("TURN OFF IN:", 1, 0);
    int number = atoi( input );	
	
	int num;
	if(min_sec == 1)
	{num = number*60;}
	if(min_sec == 0)
	{num = number;}
	
	PORTD |= 0x20; //================================= APO to pin 5 (0..7) tou PORTD vgainei to shma gia ta UV led
	for(int t= num-1; t>=0 ; t--)
	{
		
		if(t>9)
		{
			LCD_string_xy("     ", 1, 12);
			LCD_string_xy("     ", 2, 12);
			itoa(t, output, 10);
			LCD_string_xy(output, 1, 12);
			itoa((t/60), output, 10);
			LCD_string_xy(">>> Minutes:", 2, 0);
			LCD_string_xy(output, 2, 12);
		}
		else if( t==0)
		{
			LCD_xy(1,12);
			LCD_data('0');
			LCD_data('0');
			break;
		}
		else
		{
			itoa(t, output, 10);
			LCD_xy(1,12);
			LCD_data('0');
			LCD_string_xy(output, 1, 13);
			itoa((t/60), output, 10);
			LCD_string_xy(">>> Minutes:", 2, 0);
			LCD_string_xy(output, 2, 12);
		}
		_delay_ms(1000);		
	}
	PORTD &= 0x00;
	_delay_ms(500);
	PORTD |= 0b00000100;       //0b01000000 palio
	
	for( char n=0; n<6; n++)
	{
		for(char a = 0; a<3; a++)
		{
			LCD_clear_screen();
			LCD_string_xy("=== COMPLETE ===", 1, 0);
			for(unsigned int b0 = 0; b0< 700; b0++)
			{
				PORTD ^=0x02;
				PORTD ^=0x04;
				_delay_us(150);
				
			}

			LCD_clear_screen();
			LCD_string_xy("=== COMPLETE ===", 2, 0);
			for(unsigned int b1 = 0; b1< 700; b1++)
			{
				PORTD ^=0x02;
				PORTD ^=0x04;
				_delay_us(150);
			}
		}
		_delay_ms(750);
	}
	
	
	
	while(1)
	{
		
		
	}
	
	return 0;

}





		
		