#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<assert.h>
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

/* グローバル変数 */
struct _list *rect_list;
struct _list *room_list;
struct _list *couple_list;

/*
	動作確認用メインルーチン                                */
/*int main()
{
	int map[XMAX][YMAX];
	
	init_genrand((unsigned long)time(NULL)); // 必ずmainに入れること
	init_floor(map);
	show_map(map);
	
	return 0;
}
*/

/********************************************************
                     INIT FLOOR
*********************************************************/
/* 自動生成呼び出し */
struct _list* init_floor(int map[][YMAX], int bmap[][YMAX][2])
{
	int i, j;
	struct _list *li;
//	LPRECT rect;
	LPROOM room;
	LPCOUPLE couple;
	int c0x, c0y, c1x, c1y;
	
	/* map初期化 */
	for(i = 0; i < XMAX; i++){
		for(j = 0; j < YMAX; j++){
			map[i][j] = BLOCK;
			bmap[i][j][1] = SMELL_BL;
		}
	}
	rect_list = NULL;
	room_list = NULL;
	couple_list = NULL;
	
	rect_split(rect_add(0, 0, XMAX-1, YMAX-1));
	room_make();
	couple_more();
	
	/* map処理 */
/*	for(li = rect_list; li != NULL; li = li->next){
		rect = (LPRECT)li->data;
		for(i = rect->lx, j = rect->ly; i <= rect->hx; i++) map[i][j] = BLOCK;
		for(i = rect->lx, j = rect->hy; i <= rect->hx; i++) map[i][j] = BLOCK;
		for(i = rect->lx, j = rect->ly; j <= rect->hy; j++) map[i][j] = BLOCK;
		for(i = rect->hx, j = rect->ly; j <= rect->hy; j++) map[i][j] = BLOCK;
	}*/
	for(li = room_list; li != NULL; li = li->next){
		room = (LPROOM)li->data;
		for(i = room->lx; i <= room->hx; i++){
			for(j = room->ly; j <= room->hy; j++){
				map[i][j] = NONE;
				bmap[i][j][1] = SMELL_00;
			}
		}
	}
	
	map_cpy(map, bmap);
	
	for(li = couple_list; li != NULL; li = li->next){
		couple = (LPCOUPLE)li->data;
		switch(couple->v_or_h){
		case COUPLE_HORIZONAL:
			assert(couple->rect0->hx == couple->rect1->lx);
			c0x = couple->rect0->hx;
			c0y = get_random_range(couple->rect0->room->ly + 1, couple->rect0->room->hy);
			c1x = couple->rect1->lx;
			c1y = get_random_range(couple->rect1->room->ly + 1, couple->rect1->room->hy);
			line(c0x, c0y, c1x, c1y, map, bmap);
			line(couple->rect0->room->hx, c0y, c0x, c0y, map, bmap);
			line(couple->rect1->room->lx, c1y, c1x, c1y, map, bmap);
			bmap[couple->rect0->room->hx][c0y][0] = MARKER;
			bmap[couple->rect1->room->lx][c1y][0] = MARKER;
			break;
		case COUPLE_VERTICAL:
			assert(couple->rect0->hy == couple->rect1->ly);
			c0x = get_random_range(couple->rect0->room->lx + 1, couple->rect0->room->hx);
			c0y = couple->rect0->hy;
			c1x = get_random_range(couple->rect1->room->lx + 1, couple->rect1->room->hx);
			c1y = couple->rect1->ly;
			line(c0x, c0y, c1x, c1y, map, bmap);
			line(c0x, couple->rect0->room->hy, c0x, c0y, map, bmap);
			line(c1x, couple->rect1->room->ly, c1x, c1y, map, bmap);
			bmap[c0x][couple->rect0->room->hy][0] = MARKER;
			bmap[c1x][couple->rect1->room->ly][0] = MARKER;
			break;
		}
	}
	
	make_monster_road(bmap, room_list);
	
	list_free(rect_list);
	list_free(couple_list);
	
	return room_list;
}

