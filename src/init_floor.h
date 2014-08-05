#ifndef _INIT_FLOOR_H_
#define _INIT_FLOOR_H_

enum{
	MIN_ROOM_SIZE = 4, /* 最小部屋サイズ */
	MARGIN_RECT_ROOM = 2, /* 区画と部屋のマージン */
	MIN_RECT_SIZE = (MIN_ROOM_SIZE + (MARGIN_RECT_ROOM * 2)), /* 分割最小単位 */
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

/* 自動生成呼び出し */
struct _list* init_floor(int map[][YMAX], int bmap[][YMAX][2]);

/* 区画分け */
void rect_split(LPRECT);
LPRECT rect_add(int ,int ,int ,int );

/* 部屋作成 */
void room_make(void);
LPROOM room_add(int ,int ,int ,int );

/* 通路作成 */
void line(int ,int ,int ,int ,int map[][YMAX], int bmap[][YMAX][2]);
void couple_more(void);
LPCOUPLE couple_add(int ,LPRECT ,LPRECT );

#endif
