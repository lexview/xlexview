struct font {
//	unsigend char id;

	unsigned char width;
	unsigned char height;

	unsigned char *data;
};

#define FONT_MAX  0x100

int font_create(void);
int font_load(unsigned char id, unsigned char width, unsigned char height, char *fname);
int font_char(unsigned char id, int x, int y, unsigned char ch);
int font_string(unsigned char id, int x, int y, unsigned char *str, int len);
int font_unload(unsigned char id);
int font_destroy(void);