/********************************************************
                     RECT SPLIT
	rect_split
	rect_add
*********************************************************/
void rect_split(LPRECT rect_p) /* rect_p: 親 */
{
	int split_x, split_y;
	LPRECT rect_c; /* rect_c: 子 */
	
	if(get_random_range(0, 256) == 0){
		rect_p->done_split_v = TRUE;
		rect_p->done_split_h = TRUE;
	}
	if(rect_p->hy - rect_p->ly <= MIN_RECT_SIZE * 2){
		rect_p->done_split_v = TRUE;
	}
	if(rect_p->hx - rect_p->lx <= MIN_RECT_SIZE * 2){
		rect_p->done_split_h = TRUE;
	}
	if((rect_p->done_split_v) && (rect_p->done_split_h)){
		return;
	}
	rect_c = rect_add(rect_p->lx, rect_p->ly, rect_p->hx, rect_p->hy);
	
	if(rect_p->done_split_v == FALSE){
		split_y = get_random_range(rect_p->ly + MIN_RECT_SIZE, rect_p->hy - MIN_RECT_SIZE);
		rect_p->hy = split_y;
		rect_c->ly = split_y;
		rect_p->done_split_v = TRUE;
		rect_c->done_split_v = TRUE;
		couple_add(COUPLE_VERTICAL, rect_p, rect_c);
		rect_split(rect_p);
		rect_split(rect_c);
		return;
	} else {
		split_x = get_random_range(rect_p->lx + MIN_RECT_SIZE, rect_p->hx - MIN_RECT_SIZE);
		rect_p->hx = split_x;
		rect_c->lx = split_x;
		rect_p->done_split_h = TRUE;
		rect_c->done_split_h = TRUE;
		couple_add(COUPLE_HORIZONAL, rect_p, rect_c);
		rect_split(rect_p);
		rect_split(rect_c);
		return;
	}
}

LPRECT rect_add(int lx, int ly, int hx, int hy)
{
	LPRECT rect;
	rect = (LPRECT)malloc(sizeof(struct _rect));
	if(rect == NULL){
		perror("error: failed memory allocation.\n");
		list_free(rect_list);
		list_free(couple_list);
		exit(-1);
	}
	
	rect->done_split_v = FALSE;
	rect->done_split_h = FALSE;
	rect->lx = lx;
	rect->ly = ly;
	rect->hx = hx;
	rect->hy = hy;
	rect_list = list_append(rect_list, rect);
	if(rect_list == NULL){
		perror("error: failed memory allocation - rect_list.\n");
		free(rect);
		list_free(couple_list);
		exit(-1);
	}
	
	return rect;
}

/********************************************************
                         ROOM
	room_make
	room_add
*********************************************************/
void room_make(void)
{
	struct _list *li;
	LPRECT rect;
	int x, y, w, h;
	
	for(li = rect_list; li != NULL; li = li->next){
		rect = (LPRECT)li->data;
		w = get_random_range(MIN_ROOM_SIZE, rect->hx - rect->lx - (MARGIN_RECT_ROOM * 2) + 1);
		h = get_random_range(MIN_ROOM_SIZE, rect->hy - rect->ly - (MARGIN_RECT_ROOM * 2) + 1);
		x = get_random_range(rect->lx + MARGIN_RECT_ROOM, rect->hx - MARGIN_RECT_ROOM - w + 1);
		y = get_random_range(rect->ly + MARGIN_RECT_ROOM, rect->hy - MARGIN_RECT_ROOM - h + 1);
		rect->room = room_add(x, y, x+w, y+h);
	}
}

LPROOM room_add(int lx, int ly, int hx, int hy)
{
	LPROOM room;
	room = (LPROOM)malloc(sizeof(struct _room));
	if(room == NULL){
		perror("error: failed memory allocation.\n");
		list_free(room_list);
		list_free(rect_list);
		list_free(couple_list);
		exit(-1);
	}
	
	room->lx = lx;
	room->ly = ly;
	room->hx = hx;
	room->hy = hy;
	room_list = list_append(room_list, room);
	if(room_list == NULL){
		perror("error: failed memory allocation - room_list.\n");
		free(room);
		list_free(rect_list);
		list_free(couple_list);
		exit(-1);
	}
	return room;
}

