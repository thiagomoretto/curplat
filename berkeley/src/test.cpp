/***************************************************************************
 *            test.cpp
 *
 *  Sat Dec  4 01:51:47 2004
 *  Copyright  2004  User
 *  Email
 ****************************************************************************/
 
 #include <stdlib.h>
 #include <stdio.h>
 #include <errno.h>
 #include "db.h"
 
 int main (void)
 {
	DB *db;
	int ret;
	 
	ret = db_open ("test.db", DB_BTREE, DB_CREATE, 0600, NULL, NULL, &db);
	 
	if (ret) {
		perror ("db_open");
		exit (EXIT_FAILURE);
	}
	 
	db->close (db, 0);
	exit (EXIT_SUCCESS);
 }
