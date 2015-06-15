#include <avr/pgmspace.h>
#include "nmoled.h"
#include <stdlib.h>
#include <SPI.h>
#include"LCD_font_5x7.h"
#include"LCD_font_7x8.h"

extern  const uint8_t PROGMEM font_5x7[];
extern  const uint8_t PROGMEM font_7x8[];

void setpixel(int x,int y,int colour)
{
	if((x>=SSD1306_LCDWIDTH)||(y>=SSD1306_LCDHEIGHT)) return;
	Serial.begin(9600);
	if(colour==WHITE){
		int i,bit1;
		i=x+(y/8)*SSD1306_LCDWIDTH; //index of the byte
		bit1=y%8; // finds out the bit to glow
		buffer[i]=buffer[i] | 1<<bit1; // dont disturb the excisting led's status
		//  Serial.println(bit1); //DEBUG println the buffer index
	}
	if(colour==BLACK){
		int i,bit1,k=0;
		i=x+(y/8)*SSD1306_LCDWIDTH; //index of the byte
		bit1=y%8; // finds out the bit to glow
		if((buffer[i] & (1<<bit1))>=1){  // seems the condition gives out the byte value thts y its >=
			uint8_t temp=0;
			temp=temp | 1<<bit1;
			buffer[i]=buffer[i] ^ temp;
			//Serial.println("inside if");
		}
		else
		{
			// its already black so nothg to do with it
		}
	}
}

void ssd1306_data(uint8_t c)
{
	digitalWrite(dc,HIGH);
	fastSPIwrite(c);	
}

void ssd1306_command(uint8_t c)
{
	digitalWrite(dc,LOW);
	fastSPIwrite(c);	
}

void display_buffer()
{
	ssd1306_command(SSD1306_COLUMNADDR);
	ssd1306_command(0);   // Column start address (0 = reset)
	ssd1306_command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

	ssd1306_command(SSD1306_PAGEADDR);
	ssd1306_command(0); // Page start address (0 = reset)
	ssd1306_command(7); // Page end address
	digitalWrite(dc,HIGH);

	for (uint16_t i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++) {
		fastSPIwrite(buffer[i]);

	}
}

void clearDisplay() {
	memset(buffer, 0, (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8));
}


void fastSPIwrite(uint8_t c)
{
	SPI.transfer(c);
}

void lcdInit()
{
	pinMode(dc, OUTPUT);
	SPI.begin ();
	pinMode(rst, OUTPUT);
	digitalWrite(rst, HIGH);
	delay(1);
	// bring reset low
	digitalWrite(rst, LOW);
	// wait 10ms
	delay(10);
	// bring out of reset
	digitalWrite(rst, HIGH);

	ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
	ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
	ssd1306_command(0x80);                                  // the suggested ratio 0x80
	ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
	ssd1306_command(0x3F);
	ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
	ssd1306_command(0x0);                                   // no offset
	ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
	ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
	ssd1306_command(0x14); //decided by printing else part
	ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
	ssd1306_command(0x00);                                  // 0x0 act like ks0108
	ssd1306_command(SSD1306_SEGREMAP | 0x1);
	ssd1306_command(SSD1306_COMSCANDEC);
	ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
	ssd1306_command(0x12);
	ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
	ssd1306_command(0xCF); //decided***
	ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
	ssd1306_command(0xF1); //decided***
	ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
	ssd1306_command(0x40);
	ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
	ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
	//ssd1306_command(SSD1306_INVERTDISPLAY);                 // 0xA6

	ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel

}
void verticalLine(int x,int y,int h,int colour)
{
	if(h>64)return;
	int i,j;

	for(i=y;i<=h;i++)
	{
		setpixel(x,i,colour);
	}
}

void horizontalLine(int x,int y,int w,int colour)
{
	int i,j;
	if(w>128) return;
	for(i=x;i<w;i++)
	{
		setpixel(i,y,colour);
	}
}

