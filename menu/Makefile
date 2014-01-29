SDIR = src/
ODIR = obj/

OBJS = $(ODIR)test.o $(ODIR)menu.o

all: menu.o test.o linker

linker:
	g++ $(OBJS) -o menu -lpanel -lcurses

menu.o:
	g++ -c $(SDIR)menu.cpp -o $(ODIR)menu.o

test.o:
	g++ -c $(SDIR)test.cpp -o $(ODIR)test.o

	

	
