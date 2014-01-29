#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

#include "menu.h"

void quit (void) {
	endwin (); /* ncurses end */
	exit (1);
}

int main (void) {
	int i, opt;
	WINDOW *menubox;
	
	initscr 	(); /* ncurses start */
	start_color 	(); /* color enable */
	
	menu dados;

	keypad (stdscr, true);
	init_pair (1, COLOR_WHITE,  COLOR_BLUE); /* default */
	init_pair (2, COLOR_BLUE, COLOR_WHITE); /* selected */
	init_pair (3, COLOR_BLACK,  COLOR_BLUE); /* alternative */
	init_pair (4, COLOR_YELLOW, COLOR_BLUE); /* hk */
	init_pair (5, COLOR_WHITE, COLOR_BLACK); /*background */
	init_pair (NULL_PAIR, COLOR_BLACK, COLOR_BLACK);
	bkgd (COLOR_PAIR(5));

	dados.nopt 	= 8;
	dados.x 	= 5;
	dados.y 	= 5;
	dados.width 	= 20;
	dados.dpair  	= COLOR_PAIR(1);
	dados.spair  	= COLOR_PAIR(2);
	dados.hkpair 	= COLOR_PAIR(4);
	/*dados.options = alloc_opt (dados.nopt, dados.width);*/
	dados.options	= alloc_opt_c (dados.nopt, dados.width);
	dados.hotkeys 	= new int [dados.nopt];

	strcpy (dados.options [0], "Inserir");
	strcpy (dados.options [1], "Editar");
	strcpy (dados.options [2], "Fazer");
	strcpy (dados.options [3], "Desfazer");
	strcpy (dados.options [4], "Salvar");
	strcpy (dados.options [5], "Deletar");
	strcpy (dados.options [6], "Compilar");
	strcpy (dados.options [7], "Sair");

	dados.hotkeys[0] = 0;
	dados.hotkeys[1] = 0;
	dados.hotkeys[2] = 0;
	dados.hotkeys[3] = 0;
	dados.hotkeys[4] = 0;
	dados.hotkeys[5] = 2;
	dados.hotkeys[6] = 0;
	dados.hotkeys[7] = 3;

	opt = buildmenu (menubox, &dados);

	/* Mudando a posição e mantendo os dados */
	dados.x = 10;
	dados.y = 10;
	
	opt = buildmenu (menubox, &dados);

	/* Agoda mudando os dados e a posicao tambem */
	dados.nopt = 6;
	dados.x = 20;
	dados.y = 20;
	strcpy (dados.options [0], "Ttttttt");
	strcpy (dados.options [1], "Hhhhhhh");
	strcpy (dados.options [2], "Iiiiiii");
	strcpy (dados.options [3], "Aaaaaaa");
	strcpy (dados.options [4], "Ggggggg");
	strcpy (dados.options [5], "Ooooooo");

	dados.hotkeys[0] = 0;
	dados.hotkeys[1] = 0;
	dados.hotkeys[2] = 0;
	dados.hotkeys[3] = 0;
	dados.hotkeys[4] = 0;
	dados.hotkeys[5] = 0;

	opt = buildmenu (menubox, &dados);
	
	endwin ();
	printf ("%d\n", opt);

	/*free_opt (dados.options, dados.nopt);*/
	delete dados.hotkeys;
	quit (); /* execution end */
}
