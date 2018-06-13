/*
 * main.c
 *
 *  Created on: May 5, 2018
 *      Author: ahmedsafwat
 */

#include <avr/io.h>
#include "UART.h"

#define Round_constant1 (0x80)
#define Round_constant2 (0x30)

unsigned char S_BOX(unsigned char input){
    unsigned char output_i = 0x00;

    for(int i = 0; i < 2; i++){
        unsigned char input_i = input & (0x0f << (i*4) );
        input_i >>= (i*4);
        if( (input_i >> 2) == 0){
            if( (input_i & (0x03) ) == 0){
                output_i |= 0x09;
            }
            else if( (input_i & (0x03) ) == 1){
                output_i |= 0x04;
            }
            else if( (input_i & (0x03) ) == 2){
                output_i |= 0x0A;
            }
            else if( (input_i & (0x03) ) == 3){
                output_i |= 0x0B;
            }
        }
        else if( (input_i >> 2 ) == 1){
            if( (input_i & (0x03) ) == 0){
                output_i |= 0x0D;
            }
            else if( (input_i & (0x03) ) == 1){
                output_i |= 0x01;
            }
            else if( (input_i & (0x03) ) == 2){
                output_i |= 0x08;
            }
            else if( (input_i & (0x03) ) == 3){
                output_i |= 0x05;
            }
        }
        else if( (input_i >> 2 ) == 2){
            if( (input_i & (0x03) ) == 0){
                output_i |= 0x06;
            }
            else if( (input_i & (0x03) ) == 1){
                output_i |= 0x02;
            }
            else if( (input_i & (0x03) ) == 2){
                output_i |= 0x00;
            }
            else if( (input_i & (0x03) ) == 3){
                output_i |= 0x03;
            }
        }
        else if( (input_i >> 2) == 3){
            //printf("%s\n", "inside outer 3");
            if( (input_i & (0x03) ) == 0){
                output_i |= 0x0C;
            }
            else if( (input_i & (0x03) ) == 1){
                output_i |= 0x0E;
            }
            else if( (input_i & (0x03) ) == 2){
                output_i |= 0x0F;
            }
            else if( (input_i & (0x03) ) == 3){
                //printf("%s\n", "inside inner 3");
                output_i |= 0x07;
            }
        }
        output_i = ( (output_i & (0xf0) ) >> 4 ) | ( (output_i & (0x0f) ) << 4 );

    }
    return output_i;
}

unsigned char rot_nib(unsigned char input){
    input = ( (input & (0xf0) ) >> 4 ) | ( (input & (0x0f) ) << 4 );
    return input;
}

unsigned char multiply_in_mix_column(unsigned char constant, unsigned char data){
    if(constant == 1){
        return data;
    }
    if(constant == 4){
        if(data == 1){
            return 4;
        }
        else if(data == 2){
            return 8;
        }
        else if(data == 3){
            return 0xc;
        }
        else if(data == 4){
            return 3;
        }
        else if(data == 5){
            return 7;
        }
        else if(data == 6){
            return 0xB;
        }
        else if(data == 7){
            return 0xF;
        }
        else if(data == 8){
            return 6;
        }
        else if(data == 9){
            return 2;
        }
        else if(data == 0xA){
            return 0xE;
        }
        else if(data == 0xB){
            return 0xA;
        }
        else if(data == 0xC){
            return 5;
        }
        else if(data == 0xD){
            return 1;
        }
        else if(data == 0xE){
            return 0xD;
        }
        else if(data == 0xF){
            return 9;
        }
    }
    return 0;
}


