#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <panel.h>

#include "contentbox.h"

int main ()
{
	int i;
	WINDOW *contbox;
	CONTENTBOX *conteudo;
	
	initscr ();
	start_color ();

	init_pair (1, COLOR_BLACK, COLOR_WHITE);
	init_pair (2, COLOR_RED, COLOR_WHITE);

	conteudo = new_contentbox(15, 30, 2, 2, 1, 2);
	settitle (conteudo, "Testando o cidadao");
	setcontent (conteudo, "oidhasdhasda\nhduishiudsa\ndsaiudahsdai\n\n\nhauadhusa\nteste!!", 20, 25);
	show_title (conteudo);
	show_contentbox (conteudo);
	
	getch();
	endwin();
	return 1;
}

\
