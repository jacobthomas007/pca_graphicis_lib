#include <avr/pgmspace.h>
#include <stdlib.h>
#include <SPI.h>

#include "LCD_font_5x7.h"

extern  const uint8_t PROGMEM font_5x7[];

#include<nmoled.h>

// #define TSTPIXEL
#define PUTCHAR
//#define PUTSTR
// buffer


void setup()
{
  //SPI.begin();
  Serial.begin(9600);
  lcdInit();
  int i;
  clearDisplay();
  display_buffer();

  putbig();

#ifdef PUTCHAR
  //putchar1(20,10,'J');
  putchar1(2,15,'A');
#endif

#ifdef PUTSTR
 // putstring(0,0,"Neominds provides product engineering services for networking and communication equipment Network device vendors are facing the challenge of meeting increasing customer");
  putstring(25,0,"PCA MENU");
  putstring(0,9,"1.Configuration");

#endif

  //delay(1000);
  //fill_rect(20,0,10,10,0);
  //horizontalLine(2,25,63,0);
  //verticalLine(10,5,63,0);
  //fill_rect(0,0,63,127,0);
  //rect(100,18,20,20,0);
  // display_buffer();
  //rect(20,20,25,10,0);
  //     fill_rect(0,0,63,127,0);
  //   horizontalLine(0,0,127,0);
  //  horizontalLine(0,63,127,0);
  // verticalLine(0,0,63,0);
  // verticalLine(127,0,63,0);      


#ifdef TSTVERT

  verticalLine(0,0,63,0);
  verticalLine(2,25,63,0);

#endif

#ifdef TSTHORI  

  horizontalLine(0,0,127,0);
  horizontalLine(0,1,127,0);
  horizontalLine(25,50,100,0);
  horizontalLine(0,63,127,0);
  display_buffer();
  delay(2000);

#endif

#ifdef TSTPIXEL

  int htest=63;
  // for(i=0;i<h;i++)
  //setpixel(1,i,0);
  setpixel(1,63,0);
  display_buffer();
  delay(2000);

#endif

  display_buffer();
  delay(2000);


}
void loop()
{

}