int main(){
	UART_init();
	DDRD = 0xff;
	PORTD = 0xff;

	while(1){
		unsigned char x = UART_receive_byte();
		unsigned char y = UART_receive_byte();
		//UART_send_byte(x);
		//UART_send_byte(y);

		unsigned short int key = ((unsigned short)x) << 8 | ((unsigned short)y);
		unsigned char x1 = UART_receive_byte();
		unsigned char y1 = UART_receive_byte();
		//UART_send_byte(x1);
		//UART_send_byte(y1);
		unsigned short int P_text = ((unsigned short)x1) << 8 | ((unsigned short)y1) ;;

		/*unsigned short int key = 0x4AF5;
		unsigned short int P_text = 0xD728;*/

		unsigned char W1 = key;
		unsigned char W0 = (key >> 8);

		unsigned char W2, W3, W4, W5;
		W2 = W0 ^ Round_constant1 ^ S_BOX(rot_nib(W1));

		W3 = W2 ^ W1;

		W4 = W2 ^ Round_constant2 ^ S_BOX(rot_nib(W3));

		W5 = W3 ^ W4;

		unsigned short int K1 = (unsigned short)(W2 << 8) | (unsigned short)(W3);
		unsigned short int K2 = (unsigned short)(W4 << 8) | (unsigned short)(W5);

		//printf("%x\n", K1);
		//printf("%x\n", K2);
	//////////////////////////////////////////////////////
	//Round 1

		unsigned short int cipher_text = P_text ^ key;
		//printf("cipher 1 : %x\n", cipher_text);
		//Substitution
		cipher_text = (unsigned short int)(S_BOX( (unsigned char)cipher_text))  | (unsigned short int)S_BOX((unsigned char)(cipher_text >> 8) ) << 8;

		//Shift row
		cipher_text = (cipher_text & (0xf0f0)) | ((cipher_text >> 8) & (0x000f)) | ( (cipher_text << 8) & (0x0f00));

		//mix column
		unsigned char first_nibble, second_nibble, third_nibble, fourth_nibble;
		unsigned char first_nibble2, second_nibble2, third_nibble2, fourth_nibble2;

		first_nibble = (cipher_text & (0xf000)) >> 12;
		second_nibble = (cipher_text & (0x0f00)) >> 8;
		third_nibble = (cipher_text & (0x00f0)) >> 4;
		fourth_nibble = (cipher_text & (0x000f));




		first_nibble2 = multiply_in_mix_column(1, first_nibble) ^ multiply_in_mix_column(4, second_nibble);
		second_nibble2 = multiply_in_mix_column(4, first_nibble) ^ multiply_in_mix_column(1, second_nibble);
		third_nibble2 = multiply_in_mix_column(1, third_nibble) ^ multiply_in_mix_column(4, fourth_nibble);
		fourth_nibble2 = multiply_in_mix_column(4, third_nibble) ^ multiply_in_mix_column(1, fourth_nibble);

		/*printf(";;%x\n", first_nibble2);
		printf(";;%x\n", second_nibble2);
		printf(";;%x\n", third_nibble2);
		printf(";;%x\n", fourth_nibble2);*/
		//printf("before :  %x\n", cipher_text);

		cipher_text = ( (unsigned short)first_nibble2) << 12 | ((unsigned short)second_nibble2) << 8 | ( (unsigned short)third_nibble2) << 4 | ( (unsigned short)fourth_nibble2);

		//printf("cipher mix %x\n", cipher_text);


		//Adding key
		cipher_text ^= K1;

		//printf("cipher %x\n", cipher_text);

////////////////////////////////////////////////
//Round 2

		//Substitution
		cipher_text = (unsigned short int)(S_BOX( (unsigned char)cipher_text))  | (unsigned short int)S_BOX((unsigned char)(cipher_text >> 8) ) << 8;

		//Shift row
		cipher_text = (cipher_text & (0xf0f0)) | ((cipher_text >> 8) & (0x000f)) | ( (cipher_text << 8) & (0x0f00));


		//Adding key
		cipher_text ^= K2;

		if(cipher_text == 0x24ec){
			PORTD &= ~(1 << 5);
		}

		//printf("cipher final %x\n", cipher_text);

		UART_send_byte((unsigned char) (cipher_text >> 8) );
		UART_send_byte((unsigned char) (cipher_text) );

	}


}
