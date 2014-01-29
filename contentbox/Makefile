SDIR = src/
ODIR = obj/

OBJS = $(ODIR)dialog.o $(ODIR)test.o $(ODIR)contentbox.o

all: dialog.o contentbox.o test.o linker

linker:
	g++ $(OBJS) -o contentbox -lpanel -lcurses

dialog.o:
	g++ -c ../dialog/src/dialog.cpp -o $(ODIR)dialog.o

contentbox.o:
	g++ -c $(SDIR)contentbox.cpp -o $(ODIR)contentbox.o

test.o:
	g++ -c $(SDIR)test.cpp -o $(ODIR)test.o

	

	
