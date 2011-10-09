VERSION = 0.0.1

OPTIM = -g -O0
WARN = -Wextra -Wall

CONFIG_STR = -DVERSION="${VERSION}"

PKGS = gtk+-3.0 evince-document-3.0 evince-view-3.0 libgtkhtml-4.0
CFLAGS = $(shell pkg-config --cflags ${PKGS}) ${OPTIM} ${WARN}
LDFLAGS = $(shell pkg-config --libs ${PKGS})

OBJ = main ui html-view doc-view tex-make markdown-make markdown-script
OBJFILES = ${OBJ:=.o} 

all: mupit

mupit: ${OBJFILES}
	cc -o $@ ${LDFLAGS} ${OBJFILES}

ui.o: ui.c

ui.c: ui.glade bin2c/bin2c
	bin2c/bin2c ui.glade $@

markdown-script.c: markdown/Markdown.pl bin2c/bin2c
	bin2c/bin2c markdown/Markdown.pl $@

bin2c/bin2c: bin2c/bin2c.c bin2c/Makefile
	make -C bin2c

clean:
	rm -rf mupit *.o ui.c markdown-script.c
	make -C bin2c clean

.PHONY: clean all

