



#include <16F877.h>
#fuses HS,NOWDT,NOPROTECT,PUT

#use delay(clock=20000000)

#use fast_io(A)
#use fast_io(B)

#define SIG_DATA PIN_C3
#define SIG_STROBE PIN_C4
#define SIG_CLOCK PIN_C5

#define SIG_PWR_ENABLE PIN_C0
#define SIG_BLANK PIN_C6


void shiftout(int16 databyte)
{
 int16 i;

 for (i=0; i<10; i++) {
  	output_bit(SIG_DATA,(databyte & ((int16)(0x1))) ); 	

// output_bit(SIG_DATA,databyte );
    databyte >>= 1;
    delay_cycles(20);
    output_high(SIG_CLOCK);
    delay_cycles(20);
    output_low(SIG_CLOCK);
 }
  
}

void strobe()
{
    output_high(SIG_STROBE);
    delay_cycles(20);
    output_low(SIG_STROBE);
	delay_cycles(20);
}

int16 buffer[5][2];

#int_timer0
void RTCC_ISR()
{
   static unsigned int8 ziffer = 0;
   output_high(SIG_BLANK);
   if (ziffer == 4) ziffer = 0;
   else ziffer++;
   shiftout(buffer[ziffer][1]);
   shiftout(buffer[ziffer][0]);
   strobe();
   output_low(SIG_BLANK);
}


void resetbuffer()
{
	int j;

    for (j = 0; j < 5; j++) {
       if (j == 0) {
         buffer[j][0] = (int16)( ((int16)0x1 << ((int16)8)) );
         buffer[j][1] = (int16)(                           0);
        } else if (j == 4) {
         buffer[j][0] = (int16)( ((int16)0x1 << ((int16)9)) );
         buffer[j][1] = (int16)(                           0);
     } else {
         buffer[j][0] = (int16)(                           0);
         buffer[j][1] = (int16)( ((int16)0x1 << ((int16)6+j)));
        }
    }

}

void main()
{
	int number[14];
    int1 direction[14];

    int i;
    int j;
    int k;

	set_tris_a(0);
	set_tris_b(0);

    output_low(SIG_CLOCK);
    output_low(SIG_DATA);
    output_low(SIG_STROBE);

    setup_timer_0(RTCC_DIV_32);
    enable_interrupts(INT_TIMER0); // multiplex display timer
    enable_interrupts(GLOBAL);

    delay_ms(50);

	output_high(SIG_PWR_ENABLE);

	while (true) {


	  number[0] = 1;
	  direction[0] = 1;
	  number[1] = 2;
	  direction[1] = 1;
	  number[2] = 3;
	  direction[2] = 1;
	  number[3] = 4;
	  direction[3] = 1;
	  number[4] = 5;
	  direction[4] = 1;

      for (k = 0; k < 100; k++) {

		delay_ms(80);

        for (j = 0; j < 5; j++) {

	        if (direction[j]) {
	
		     if (number[j] == 5) {
				direction[j] = 0;
	         }
	         number[j]++;
	
	        } else {
	
		     if (number[j] == 1) {
				direction[j] = 1;
	         }
	         number[j]--;
	
	        }
            if (j == 0) {
             buffer[j][0] = (int16)( ((int16)0x1 << ((int16)8)) | ((int16)1 << number[j]));
             buffer[j][1] = (int16)(                              ((int16)1 << number[j]));
            } else if (j == 4) {
             buffer[j][0] = (int16)( ((int16)0x1 << ((int16)9)) | ((int16)1 << number[j]));
             buffer[j][1] = (int16)(                              ((int16)1 << number[j]));
	        } else {
             buffer[j][0] = (int16)(                                ((int16)1 << number[j]));
             buffer[j][1] = (int16)( ((int16)0x1 << ((int16)6+j)) | ((int16)1 << number[j]));
            }
        } // for j

      } // for k




      number[0] = 2;
      direction[0] = 1;

      number[1] = 9;
      direction[1] = 0;

      number[2] = 2;
      direction[2] = 1;

      number[3] = 9;
      direction[3] = 0;

      number[4] = 2;
      direction[4] = 1;

      number[5] = 9;
      direction[5] = 0;

      number[6] = 2;
      direction[6] = 1;

      number[7] = 9;
      direction[7] = 0;

      number[8] = 2;
      direction[8] = 1;

      number[9] = 9;
      direction[9] = 0;

      number[10] = 2;
      direction[10] = 1;

      number[11] = 9;
      direction[11] = 0;

      number[12] = 2;
      direction[12] = 1;

      number[13] = 9;
      direction[13] = 0;

      for (k = 0; k < 100; k++) {

		delay_ms(80);

		resetbuffer();

        for (j = 0; j < 14; j++)
         {
	        if (direction[j]) {
	
		     if (number[j] == 9) {
				direction[j] = 0;
	         }
	         number[j]++;
	
	        } else {
	
		     if (number[j] == 2) {
				direction[j] = 1;
	         }
	         number[j]--;	
	        }

            for (i = 0; i < 5; i++) {
              if ( (number[j] > (i+1)) && (number[j] < (i+6)) ) {
                if (j%2)
				 buffer[i][0] |= ((int16)1 << (int16)(j >> 1));
                else
				 buffer[i][1] |= ((int16)1 << (int16)(j >> 1));
			  }
            }
         }

      } // for k





		resetbuffer();

		for (k = 0; k < 50; k++) {
	
			delay_ms(160);

			resetbuffer();
	
	        for (j = 0; j < 5; j++) {

				if (k & 0x1) {
                  buffer[j][0] |= 0b01111111;
                  buffer[j][1] |= 0b00000000;
                } else {
                  buffer[j][0] |= 0b00000000;
                  buffer[j][1] |= 0b01111111;
                }

			}
		  
			
		}


		resetbuffer();

        for (i = 0; i < 2; i++) {
	
		    number[0] = 0;
		    number[1] = 0;
		    number[2] = 0;
		    number[3] = 0;
		    number[4] = 0;
	
	        for (j = 0; j < 5; j++) {
		
		      for (k = 0; k < 7; k++) {
		
				delay_ms(80);
		
		        buffer[j][0] |= ((int16)1 << number[j]);
		        buffer[j][1] |= ((int16)1 << number[j]);
	
		        number[j]++;
	
		       } // for j
		
		     } // for k
		
		     number[0] = 0;
		     number[1] = 0;
		     number[2] = 0;
		     number[3] = 0;
		     number[4] = 0;
	
	         for (j = 0; j < 5; j++) {
		
		       for (k = 0; k < 7; k++) {
		
				delay_ms(80);
		
		        buffer[j][0] &= ~((int16)1 << number[j]);
		        buffer[j][1] &= ~((int16)1 << number[j]);
	
		        number[j]++;
	
		       } // for j
		
		     } // for k
	

         } // for i


	} // while
}