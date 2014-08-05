#include<stdio.h>
#include<stdlib.h>
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

static int aryMush[MUSH_ARRAY][2];

/********************************************************
                       MUSHROOM
*********************************************************/
void init_mush(int map[][YMAX], struct _list *room_list)
{
	int i, x, y, select_room, cnt;
	LPROOM room;
	
	for(i = 0; i < MUSH_ARRAY; i++){
		aryMush[i][0] = 0;
		aryMush[i][1] = 0;
	}
	
	x = 0; y = 0;
	for(i = 0; i < MUSH_01_MAX; i++){
		select_room = get_random_range(0, list_length(room_list));
		room = list_nth(room_list, select_room)->data;
		cnt = 0;
		while(map[x][y] != NONE && cnt < 100){
			x = get_random_range(room->lx, room->hx);
			y = get_random_range(room->ly, room->hy);
			cnt++;
		}
		if(cnt < 100){
			aryMush[i][0] = x;
			aryMush[i][1] = y;
			if(get_random_range(0, 2) == 0){
				map[x][y] = MUSH_01;
			}
		} else i--;
	}
	
	for(i = 0; i < MUSH_02_MAX; i++){
		select_room = get_random_range(0, list_length(room_list));
		room = list_nth(room_list, select_room)->data;
		cnt = 0;
		while(map[x][y] != NONE && cnt < 100){
			x = get_random_range(room->lx, room->hx);
			y = get_random_range(room->ly, room->hy);
			cnt++;
		}
		if(cnt < 100){
			aryMush[i+MUSH_01_MAX][0] = x;
			aryMush[i+MUSH_01_MAX][1] = y;
			if(get_random_range(0, 2) == 0){
				map[x][y] = MUSH_02;
			}
		} else i--;
	}
}

/********************************************************
                           EAT
*********************************************************/
void eat_mush(LPSPLAYER pl, int mush)
{
	switch(mush){
	case MUSH_01:
		find_mush_01(pl);
		break;
	case MUSH_02:
		find_mush_02(pl);
		break;
	}
}

/***********
mushroom 01
***********/
void find_mush_01(LPSPLAYER pl)
{
	int n[3], plus, key, g;
	int rate[9] = {-40, 5, 40, 15, -5, 30, -10, 20, 99};
	char str1[][15] = {"赤くて", "青くて", "黄色くて"},
	str2[][15] = {"甘そうな", "辛そうな", "酸っぱそうな"},
	str3[][31] = {"もじゃもじゃキノコ", "猛々しいキノコ", "光ってるキノコ"};
	
	n[0] = get_random_range(0, 3); printf("%s", str1[n[0]]);
	n[1] = get_random_range(0, 3); printf("%s", str2[n[1]]);
	n[2] = get_random_range(0, 3); printf("%s", str3[n[2]]);
	plus = get_random_range(0, 10);
	if(plus > 0) printf("(+%d)", plus);
	printf("だ\n");
	printf("拾って食べますか？(Y/N) ->");
	key = input_yn_key();
	if(key == KEY_Y){
		printf("eating.....\n");
		input_enter();
		g = rate[n[0]+n[1]+n[2]];
		if(g > 0){
			printf("good taste!\n");
			if(get_random_range(0,2) == 0){
				printf("体力が%d回復", g + plus*10);
				if((pl->hp += g + plus*10) > pl->hpmax) pl->hp = pl->hpmax;
			} else {
				printf("MPが%d回復", g + plus*10);
				if((pl->mp += g + plus*10) > pl->mpmax) pl->mp = pl->mpmax;
			}
		} else {
			printf("まずッ....\n");
			if(get_random_range(0,2) == 0){
				printf("体力が%d減少", -1*(g - plus*10));
				if((pl->hp += g - plus*10) < 1) pl->hp = 1;
			} else {
				printf("MPが%d減少", -1*(g - plus*10));
				if((pl->mp += g - plus*10) < 0) pl->mp = 0;
			}
		}
		input_enter();
	} else {
		printf("キノコを破棄します ");
		input_enter();
	}
}

