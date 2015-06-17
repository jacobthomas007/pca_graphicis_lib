#include <avr/pgmspace.h>
#include <stdlib.h>
#include <SPI.h>
#include<nmoled.h>

#define MENU_ITEM_LISTS 3//size of menus_items
#define MENU_START_INDEX 0

struct menu_item
{
//  char type;
  //struct menu_item_list *parent;
  //struct menu_item_list *child;
  char name[20];
};

struct menu_item_list
{
  //char type;
  unsigned char last_index;
  unsigned char onscreen_start_index;
  unsigned char onscreen_end_index;
  unsigned char curr_selection;
  struct menu_item *item_list[MENU_ITEM_LISTS];
 //char name[20];
};

//struct menu
//{
//  struct menu_item_list *list[];
//  struct menu_item_list *curr_level;
//};

void show_menu_list(struct menu_item_list *tmp)
{
  int i,y=16;
  for(i=0;i<MENU_ITEM_LISTS;i++){
  putstring(tmp->onscreen_start_index,y,tmp->item_list[i]->name,WHITE,2);
  y+=16;
  tmp=++tmp;
  }
  display_buffer();
}

void setup()
{
  lcdInit();
  clearDisplay();
  struct menu_item_list menu_item_obj[MENU_ITEM_LISTS];
  int i;
  for(i=0;i<MENU_ITEM_LISTS;i++){
  menu_item_obj[i].onscreen_start_index=MENU_START_INDEX;
  menu_item_obj[i].item_list[i]=(struct menu_item *)malloc(sizeof(menu_item));
  }
  strcpy(menu_item_obj[0].item_list[0]->name,"1.Configuration");
  strcpy(menu_item_obj[1].item_list[1]->name,"2.Firmware_Up");
  strcpy(menu_item_obj[2].item_list[2]->name,"3.Exit");
  show_menu_list(&menu_item_obj[0]);
  
}
void loop()
{}
