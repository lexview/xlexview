#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <X11/Xlib.h>

#include "font.h"


struct font font[FONT_MAX];

extern Display *display; 
extern Window  window; 
extern GC gc;


int font_create(void)
{
	int id;

	for(id=0; id < FONT_MAX; id++) {
		font[id].width=0;
		font[id].height=0;
		font[id].data=NULL;
	}

	return 0;
}

int font_load(unsigned char id, unsigned char width, unsigned char height, char *fname)
{
	int fd;

	fd=open(fname, O_RDONLY);
	if (fd > 0) {
		font[id].width=width;
		font[id].height=height;
		font[id].data=malloc(0x100*width*height*8);
		read(fd, (void *)font[id].data, (size_t)0x100*width*height*8);
		close(fd);
		
		return 1;
	}
	return 0;
}

inline int font_char(unsigned char id, int x, int y, unsigned char ch)
{
	register int cx, cy;
	char bm;

	for(cy=0; cy < font[id].height; cy++) {
		bm = *(font[id].data + (font[id].width*font[id].height>>3)*ch + (font[id].width>>3)*cy);

		for(cx=0; cx < font[id].width; cx++) {
			if ( bm>>cx & 1) 
				XDrawPoint(display, window, gc, x + font[id].width-cx, y + cy);
		}
	}



	return 0;
}

int font_string(unsigned char id, int x, int y, unsigned char *str, int len)
{
	int n;

	for(n=0; n < len; n++)
		font_char(id, x + n*font[id].width, y, *(str+n) );

	return 0;
}

int font_unload(unsigned char id)
{
	font[id].width=0;
	font[id].height=0;
	if (font[id].data != NULL) {
		free(font[id].data);
		font[id].data=NULL;
	}

	return 0;
}

int font_destroy(void)
{
	int id;

	for(id=0; id < FONT_MAX; id++)
		font_unload(id);

	return 0;
}