/***********
mushroom 02
***********/
void find_mush_02(LPSPLAYER pl)
{
	int n[2], key, g;
	int rate[4][4] = {{1, 0, 2, 3}, {3, 1, 0, 2}, {4, 2, 3, 1}, {2, 3, 1, 0}};
	char str1[][7] = {"黄金の", "白銀の", "真紅の", "深緑の"},
	str2[][15] = {"もりもりキノコ", "わくわくキノコ", "もっちりキノコ", "ふさふさキノコ"};
	
	n[0] = get_random_range(0, 4); printf("%s", str1[n[0]]);
	n[1] = get_random_range(0, 4); printf("%s", str2[n[1]]);
	printf("だ\n");
	printf("拾って食べますか？(Y/N) ->");
	key = input_yn_key();
	if(key == KEY_Y){
		printf("eating.....\n");
		input_enter();
		if(get_random_range(0, 5) > 2){
			printf("good taste!\n");
			g = rate[n[0]][n[1]];
			switch(g){
			case 0:
				printf("体力が上昇 ");
				if((pl->hpmax += 10) > 999) pl->hpmax = 999;
				break;
			case 1:
				printf("MPが上昇 ");
				if((pl->mpmax += 10) > 999) pl->mpmax = 999;
				break;
			case 2:
				printf("力が上昇 ");
				if((pl->p += 1) > 99) pl->p = 99;
				break;
			case 3:
				printf("防御が上昇 ");
				if((pl->d += 1) > 99) pl->d = 99;
				break;
			case 4:
				printf("体力・MPが上昇 ");
				if((pl->hpmax += 10) > 999) pl->hpmax = 999;
				if((pl->mpmax += 10) > 999) pl->mpmax = 999;
				break;
			}
		} else {
			printf("まずッ....\n");
			printf("体力・MPが半減... ");
			if((pl->hp = pl->hp / 2) < 1) pl->hp = 1;
			if((pl->mp = pl->mp / 2) < 1) pl->mp = 1;
		}
		input_enter();
	} else {
		printf("キノコを破棄します ");
		input_enter();
	}
	delete_mush(pl->x, pl->y);
}

/********************************************************
                   MUSHROOM OTHER
*********************************************************/
void delete_mush(int x, int y)
{
	int i;
	
	for(i = 0; i < MUSH_ARRAY; i++){
 		if(aryMush[i][0] == x && aryMush[i][1] == y){
			aryMush[i][0] = 0;
 			aryMush[i][1] = 0;
 		}
	}
}

/* p[0] = map, p[1] = &player */
task_func grow_mush(LPTCB t)
{
	int i, n, mush, flg;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	n = get_random_range(0, MUSH_ARRAY);
	if((pl->fturn % 20) == 0){
		flg = ON;
		for(i = n; i < MUSH_ARRAY; i++){
			mush = xy_map(t->p[0], 0, 0, aryMush[i][0], aryMush[i][1]);
			if(mush == NONE){
				if(i < MUSH_01_MAX){
					move_map_obj(t->p[0], 0, 0, aryMush[i][0], aryMush[i][1], MUSH_01);
				} else {
					move_map_obj(t->p[0], 0, 0, aryMush[i][0], aryMush[i][1], MUSH_02);
				}
				flg = OFF;
				break;
			}
		}
		if(flg == ON){
			for(i = 0; i < n; i++){
				mush = xy_map(t->p[0], 0, 0, aryMush[i][0], aryMush[i][1]);
				if(mush == NONE){
					if(i < MUSH_01_MAX){
						move_map_obj(t->p[0], 0, 0, aryMush[i][0], aryMush[i][1], MUSH_01);
					} else {
						move_map_obj(t->p[0], 0, 0, aryMush[i][0], aryMush[i][1], MUSH_02);
					}
					break;
				}
			}
		}
	}
}

/********************************************************
                   SHOW MAP FOR DEBUG
*********************************************************/
/* mapを表示 */
void show_map(int map[][YMAX])
{
	int i, j;
	
	for(i = 0; i < XMAX; i++){
		for(j = 0; j < YMAX; j++){
			switch(map[i][j]){
			case NONE:
				printf("　");
				break;
			case BLOCK:
				printf("■");
				break;
			case STAIRS:
				printf(">>");
				break;
			case PLAYER:
				printf("pl");
				break;
			case MONSTER:
				printf("mo");
				break;
			case MO_BOSS:
				printf("BO");
				break;
			case MUSH_01:
			case MUSH_02:
				printf("↑");
				break;
			}
		}
		printf("\n");
	}
	
	printf("\n");
}

