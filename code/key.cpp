#include <ncurses.h>

int main (void)
{
	int key;

	initscr ();
	keypad (stdscr, true);	

	while (1)
	{
		key = wgetch (stdscr);

		printw ("%5d", key);

		if (key == 27) {
			endwin();
			return 1;
		}
	}

	return 1;
}
