/***************************************************************************
 *            fileman.cpp
 *
 *  Thu Nov 29 16:12:00 2004
 *  Copyright  2004  tmoretto
 *  thiago.moretto@gmail.com
 ****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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

USR * new_data ( int n_alloc )
{
	USR *data;
	data = new USR [n_alloc];
	return data;
}

USR * load_data ( int n_alloc )
{	
#if 0
	int per = access (DATA_FILE , R_OK);
	if (per == -1)
		return (USR *)NULL;	
#endif
	USR *n_data;
	n_data = new_data (n_alloc);
	
	FILE *fp;
	if ( (fp = fopen (DATA_FILE, "rb")) == NULL )
			return (USR *)NULL;
	
	fread (n_data , sizeof (USR), n_alloc, fp);
	fclose (fp);
	return n_data;
}


int save_data ( USR *data , int n_alloc )
{
	int per = access (DATA_FILE , W_OK);
	if (per == -1) 
		return P_DENIED;
	
	FILE *fp;
	if ( (fp = fopen (DATA_FILE, "wb")) == NULL )
			return FAILURE;
	
	fwrite (data , sizeof (USR), n_alloc, fp);
	fclose (fp);
	return 1;
}

void find_data ( USR *data, int n_alloc )
{
	return ;
}

int put_nalloc ( int n_alloc )
{
	int per = access (N_ALLOC , W_OK);
	if (per == -1)
		return P_DENIED;

	if (n_alloc <= 0)
		n_alloc = 0;	
	
	FILE *fp;
	if ( (fp = fopen (N_ALLOC, "wb")) == NULL )
			return FAILURE;
	
	fwrite ( &n_alloc , sizeof (int), 1, fp);
	fclose (fp);
	return 1;
}

int get_nalloc (void)
{
	int n_alloc;
	int per = access (N_ALLOC , R_OK);
	if (per == -1)
		return P_DENIED;
	
	FILE *fp;
	
	if ( (fp = fopen (N_ALLOC, "rb")) == NULL ) {
			put_nalloc(0);
			return get_nalloc ();
	}
	
	if (fread (&n_alloc , sizeof (int), 1, fp) <= 0) {
		put_nalloc (0);
		return 0;
	}
	
	fclose (fp);
	return n_alloc;
}
