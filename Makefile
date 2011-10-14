VERSION = 0.3

PREFIX ?= /usr/local
INSTALL = install -c

OPTIM = -g -O0
WARN = -Wextra -Wall -Wno-unused-parameter

CONFIG_STR = -DVERSION="${VERSION}"

PKGS = gtk+-3.0 evince-document-3.0 evince-view-3.0 libgtkhtml-4.0
CFLAGS = $(shell pkg-config --cflags ${PKGS}) ${OPTIM} ${WARN}
LDFLAGS = $(shell pkg-config --libs ${PKGS})

TEMPLATE_LIST = tex
TEMPLATE = $(sort ${TEMPLATE_LIST})
TEMPLATE_OBJ = template-${TEMPLATE}

OBJ_LIST = main ui html-view dvi-view pdf-view html-make tex-make markdown-make markdown-script asciidoc-make common ${TEMPLATE_OBJ}
OBJ = $(sort ${OBJ_LIST})
OBJFILES = ${OBJ:=.o} 

all: mupit

mupit: ${OBJFILES}
	cc -o $@ ${LDFLAGS} ${OBJFILES}

ui.o: ui.c

ui.c: ui.glade bin2c/bin2c
	bin2c/bin2c ui.glade $@

markdown-script.c: markdown/Markdown.pl bin2c/bin2c
	bin2c/bin2c markdown/Markdown.pl $@

template-tex.c: template/template.tex bin2c/bin2c
	bin2c/bin2c template/template.tex $@

bin2c/bin2c: bin2c/bin2c.c bin2c/Makefile
	make -C bin2c

checkdeps: check-deps.sh
	@./check-deps.sh
	@echo All dependencies good

install: mupit
	${INSTALL} mupit ${PREFIX}/bin

clean:
	rm -rf mupit *.o ui.c markdown-script.c
	make -C bin2c clean

.PHONY: clean all checkdeps install

