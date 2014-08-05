#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include"objecttype.h"
#include"task.h"
#include"list.h"
#include"main.h"
#include"init_floor.h"
#include"player.h"
#include"monster.h"
#include"mo_move.h"
#include"setobject.h"
#include"obj_action.h"
#include"title.h"
#include"MT.h"

/* ���X�g����
	list_append
	list_end_search
	list_free                                       */
/* list�̍Ō��data�̃A�h���X���L���������X�g��ǉ� */
struct _list *list_append(struct _list *list, void *data)
{
	struct _list *list_end, *ptmp;
	
	ptmp = (struct _list*)malloc(sizeof(struct _list));
	if(ptmp == NULL){
		perror("error: failed memory allocation.\n");
		list_free(list);
		return NULL;
	}
	
	list_end = list_end_search(list);
	if(list_end) list_end->next = ptmp;
	else list = ptmp;
	
	ptmp->prev = list_end;
	ptmp->next = NULL;
	ptmp->data = data;
	
	return list;
}

/* ���X�g�̃R�s�[ */
struct _list *list_copy(struct _list *list1)
{
	struct _list *list2, *li, *list_end, *ptmp;
	
	list2 = NULL;
	list_end = list2;
	for(li = list1; li != NULL; li = li->next){
		ptmp = (struct _list*)malloc(sizeof(struct _list));
		if(ptmp == NULL){
			perror("error: failed memory allocation - list_copy.\n");
			list_only_free(list2);
			return NULL;
		}
		
		if(list_end) list_end->next = ptmp;
		else list2 = ptmp;
		
		ptmp->prev = list_end;
		ptmp->next = NULL;
		ptmp->data = li->data;
		list_end = ptmp;
	}
	
	return list2;
}

/* ���X�g�̍Ō������ */
struct _list *list_end_search(struct _list *list)
{
	struct _list *p, *backp;
	backp = NULL;
	for(p = list; p; p = p->next){
		backp = p;
	}
	return backp;
}

/* ���X�g��n�Ԗڂ̃A�h���X��Ԃ� */
struct _list *list_nth(struct _list *list, int n)
{
	int i;
	struct _list *li;
	
	li = list;
	for(i = 0; i != n; i++){
		if(li == NULL) return NULL;
		li = li->next;
	}
	return li;
}

/* ���X�g��� */
void list_free(struct _list *list)
{
	struct _list *p, *pnext;
	
	if(list != NULL){
		for(p = list; p; p = pnext){
			pnext = p->next;
			free(p->data); /* ���X�g�̎���data����� */
			free(p); /* ���X�g����� */
		}
		list = NULL;
	}
}

void list_only_free(struct _list *list)
{
	struct _list *p, *pnext;
	
	if(list != NULL){
		for(p = list; p; p = pnext){
			pnext = p->next;
			free(p); /* ���X�g����� */
		}
		list = NULL;
	}
}

/* ���X�g�̗v�f�� */
int list_length(struct _list *list)
{
	int n;
	struct _list *li;
	
	n = 0;
	for(li = list; li != NULL; li = li->next){
		n++;
	}
	return n;
}

/* �m�F�p */
/* ���X�g�m�F�p */
void list_print(struct _list *list)
{
	int i = 0;
	struct _room *tmp; /* ���X�g�̎���data�̎�ނ� */
	struct _list *p;
	printf("list_print\n");
	for(p = list;p != NULL; p = p->next){
		tmp = (struct _room *)p->data; /* ���X�g�̎���data�̎�ނ� */
		printf("list[%d]\n", i++);
		printf("p: %x, prev:%x, next:%x\n", p, p->prev, p->next);
		printf("data: %x\n", p->data);
		printf("lx: %d, ly: %d, hx: %d, hy: %d\n", tmp->lx, tmp->ly, tmp->hx, tmp->hy);
	}
	printf("end\n");
}
