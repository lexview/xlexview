#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

#include "font.h"
#include "list.h"
#include "parse.h"

#include "main.h"

// Display

Display *display;
Window window;
XEvent event;
XSetWindowAttributes attributes; 
XWindowAttributes window_attr;
GC gc; 
Visual *visual;
#ifdef X11R4UP
Atom wm_delete_window;
#endif
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
	char str[256];
	int x,y;
	int n;
	
	// Check parameter.
	if (argc != 2) {
		fprintf(stdout, "usage: lexview lexicon_file\n");
		return -1;
	}

	// Create font.
	font_create();

	// Load font.
	for (n=0; n < 10; n++) {
	    int rc;
	    sprintf(str, FONT_PATH"/font%d.fnt", n);
	    rc=font_load(0x30 + n, 8, 19, str);
	    if (!rc) {
		fprintf(stderr, "error load file %s\n", str);
	    }
	}
	
	// Load file.
	list_create(&list_first);
	list_load(&list_first, argv[1]);

	// 
	
	display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "error: can't open display.\n");
		exit(1);
	}

	screen = DefaultScreen(display);
	visual = DefaultVisual(display,screen);
	depth  = DefaultDepth(display,screen); 

	attributes.event_mask            = ExposureMask | StructureNotifyMask;
	attributes.background_pixel      = white = XWhitePixel(display,screen); 
	attributes.border_pixel          = black = XBlackPixel(display,screen); 
	attributes.override_redirect     = 0; 
  
	window= XCreateWindow(display, XRootWindow(display,screen),
                   200,200, 600,500,5, depth, InputOutput, visual,
                   CWBackPixel | CWBorderPixel | CWBorderPixel, // CWOverrideRedirect,
		   &attributes);

	XSelectInput(display, window, ExposureMask | ButtonPressMask | KeyPressMask );

	#ifdef X11R4UP
	wm_delete_window=XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &wm_delete_window, 1);
	#endif

	XMapWindow(display,window); 

	gc = XCreateGC(display, window, 0, NULL);
  
	while (!quit) {
		XPending(display);
		XNextEvent(display, &event); 
		XGetWindowAttributes(display, window, &window_attr);
		if (event.type == Expose) { 

			if (event.xexpose.count > 0)
				continue;

			XSetForeground(display, gc, white);
			XFillRectangle(display, window, gc, 0, 0, window_attr.width, window_attr.height);
			
			XSetForeground(display, gc, black);

			list_current=list_first;
			cur=cx=cy=0;
			while ((list_current != NULL) && ( (cur-line)*19 < window_attr.height)) {
				if (cur >= line) {
					parse(cx, cy, list_current->value, strlen(list_current->value));
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
					event.xexpose.count=0;
					XSendEvent(display, window, True, ExposureMask, &event);
					break;
				case Button5:
					XDrawPoint(display, window, gc, x, y);
					if (1)
						line++;

					event.type=Expose;
					event.xexpose.count=0;
					XSendEvent(display, window, True, ExposureMask, &event);
					break;
			}
		} else
		if (event.type == KeyPress) {
			KeySym ks;

			ks = XLookupKeysym(&event.xkey, 0);

//			ks = XKeycodeToKeysym(display, ((XKeyEvent *)&event)->keycode, 1);

			if (ks == XK_Home) {
				line=0;

				event.type=Expose;
				event.xexpose.count=1;
				XSendEvent(display, window, True, ExposureMask, &event);
			} else
			if (ks == XK_Up) {
				if (line > 0)
					line--;

				event.type=Expose;
				event.xexpose.count=1;
				XSendEvent(display, window, True, ExposureMask, &event);
			} else
			if (ks == XK_Down) {
				line++;

				event.type=Expose;
				event.xexpose.count=1;
				XSendEvent(display, window, True, ExposureMask, &event);
			} else
			if (ks == XK_End) {
				line=list_count(&list_first);
				
				event.type=Expose;
				event.xexpose.count=1;
				XSendEvent(display, window, True, ExposureMask, &event);
			} else
			if (ks == XK_Escape)
				quit=1;
		}
#ifdef X11R4UP
		else
		if (event.type == ClientMessage) {
			if (event.xclient.data.l[0] == wm_delete_window)
				quit = 1;
		}
#endif
	}

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
