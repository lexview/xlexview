#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>

#include "font.h"
#include "list.h"
#include "parse.h"

// Display

Display *display;
Window window;
XEvent event;
XSetWindowAttributes attributes; 
XWindowAttributes window_attr;
GC gc; 
Visual *visual; 
int screen;
int depth;

// Declaration.

unsigned long black, white;
struct list *list_first=NULL;
struct list *list_current=NULL;
int cx=0, cy=0;
int line=0;
int cur=0;

// Function.

int main(int argc, char *argv[])
{
	unsigned char quit=0;
	int x,y;
	
	// Check parameter.
	if (argc != 2) {
		fprintf(stdout, "usage: lexview lexicon_file\n");
		return -1;
	}

	// Create font.
	font_create();

	// Load font.
	font_load(0x30, 8, 19, "fonts/font0.fnt");
	font_load(0x31, 8, 19, "fonts/font1.fnt");
	font_load(0x32, 8, 19, "fonts/font2.fnt");
	font_load(0x33, 8, 19, "fonts/font3.fnt");
	font_load(0x34, 8, 19, "fonts/font4.fnt");
	font_load(0x35, 8, 19, "fonts/font5.fnt");
	font_load(0x36, 8, 19, "fonts/font6.fnt");
	font_load(0x37, 8, 19, "fonts/font7.fnt");
	font_load(0x38, 8, 19, "fonts/font8.fnt");
	font_load(0x39, 8, 19, "fonts/font9.fnt");

	// Load file.
	list_create(&list_first);
	list_load(&list_first, argv[1]);

	// 
	
	display = XOpenDisplay(NULL);
	screen = DefaultScreen(display);
	visual = DefaultVisual(display,screen);
	depth  = DefaultDepth(display,screen); 

	attributes.background_pixel      = white = XWhitePixel(display,screen); 
	attributes.border_pixel          = black = XBlackPixel(display,screen); 
	attributes.override_redirect     = 0; 
  
	window= XCreateWindow(display, XRootWindow(display,screen),
                   200,200, 300,200,5, depth, InputOutput, visual,
                   CWBackPixel | CWBorderPixel | CWOverrideRedirect,
		   &attributes);

	XSelectInput(display, window, ExposureMask | ButtonPressMask | KeyPressMask);

	XMapWindow(display,window); 

	gc = XCreateGC(display, window, 0, NULL);
  
	do { 
		XNextEvent(display, &event); 
		XGetWindowAttributes(display, window, &window_attr);
		if (event.type == Expose) { 
//			if (event.xexpose.count > 0)
//				break;

			XSetForeground(display, gc, white);
			XFillRectangle(display, window, gc, 0, 0, window_attr.width, window_attr.height);
			
			XSetForeground(display, gc, black);

			list_current=list_first;
			cur=cx=cy=0;
			while ((list_current != NULL) && ( (cur-line)*19 < window_attr.height)) {
				if (cur > line) {
					parse(0 + cx, 0 + cy, list_current->value, strlen(list_current->value));
					/*cx=0;*/ cy += 19;
				}

				list_current=list_current->next;
				cur++;
			}
		} else
		if (event.type == ButtonPress) {
			x = event.xbutton.x;
			y = event.xbutton.y;
			
			switch (event.xbutton.button) {
				case Button4:
					XDrawPoint(display, window, gc, x, y);
					if (line > 0)
						line--;

					event.type=Expose;
//					event.xexpose.count=1;
					XSendEvent(display, window, True, ExposureMask, &event);
					break;
				case Button5:
					XDrawPoint(display, window, gc, x, y);
						if (1)
							line++;

					event.type=Expose;
//					event.xexpose.count=1;
					XSendEvent(display, window, True, ExposureMask, &event);
					break;
			}
		} else
		if (event.type == KeyPress) {
			quit=1;
		}
	} while (!quit); 
  
	XCloseDisplay(display); 

	// Unload file.
	list_destroy(&list_first);

	// Unload font.
	font_unload(0x39);
	font_unload(0x38);
	font_unload(0x37);
	font_unload(0x36);
	font_unload(0x35);
	font_unload(0x34);
	font_unload(0x33);
	font_unload(0x32);
	font_unload(0x31);
	font_unload(0x30);

	// Font destroy.
	font_destroy();

	return 0;
}