void rect(int x,int y,int h,int w,int colour)
{

	if((x > SSD1306_LCDWIDTH || w > SSD1306_LCDWIDTH)||(y > SSD1306_LCDHEIGHT || h > SSD1306_LCDHEIGHT)) return; //value grater than the display size
	horizontalLine(x,y,x+w,colour);
	horizontalLine(x,y+h,x+w,colour);
	verticalLine(x,y,y+h,colour);
	verticalLine(w+x,y,y+h,colour);
}

void fill_rect(int x,int y,int h,int w,int colour)
{
	int i; 
	if((x > SSD1306_LCDWIDTH || w > SSD1306_LCDWIDTH)||(y > SSD1306_LCDHEIGHT || h > SSD1306_LCDHEIGHT)) return; //value grater than the display size
	if(x+w > SSD1306_LCDWIDTH){ w = 120;} //exits the width of the screen
	for(i=0;i<=h;i++)
		horizontalLine(x,y+i,x+w,colour); 
}

int setcursor(int x,int y)
{
	// tels the starting byte 
	if(x > SSD1306_LCDWIDTH || y > SSD1306_LCDHEIGHT) return 0;
	int arry_index=x+(y/8)*SSD1306_LCDWIDTH;
	return arry_index;

}

void putcharecter(int x,int y,char co,int colour,int textsize)
{
	int i,byte1,r,c,ch;
	int kk=0;
	char value;
	int index;
	Serial.begin(9600);
	char fontview[10];
	//Serial.println(fontview[1]);
#ifdef DEBUG	
	//	while(kk<5){	
	//	Serial.println(kk);
	//	delay(1000);
#endif
	if(textsize==1)
	{
		if(colour==WHITE)
		{
			for(c=0;c<5;c++){
				for(r=0;r<7;r++){
					byte1=(co-32)*5+c;
					fontview[c]=pgm_read_byte(&font_5x7[byte1]);
					if(fontview[c]&(1<<r)){setpixel(c+x,r+y,WHITE);}
					//Serial.println(k++);
				}
				kk++;
			}
		}

		if(colour==BLACK)
		{
			for(c=0;c<5;c++){
				for(r=0;r<7;r++){
					byte1=(co-32)*5+c;
					fontview[c]=pgm_read_byte(&font_5x7[byte1]);
					if(fontview[c]&(1<<r)){setpixel(c+x,r+y,BLACK);}
					//Serial.println(k++);
				}
				kk++;
			}
		}
	}
	if(textsize==2)
	{
		if(colour==WHITE)
		{
			for(c=0;c<7;c++){
				for(r=0;r<8;r++){
					byte1=(co-32)*7+c;
					fontview[c]=pgm_read_byte(&font_7x8[byte1]);
					if(fontview[c]&(1<<r)){setpixel(c+x,r+y,WHITE);}
					//Serial.println(k++);
				}
				kk++;
			}
		}

		if(colour==BLACK)
		{
			for(c=0;c<7;c++){
				for(r=0;r<8;r++){
					byte1=(co-32)*7+c;
					fontview[c]=pgm_read_byte(&font_7x8[byte1]);
					if(fontview[c]&(1<<r)){setpixel(c+x,r+y,BLACK);}
					//Serial.println(k++);
				}
				kk++;
			}
		}

	}

}

void putstring(int x,int y,char *s,int colour,int textsize)
{
	if (x > SSD1306_LCDWIDTH || y > SSD1306_LCDHEIGHT) return;
	int next=0;
	while(*s!='\0')
	{
		if(textsize==1)
		{
			if( (x+next) >122)  // end of a row
			{x=1;y+=8;next=0;}
			if(y > SSD1306_LCDHEIGHT - 8)
			{y = SSD1306_LCDHEIGHT - 8;}
			putcharecter(x+next,y,*s,colour,textsize);
			s++;
			next=next+6;
		}
		if(textsize==2)
		{
			if( (x+next) >120)  // end of a row
			{x=1;y+=10;next=0;}

			// overwrite the screens last row agin if y exits its limit

			if(y > SSD1306_LCDHEIGHT - 10)
			{y = SSD1306_LCDHEIGHT - 10;}


			putcharecter(x+next,y,*s,colour,textsize);
			s++;
			next=next+8;
		}	


	}
}

