#ifndef _LIST_H_
#define _LIST_H_

struct _list{
	void* data;
	struct _list *next;
	struct _list *prev;
};

/* �o�������X�g���� */
struct _list *list_append(struct _list *, void *);
struct _list *list_copy(struct _list *);
struct _list *list_end_search(struct _list *);
struct _list *list_nth(struct _list *, int );
void list_free(struct _list *);
void list_only_free(struct _list *);
int list_length(struct _list *);

/* �m�F�p */
void list_print(struct _list *);

#endif
