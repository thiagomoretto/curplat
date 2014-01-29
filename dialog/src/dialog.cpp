/***************************************************************************
 *            dialog.cpp v 0.1
 *
 *  Thu Nov 25 20:44:53 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/

#include <curses.h>
#include <panel.h>

typedef struct {
	int nlines, cols;
	int begin_x , begin_y;
	int color_pair;
	PANEL *dialog_panel;
	WINDOW *dialog_window;
	int shadow_switch;
	int shadow_color_pair;
	PANEL *dialog_shadow_panel;
	WINDOW *dialog_shadow_window;
} DIALOG;

void hide_dialog (DIALOG *__dialog)
{
	(void) hide_panel (__dialog->dialog_panel);
	(__dialog->shadow_switch == true) ? hide_panel (__dialog->dialog_shadow_panel) : NULL;
	update_panels (), doupdate ();
}

void show_dialog (DIALOG *__dialog)
{
	(void) show_panel (__dialog->dialog_panel);
	(__dialog->shadow_switch == true) ? show_panel (__dialog->dialog_shadow_panel) : NULL;
	update_panels (), doupdate ();
}

void move_dialog (DIALOG *__dialog, int new_y, int new_x )
{
	(void) move_panel (__dialog->dialog_panel, new_y,  new_x );
	(__dialog->shadow_switch == true) ? move_panel (__dialog->dialog_shadow_panel, new_y + 1, new_x + 1) : NULL;
	__dialog->begin_y = new_y;
	__dialog->begin_x = new_x;
}

void refresh_dialog (DIALOG *__dialog)
{
	show_dialog (__dialog) ;
	top_panel ( __dialog->dialog_panel );
	update_panels (), doupdate();
}

void free_dialog (DIALOG *__dialog)
{
	delete __dialog;
}

void del_dialog (DIALOG *__dialog)
{
	(void) hide_panel (__dialog->dialog_panel);
	(void) del_panel (__dialog->dialog_panel);
	(void) delwin (__dialog->dialog_window);
	if (__dialog->shadow_switch == true) {
		(void) hide_panel(__dialog->dialog_shadow_panel);
		(void) del_panel(__dialog->dialog_shadow_panel);
		(void) delwin(__dialog->dialog_shadow_window);
	}
	free_dialog (__dialog);
}

void change_dialog_color_pair (DIALOG *__dialog, int color_pair )
{
	__dialog->color_pair = color_pair;
}

DIALOG * new_dialog ( int nlines , int cols, int begin_y , int begin_x , int color_pair )
{
	DIALOG *__dialog;
	__dialog = new DIALOG;

	__dialog->dialog_window = newwin ( nlines, cols , begin_y, begin_x );
	__dialog->dialog_panel = new_panel (__dialog->dialog_window);
	__dialog->nlines = nlines, __dialog->cols = cols;
	__dialog->begin_x = begin_x, __dialog->begin_y = begin_y;
	__dialog->color_pair = color_pair;
	__dialog->shadow_switch = false;
	
	return __dialog;
}

DIALOG * sub_dialog ( DIALOG *__dialog_supr , int nlines , int cols, int begin_y , int begin_x , int color_pair )
{
	DIALOG *__dialog;
	__dialog = new DIALOG;

	__dialog->dialog_window = subwin ( __dialog_supr->dialog_window,  nlines, cols , begin_y, begin_x );
	__dialog->dialog_panel = new_panel (__dialog->dialog_window);
	__dialog->nlines = nlines, __dialog->cols = cols;
	__dialog->begin_x = begin_x, __dialog->begin_y = begin_y;
	__dialog->color_pair = color_pair;
	__dialog->shadow_switch = false;
	
	return __dialog;
}

void dialog ( DIALOG *__dialog )
{
	wbkgd (__dialog->dialog_window , COLOR_PAIR (__dialog->color_pair));
	box (__dialog->dialog_window, ACS_VLINE, ACS_HLINE);
	
	update_panels(), doupdate ();
}

WINDOW *request_window (DIALOG *__dialog)
{
	return __dialog->dialog_window;
}

PANEL *request_panel (DIALOG *__dialog)
{
	return __dialog->dialog_panel;
}

void enable_shadow (DIALOG *__dialog, int shadow_color_pair )
{
	__dialog->dialog_shadow_window = newwin ( __dialog->nlines, __dialog->cols , __dialog->begin_y + 1, __dialog->begin_x + 1);
	__dialog->dialog_shadow_panel = new_panel (__dialog->dialog_shadow_window);
	wbkgd (__dialog->dialog_shadow_window, COLOR_PAIR(shadow_color_pair));
	__dialog->shadow_color_pair = shadow_color_pair;
	__dialog->shadow_switch = true;
	top_panel (__dialog->dialog_panel), bottom_panel (__dialog->dialog_shadow_panel);
	update_panels ();
	doupdate ();
}

void disable_shadow (DIALOG *__dialog)
{
	hide_panel (__dialog->dialog_shadow_panel);
	del_panel (__dialog->dialog_shadow_panel);
	delwin (__dialog->dialog_shadow_window);
	__dialog->shadow_switch = false;
}
