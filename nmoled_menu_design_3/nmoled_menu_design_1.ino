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
  if (tmp == NULL)  Serial.println("MF"); //delay(50);}
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
      Serial.println("i am inside if add munu func");
      //item1->onscreen_start_index=item2;
   #endif  
    }
      else
      {
        item1->item_list[item1->last_index]=item2;
        item1->last_index++;
        #ifdef DEBUG
        //Serial.println("i am inside else of add_menu func");
        #endif
        //delay(100);
      }
     
}

void show_menu_list(struct menu_item *tmp)
{
  clearDisplay();
  struct menu_item *dup;
  unsigned char i,y=16; // start displaying from after the status bar
  unsigned char y_sec=16;
  
  dup=tmp->item_list[0];
  unsigned char start_index=0;
  unsigned char next_index=1;
  if(tmp->last_index < 4){
  for(i=0;i<tmp->last_index;i++){
  dup=tmp->item_list[i];
  putstring(0,y,dup->name,WHITE,2);
  y+=16;// move to the next 16th pixel
  
  }
  display_buffer();
  delay(100);
  }// end if
  else
  {
    // find out a way to display items one below the other
    //(tmp->last_index)*2 mul by 2 is because it shoud have a scrolling effect
    for(i=start_index;i<(tmp->last_index)*2;i++)
   // for(i=start_index;i<13;i++)
      {
        dup=tmp->item_list[start_index];
        start_index++;
        putstring(0,y_sec,dup->name,WHITE,2);
        display_buffer();
        y_sec+=16;// move to the next 16th pixel
       // if(i != 0 && i % 3 == 0)
           
         if(y_sec>48)
        {
          clearDisplay();
       //  Serial.println(i);
          start_index=next_index;
          next_index++;
          y_sec=16;
          delay(3000);
          //clearDisplay();
        }
        
        //display_buffer();
        //delay(1000);
      }   
  //#ifdef DEBUG
  //Serial.print("last_index=");
  //Serial.println(tmp->last_index);
  //#endif
  }
  
  
}

void setup()
{
  int i;
  lcdInit();
  clearDisplay();
  Serial.begin(9600);
  struct menu_item *item1,*item1_sub1,*item1_sub2,*item1_sub3; //level 1
  
  
  #if 0
  
  item1=create_menu_item_new(1,"MAIN MENU",MENU);
  item1_sub1=create_menu_item_new(2,"1.Configuration",MENU);
  item1_sub2=create_menu_item_new(3,"2.Firmware_Up",MENU);
  item1_sub3=create_menu_item_new(4,"3.Exit",ITEM);
  
  
  add_menu_item(item1,item1_sub1);
  add_menu_item(item1,item1_sub2);
  add_menu_item(item1,item1_sub3);
  
  struct menu_item *item1_sub11,*item1_sub12,*item1_sub13,*item1_sub14,*item1_sub15,*item1_sub16,*item1_sub17,*item1_sub18;//level 2
  
  item1_sub11=create_menu_item_new(5,"1.Media",MENU);
  item1_sub12=create_menu_item_new(6,"2.Line",MENU);
  item1_sub13=create_menu_item_new(7,"3.Voice",MENU);
  item1_sub14=create_menu_item_new(8,"4.IP",MENU);
  item1_sub15=create_menu_item_new(9,"5.FEC",MENU);
  item1_sub16=create_menu_item_new(10,"6.Serial",MENU);
  item1_sub17=create_menu_item_new(11,"7.Logging",MENU);
  //item1_sub18=create_menu_item_new(12,"8.TDMA",MENU);
  
  
  add_menu_item(item1_sub1,item1_sub11);
  add_menu_item(item1_sub1,item1_sub12);
  add_menu_item(item1_sub1,item1_sub13);  
  add_menu_item(item1_sub1,item1_sub14);  
  add_menu_item(item1_sub1,item1_sub15);  
  add_menu_item(item1_sub1,item1_sub16);  
  add_menu_item(item1_sub1,item1_sub17);  
  //add_menu_item(item1_sub1,item1_sub18);  
  
  
  #ifdef DISPLAY

  show_menu_list(item1);
  //Serial.println(item1->last_index);
  delay(5000);
  show_menu_list(item1_sub1);
  delay(100);

  #endif
#endif

Serial.println("test message");
void *tmp;
int t = 0; 
do 
{
for (int i =0;i<11;i++)
{
  tmp=(struct menu_item *)malloc(sizeof(struct menu_item));
  t+= sizeof(struct menu_item); if (tmp != NULL)  {Serial.print("Allocation=");Serial.println(i);Serial.println(t);} //delay(50);}
  
}
}while (tmp != NULL);
Serial.println("done");

}
void loop()
{
}

