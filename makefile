AR=      /usr/bin/ar
RANLIB=  /usr/bin/ar ts
IPATH=   -I. -Isput-1.3.0
CC=cc

override CFLAGS += $(IPATH) -ansi -g -Wall -Wformat -pedantic -O1 -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes

OBJ=dynar.o
SOURCES=dynar.c
HEADERS=dynar.h
TESTDIR=test

$(OBJ):
	$(CC) $(CFLAGS) -c $(HEADERS) $(SOURCES)


TEST_OPEN=test_open
TEST_OPEN_SRC=$(TESTDIR)/test_open.c
$(TEST_OPEN):
	$(CC) $(CFLAGS) $(TEST_OPEN_SRC) -o $(TESTDIR)/$(TEST_OPEN)

all: clean obj

test: $(TEST_OPEN)

obj: $(OBJ)

clean:
	rm -f $(OBJ) $(LIB) $(HEADERS).gch
