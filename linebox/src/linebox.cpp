#include <curses.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>

#include "dialog.h"

#define linebox_enter 		10
#define linebox_tab 		9
#define linebox_backspace 	263
#define linebox_backspace_127 127
#define linebox_insert		331
#define linebox_home		262
#define linebox_end			360
#define linebox_delete		330
#define linebox_left		260
#define linebox_right		261
#define linebox_pgup		339
#define linebox_pgdown		338

#define NORMAL 0
#define PASSWORD 1

typedef struct {
	int begin_x, begin_y;
	int nlines, cols;
	int max;
	int insert;
	int border;
	int color_pair;
	int cini, cend;
	int cursor;
	int nchar;
	int type;
	char *buffer;
	char *valid;
	DIALOG *dialog_linebox;
} LINEBOX;

LINEBOX * new_linebox ( DIALOG *__dialog, int nlines , int cols, int begin_y, int begin_x , int color_pair  , int border, int max )
{
	LINEBOX *__linebox;
	__linebox = new LINEBOX;
	__linebox->dialog_linebox = sub_dialog (__dialog, nlines + (border*2), cols + (border*2),  begin_y,  begin_x, color_pair);
	__linebox->color_pair = color_pair, __linebox->begin_y = begin_y, __linebox->begin_x = begin_x;
	__linebox->nlines = nlines, __linebox->cols = cols, __linebox->border = border;
	__linebox->cursor = 0, __linebox->max = 0;
	__linebox->cini = 0, __linebox->cend = 0;
	__linebox->buffer = new char [max + 1];
	__linebox->max = max, __linebox->insert = 0;
	__linebox->nchar = 0;
	__linebox->type = NORMAL;
	
	wbkgd (__linebox->dialog_linebox->dialog_window, COLOR_PAIR (color_pair));
	(border) ? box (__linebox->dialog_linebox->dialog_window, ACS_VLINE, ACS_HLINE) : NULL;
	keypad (__linebox->dialog_linebox->dialog_window, true);
	return __linebox;
}

void free_linebox ( LINEBOX *__linebox )
{
	delete __linebox;
}

void del_linebox ( LINEBOX *__linebox )
{
	hide_dialog (__linebox->dialog_linebox);
	del_dialog (__linebox->dialog_linebox);
	free_linebox (__linebox);
}

void change_type ( LINEBOX *__linebox, int type )
{
	__linebox->type = type;
}

void print_linebox ( LINEBOX *__linebox )
{
	box (__linebox->dialog_linebox->dialog_window , ACS_VLINE, ACS_HLINE);
	int i;
	for (i=0; i<__linebox->cols; i++)
		mvwprintw (__linebox->dialog_linebox->dialog_window, __linebox->border, i+ (__linebox->border), "_");
	wmove (__linebox->dialog_linebox->dialog_window, __linebox->border,  __linebox->border );
	wrefresh ( __linebox->dialog_linebox->dialog_window );
}

void linebox_move (char *str, int cursor, int nchar){
	str[cursor] = str[cursor+1];
	while(nchar > cursor){
		str[cursor+1] = str[cursor+2];
		cursor++;
	}
}

void linebox_insert_move(char *str, int cursor, int nchar){
	str[nchar+1] = str[nchar];
	while(cursor < nchar){
		str[nchar] = str[nchar-1];
		nchar--;
	}
}

void valid_chars ( LINEBOX *__linebox , char *accept )
{
	__linebox->valid = new char [strlen (accept)];
	strcpy (__linebox->valid, accept );
}

void print_str ( LINEBOX *__linebox )
{
	int i;
	for (i=__linebox->cini; i<__linebox->cend; i++) {
		mvwaddch (__linebox->dialog_linebox->dialog_window, 
		__linebox->border , __linebox->border + i - __linebox->cini 
		, (__linebox->type == PASSWORD) ? '*' : __linebox->buffer[i]);
	}
}

void reset_linebox ( LINEBOX *__linebox ) {
	__linebox->nchar = __linebox->cursor = 0;
	__linebox->buffer[0] = '\x0';
	print_linebox (__linebox);
}

