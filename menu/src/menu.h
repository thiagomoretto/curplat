/***************************************************************************
 *            menu.h
 *
 *  Wed Nov 24 16:43:34 2004
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
 
#ifndef _MENU_H
#define _MENU_H

#ifndef NULL_PAIR
	#define NULL_PAIR 10
#endif

#ifndef WINPAIR
	#define WINPAIR 1
#endif

typedef struct {
	int nopt;
	int x, y;
	int width;
	int dpair, spair;
	int hkpair;
	char **options;
	int *hotkeys;
	int color_pair;
} menu;

typedef struct {
	int begin_y;
	int begin_x;
	int nlines;
	int cols;
} menu_position;

extern void erase_window (WINDOW *);
extern char **alloc_opt (int , int );
extern char **alloc_opt_c (int , int );
extern void free_opt (char **, int );
extern int buildmenu (WINDOW *, menu * , menu_position * mp);

#endif /* _MENU_H */
