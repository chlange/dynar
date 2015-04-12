AR=      /usr/bin/ar
RANLIB=  /usr/bin/ar ts
IPATH=   -I. -Isput-1.3.1
CC=cc

override CFLAGS += $(IPATH) -Werror -ansi -Wall -Wformat -pedantic -O1 -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes

OBJ=dynar.o
SOURCES=dynar.c
HEADERS=dynar.h
TESTDIR=test
TESTSRCS=$(wildcard $(TESTDIR)/*.c)
TESTBINARIES=$(patsubst %.c,%,$(wildcard $(TESTDIR)/*.c))

$(OBJ):
	$(CC) $(CFLAGS) -c $(HEADERS) $(SOURCES)

$(TESTBINARIES): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $@.c -o $@

obj: $(OBJ)

tests: $(TESTBINARIES)

all: obj tests

clean:
	rm -f $(OBJ) $(LIB) $(HEADERS).gch $(TESTBINARIES)