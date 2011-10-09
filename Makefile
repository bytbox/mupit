VERSION = 0.0.1

OPTIM = -g -O0
WARN = -Wextra -Wall

CONFIG_STR = -DVERSION="${VERSION}"

PKGS = gtk+-3.0 evince-document-3.0 evince-view-3.0
CFLAGS = $(shell pkg-config --cflags ${PKGS}) ${OPTIM} ${WARN}
LDFLAGS = $(shell pkg-config --libs ${PKGS})

all: mupit

mupit: main.o ui.o
	cc -o $@ ${LDFLAGS} main.o ui.o

ui.o: ui.c

ui.c: ui.glade bin2c/bin2c
	bin2c/bin2c ui.glade ui.c

bin2c/bin2c: bin2c/bin2c.c bin2c/Makefile
	make -C bin2c

clean:
	rm -rf mupit *.o ui.c
	make -C bin2c clean

.PHONY: clean all

