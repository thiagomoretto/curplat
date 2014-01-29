/*
 * fopn.c
 * Kurt Wall 140
 */
 

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main (void)
{
	int fd;
	char path [] = "hello.txt";
	
	if (fd = open (path, O_CREAT | O_RDWR, 0666) < 0) {
		perror ("open");
		exit (EXIT_FAILURE);
	}
	else
	{
		printf ("%d\n", fd);
	}
	
	if (close (fd) < 0)
	{
		perror ("close");
		exit (EXIT_FAILURE);
	}
	exit (EXIT_SUCCESS);
}
