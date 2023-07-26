#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

typedef struct {
  short col, row;
} Pos;

#define LED BIT6

#define SWITCHES 15

short col = 0;
short pastRow = 0;
short currentRow = 0;

short controlCol = 64;
short controlRow = 0;

short interrupts = 1;
short drawBlock = 1;

void draw(){
  fillRectangle(controlCol , controlRow + pastRow, 10, 50, BLACK);
  fillRectangle(controlCol, controlRow + currentRow, 10, 50, COLOR_BLUE);
  pastRow = currentRow;
}

void switch_init(){
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;
}

 //Implement WDT interrupt so you can make the shape move down every second
 //Test collisions at certain coordinates maybe with a line or just edge of screen
 //Implement buttons that can interact with program during collisions
void __interrupt_vec(WDT_VECTOR) WDT(){
  if((interrupts++) == 250){
    interrupts = 1;
    currentRow+=10;
    // draw();
    drawBlock = 1;
  }
}

void main(){
  P1DIR |= LED;
  P1OUT |= LED;
  
  configureClocks();
  lcd_init();
  switch_init();

  enableWDTInterrupts();
  or_sr(0x8);

  clearScreen(BLACK);
  
  while(1){
    if(drawBlock){
      drawBlock = 0;
      draw();
      
    }
    P1OUT &= ~LED;
    or_sr(0x10);
    P1OUT |= LED;
  } 
}