int run_linebox ( LINEBOX *__linebox )
{
	int i, key= 0;
	char *check_in = (char *) malloc (2*sizeof (char));
	check_in[1] = '\x0';
	
	curs_set (1);
	noecho ();
	wmove ( __linebox->dialog_linebox->dialog_window, __linebox->border ,
		__linebox->cursor + __linebox->border + ((__linebox->cursor > __linebox->cols) ? 1:0) - __linebox->cini );
	
	wrefresh (__linebox->dialog_linebox->dialog_window );
	
	do {
		key = wgetch (__linebox->dialog_linebox->dialog_window);
		
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
			case 0:
				break;
			case linebox_backspace_127:
			case linebox_backspace:
				if (__linebox->cursor > 0) {
					linebox_move (__linebox->buffer, __linebox->cursor - 1, __linebox->nchar);
					__linebox->buffer [--__linebox->nchar] = '\x0';
					__linebox->cursor --;
					(__linebox->cini > 0) ? __linebox->cini -- : NULL;
					(__linebox->cini > 0) ? __linebox->cend -- : NULL;
				}
			break;
			case linebox_delete:
				if(__linebox->cursor < __linebox->nchar){
						linebox_move (__linebox->buffer, __linebox->cursor, __linebox->nchar);
						__linebox->buffer[--__linebox->nchar] = '\x0';
						(__linebox->cini > 0) ? __linebox->cini -- : NULL;
						(__linebox->cini > 0) ? __linebox->cend -- : NULL;
				}
			break;
			case linebox_insert:
				__linebox->insert = !__linebox->insert;
			break;
			case linebox_left:
				if (__linebox->cursor > 0)
					__linebox->cursor --;
			break;
			case linebox_pgup:
				return -1;
			case linebox_pgdown:
				return 1;
			case linebox_enter:
				return 1;
			case linebox_tab:
				return 1;
			case linebox_right:
				if (__linebox->cursor < __linebox->nchar)
					__linebox->cursor ++;
			break;
			case linebox_home:
				__linebox->cursor = 0;
				__linebox->cini   = 0;
			break;
			case linebox_end:
				__linebox->cursor = __linebox->nchar;
				__linebox->cend = __linebox->nchar;
				__linebox->cini = __linebox->nchar - __linebox->cols ;
				if (__linebox->cini < 0) __linebox->cini = 0;
			break;
			default:
				check_in [0] = (char) key;
				if (strspn (check_in, __linebox->valid) == strlen (check_in)) {
					if (__linebox->insert && __linebox->cursor < __linebox->max) { 
						__linebox->buffer [__linebox->cursor + __linebox->cini] = (char)key;
						__linebox->cursor ++;
						(__linebox->cursor > __linebox->nchar) ? __linebox->nchar ++ : 0;
						if (__linebox->cursor+1 > __linebox->cols && __linebox->cursor+1 > __linebox->nchar) {
							__linebox->cini ++, __linebox->cend ++;
						}
					}
					if (!__linebox->insert && __linebox->nchar < __linebox->max) {
						linebox_insert_move(__linebox->buffer, __linebox->cursor, __linebox->nchar);
						__linebox->buffer [__linebox->cursor] = (char)key;
						__linebox->cursor ++;
						__linebox->nchar  ++;
						if (__linebox->cursor+1 > __linebox->cols && __linebox->cursor+1 > __linebox->nchar) {
							__linebox->cini ++, __linebox->cend ++;
						}
					}
				}
			break;
		}

		__linebox->cend = __linebox->nchar;
		
		for (i=0; i<__linebox->cols; i++)
			mvwprintw (__linebox->dialog_linebox->dialog_window, __linebox->border, i+ (__linebox->border), "_");

		print_str (__linebox);
		wmove ( __linebox->dialog_linebox->dialog_window, __linebox->border , __linebox->cursor + __linebox->border + ((__linebox->cursor > __linebox->cols) ? 1:0) - __linebox->cini );
		refresh_dialog (__linebox->dialog_linebox);
		wrefresh (__linebox->dialog_linebox->dialog_window);
		__linebox->buffer [__linebox->nchar] = '\x0';

	} while (1);
	
	curs_set (0);
	return 1;
}
