#include <curses.h>
#include <panel.h>
#include <string.h>
#include <stdlib.h>

#define KEY_ESC 	27
#define KEY_RETURN 	10

#ifndef NULL_PAIR
	#define NULL_PAIR 10
#endif

#ifndef WINPAIR
	#define WINPAIR 1
#endif

typedef struct {
	int nopt; 				/* options number */
	int x, y; 				/* initial position */
	int width; 				/* width menu*/
	int dpair, spair;		/* pair and selected pair */
	int hkpair;				/* hotkey pair */
	char **options;			/* options */
	int *hotkeys;			/* hotkeys */
	int color_pair;
} menu;

typedef struct {
	int begin_y;
	int begin_x;
	int nlines;
	int cols;
} menu_position;

void erase_window (WINDOW *winbox) {
	werase  (winbox); 							/* Erase Window */
	wclear  (winbox);							/* Clear Window */
	wbkgd 	(winbox, COLOR_PAIR(NULL_PAIR)); 	/* Define the NULL_PAIR*/
	wrefresh(winbox); 							/* Refresh Win */
	delwin  (winbox); 							/* Delete Win Reference */
}

char **alloc_opt (int h, int w)
{
	int i;
	char **ptr;
	ptr = new char *[h];
	for (i=0;i<h;i++);
	      ptr[i] = new char [w];
	return ptr;
}

char **alloc_opt_c (int h, int w)
{
	int i;
	char **ptr;
	ptr = (char **) calloc (h, sizeof (char **));
	for (i=0; i<h; i++)
		ptr [i] = (char *)calloc (w, sizeof (char));
	return ptr;
}

void free_opt (char **ptr, int h)
{
	int i;
	for (i=0;i<h;i++)
		delete ptr[i];
	delete ptr;
}

int buildmenu (WINDOW* winbox, menu *data, menu_position *mp )
{
	int pt = 0, i, j, l, key, len, fmove = 0, cm = 1;
	int mx, my;
	PANEL *panelbox, *shadowpanel;
	WINDOW *shadow;
	
	winbox = newwin( data->nopt + 2, data->width + 2, data->y, data->x);
	shadow = newwin( data->nopt + 2, data->width + 2, data->y + 1, data->x + 1);
	
	mp->nlines = data->nopt + 2;
	mp->cols = data->width + 2;
	mx = data->x;
	my = data->y;
	
	shadowpanel = new_panel (shadow);
	
	werase (winbox);
	wrefresh (shadow);
	panelbox = new_panel (winbox);
	top_panel (panelbox);
	
	
	wbkgd (winbox, COLOR_PAIR(data->color_pair));
	box (winbox, ACS_VLINE, ACS_HLINE);
	wrefresh (winbox);

	curs_set (0); /* Desabilita o uso do cursor */
	
	keypad (winbox, true); 	/* keypad enable */
	noecho ();				/* set no print keys */

	update_panels ();
	doupdate ();
	
	do {

		for (i = 0;i<data->nopt;i++)
		{
			(i == pt) ? wattron (winbox, COLOR_PAIR(data->spair)) : wattron(winbox, COLOR_PAIR(data->dpair));
				
			for (j=0;j<strlen (data->options[i]);j++) {
				(j == data->hotkeys[i] && i != pt) ? 
					wattron (winbox, COLOR_PAIR(data->hkpair)) : 0; /* Hotkey */

				mvwprintw (winbox,  i + 1, j + 1, "%c", data->options[i][j]);

				len = strlen (data->options[i]);
				for (l=len;l<data->width;l++)
					mvwprintw (winbox, i+1, l+1, " ");

				(j == data->hotkeys[i] && i != pt) ? 
					wattroff (winbox, COLOR_PAIR(data->hkpair)) : 0; /* Hotkey */
				
				/* Apos que imprimiu o hotkey, volta ao default pair ou selected pair */
				(i == pt) ? wattron (winbox, COLOR_PAIR(data->spair)) : wattron (winbox, COLOR_PAIR(data->dpair));
			}
			
			(i == pt) ? wattroff (winbox, COLOR_PAIR(data->spair)) : wattroff(winbox, COLOR_PAIR(data->dpair));
		}

		update_panels ();
		doupdate ();
		wrefresh (winbox);
		key = wgetch (winbox);

		switch (key) {
			case KEY_UP:
				if (pt == 0) pt = data->nopt;
				if (pt  > 0) pt--;
			break;
			case KEY_DOWN:
				if (pt < data->nopt  - 1) pt++;
				else 
					if (pt == data->nopt - 1) pt = 0;
			break;
			case KEY_RETURN:
				hide_panel (panelbox);
				erase_window(winbox);
				hide_panel (shadowpanel);
				del_panel (shadowpanel);
				delwin(shadow);
				return pt;
			break;
			case KEY_ESC:
				hide_panel (panelbox);
				erase_window(winbox);
				hide_panel (shadowpanel);
				del_panel (shadowpanel);
				delwin(shadow);
				return -1;
			break;
			case KEY_F(6):
				if (fmove) {	
					mvwprintw (winbox, 0, 1, "*");
					cm = 1;
					
					while (cm)
					{
						key = wgetch (winbox);
						switch (key) {
							case KEY_LEFT:
								if (mx > 0) {
									move_panel (panelbox, my, --mx );
									move_panel (shadowpanel, my + 1, mx + 1);
									update_panels();
									doupdate ();
								}
							break;
							case KEY_RIGHT:
								if (mx < COLS-1) {
									move_panel (panelbox, my, ++mx );
									move_panel (shadowpanel, my + 1, mx + 1);
									update_panels ();
									doupdate ();									
								}
							break;
							case KEY_UP:
								if (my > 0) {
									move_panel (panelbox, --my, mx );
									move_panel (shadowpanel, my + 1, mx + 1);
									update_panels();
									doupdate ();
								}
							break;
							case KEY_DOWN:
								if (my < LINES-1) {
									move_panel (panelbox, ++my, mx );
									move_panel (shadowpanel, my + 1, mx + 1);
									update_panels ();
									doupdate ();									
								}
							break;
							case KEY_F(6):
								fmove = !fmove;
								cm = 0;
							break;
						}
						mp->begin_x = mx;
						mp->begin_y = my;
					}
					
				} 
				
				box (winbox, ACS_VLINE, ACS_HLINE);
				fmove = !fmove;
			break;
			default:
				for (i=0;i<data->nopt;i++) {
					if ( key     == (int)data->options[i][data->hotkeys[i]] ||
					    (key-32) == (int)data->options[i][data->hotkeys[i]]) {
						pt = i;
						break;
					}
				}
			break;
		}

	} while (true);
}
