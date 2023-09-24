#ifndef KEYPAD_H
#define KEYPAD_H

#include <avr/io.h>
#include <util/delay.h>

#define keypad_PORT PORTC
#define keypad_DDR DDRC   // These should be changed if another PORT is used for keypad
#define keypad_PIN PINC

//================================================ Function Declerations ==========================================

unsigned char keypad(unsigned char matrix[][4]); 
unsigned char wait_until_key_is_pressed(unsigned char matrix[][4]); 

//================================================ Function Definitions ===========================================

unsigned char stoixeio[4][4] ={{'1','2','3','A'},  //---- This matrix keeps the character values mapped according to the keypad positions 
								{'4','5','6','B'},
								{'7','8','9','C'},
								{'*','0','#','D'}};
								

unsigned char keypad(unsigned char matrix[][4])
{
	unsigned char character = 0xFF; //------Set an initial value to var character so as, in case no key is pressed this value (0xFF) will
									//------ be returned from function. Value 0xFF has been chosen because there is no such value in ASCII 
									//------ code to mean something (e.g. some character)
	
	keypad_DDR = 0x00;    
	keypad_PORT = 0xF0;//-------------------- Enable the Pull- up resistors for the columns (bits 7..4), make the row under test zero and 
				       //-------------------- other rows High Z
	
	for(unsigned char row = 0; row<4; row++) //---------------- For every row
	{
		
		keypad_DDR = (1<<row);//---------- Set only the row you want to test as output, all other rows as inputs and all columns as inputs
		
					 
		for(unsigned char col = 0; col<4; col++)//------------- for every column
		{
			_delay_ms(5);//------------------- Put A small delay before checking the key, just to make sure that the key is pressed for good
			
			if(!(keypad_PIN & (1<<(col+4))))  // Since its pressed, fetch from stoixeio[][] the exact character for the chosen row and column 

			{	
				 character = matrix[row][col];
				while(!(keypad_PIN & (1 << (col+4))))  // after the key is brought from stoixeio[][] Display a message to release the key 
				{ 									   // and repeat until the key on the keypad is released
					LCD_string_xy("OK", 2, 0);
					LCD_data(0x7E);
					LCD_data(character);
					LCD_string_xy("             ", 2, 4);
					_delay_ms(300); // ---------------------- Leave the printed message 300msec before checking again if key is released
					
				}
				_delay_ms(5);//--------------------- A small delay after checking the key to make sure that the key is released for good
			}
		}
	}
	return character;  //========== export the fetched key (e.g. character '2' from matrix stoixeio[][])
}

unsigned char wait_until_key_is_pressed(unsigned char matrix[][4])// this function holds the program from going further til a key is pressed
{
	unsigned char character1;
	
	do
	{
		character1 = keypad(matrix); // while no key is pressed (0xFF from above function) check again
	}
	while( character1 == 0xFF);
	return character1;  //  if some key is pressed (not 0xff) return the value of the key (e.g. character '2' from matrix stoixeio[][]
} 


#endif