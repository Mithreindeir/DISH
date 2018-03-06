CXX = gcc
EXE = shell
SOURCES = $(wildcard *.c)
SOURCES += ascii/draw.c ascii/escape.c ascii/syntax.c ascii/term.c
OBJS = $(SOURCES:.c=.o)
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	CXXFLAGS = -ggdb3 -std=c99 -pedantic -Wall -Wextra
endif

.c.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete.

$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS) -lc

clean:
		rm $(EXE) $(OBJS)
