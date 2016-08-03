struct list {
	char *value;
	struct list *next;
	struct list *prev;
	void *userdata;
};

#undef BUFF
#define BUFF 0x0200

int list_create(struct list **list);
int list_load(struct list **list, char *fname);
int list_count(struct list **list);
int list_save(struct list **list, char *fname);
int list_destroy(struct list **list);
