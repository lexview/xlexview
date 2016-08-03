CC=gcc
CFLAGS=-Wall -g -O2
CFLAGS+=-I/usr/local/include -DX11R4UP
LFLAGS=-L/usr/local/lib -lX11
INSTALL=install

OBJS=src/main.o src/font.o src/list.o src/parse.o

all: xlexview

xlexview: $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS)

%.o : %.c Makefile
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o xlexview
