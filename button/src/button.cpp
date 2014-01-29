/***************************************************************************
 *            button.cpp v 0.1
 *
 *  Thu Nov 25 20:45:13 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/

#include <curses.h>
#include <panel.h>
#include <string.h>

#include "dialog.h"

#define TAB   9
#define ENTER 10
#define PGUP 339
#define PGDOWN 338

typedef struct {
	int color_pair ;
	int color_pair_p ;
	char *label;
	DIALOG *dialog_button;
} BUTTON;

BUTTON * new_button ( DIALOG *__dialog ,  int nlines, int cols, int begin_y, int begin_x, int color_pair , int color_pair_p )
{
	BUTTON * __button;
	__button = new BUTTON;
	
	__button->dialog_button = sub_dialog ( __dialog , nlines, cols, begin_y, begin_x , color_pair );
	__button->color_pair = color_pair, __button->color_pair_p = color_pair_p;
	__button->label = NULL;

	return __button;	
}

void free_button ( BUTTON *__button )
{
	delete __button;
}

void del_button ( BUTTON *__button )
{
	hide_dialog (__button->dialog_button);
	del_dialog (__button->dialog_button);
	free_button ( __button );
}

void del_buttons (int n, ...) {
	int i;
	va_list b;
	va_start (b, n);
	
	for (i=0; i<n; i++)
		del_button (   va_arg(b, BUTTON *)  );

	va_end (b);
}

void setlabel ( BUTTON *__button, char *label)
{
	__button->label = new char [ strlen (label) + 1];
	strcpy (__button->label, label);
}

void print_button ( BUTTON *__button )
{
	wbkgd (__button->dialog_button->dialog_window , COLOR_PAIR (__button->color_pair));
	box (__button->dialog_button->dialog_window , ACS_VLINE, ACS_HLINE);
	
	if (__button->label != NULL)
	mvwprintw (__button->dialog_button->dialog_window, 1, (__button->dialog_button->cols - strlen(__button->label))/2, "%s", __button->label);
	
	wrefresh (__button->dialog_button->dialog_window );
}

void select_button ( BUTTON *__button )
{
	wbkgd (__button->dialog_button->dialog_window, COLOR_PAIR (__button->color_pair_p));
	box (__button->dialog_button->dialog_window , ACS_VLINE, ACS_HLINE);
	wrefresh (__button->dialog_button->dialog_window);
}

void deselect_button ( BUTTON *__button )
{
	wbkgd (__button->dialog_button->dialog_window, COLOR_PAIR (__button->color_pair));
	box (__button->dialog_button->dialog_window , ACS_VLINE, ACS_HLINE);
	wrefresh (__button->dialog_button->dialog_window);
}

int run_button ( BUTTON *__button )
{
	int key, opt;
	select_button ( __button );
	keypad (__button->dialog_button->dialog_window, true);
	curs_set (0);
	
	do {
	 key = wgetch (__button->dialog_button->dialog_window);
	 switch (key) {
#ifdef USERCONF
			case KEY_F(1): deselect_button ( __button );return KEY_F(1);
			case KEY_F(2): deselect_button ( __button );return KEY_F(2);
			case KEY_F(3): deselect_button ( __button );return KEY_F(3);
			case KEY_F(4): deselect_button ( __button );return KEY_F(4);
			case KEY_F(5): deselect_button ( __button );return KEY_F(5);
			case KEY_F(6): deselect_button ( __button );return KEY_F(6);
			case KEY_F(7): deselect_button ( __button );return KEY_F(7);
			case KEY_F(8): deselect_button ( __button );return KEY_F(8);
			case KEY_F(9): deselect_button ( __button );return KEY_F(9);
			case KEY_F(10): deselect_button ( __button );return KEY_F(10);
			case KEY_F(11): deselect_button ( __button );return KEY_F(11);
			case KEY_F(12): deselect_button ( __button );return KEY_F(12);
#endif				
		case ENTER:
			deselect_button ( __button );
			return 0;
		case TAB:
			deselect_button ( __button );
			return 1;
		case KEY_RIGHT:
			deselect_button ( __button );
			return 1;
		case KEY_DOWN:
			deselect_button ( __button );
			return 1;
		case KEY_LEFT:
			deselect_button ( __button );
			return -1;
		case KEY_UP:
			deselect_button ( __button );
			return -1;
		case PGUP:
			deselect_button ( __button );
			return -1;
		case PGDOWN:
			deselect_button ( __button );
			return 1;
		default:
			continue;
	 }
    } while (1);
	
	return opt;
}

void update_xy ( BUTTON *__button, int var_y, int var_x )
{
	__button->dialog_button->begin_y += var_y;
	__button->dialog_button->begin_x += var_x;
	mvwin (	__button->dialog_button->dialog_window,
			__button->dialog_button->begin_y,
			__button->dialog_button->begin_x);
	wrefresh (__button->dialog_button->dialog_window);
}
