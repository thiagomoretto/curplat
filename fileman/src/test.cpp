#include <stdio.h>
#include <string.h>

#include "fileman.h"

#if 0
typedef struct {
	int id;
	char fullname	[40];
	char login		[30];
	char dir 		[30];
	char group		[30];
} USR;
#endif

int main (void)
{
	int i;
	USR *users;
	USR *users_data;

	users = new_data (2);
	
	users[0].id = 0;
	strcpy (users[0].fullname, "Ze Ruela");
	strcpy (users[0].login, "zeruela");
	strcpy (users[0].dir, "/home/ze");
	strcpy (users[0].group, "zelezins");

	users[1].id = 1;
	strcpy (users[1].fullname, "Abcdef");
	strcpy (users[1].login, "joao");
	strcpy (users[1].dir, "/home/joao");
	strcpy (users[1].group, "joao");

	if (save_data (users, 2) == FAILURE)
		printf ("deu pals!");
	
	put_nalloc (3);
	users_data = load_data (2);
	
	if (users_data != NULL) {
		for (i=0; i<2; i++) {
			printf ("Fullname: %s\n", users_data[i].fullname);
			printf ("Login: %s\n", users_data[i].login);
			printf ("Diretorio: %s\n", users_data[i].dir);
			printf ("Grupo: %s\n", users_data[i].group);
			printf ("----------------------\n");
		}
	}
	
	printf ("%d\n", get_nalloc());
	return 1;
}
