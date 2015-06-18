/********************************

         PCA MENU APP


*********************************/



#include <avr/pgmspace.h>
#include <stdlib.h>
#include <SPI.h>
#include<nmoled.h>

#define MAX_ITEMS_IN_MENU 3//size of menus_items

#define MENU 0
#define ITEM 1
#define NULL 0
//#define DEBUG //to print all print statements
#define DISPLAY


struct menu_item
{
  unsigned char type; //MENU or ITEM\

  struct menu_item_list *parent;
  struct menu_item_list *child;

  unsigned char id;
  unsigned char last_index;
  unsigned char onscreen_start_index; // used to point the start item in the screen
  unsigned char onscreen_end_index;// used to point the end item in the screen
  unsigned char curr_selection;// points to the current selected item in the screen

  char name[20];
  void *value;
  void (*action_func)();
  struct menu_item *item_list[MAX_ITEMS_IN_MENU];
};

/**********************************************************************
  
  struct menu_item* create_menu_item_new(int id,char *item_name)
  This function creates a structure block and fills the required parameters
  returns the pointer to the funcction.
  
  1> copy the item_name to the first structure int the list
  
  
 ***********************************************************************/

struct menu_item* create_menu_item_new(unsigned char id1,char *item_name,unsigned char type)
{
  struct menu_item *tmp;
  int i;
  tmp=(struct menu_item *)malloc(sizeof(struct menu_item));
  tmp->type=type;
  tmp->id=id1;
  tmp->last_index=0;
//  tmp->onscreen_start_index=0;
 // tmp->onscreen_end_index=-1;

  strcpy(tmp->name,item_name);  
  
  //if type is a menu then we will have to display its sub menus
  //else if type=ITEM then arry pointer would be NULL;
  if(tmp->type==ITEM)
  {
    // the arry shouls point to null pointer
  }
  
  
  
  return tmp;

}

/*****************************************************************

  add_menu_item(struct1 menu_item *,struct2 menu_item *) this function is used to combine or
  link two structures so that there is a link between two strutures.


*******************************************************************/

void add_menu_item(struct menu_item *item1,struct menu_item *item2)
{
  #ifdef DEBUG
  Serial.println("Insde add_menu_func");
  #endif
  
      if(item1->last_index==0){
      item1->item_list[0]=item2;
      item1->last_index++;
   #ifdef DEBUG
      Serial.println("i am inside if");
      //item1->onscreen_start_index=item2;
   #endif  
    }
      else
      {
        item1->item_list[item1->last_index]=item2;
        item1->last_index++;
        #ifdef DEBUG
        Serial.println("i am inside else");
        #endif
        delay(100);
      }
      
}

void show_menu_list(struct menu_item *tmp)
{
  clearDisplay();
  struct menu_item *dup;
  int i,y=16; // start displaying from after the status bar
  dup=tmp->item_list[0];
  for(i=0;i<tmp->last_index;i++){
  dup=tmp->item_list[i];
  putstring(0,y,dup->name,WHITE,2);
  y+=16;// move to the next 16th pixel
  
  }
  display_buffer();
  delay(100);
}

void setup()
{
  int i;
  lcdInit();
  clearDisplay();
  Serial.begin(9600);
  struct menu_item *item1,*item1_sub1,*item1_sub2,*item1_sub3; //level 1
  
  
  item1=create_menu_item_new(1,"MAIN MENU",MENU);
  item1_sub1=create_menu_item_new(2,"1.Configuration",MENU);
  item1_sub2=create_menu_item_new(3,"2.Firmware_Up",MENU);
  item1_sub3=create_menu_item_new(4,"3.Exit",ITEM);
  
  
  add_menu_item(item1,item1_sub1);
  add_menu_item(item1,item1_sub2);
  add_menu_item(item1,item1_sub3);
  
  struct menu_item *item1_sub11,*item1_sub12,*item1_sub13;//level 2
  
  item1_sub11=create_menu_item_new(5,"1.Media",MENU);
  item1_sub12=create_menu_item_new(6,"2.Line",MENU);
  item1_sub13=create_menu_item_new(7,"3.Voice",MENU);
  
  add_menu_item(item1_sub1,item1_sub11);
  add_menu_item(item1_sub1,item1_sub12);
  add_menu_item(item1_sub1,item1_sub13);  
  
  #ifdef DISPLAY

  show_menu_list(item1);
  //Serial.println(item1->last_index);
  delay(5000);
  show_menu_list(item1_sub1);
  delay(100);

  #endif

}
void loop()
{
}

