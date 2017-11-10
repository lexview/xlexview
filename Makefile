
CC=gcc

DATADIR=/usr/local/share/xlv

CCFLAGS = -Wall -g -O2 -I/usr/local/include -DX11R4UP

LDCCFLAGS = -L/usr/local/lib -lX11

INSTALL=install


all: xlv


install:
	install -m 0755 ./xlv /usr/local/bin/xlv
	install -d -m 0755 $(DATADIR)/fonts
	install -m 0644 ./fonts/font0.fnt $(DATADIR)/fonts/font0.fnt
	install -m 0644 ./fonts/font1.fnt $(DATADIR)/fonts/font1.fnt
	install -m 0644 ./fonts/font2.fnt $(DATADIR)/fonts/font2.fnt
	install -m 0644 ./fonts/font3.fnt $(DATADIR)/fonts/font3.fnt
	install -m 0644 ./fonts/font4.fnt $(DATADIR)/fonts/font4.fnt
	install -m 0644 ./fonts/font5.fnt $(DATADIR)/fonts/font5.fnt
	install -m 0644 ./fonts/font6.fnt $(DATADIR)/fonts/font6.fnt
	install -m 0644 ./fonts/font7.fnt $(DATADIR)/fonts/font7.fnt
	install -m 0644 ./fonts/font8.fnt $(DATADIR)/fonts/font8.fnt
	install -m 0644 ./fonts/font9.fnt $(DATADIR)/fonts/font9.fnt


clean:
	find . -name "*.o" -delete


xlv:
	$(CC) $(CCFLAGS) -o src/main.o -c src/main.c
	$(CC) $(CCFLAGS) -o src/font.o -c src/font.c
	$(CC) $(CCFLAGS) -o src/list.o -c src/list.c
	$(CC) $(CCFLAGS) -o src/parse.o -c src/parse.c
	$(CC) $(CCFLAGS) -o xlv src/main.o src/font.o src/list.o src/parse.o $(LDCCFLAGS)

