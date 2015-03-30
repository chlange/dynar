AR=      /usr/bin/ar
RANLIB=  /usr/bin/ar ts
IPATH=   -I. -Isput-1.3.0
CC=cc

override CFLAGS += $(IPATH) -Werror -ansi -g -Wall -Wformat -pedantic -O1 -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes

OBJ=dynar.o
SOURCES=dynar.c
HEADERS=dynar.h
TESTDIR=test

$(OBJ):
	$(CC) $(CFLAGS) -c $(HEADERS) $(SOURCES)


TEST_CREATE=testCreate
TEST_CREATE_SRC=$(TESTDIR)/testCreate.c
$(TEST_CREATE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(TEST_CREATE_SRC) -o $(TESTDIR)/$(TEST_CREATE)

all: clean obj

test: clean $(TEST_CREATE)

obj: $(OBJ)

clean:
	rm -f $(OBJ) $(LIB) $(HEADERS).gch $(TESTDIR)/$(TEST_CREATE)
