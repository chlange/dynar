AR=      /usr/bin/ar
RANLIB=  /usr/bin/ar ts
IPATH=   -I.
CC=cc

override CFLAGS += -ansi -g -Wall -Wformat -pedantic -O1 -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes

OBJ=dynar.o
SOURCES=dynar.c
HEADERS=dynar.h

$(OBJ):
	$(CC) $(CFLAGS) -c $(HEADERS) $(SOURCES)

all: clean obj

obj: $(OBJ)

clean:
	rm -f $(OBJ) $(LIB) $(HEADERS).gch
