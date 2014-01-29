/***************************************************************************
 *            contentbox.h
 *
 *  Wed Nov 24 16:13:41 2004
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
 
#ifndef _CONTENTBOX_H
#define _CONTENTBOX_H

#include "dialog.h"

/**
FALTA
- Scrollock
*/

typedef struct {
	int enable_title;
	char *title;
	char **content;
	int clines, ccols;
	int bini, bend;
	int color_pair, color_pair_t;
	int nlines, cols ;
	int begin_y, begin_x;
	int in_dialog;
	DIALOG *dialog_contentbox;
} CONTENTBOX;

extern CONTENTBOX * new_contentbox ( int nlines, int cols, int begin_y, int begin_x , int color_pair, int color_pair_t );
extern CONTENTBOX * new_content ( int nlines, int cols, int begin_y, int begin_x, int color_pair, int color_pair_t );
extern void settitle ( CONTENTBOX *__contentbox, char *title );
extern void setcontent ( CONTENTBOX *__contentbox , const char * content , int lines , int cols );
extern void show_title ( CONTENTBOX *__contentbox );
extern void show_content ( CONTENTBOX *__contentbox );
extern void show_contentbox ( CONTENTBOX *__contentbox );

#endif /* _CONTENTBOX_H */
