CC=gcc
CFLAGS=-O3 -Wall -I /usr/local/include -I /usr/X11R6/include
LFLAGS=-L /usr/X11R6/lib -lX11


xlv: src/main.o src/font.o src/list.o src/parse.o
	$(CC) -o xlv src/main.o src/font.o src/list.o src/parse.o $(LFLAGS)


main.o: main.c
	$(CC) -c main.c $(CFLAGS)

font.o: font.c
	$(CC) -c font.c $(CFLAGS)

list.o: list.c
	$(CC) -c list.c $(CFLAGS)

parse.o: parse.c
	$(CC) -c parse.c $(CFLAGS)


install:
	echo "Not implemented yet!"


clean:
	rm -f *.o *~ xlv
