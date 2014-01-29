/*
 * is_root.c
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main (void)
{
    struct passwd *pentry;
	char *login = getlogin ();
	uid_t user_id ;
    
	user_id = getuid ();

	if(user_id == 0) { // eh root
		if ((pentry = getpwnam(login)) == NULL) {
			perror ("getpwnam");
			
		}
	}		
	
	printf ("%d\n" , user_id);
                                                                                                                             
        return 1;
}
