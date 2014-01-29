TOPDIR := $(shell /bin/pwd)
VERSION := $(shell /bin/cat $(TOPDIR)/../.VERSION)
SOURCEDIR := ../
OBJDIR := $(TOPDIR)/obj/
BINNAME := userconf
BUILD_ROOT := $(BINNAME)-$(VERSION)/

OBJS = $(OBJDIR)grid.o $(OBJDIR)fileman.o $(OBJDIR)button.o $(OBJDIR)contentbox.o $(OBJDIR)dialog.o $(OBJDIR)linebox.o $(OBJDIR)menu.o $(OBJDIR)main.o

HOSTCC = g++
HOSTCFLAGS := -I../include -lpanel -lcurses -ggdb -DVERSION=\"$(VERSION)\" -DHORARIO
OBJCFLAGS := -I../include -DVERSION=\"$(VERSION)\" -DUSERCONF

RPMDIR = /usr/src/redhat/
RPMBUILD = $(shell /usr/bin/which rpmbuild)

all: grid.o fileman.o button.o contentbox.o dialog.o linebox.o menu.o main.o linker

linker:
	$(HOSTCC) $(OBJS) -o $(BINNAME) $(HOSTCFLAGS) -p

main.o:
	g++ -c src/main.cpp -o $(OBJDIR)main.o $(OBJCFLAGS)

button.o:
	g++ -c $(SOURCEDIR)button/src/button.cpp -o $(OBJDIR)button.o $(OBJCFLAGS)

contentbox.o:
	g++ -c $(SOURCEDIR)contentbox/src/contentbox.cpp -o $(OBJDIR)contentbox.o $(OBJCFLAGS)

dialog.o:
	g++ -c $(SOURCEDIR)dialog/src/dialog.cpp -o $(OBJDIR)dialog.o $(OBJCFLAGS)

linebox.o:
	g++ -c $(SOURCEDIR)linebox/src/linebox.cpp -o $(OBJDIR)linebox.o $(OBJCFLAGS)

menu.o:
	g++ -c $(SOURCEDIR)menu/src/menu.cpp -o $(OBJDIR)menu.o $(OBJCFLAGS)

fileman.o:
	g++ -c $(SOURCEDIR)fileman/src/fileman.cpp -o $(OBJDIR)fileman.o $(OBJCFLAGS)

grid.o:
	g++ -c $(SOURCEDIR)grid/src/grid.cpp -o $(OBJDIR)grid.o $(OBJCFLAGS)

clean:
	rm $(OBJDIR)/*.o

devel-pkg:
	echo "#!/bin/sh " > tmpmake;\
	echo " cd ../../ " >> tmpmake; \
	echo " tar czf curplat-$(VERSION)-devel-pkg.tar.gz curplat-$(VERSION)-devel 2> \
			curplat-$(VERSION)-devel/.devel-pkg-error.log " \
			>> tmpmake;
	chmod +x tmpmake
	./tmpmake
	rm -f tmpmake
	@echo "Error/Warning found: $(shell /usr/bin/wc -l ../.devel-pkg-error.log | cut -c1-2)"
	@cat ../.devel-pkg-error.log

release:
#	if [ -d $(BUILD_ROOT) ]; then \
		rm $(BUILD_ROOT) -rf;
		mkdir $(BUILD_ROOT)/src -p;
#	fi
	cp Makefile.release $(BUILD_ROOT)Makefile; \
	cp $(SOURCEDIR)button/src/button.* $(BUILD_ROOT)src -f; \
	cp $(SOURCEDIR)contentbox/src/contentbox.* $(BUILD_ROOT)src -f; \
	cp $(SOURCEDIR)dialog/src/dialog.* $(BUILD_ROOT)src -f; \
	cp $(SOURCEDIR)linebox/src/linebox.* $(BUILD_ROOT)src -f;  \
	cp $(SOURCEDIR)menu/src/menu.* $(BUILD_ROOT)src -f; \
	cp src/main.cpp $(BUILD_ROOT)src; \
	echo "$(VERSION)" > $(BUILD_ROOT).VERSION
	tar czf ../$(BINNAME)-$(VERSION).tar.gz $(BUILD_ROOT)
	rm -rf $(BUILD_ROOT)
	
rpm: release
	@if [ ! -e $(RPMBUILD) ]; then \
		echo "Is not a RPM-based system"; \
	else \
		if [ ! -d $(RPMDIR) ]; then \
			mkdir $(RPMDIR)/SOURCES -p; \
			mkdir $(RPMDIR)/BUILD/i386 -p; \
		fi \
		cp ../$(BINNAME)-$(VERSION).tar.gz $(RPMDIR)SOURCES; \
		touch $(RPMDIR)SPECS/$(BINNAME)-$(VERSION).spec; \
	fi
	
