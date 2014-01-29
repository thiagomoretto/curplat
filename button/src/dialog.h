/***************************************************************************
 *            dialog.h
 *
 *  Thu Nov 25 20:46:08 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/

#ifndef _DIALOG_H
#define _DIALOG_H

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

extern void hide_dialog (DIALOG *__dialog);
extern void show_dialog (DIALOG *__dialog);
extern void move_dialog (DIALOG *__dialog, int new_y, int new_x );
extern void refresh_dialog (DIALOG *__dialog);
extern void del_dialog (DIALOG *__dialog);
extern void change_dialog_color_pair (DIALOG *__dialog, int color_pair );
extern DIALOG * new_dialog ( int nlines , int cols, int begin_y , int begin_x , int color_pair );
extern DIALOG * sub_dialog ( DIALOG *__dialog_supr , int nlines , int cols, int begin_y , int begin_x , int color_pair );
extern void dialog ( DIALOG *__dialog );
extern WINDOW *request_window (DIALOG *__dialog);
extern PANEL *request_panel (DIALOG *__dialog);
extern void enable_shadow (DIALOG *__dialog, int shadow_color_pair );
extern void disable_shadow (DIALOG *__dialog);

#endif /* _DIALOG_H */
