#include <string.h>

#include "font.h"

#include "parse.h"

int parse(int x, int y, char *str, int len)
{
	int underline=0;
	int command=0;
	unsigned char font=0;
	unsigned char ch;
	int pos;
	int cx=0, cy=0;

	for(pos=0; pos < len; pos++) {
		if ( command ) {
			ch=(unsigned char)*(str+pos);
			switch (ch)
			{
				case 0x2E:
					underline=0;
					break;

				case 0x5F:
					underline=1;
					break;

				case 0x30:
					font = 0;
					break;

				case 0x31:
					font = 1;
					break;

				case 0x32:
					font = 2;
					break;

				case 0x33:
					font = 3;
					break;

				case 0x34:
					font = 4;
					break;

				case 0x35:
					font = 5;
					break;

				case 0x36:
					font = 6;
					break;

				case 0x37:
					font = 7;
					break;

				case 0x38:
					font = 8;
					break;

				case 0x39:
					font = 9;
					break;

				case 0xFF:
					font_char(0x30 + font, x + cx, y + cy, 0xFF);
					break;

			}
			command=0;

		} else {
			switch ( (unsigned char)*(str+pos) )
			{
				case 0x09:
					cx = (cx/8+1)*8;
					break;

				case 0xFF:
					command=1;
					break;

				default:
					font_char(0x30 + font, x + 8*cx, y + cy, *(str+pos));

					if (underline) {
						font_char(0x30 + font, x + 8*cx, y + cy, '_');
					}

					cx++;
			}
		}
	}

	return 0;
}
