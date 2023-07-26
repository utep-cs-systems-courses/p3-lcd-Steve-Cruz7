/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

int max(int a, int b) {return a > b ? a : b;}
int min(int a, int b) {return a > b ? b : a;}
/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLACK);

  //  drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  // fillRectangle(30,30, 60, 60, COLOR_ORANGE);

  /*for(int j = 0, j < 30, j++){
    drawPixel(j, j, COLOR_WHITE);
    }*/
  int controlCol = screenWidth >> 1;
  int controlRow = screenHeight >> 1;
  int c = 0;
  int r = 0;
  for (int j = 0; j <= 30; j++){
    int upper = min(+r, -r);
    int lower = max(+r, -r);
    int drawCol = c;
    for (int drawRow = upper; drawRow <= lower; drawRow++ ){
      drawPixel(controlCol + drawCol, controlRow+drawRow, COLOR_WHITE);
      drawPixel(controlCol - drawCol, controlRow+drawRow, COLOR_WHITE); 
    }
    c += 1;
    r += 1;
  }
  
}



