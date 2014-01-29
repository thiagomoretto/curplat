/****************************************************************************
 *            grid.h
 *
 *  Wed Dec 01 16:12:00 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/
 
#ifndef _GRID_H
#define _GRID_H

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

extern GRID * new_grid ( WINDOW *top_window , int nlines, int cols, int begin_y, int begin_x , int color_pair, int color_pair_s );
extern void setnreg ( GRID *__grid, int nreg );
extern int setregline ( GRID *__grid, char *line, int n );
extern void put_text ( GRID *__grid );
extern void print_grid ( GRID *__grid );
extern int run_grid ( GRID *__grid );
extern void free_grid ( GRID *__grid );

#endif
