SDIR = src/
ODIR = obj/

OBJS = $(ODIR)dialog.o $(ODIR)test.o $(ODIR)linebox.o

all: dialog.o linebox.o test.o linker

linker:
	g++ $(OBJS) -o linebox -lpanel -lcurses

dialog.o:
	g++ -c ../dialog/src/dialog.cpp -o $(ODIR)dialog.o

linebox.o:
	g++ -c $(SDIR)linebox.cpp -o $(ODIR)linebox.o

test.o:
	g++ -c $(SDIR)test.cpp -o $(ODIR)test.o

	

	
