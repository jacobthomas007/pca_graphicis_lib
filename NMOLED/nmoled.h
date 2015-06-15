#ifndef nmoled
#define nmoled

#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64

#define dc     6
#define cs     7
#define rst    8

#define WHITE 1
#define BLACK 0

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8] = {
}; 

void setpixel(int x,int y,int colour);
void ssd1306_data(uint8_t c);
void ssd1306_command(uint8_t c);
void display_buffer();
void clearDisplay();
void fastSPIwrite(uint8_t c);
void lcdInit();
void verticalLine(int x,int y,int h,int clour);
void horizontalLine(int x,int y,int w,int colour);
void rect(int x,int y,int h,int w,int colour);
void fill_rect(int x,int y,int h,int w,int colour);
int  setcursor(int x,int y);
void putcharecter(int x,int y,char c,int colour);
void putstring(int x,int y,char *s,int colour,int textsize);

#endif