void show_map2(int bmap[][YMAX][2])
{
	int i, j;
	
	for(i = 0; i < XMAX; i++){
		for(j = 0; j < YMAX; j++){
			switch(bmap[i][j][0]){
			case NONE:
				printf("　");
				break;
			case BLOCK:
				printf("■");
				break;
			case STAIRS:
				printf(">>");
				break;
			case PLAYER:
				printf("pl");
				break;
			case MONSTER:
				printf("mo");
				break;
			case MO_BOSS:
				printf("BO");
				break;
			case M_ROAD:
				printf("RO");
				break;
			case MARKER:
				printf("MA");
				break;
			case MUSH_01:
			case MUSH_02:
				printf("↑");
				break;
			}
		}
		printf("\n");
	}
	
	printf("\n");
}

void show_map3(int bmap[][YMAX][2])
{
	int i, j;
	
	for(i = 0; i < XMAX; i++){
		for(j = 0; j < YMAX; j++){
			switch(bmap[i][j][1]){
			case SMELL_ST:
				printf(">>");
				break;
			case SMELL_BL:
				printf("■");
				break;
			case SMELL_MO:
				printf("MO");
				break;
			case SMELL_BOSS:
				printf("BO");
				break;
			case SMELL_00:
				printf("　");
				break;
			case SMELL_01:
				printf("S1");
				break;
			case SMELL_02:
				printf("S2");
				break;
			case SMELL_03:
				printf("S3");
				break;
			case SMELL_PL:
				printf("PL");
				break;
			}
		}
		printf("\n");
	}
	
	printf("\n");
}

/********************************************************
                  SHOW MAP FOR GAME
*********************************************************/
void show_mmap(int map[][YMAX], int flg)
{
	int i, j;
	
	if(flg == KEY_Y){
		for(i = 0; i < XMAX; i++){
			for(j = 0; j < YMAX; j++){
				switch(map[i][j]){
				case BLOCK:
					printf("■");
					break;
				case STAIRS:
					printf(">>");
					break;
				case PLAYER:
					printf("pl");
					break;
				case MONSTER:
					printf("mo");
					break;
				case MO_BOSS:
					printf("BO");
					break;
				case MUSH_01:
				case MUSH_02:
					printf("↑");
					break;
				default:
					printf("　");
					break;
				}
			}
			printf("\n");
		}
	} else {
		for(i = 0; i < XMAX; i++){
			for(j = 0; j < YMAX; j++){
				switch(map[i][j]){
				case BLOCK:
					printf("■");
					break;
				case STAIRS:
					printf(">>");
					break;
				case PLAYER:
					printf("pl");
					break;
				default:
					printf("　");
					break;
				}
			}
			printf("\n");
		}
	}
	
	printf("\n");
}

void show_game_map(int map[][YMAX], int px, int py)
{
	int i, j, sx, sy;
	int gmap[G_MAP][G_MAP];
	
	sx = px - PLAYER_SITE;
	if(sx < 0) sx = 0;
	if(sx > XMAX-G_MAP) sx = XMAX-G_MAP;
	for(i = 0; i < G_MAP; i++){
		sy = py - PLAYER_SITE;
		if(sy < 0) sy = 0;
		if(sy > YMAX-G_MAP) sy = YMAX-G_MAP;
		for(j = 0; j < G_MAP; j++){
			gmap[i][j] = map[sx][sy];
			sy++;
		}
		sx++;
	}
	
	for(i = 0; i < G_MAP; i++) printf("-+");
	printf("\n");
	for(i = 0; i < G_MAP; i++){
		for(j = 0; j < G_MAP; j++){
			switch(gmap[i][j]){
			case NONE:
				printf("　");
				break;
			case BLOCK:
				printf("■");
				break;
			case STAIRS:
				printf(">>");
				break;
			case PLAYER:
				printf("pl");
				break;
			case MONSTER:
				printf("mo");
				break;
			case MO_BOSS:
				printf("BO");
				break;
			case MUSH_01:
			case MUSH_02:
				printf("↑");
				break;
			}
		}
		printf("|\n");
	}
	for(i = 0; i < G_MAP; i++) printf("-+");
	printf("\n");
}

/********************************************************
                  SHOW ANY MESSAGE
*********************************************************/
void show_equip(int wb, int n)
{
	char w[][31] = {"無し", "ナイフ", "剣", "ハンマー", "帽子"};
	char b[][31] = {"無し", "鎧", "バンダナ"};
	
	switch(wb){
	case S_W:
		printf("%s", w[n]);
		break;
	case S_B:
		printf("%s", b[n]);
		break;
	default:
		break;
	}
}

