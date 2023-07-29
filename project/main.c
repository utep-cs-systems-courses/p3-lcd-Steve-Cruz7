#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"

#define LED BIT6

#define TSW1 BIT0
#define TSW2 BIT1
#define TSW3 BIT2
#define TSW4 BIT3

#define SWITCHES (TSW1 | TSW2 | TSW3 | TSW4)

#define BG_COLOR COLOR_BLACK
#define LINE_COLOR COLOR_WHITE


short col = 0;
short pastRow = 0;
short currentRow = 0;
short velocity = 30;
short height = 30;
short lineRow = 100;

short controlCol = screenWidth/2;
short controlRow = 0;

short interrupts = 1;
short drawBlock = 1;
short redrawLine = 0;

//Button pressing related states
short correct = 0;
short playTheme = 0;

short BLOCK_COLOR = COLOR_BLUE;

void draw(){
  and_sr(~8);    //Masking interrupts
  fillRectangle(controlCol , controlRow + pastRow, 10, height, BG_COLOR);
  fillRectangle(controlCol, controlRow + currentRow, 10, height, BLOCK_COLOR);
  pastRow = currentRow;
  or_sr(8);    //Unmasking interrupts
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
 //Translate one state machine to assembly
 //Add visible counter of correctness 0-5
 //Add reset position for box before the end of the screen
 //Add decrement of correct on incorrect inputs and no input
 //Add win screen and FF victory fanfare
void wdt_c_handler()
{
  if((interrupts++) == 250){    //every second do this    
    interrupts = 1;                                       
    currentRow+=velocity;                                      
    //draw();                                             
    drawBlock = 1;                                                          
  }
}


void __interrupt_vec(PORT2_VECTOR) Port_2()
{
  if (P2IFG & TSW1){
    P2IFG &= ~TSW1;               //If they press it when its in the region, correct+1
    if ((controlRow+currentRow) <= lineRow && lineRow <= ((controlRow + currentRow) + height )){
      correct++;
      currentRow = (-1*velocity);   //reset the shape's position
      redrawLine = 1;
      correctPress();
    }
    else{
      currentRow = (-1*velocity);
      redrawLine = 1;
      incorrectPress();
}
  }
  else if (P2IFG & TSW2){  //increment speed
    P2IFG &= ~TSW2;
    if (velocity < 50)
      velocity += 20;
    else
      velocity = 10;
  }
  else if (P2IFG & TSW3){           //temporary off button
    P2IFG &= ~TSW3;
    if (BLOCK_COLOR < COLOR_PURPLE)
      BLOCK_COLOR = COLOR_PURPLE;
    else if (BLOCK_COLOR < COLOR_BLUE)
      BLOCK_COLOR = COLOR_BLUE;
    else
      BLOCK_COLOR = COLOR_RED;
  }
  else if (P2IFG & TSW4){
    P2IFG &= ~TSW4;
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
    drawPixel(i, lineRow, LINE_COLOR);
  }
  
  while (1){
    if (drawBlock){
      drawBlock = 0;
      draw(); 
    }
    if (redrawLine){
      redrawLine = 0;
      for (int i = 0; i < screenWidth; i++){ 
         drawPixel(i, lineRow, LINE_COLOR);   
      }
    }
    P1OUT &= ~LED;
    or_sr(0x10);   //turning off cpu
    P1OUT |= LED;
  }
}
