/***************************************************************************
 *            button.h
 *
 *  Thu Nov 25 20:47:02 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/

#ifndef _BUTTON_H
#define _BUTTON_H

typedef struct {
	int color_pair ;
	int color_pair_p ;
	char *label;
	DIALOG *dialog_button;
} BUTTON;

extern BUTTON * new_button ( DIALOG *__dialog ,  int nlines, int cols, int begin_y, int begin_x, int color_pair , int color_pair_p );
extern void setlabel ( BUTTON *__button, char *label);
extern void print_button ( BUTTON *__button );
extern void select_button ( BUTTON *__button );
extern void deselect_button ( BUTTON *__button );
extern int run_button ( BUTTON *__button );
extern void update_xy ( BUTTON *__button, int var_y, int var_x );
extern void free_button ( BUTTON *__button );
extern void del_button ( BUTTON *__button );
extern void del_buttons (int n, ...);

#endif /* _BUTTON_H */
