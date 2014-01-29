/***************************************************************************
 *            fileman.h
 *
 *  Thu Nov 29 16:12:00 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/

#ifndef _FILEMAN_H
#define _FILEMAN_H

#define FAILURE -2
#define P_DENIED -1

#define N_ALLOC   "data/.n_alloc"
#define DATA_FILE "data/.users"

typedef struct {
	int id;
	char fullname	[40];
	char login		[30];
	char dir 		[30];
	char group		[30];
} USR;

extern USR * new_data ( int n_alloc );
extern USR * load_data ( int n_alloc );
extern int save_data ( USR *data , int n_alloc );
extern int find_data ( USR *data, int n_alloc );
extern int put_nalloc ( int n_alloc );
extern int get_nalloc (void);

#endif /* _FILEMAN_H */
