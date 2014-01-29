#include <string.h>
#include <curses.h>
#include <panel.h>

#include "dialog.h"

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

CONTENTBOX * new_contentbox ( int nlines, int cols, int begin_y, int begin_x , int color_pair, int color_pair_t )
{
	CONTENTBOX *__contentbox;
	__contentbox = new CONTENTBOX;

	__contentbox->dialog_contentbox = new_dialog ( nlines, cols, begin_y, begin_x , color_pair );
	__contentbox->nlines = nlines, __contentbox->cols = cols;
	__contentbox->begin_y = begin_y, __contentbox->begin_x = begin_x;
	__contentbox->color_pair = color_pair, __contentbox->color_pair_t = color_pair_t;
	__contentbox->in_dialog = true;
	__contentbox->enable_title = false;
	__contentbox->content = NULL;
}

CONTENTBOX * new_content ( int nlines, int cols, int begin_y, int begin_x, int color_pair, int color_pair_t )
{
	CONTENTBOX *__contentbox;
	__contentbox = new CONTENTBOX;
	__contentbox->nlines = nlines, __contentbox->cols = cols;
	__contentbox->begin_y = begin_y, __contentbox->begin_x = begin_x;
	__contentbox->color_pair = color_pair, __contentbox->color_pair_t = color_pair_t;
	__contentbox->in_dialog = false;
	__contentbox->enable_title = false;
	__contentbox->content = NULL;
}

void settitle ( CONTENTBOX *__contentbox, char *title )
{
	__contentbox->title = new char [strlen (title) + 1];
	strcpy (__contentbox->title, title );
	__contentbox->enable_title = true;
}

void setcontent ( CONTENTBOX *__contentbox , const char * content , int lines , int cols )
{
	int i , count , charc, linec;
	__contentbox->content = new char *[lines+1];
	for (i=0; i<lines; i++)
		__contentbox->content [i] = new char [cols+1];
	__contentbox->clines = lines;
	__contentbox->ccols = cols;
	
	charc = linec = i = count = 0;
	while ( !count ) {
      charc = 0;
      while ( content [i] != '\n')
        {
			__contentbox->content [linec][charc]  = content [i];
			i++, charc++;
			if (charc > cols) {
				count = 1;
				break;
			}
        }
		(charc == 0) ?__contentbox->content [linec][0] = '\x0' : 0;
		
        linec++,i++;
		if (linec > lines) break;
    }
	
	__contentbox->bini = 0;
	__contentbox->bend = lines;
}

void show_title ( CONTENTBOX *__contentbox )
{
	wattron (__contentbox->dialog_contentbox->dialog_window, COLOR_PAIR (__contentbox->color_pair_t));
	mvwprintw (__contentbox->dialog_contentbox->dialog_window,
		1,	1, "%s", __contentbox->title);
	wattroff (__contentbox->dialog_contentbox->dialog_window, COLOR_PAIR (__contentbox->color_pair_t));
}

void show_content ( CONTENTBOX *__contentbox )
{
	int i;
	wattron (__contentbox->dialog_contentbox->dialog_window, COLOR_PAIR (__contentbox->color_pair));
	for (i=__contentbox->bini; i<__contentbox->bend; i++)
		mvwprintw (__contentbox->dialog_contentbox->dialog_window,
				2 + i,	1, __contentbox->content[i]);
	wattroff (__contentbox->dialog_contentbox->dialog_window,COLOR_PAIR (__contentbox->color_pair));
}

void show_contentbox ( CONTENTBOX *__contentbox )
{
	dialog (__contentbox->dialog_contentbox);
	refresh_dialog (__contentbox->dialog_contentbox);
	show_title (__contentbox);
	show_content (__contentbox);
}
