#include <curses.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>

#include "linebox.h"
#include "dialog.h"

int main (void)
{
	LINEBOX *line;
	DIALOG *_dialog;

	initscr ();
	start_color ();
	

	init_pair (1, COLOR_WHITE, COLOR_RED);
	_dialog = new_dialog (20, 50, 1, 1, 1);
	dialog (_dialog);
	refresh_dialog (_dialog);
	line = new_linebox (_dialog, 1, 30, 10, 10, 1, 0, 20);

	valid_chars (line, "abcdefghijklmnopqrstuvxyz ");
	print_linebox (line);
	run_linebox (line);
	
	getch ();	
	
	endwin ();
}
