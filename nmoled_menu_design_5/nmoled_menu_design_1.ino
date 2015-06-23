/********************************

         PCA MENU APP


*********************************/



#include <avr/pgmspace.h>
#include <stdlib.h>
#include <SPI.h>
#include<nmoled.h>

#define MAX_ITEMS_IN_MENU 10//size of menus_items

#define MENU 0
#define ITEM 1
#define NULL 0
//#define DEBUG //to print all print statements
#define DISPLAY
//#define FREERAM //tells the amount of free ram space left

struct menu_item
{
  unsigned char type; //MENU or ITEM\

  struct menu_item *parent;
  struct menu_item *child;

  unsigned char id;
  unsigned char last_index;
  unsigned char onscreen_start_index; // used to point the start item in the screen
  unsigned char onscreen_end_index;// used to point the end item in the screen
  struct menu_item *curr_selection;// points to the current selected item in the screen

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
  if (tmp == NULL)  Serial.println("MF"); // Heap memory overflow error
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
  
  for(i=0;i<MAX_ITEMS_IN_MENU;i++)
  memset(tmp->item_list[0],0,MAX_ITEMS_IN_MENU);//initializeing the array elementss
  
  tmp->curr_selection=tmp->item_list[0];
  
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
      item2->parent=item1;
   #ifdef DEBUG
      Serial.println("i am inside if add munu func");
      //item1->onscreen_start_index=item2;
   #endif  
    }
      else
      {
        item1->item_list[item1->last_index]=item2;
        item1->last_index++;
        item2->parent=item1;
        #ifdef DEBUG
        //Serial.println("i am inside else of add_menu func");
        #endif
        //delay(100);
      }
     
}
/*

This function is used to travese back to it parent node
as the button is pressed

*/

void go_back(struct menu_item *tmp)
{
  struct menu_item *curr_ptr;
  curr_ptr=tmp;
  curr_ptr=curr_ptr->parent;
  if(curr_ptr==NULL)
  {
    show_menu_list(tmp);
  }
  else
  show_menu_list(curr_ptr);
}

/*

This funcction highlests the menu item which is selected

*/

void highlight_menu()
{
  
}

void show_menu_list(struct menu_item *tmp)
{
  clearDisplay();
  struct menu_item *dup;
  unsigned char i,j,y=16; // start displaying from after the status bar 
  dup=tmp->item_list[0];
  unsigned char start_index=0;
  unsigned char end_index=2;// tot item to be displayed on the screen
  unsigned char next_index=1;
  
  /*
  tmp->last_index-n is used to traverse hw many time the inside loop should be called
  n-> tot element to be avoided now its 2 then it displays 3 elements in the screen 
   
  */
  for(i=0;i<(tmp->last_index-2);i++)
  {
       
    for(j=start_index;j<=end_index;j++)
    {
        if(y > 48) // 48 tells the screen is full something like end of line
        {
          clearDisplay();
          start_index=next_index;
          next_index++;
          y=16;
          delay(1000);  // this is to show the difference in the scrolling screen
        }
      
        dup=tmp->item_list[j];
        putstring(1,y,dup->name,WHITE,2);
       // rect(0,14,11,127,WHITE);
        display_buffer();
        delay(50);
        y+=16;// move to the next 16th pixel
          
             }
    start_index++;
    end_index++;
  }
//  display_buffer();
  
}




#ifdef FREERAM
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
#endif

void setup()
{
  int i;
  lcdInit();
  clearDisplay();
  Serial.begin(9600);
  struct menu_item *item1,*item1_sub1,*item1_sub2,*item1_sub3; //level 1
  
  

  
  item1=create_menu_item_new(1,"MAIN MENU"            ,MENU);
  item1->parent=NULL;
  item1_sub1=create_menu_item_new(11,"1.Configuration",MENU);
  item1_sub2=create_menu_item_new(12,"2.Firmware_Up"  ,MENU);
  item1_sub3=create_menu_item_new(13,"3.Exit"         ,ITEM);
  
  
  add_menu_item(item1,item1_sub1);
  add_menu_item(item1,item1_sub2);
  add_menu_item(item1,item1_sub3);
  
  struct menu_item *item1_sub11,*item1_sub12,*item1_sub13,*item1_sub14,*item1_sub15,*item1_sub16;//level 2
  
  item1_sub11=create_menu_item_new(111,"1.Media",MENU);
  item1_sub12=create_menu_item_new(112,"2.Line" ,MENU);
  item1_sub13=create_menu_item_new(113,"3.Voice",MENU);
  item1_sub14=create_menu_item_new(114,"4.IP"   ,MENU);
  item1_sub15=create_menu_item_new(115,"5.FEC"  ,MENU);
  item1_sub16=create_menu_item_new(116,"6.Serial",MENU);


  
  
  add_menu_item(item1_sub1,item1_sub11);
  add_menu_item(item1_sub1,item1_sub12);
  add_menu_item(item1_sub1,item1_sub13);  
  add_menu_item(item1_sub1,item1_sub14);  
  add_menu_item(item1_sub1,item1_sub15);  
  add_menu_item(item1_sub1,item1_sub16);  
  
  
  #ifdef DISPLAY

  show_menu_list(item1);
  //Serial.println(item1->last_index);
  delay(3000);
  show_menu_list(item1_sub1);
  delay(3000);
  go_back(item1_sub1);
  //delay(100);


#ifdef DEBUG
Serial.print("item1_sub1");
Serial.println((int)item1_sub1->parent);
Serial.print("item1");
Serial.println((int)item1);
delay(1000);
#endif
 
 #ifdef FREERAM 
 
    //Serial.println("\n[memCheck]");
    //Gives the total memory lest 
    // in the above there is only 207 bytes left free
    Serial.println(freeRam());
  #endif // freeram
  #endif

}
void loop()
{
}

