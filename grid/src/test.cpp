/***************************************************************************
 *            test.cpp
 *
 *  Wed Dec  1 19:20:44 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/

#include <curses.h>

#include "grid.h"

int main (void)
{
	GRID *gridbox;
	
	initscr ();
	start_color ();
	init_pair (1, COLOR_BLACK, COLOR_WHITE);
	init_pair (2, COLOR_WHITE, COLOR_RED);
	
	gridbox = new_grid (stdscr, 3, 30, 2, 2, 1, 2 );
	
	setnreg (gridbox, 2);
	setregline (gridbox, "Teste 1", 0);
	setregline (gridbox, "Teste 2", 1);
	//setregline (gridbox, "Teste 3", 2);
//	setregline (gridbox, "Teste 4", 3);
//	setregline (gridbox, "Teste 5", 4);
	
	print_grid (gridbox);
	run_grid (gridbox);
	
	getch();
	endwin();
	return 1;
}
