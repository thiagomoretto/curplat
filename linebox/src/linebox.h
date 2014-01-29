/***************************************************************************
 *            linebox.h
 *
 *  Wed Nov 24 16:36:36 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#ifndef _LINEBOX_H
#define _LINEBOX_H

#define NORMAL   0
#define PASSWORD 1

#include "dialog.h"

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

extern LINEBOX * new_linebox ( DIALOG *__dialog, int nlines , int cols, int begin_y, int begin_x , int color_pair  , int border, int max );
extern void change_type ( LINEBOX *__linebox, int type );
extern void print_linebox ( LINEBOX *__linebox );
extern void valid_chars ( LINEBOX *__linebox , char *accept );
extern void linebox_move (char *str, int cursor, int nchar );
extern void linebox_insert_move(char *str, int cursor, int nchar);
extern void print_str ( LINEBOX *__linebox );
extern int run_linebox ( LINEBOX *__linebox );
extern void free_linebox ( LINEBOX *__dialog );
extern void del_linebox ( LINEBOX *__linebox );
extern void reset_linebox ( LINEBOX *__linebox );

#endif /* _LINEBOX_H */
