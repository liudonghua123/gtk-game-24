ALL_EXES=$(shell ls *.exe 2>/dev/null)
.PHONY:  all clean
.PRECIOUS: %.exe

CC=gcc
CFLAGS=-g -Wall
GTKFLAGS=$(shell pkg-config --cflags gtk+-3.0)
GTKLIBS=$(shell pkg-config --libs gtk+-3.0)
YAMLFLAGS=$(shell pkg-config --cflags  yaml-0.1)
YAMLLIBS=$(shell pkg-config --libs  yaml-0.1)
PWD=$(shell pwd)

clean:
	@echo cleanning $(shell ls *.exe 2>/dev/null)
	$(shell sh -c "rm *.exe 2>/dev/null")

%: %.c
	@echo building $^
	$(CC) $(GTKFLAGS) -o $@ $^ $(GTKLIBS) $(CFLAGS) 

run_%: %.exe
	@echo running $<
	$(PWD)/$<

update-main-code: 
	glade-ui-parse-cli -s main.glade

main: main.c game.c game-data-parser.c
	@echo building $^
	$(CC) $(GTKFLAGS) $(YAMLFLAGS) -o $@ $^ $(GTKLIBS) $(YAMLLIBS) $(CFLAGS) 