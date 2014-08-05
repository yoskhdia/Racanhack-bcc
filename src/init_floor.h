#ifndef _INIT_FLOOR_H_
#define _INIT_FLOOR_H_

enum{
	MIN_ROOM_SIZE = 4, /* �ŏ������T�C�Y */
	MARGIN_RECT_ROOM = 2, /* ���ƕ����̃}�[�W�� */
	MIN_RECT_SIZE = (MIN_ROOM_SIZE + (MARGIN_RECT_ROOM * 2)), /* �����ŏ��P�� */
	COUPLE_VERTICAL = 0,
	COUPLE_HORIZONAL = 1,
};

typedef struct _rect{
	GBOOLEAN done_split_v;
	GBOOLEAN done_split_h;
	int lx;
	int ly;
	int hx;
	int hy;
	struct _room *room;
} RECT, *LPRECT;

typedef struct _room{
	int lx;
	int ly;
	int hx;
	int hy;
} ROOM, *LPROOM;

typedef struct _couple{
	int v_or_h;
	struct _rect *rect0, *rect1;
} COUPLE, *LPCOUPLE;

/* ���������Ăяo�� */
struct _list* init_floor(int map[][YMAX], int bmap[][YMAX][2]);

/* ��敪�� */
void rect_split(LPRECT);
LPRECT rect_add(int ,int ,int ,int );

/* �����쐬 */
void room_make(void);
LPROOM room_add(int ,int ,int ,int );

/* �ʘH�쐬 */
void line(int ,int ,int ,int ,int map[][YMAX], int bmap[][YMAX][2]);
void couple_more(void);
LPCOUPLE couple_add(int ,LPRECT ,LPRECT );

#endif
