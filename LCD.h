#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>   // Auth xreiazetai gia thn itoa() 

#define CONTROL PORTB
#define READ PORTB | (1<<1)
#define WRITE PORTB & ~(1<<1)
#define DATA PORTB | (1<<0)
#define COMMAND PORTB & ~(1<<0)
#define INPUT 0x00
#define OUTPUT 0xFF
#define ENABLE PORTB | (1<<2);
#define DISABLE PORTB & ~(1<<2);


//=======================================  4  BASIC FUNCTIONS FOR THE LCD ====================================================================

void check_if_LCD_busy(void);//======================= This checks if the LCD is busy and waits until the LCD is free for next instruction
void LCD_command(unsigned char something );//========= THis Sends a command to the LCD
void LCD_data(unsigned char something ); //=========== This Prints a character on the LCD's DDRAM 
void enable_ON_OFF(void);//=========================== This clocks in the character or command in the LCD

//========================= WITH THE ABOVE 4 BASIC FUNCTIONS I CAN BUILD ANY OTHER FUNCTION I WANT FOR THE LCD ============================== 

void LCD_init(void);//======================================= Initialize the LCD << VERY IMPORTANT >> =======================================
void LCD_string2x16( char string[]);//======================= Prints a string on the LCD. ONLY if the string starts from top left corner=====
									//======================= EXAMPLE --> LCD_string2x16("Geia sou Li"); ====================================
void LCD_string_xy( char string[], unsigned char grammh, unsigned char sthlh);// Print a string starting from a specific location ==========
									//======================= EXAMPLE --> LCD_string_xy("Geia sou Li", 2, 5); ===============================
void LCD_xy(unsigned char grammh, unsigned char sthlh);//==== Set the Cursor's position to a specific location ==============================
									//======================= EXAMPLE --> LCD_xy(2, 5); =====================================================
void LCD_space(unsigned char grammh, unsigned char sthlh);//= Place a space character at a specific location ================================
									//======================= EXAMPLE --> LCD_space(1, 3); =====================================================
void LCD_clear_screen(void);//=============================== Clear the LCD and send the Cursor at the top left corner ======================


//==================================================== LCD FUNCTION DEFINITIONS ==============================================================

void LCD_init(void)
{
	
	_delay_ms(20);
	check_if_LCD_busy();
	CONTROL = WRITE;
	CONTROL = COMMAND;
	PORTA = 0x30;
	
	CONTROL = ENABLE;
	_delay_ms(20);
	CONTROL = DISABLE;
	
	_delay_ms(20);
	
	CONTROL = ENABLE;
	_delay_ms(20);
	CONTROL = DISABLE;

    _delay_ms(20);
	
	CONTROL = ENABLE;
	_delay_ms(20);
	CONTROL = DISABLE;
	
	LCD_command(0x0E);   // Display ON, Cursor ON, Cursor Blink OFF
	_delay_ms(10);
	LCD_command(0x38);	 // 8-bit interface, Display in 2 lines
	_delay_ms(10);
	LCD_command(0x01);	 // Clear Screen
	_delay_ms(10);

}	


void check_if_LCD_busy(void)
{	unsigned char aaa;
	DDRA = INPUT;
	PORTA = 0x00;
	CONTROL = READ;
	CONTROL = COMMAND;
	do
	{
		CONTROL = ENABLE;
	    asm volatile ("nop");
		asm volatile ("nop"); 
		aaa = PINA;
	    CONTROL = DISABLE;
	}while (aaa >= 0x80);
	DDRA = OUTPUT;
}

void LCD_command(unsigned char something)
{
	check_if_LCD_busy();
	
	CONTROL = WRITE;
	CONTROL = COMMAND;
	PORTA = something;
	enable_ON_OFF();
	check_if_LCD_busy();
}	


void LCD_data(unsigned char something )
{
	check_if_LCD_busy();
	
	CONTROL = WRITE;
	CONTROL = DATA;
	PORTA = something;
	enable_ON_OFF();
	check_if_LCD_busy();
}	

void enable_ON_OFF()
{
	PORTB = ENABLE;
	asm volatile ("nop");
	asm volatile ("nop"); 
	PORTB = DISABLE;
	
}

void LCD_string_xy( char string[], unsigned char grammh, unsigned char sthlh)
{
	unsigned char i = 0;
	
	LCD_xy(grammh, sthlh);
	
	while( string[i]  != '\0')
	{
		LCD_data(string[i]);
		i++;
	}
}

void LCD_string2x16( char string[])    
{
	unsigned char i = 0;
	unsigned char check_flag = 0;
	
	while( string[i]  != '\0')
	{
		if (i < 16) 
		{
			if( check_flag == 1)
			{
				check_flag = 0;
			}
			LCD_data(string[i]);
		}
		
		if ( (i >= 16) && (i<32) )
		{
			if (check_flag == 0)
			{
				LCD_xy(2, 0);    
				check_flag = 1;
			}
			LCD_data(string[i]);
			
		}
		
		i++;
		
	}
}

void LCD_xy( unsigned char grammh, unsigned char sthlh)
{
	unsigned char xy = 0;
	
	if (grammh == 1)
	{
		if( sthlh < 16)
		{
			xy = 0x80 | sthlh;
			LCD_command(xy);
		}
	}
	
	if (grammh == 2)
	{
		if(sthlh < 16)
		{
			xy = 0x80 | 0x40 | sthlh;
			LCD_command(xy);
		}
	}
}

void LCD_space(unsigned char grammh, unsigned char sthlh)
{
	LCD_xy(grammh, sthlh);
	LCD_data(' ');
}

void LCD_clear_screen(void)
{
	LCD_command(0x01); //clear screen
	LCD_command(0x02); //cursor home
}
#endif