/********************************************************
                         LINE
	line
	couple_more
	coupe_add
*********************************************************/
void line(int x0, int y0, int x1, int y1, int map[][YMAX], int bmap[][YMAX][2])
{
	int min_x, max_x, min_y, max_y, i, j;
	
	min_x = MIN(x0, x1);
	max_x = MAX(x0, x1);
	min_y = MIN(y0, y1);
	max_y = MAX(y0, y1);
	assert((min_x >= 0) && (max_x < XMAX) && (min_y >= 0) && (max_y < YMAX));
	if((x0 <= x1) && (y0 >= y1)){
		for(i = min_x; i <= max_x; i++){
			map[i][max_y] = NONE;
			bmap[i][max_y][0] = M_ROAD;
			bmap[i][max_y][1] = SMELL_00;
		}
		for(j = min_y; j <= max_y; j++){
			map[max_x][j] = NONE;
			bmap[max_x][j][0] = M_ROAD;
			bmap[max_x][j][1] = SMELL_00;
		}
		return;
	}
	if((x0 > x1) && (y0 > y1)){
		for(i = min_x; i <= max_x; i++){
			map[i][min_y] = NONE;
			bmap[i][min_y][0] = M_ROAD;
			bmap[i][min_y][1] = SMELL_00;
		}
		for(j = min_y; j <= max_y; j++){
			map[max_x][j] = NONE;
			bmap[max_x][j][0] = M_ROAD;
			bmap[max_x][j][1] = SMELL_00;
		}
		return;
	}
	if((x0 > x1) && (y0 <= y1)){
		for(i = min_x; i <= max_x; i++){
			map[i][min_y] = NONE;
			bmap[i][min_y][0] = M_ROAD;
			bmap[i][min_y][1] = SMELL_00;
		}
		for(j = min_y; j <= max_y; j++){
			map[min_x][j] = NONE;
			bmap[min_x][j][0] = M_ROAD;
			bmap[min_x][j][1] = SMELL_00;
		}
		return;
	}
	if((x0 <= x1) && (y0 < y1)){
		for(i = min_x; i <= max_x; i++){
			map[i][max_y] = NONE;
			bmap[i][max_y][0] = M_ROAD;
			bmap[i][max_y][1] = SMELL_00;
		}
		for(j = min_y; j <= max_y; j++){
			map[min_x][j] = NONE;
			bmap[min_x][j][0] = M_ROAD;
			bmap[min_x][j][1] = SMELL_00;
		}
		return;
	}
}

void couple_more()
{
	struct _list *li;
	LPRECT rect;
	LPRECT rectmap[XMAX][YMAX];
	int i, j;
	
	for(li = rect_list; li != NULL; li = li->next){
		rect = (LPRECT)li->data;
		for(i = rect->lx; i < rect->hx; i++){
			for(j = rect->ly; j < rect->hy; j++){
				rectmap[i][j] = rect;
			}
		}
	}
	for(i = 0; i < XMAX - 2; i++){
		for(j = 0; j < YMAX - 2; j++){
			assert(rectmap[i][j] != NULL);
			assert(rectmap[i + 1][j] != NULL);
			assert(rectmap[i][j + 1] != NULL);
			if(rectmap[i][j] != rectmap[i][j + 1]){
				if(get_random_range(0, 64) == 0){
					couple_add(COUPLE_VERTICAL, rectmap[i][j], rectmap[i][j + 1]);
				}
			}
			if(rectmap[i][j] != rectmap[i + 1][j]){
				if(get_random_range(0, 64) == 0){
					couple_add(COUPLE_HORIZONAL, rectmap[i][j], rectmap[i + 1][j]);
				}
			}
		}
	}
}

LPCOUPLE couple_add(int v_or_h, LPRECT rect0, LPRECT rect1)
{
	LPCOUPLE couple;
	couple = (LPCOUPLE)malloc(sizeof(struct _couple));
	if(couple == NULL){
		perror("error: failed memory allocation.\n");
		list_free(room_list);
		list_free(rect_list);
		list_free(couple_list);
		exit(-1);
	}
	
	couple->v_or_h = v_or_h;
	couple->rect0 = rect0;
	couple->rect1 = rect1;
	couple_list = list_append(couple_list, couple);
	if(couple_list == NULL){
		perror("error: failed memory allocation - couple_list.\n");
		free(couple);
		list_free(rect_list);
		list_free(room_list);
		exit(-1);
	}
	
	return couple;
}
