/****************************************************************************
 *            grid.cpp
 *
 *  Wed Dec 01 16:12:00 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/
 
#include <string.h>
#include <curses.h>
#include <panel.h>

#define ENTER 10
#define TAB 9

#define GRID_TAB -2
#define GRID_ENTER GRID_TAB

#ifdef USERCONF
	#define KEY_DELETE 330
	#define RET_DELETE 3
#endif

typedef struct 
{
	int nlines, cols;
	int begin_y, begin_x;
	int color_pair, color_pair_s;
	int bini, bend;
	int nreg; 
	char **content;
	int active ;
	int moviment;
	WINDOW *grid_window;
} GRID;

int abs (int x) {
	if (x < 0)
		return x*-1;
	return x;
}

/****
 * Novo GRID box
 ***/
GRID * new_grid ( WINDOW *top_window , int nlines, int cols, int begin_y, int begin_x , int color_pair, int color_pair_s )
{
	GRID *__grid;
	__grid = new GRID;
	__grid->grid_window = subwin (top_window, (__grid->nlines = nlines, nlines), cols, begin_y, begin_x);
	__grid->begin_y = begin_y , __grid->begin_x = begin_x;
	__grid->nlines = nlines , __grid->cols = cols;
	__grid->color_pair = color_pair, __grid->color_pair_s = color_pair_s;
	__grid->content = NULL;
	__grid->active = -1;
	__grid->bini = 0;
	__grid->bend = nlines;
	__grid->moviment = 0;
	
	return __grid;
}

void free_grid ( GRID *__grid )
{
	delete __grid;
}

/****
 * Seta o número de registros
 ***/
void setnreg ( GRID *__grid, int nreg )
{
	int k;
	__grid->content = new char *[nreg];
	for (k=0; k<nreg; k++)
		__grid->content[k] = NULL;
	__grid->nreg = nreg;
}

/****
 * Copia registro N
 ***/
int setregline ( GRID *__grid, char *line, int n )
{
	if ( !(n < __grid->nreg && __grid->content != NULL) )
		return -1;
			
	__grid->content[n] = new char [strlen (line) +1];
	strcpy (__grid->content [n], line );

	return 1;	
}

/****
 * Imprime o texto da GRID
 ***/
void put_text ( GRID *__grid )
{
	int i, c = 0, k, move = 0, a = 0;
	if ( __grid->content == NULL )
		return;
	
	/*DOWN*/
	if (__grid->moviment == KEY_DOWN)
	{
		if (__grid->active+1 > __grid->bend) {
			__grid->bini ++;
			__grid->bend ++;
		}
	
		if (__grid->active == __grid->nreg) {
			__grid->bini = 0;
			__grid->bend = __grid->nlines;
			__grid->active = 0;
		}
	}
	
	/*UP*/
	if (__grid->moviment == KEY_UP)
	{
		if (__grid->active < __grid->bini) {
			__grid->bini --;
			__grid->bend --;
		}
		
		if (__grid->active == -1) {
			__grid->bini = abs (__grid->nreg - __grid->nlines);
			__grid->bend = __grid->nreg; 
			__grid->active = __grid->nreg - 1;
		}
	}
	
	if (__grid->bend > __grid->nreg)
		__grid->bend = __grid->nreg;
	
	for (i=__grid->bini; i<__grid->bend; i++)
	{
		if (__grid->active != -1 && __grid->active == i) 
			wattron (__grid->grid_window, (COLOR_PAIR (__grid->color_pair_s)));

		for (a = 0; a<__grid->cols; a++)
		mvwaddch (__grid->grid_window, c, a , ' ');
		
		k = move = a = 0;
		while (__grid->content [i][k] != '\x0') {
			if (__grid->content[i][k] == '\t') {
				move += 15;
				a = 0;
			}
			mvwaddch (__grid->grid_window, c, a + move, __grid->content[i][k]);
			k ++;
			a ++;
		}

		if (__grid->active != -1 && __grid->active == i) 
			wattroff (__grid->grid_window, (COLOR_PAIR (__grid->color_pair_s)));
		c++;
	}
	__grid->moviment = 0;
}


/****
 * Imprime o GRID (mas não a interacao com o usuario)
 ***/
void print_grid ( GRID *__grid )
{
	wbkgd (__grid->grid_window, COLOR_PAIR (__grid->color_pair));
	wrefresh (__grid->grid_window);
	
	put_text (__grid);
}


/****
 * Interaçao usuario<->gridbox
 ***/
int run_grid ( GRID *__grid )
{
	int key;
	keypad (__grid->grid_window, true);
	noecho ();
	curs_set(false);
	
	if (__grid->active < 0)
		__grid->active = 0;
	
	while (true)
	{
		werase (__grid->grid_window);
		put_text (__grid);
		wrefresh (__grid->grid_window);

		key = wgetch (__grid->grid_window);
		
		switch (key)
		{
		#ifdef USERCONF
			case KEY_F(1): return KEY_F(1);
			case KEY_F(2): return KEY_F(2);
			case KEY_F(3): return KEY_F(3);
			case KEY_F(4): return KEY_F(4);
			case KEY_F(5): return KEY_F(5);
			case KEY_F(6): return KEY_F(6);
			case KEY_F(7): return KEY_F(7);
			case KEY_F(8): return KEY_F(8);
			case KEY_F(9): return KEY_F(9);
			case KEY_F(10): return KEY_F(10);
			case KEY_F(11): return KEY_F(11);
			case KEY_F(12): return KEY_F(12);
		#endif			
			case 9:
				return GRID_TAB;
			case 10:
				return GRID_ENTER;
			break;
			case KEY_DOWN:
				if (__grid->active < __grid->nreg)
					__grid->active++;

				__grid->moviment = KEY_DOWN;
			break;
			case KEY_UP:
				if (__grid->nreg < __grid->nlines)
				{
					if (__grid->active == 0)
						__grid->active = __grid->nreg - 1;	
					else if (__grid->active > 0)
						__grid->active--;
				}
				else
					__grid->active--; /* Dexo a print_grid cuidar da scrollbar*/
				__grid->moviment = KEY_UP;
			break;
#ifdef USERCONF
			case KEY_DELETE: /*DELETE*/
				return RET_DELETE;
#endif
		}
	}
	
}
