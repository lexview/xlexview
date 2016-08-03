#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"


int list_create(struct list **list)
{
//	struct list *list_first = *list;
//	struct list *list_current = list_first;

	*list=NULL;

	return 0;
}

int list_load(struct list **list, char *fname)
{
	struct list *list_first = *list;
	struct list *list_current = list_first;

	FILE *f = NULL;

	char buff[BUFF];
	int len;

	f=fopen(fname, "r");
	if (f==NULL) {
		return -1;
	}

	while ( fgets(buff, BUFF, f) ) {

		while ((len=strlen(buff)) && ((buff[len-1] == '\r') || (buff[len-1] == '\n')))
			buff[len-1]='\0';

		if ( list_first == NULL )
			*list=list_first=list_current=(struct list *)malloc(sizeof(struct list)); else
			list_current=list_current->next=(struct list *)malloc(sizeof(struct list));

		list_current->value=strdup(buff);
		list_current->next=NULL;
	}
	(void)fclose(f);

	return 0;
}

int list_count(struct list ** list)
{
	struct list * list_first = *list;
	struct list * list_current = list_first;

	unsigned int lines = 0;

	// list_current = list_first;
	while (list_current) {
		lines++;
		list_current = list_current->next;
	}

	return lines;
}

int list_save(struct list **list, char *fname)
{
	struct list *list_first = *list;
	struct list *list_current = list_first;

	FILE *f=NULL;

	f=fopen(fname, "w");
	if (f==NULL) {
		return -1;
	}

	while ( list_current != NULL ) {
		fprintf(f, "%s\n", list_current->value);

		list_current=list_current->next;
	}
	(void)fclose(f);

	return 0;
}

int list_destroy(struct list **list)
{
	struct list *list_first = *list;
	struct list *list_current = list_first;

	while ( list_current != NULL ) {
		*list = list_current->next;
		free(list_current->value);
		free(list_current);
		list_current = *list;
	}

	return 0;
}
