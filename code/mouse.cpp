#include <curses.h>
#include <ctype.h>

int main (void)
{
	mmask_t mask;
	MEVENT event;
	int ch;

	initscr();
	cbreak();
	keypad (stdscr, true);

	/*Detectando eventos do mouse*/
	mousemask (mask, NULL);
	
	while ((toupper(ch = getch())) != 'Q')
	{
		if (ch == KEY_MOUSE) {
			getmouse (&event);
			switch (event.bstate) {
				case BUTTON1_RELEASED:
					printw  ("botao 1 pressionado!\n");
					break;
				case BUTTON2_RELEASED:
					printw  ("botao 2 pressionado!\n");
					break;
				case BUTTON3_RELEASED:
					printw  ("botao 3 pressionado!\n");
					break;
				default:
					printw ("evento desconhecido");
			}
		refresh ();
		}
	}

	endwin ();
}
