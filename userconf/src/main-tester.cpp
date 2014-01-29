/***************************************************************************
 *            main-tester.cpp
 *
 *  Thu Nov 25 20:43:49 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <panel.h>
#include <unistd.h>

#include "dialog.h"
#include "button.h"
#include "contentbox.h"

void move_window ( DIALOG * janela , BUTTON * );

int main (void)
{
	DIALOG * janela;
	BUTTON * botao_sair, *botao_cancelar, *botao_salvar;
	int key;
	int x = 1, y = 1, tab = 1, ret = 0;
	PANEL * stdpanel;
	WINDOW * tester;
	
	
	stdscr = initscr ();
	start_color ();
	keypad (stdscr , true);
	curs_set (0);
	
	stdpanel = new_panel (stdscr);
	
	init_pair (1, COLOR_BLACK, 7);
	init_pair (2, COLOR_WHITE, COLOR_BLUE);
	init_pair (3, COLOR_WHITE, COLOR_RED);
	
	bkgd (COLOR_PAIR (2));
	
	janela = new_dialog(30, 90, y, x, 1);

	enable_shadow (janela, 0);
	dialog (janela);
	
	botao_salvar = new_button (janela, 3, 10, 0, 0, 3, 2);
	setlabel (botao_salvar, "Salvar");
	print_button (botao_salvar);

	botao_cancelar = new_button (janela, 3, 10, 20, 23, 3, 2);
	setlabel (botao_cancelar, "Mover");
	print_button (botao_cancelar);

	botao_sair = new_button (janela, 3, 10, 20, 34, 3, 2);
	setlabel (botao_sair, "Sair");
	print_button (botao_sair);

	refresh_dialog (janela);
	refresh ();
	
	while (1)
	{
		if (tab == 1) {
			ret = run_button (botao_salvar);
			if (ret == -1) tab--;
			if (ret ==  0) { del_dialog(janela); refresh (); };
			if (ret ==  1) tab++;
		}

		if (tab == 2) {
			ret = run_button (botao_cancelar);
			if (ret == -1) tab--;
			if (ret ==  0) { move_window (janela, botao_cancelar); };
			if (ret ==  1) tab++;
		}

		if (tab == 3) {
			ret = run_button (botao_sair);
			if (ret == -1) tab--;
			if (ret ==  0) { endwin(); exit(1); };
			if (ret ==  1) tab++;
		}
		if (tab == 4) tab = 1;
		if (tab == 0) tab = 3;
	}
	
	wrefresh (stdscr);
	wgetch (stdscr);
	
	del_dialog (janela);
	endwin ();
	return 1;
	


}

void move_window ( DIALOG * janela , BUTTON *botao )
{
	int key;
	int x = janela->begin_x;
	int y = janela->begin_y;
	int var_x = janela->begin_x; 
	int var_y = janela->begin_y;
	
	do {
		key = wgetch (stdscr);
		switch (key)
		{
			case KEY_LEFT: 
				if (x > 0) {
					move_dialog (janela, y, --x );
					mvwprintw (janela->dialog_window, 1, 1, "Movendo para a esquerda ");
					mvwprintw (janela->dialog_window, 0, 1, "%2dx%2d", x, y);
					refresh_dialog(janela);
				}
			break;
			case KEY_RIGHT: 
				if (x < 79) {
					move_dialog (janela, y, ++x );
					mvwprintw (janela->dialog_window, 1, 1, "Movendo para a direita ");
					mvwprintw (janela->dialog_window, 0, 1, "%2dx%2d", x, y);
					refresh_dialog(janela);
				}
			break;
			case KEY_UP: 
				if (y > 0) {
					move_dialog (janela, --y, x );
					mvwprintw (janela->dialog_window, 1, 1, "Movendo para cima       ");
					mvwprintw (janela->dialog_window, 0, 1, "%2dx%2d", x, y);
					refresh_dialog(janela);
				}
			break;
			case KEY_DOWN: 
				if (y < 24) {
					move_dialog (janela, ++y, x );
					mvwprintw (janela->dialog_window, 1, 1, "Movendo para baixo       ");
					mvwprintw (janela->dialog_window, 0, 1, "%2dx%2d", x, y);
					refresh_dialog(janela);
				}
			break;
		}
		refresh ();
	} while (key != 27);	
	
	//update_xy ( botao, y + 5, x + 5 );
	
}
