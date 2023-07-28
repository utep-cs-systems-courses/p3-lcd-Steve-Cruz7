#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"

typedef struct {
  short col, row;
} Pos;

#define LED BIT6

#define TSW1 BIT0
#define TSW2 BIT1
#define TSW3 BIT2
#define TSW4 BIT3

#define SWITCHES (TSW1 | TSW2 | TSW3 | TSW4)

short col = 0;
short pastRow = 0;
short currentRow = 0;

short controlCol = screenWidth/2;
short controlRow = 0;

short interrupts = 1;
short drawBlock = 1;

//Button pressing related states
short correct = 0;
short playTheme = 0;

void draw(){
  and_sr(~8);    //Masking interrupts
  fillRectangle(controlCol , controlRow + pastRow, 10, 30, BLACK);
  fillRectangle(controlCol, controlRow + currentRow, 10, 30, COLOR_BLUE);
  pastRow = currentRow;
  or_sr(8);
}

void correctPress(){
  if(correct == 1)
    play4Csharp();
  if(correct == 2)
    play4D();
  if(correct == 3)
    play4Fsharp();
  if(correct == 4)
    play4G();
  if(correct == 5){
    play4A();
  }
}

void incorrectPress(){
  buzzer_set_period(500);
}    

void switch_init(){
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;
}


 //Add button to increment size
 //Add button to increase speed
 //Add button to change color
 //Translate one state machine to assembly
 //Add decrement of correct on incorrect inputs
 //Add visible counter of correctness 0-5
void wdt_c_handler()
{
  if((interrupts++) == 250){    //every second do this    
    interrupts = 1;                                       
    currentRow+=30;                                      
    //draw();                                             
    drawBlock = 1;                                        
                      
  }
}


void __interrupt_vec(PORT2_VECTOR) Port_2()
{
  if(P2IFG & TSW1){
    P2IFG &= ~TSW1;               //If they press it when its in the region, correct+1
    if ((controlRow+currentRow) < 100 && 100 < ((controlRow + currentRow) + 30 )){
      correct++;
      currentRow = 0;   //reset the shape's position
      //redrawline = 1
      correctPress();
    }
    else{
      currentRow = 0;
      incorrectPress();
    }
  }
  else if(P2IFG & TSW2){           //temporary off button
    P2IFG &= ~TSW2;
    buzzer_set_period(0);    
  }
}


void main(){
  P1DIR |= LED;
  P1OUT |= LED;
  
  configureClocks();
  lcd_init();
  switch_init();
  buzzer_init();

  enableWDTInterrupts();
  or_sr(0x8);             //enable GIE

  clearScreen(BLACK);
  for (int i = 0; i < screenWidth; i++){
    drawPixel(i, 100, WHITE);
  }
  
  while(1){
    if(drawBlock){
      drawBlock = 0;
      draw(); 
    }
    
    P1OUT &= ~LED;
    or_sr(0x10);   //turning off cpu
    P1OUT |= LED;
  }
}
