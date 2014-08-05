#include<stdio.h>
#include<string.h>
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

/********************************************************
                     INPUT FUNCTION
*********************************************************/
int input_key(void)
{
	char buf[BUF_LEN];
	int c, t;
	
	fgets(buf, 2, stdin);
	sscanf(buf, "%c", &t);
	if(strchr(buf, '\n') == NULL){
		while((c = getchar()) != '\n' && c != EOF);
	}
	if((t >= ZERO && t <= NINE) ||
		(t >= KEY_A && t <= KEY_Z) || (t >= KEY_a && t <= KEY_z) ||
		t == ENTER){
			return t;
	}
	
	return -1;
}

int input_num_key(void)
{
	char buf[BUF_LEN];
	int c, t;
	
	fgets(buf, 2, stdin);
	sscanf(buf, "%c", &t);
	if(strchr(buf, '\n') == NULL){
		while((c = getchar()) != '\n' && c != EOF);
	}
	if((t >= ZERO && t <= NINE) || t == ENTER){
		return t;
	}
	
	return -1;
}

int input_char_key(void)
{
	char buf[BUF_LEN];
	int c, t;
	
	fgets(buf, 2, stdin);
	sscanf(buf, "%c", &t);
	if(strchr(buf, '\n') == NULL){
		while((c = getchar()) != '\n' && c != EOF);
	}
	if((t >= KEY_A && t <= KEY_Z) || (t >= KEY_a && t <= KEY_z) ||
		t == ENTER){
			return t;
	}
	
	return -1;
}

int input_yn_key(void)
{
	char buf[BUF_LEN];
	int c, t;
	
	fgets(buf, 2, stdin);
	sscanf(buf, "%c", &t);
	if(strchr(buf, '\n') == NULL){
		while((c = getchar()) != '\n' && c != EOF);
	}
	if(t == KEY_Y || t == KEY_y || t == ONE) return KEY_Y;
	if(t == KEY_N || t == KEY_n || t == ZERO) return KEY_N;
	
	return -1;
}

void input_enter(void)
{
	int c, t;
	char buf[BUF_LEN];
	
	fgets(buf, 1, stdin);
	sscanf(buf, "%d", &t);
	if(strchr(buf, '\n') == NULL){
		while((c = getchar()) != '\n' && c != EOF);
	}
}

/********************************************************
                      CHANGE KEY
*********************************************************/
void change_key_to_xy(int *x, int *y, int ar)
{
	switch(ar){
	case ONE:
		*x =  1;
		*y = -1;
		break;
	case TWO:
		*x = 1;
		*y = 0;
		break;
	case THREE:
		*x = 1;
		*y = 1;
		break;
	case FOUR:
		*x =  0;
		*y = -1;
		break;
	case SIX:
		*x = 0;
		*y = 1;
		break;
	case SEVEN:
		*x = -1;
		*y = -1;
		break;
	case EIGHT:
		*x = -1;
		*y =  0;
		break;
	case NINE:
		*x = -1;
		*y =  1;
		break;
	default:
		*x = 0;
		*y = 0;
		break;
	}
}

/* int to *char for 32bit signed(int, long) */
void change_i_to_ch32(int n, char *ch)
{
	int i, m, digit;
	char idtmp[32];
	
	/*
		エラー処理未実装
		しかし、Cで桁あふれをチェックできるのか。。
	*/
	
	digit = 1000000000; /* 10億 0x3B9ACA00 */
	if(n < 0){
		idtmp[0] = '-';
		n *= -1;
		i = 1;
	} else i = 0;
	
	for(; n/digit == 0; digit = digit/10){
		if(digit <= 1) break;
	}
	
	while(digit > 1){
		m = n/digit;
		switch(m){
		case 0:
			idtmp[i] = ZERO;
			break;
		case 1:
			idtmp[i] = ONE;
			break;
		case 2:
			idtmp[i] = TWO;
			break;
		case 3:
			idtmp[i] = THREE;
			break;
		case 4:
			idtmp[i] = FOUR;
			break;
		case 5:
			idtmp[i] = FIVE;
			break;
		case 6:
			idtmp[i] = SIX;
			break;
		case 7:
			idtmp[i] = SEVEN;
			break;
		case 8:
			idtmp[i] = EIGHT;
			break;
		case 9:
			idtmp[i] = NINE;
			break;
		}
		i++;
		n -= m * digit;
		digit = digit / 10;
	}
	
	switch(n){
	case 0:
		idtmp[i] = ZERO;
		break;
	case 1:
		idtmp[i] = ONE;
		break;
	case 2:
		idtmp[i] = TWO;
		break;
	case 3:
		idtmp[i] = THREE;
		break;
	case 4:
		idtmp[i] = FOUR;
		break;
	case 5:
		idtmp[i] = FIVE;
		break;
	case 6:
		idtmp[i] = SIX;
		break;
	case 7:
		idtmp[i] = SEVEN;
		break;
	case 8:
		idtmp[i] = EIGHT;
		break;
	case 9:
		idtmp[i] = NINE;
		break;
	}
	i++;
	idtmp[i] = '\0';
	strcpy(ch, idtmp);
}

/********************************************************
                       MAP COPY
*********************************************************/
void map_cpy(int map[][YMAX], int bmap[][YMAX][2])
{
	int i, j;
	
	for(i = 0; i < XMAX; i++){
		for(j = 0; j < YMAX; j++){
			bmap[i][j][0] = map[i][j];
		}
	}
}

void map_cpy2(int map[][YMAX][2], int bmap[][YMAX][2])
{
	int i, j;
	
	for(i = 0; i < XMAX; i++){
		for(j = 0; j < YMAX; j++){
			bmap[i][j][0] = map[i][j][0];
			bmap[i][j][1] = map[i][j][1];
		}
	}
}

/********************************************************
                      SET OBJECT
*********************************************************/
void set_object(int *x, int *y, struct _list *room_list, int map[][YMAX], int obj_type)
{
	int select_room;
	LPROOM room;
	
	select_room = get_random_range(0, list_length(room_list));
	room = list_nth(room_list, select_room)->data;
	for(*x = get_random_range(room->lx, room->hx), *y = get_random_range(room->ly, room->hy);
		map[*x][*y] != NONE;
		*x = get_random_range(room->lx, room->hx), *y = get_random_range(room->ly, room->hy));
	map[*x][*y] = obj_type;
}

void set_bmap_smell(int bmap[][YMAX][2], int x, int y, int smell)
{
	bmap[x][y][1] = smell;
}

void set_bmap_smell_down(int bmap[][YMAX][2], int x, int y, int n)
{
	if(n >= 0) bmap[x][y][1] -= n;
	else bmap[x][y][1] = SMELL_00;
}

void set_nine_floor(int bmap[][YMAX][2], int x, int y, int nine[][3][2])
{
	int i, j;
	
	/* nine[0]:map object, nine[1]:smell */
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			nine[i][j][1] = bmap[x-1+i][y-1+j][1];
			if(nine[i][j][1] == SMELL_PL) nine[i][j][0] = PLAYER;
			else if(nine[i][j][1] == SMELL_ST) nine[i][j][0] = STAIRS;
			else if(nine[i][j][1] == SMELL_MO) nine[i][j][0] = MONSTER;
			else if(nine[i][j][1] == SMELL_BOSS) nine[i][j][0] = MO_BOSS;
			else if(nine[i][j][1] == SMELL_BL) nine[i][j][0] = BLOCK;
			else nine[i][j][0] = bmap[x-1+i][y-1+j][0];
		}
	}
	if(nine[0][1][0] == BLOCK) {nine[0][0][0] = BLOCK; nine[0][2][0] = BLOCK;}
	if(nine[1][0][0] == BLOCK) {nine[0][0][0] = BLOCK; nine[2][0][0] = BLOCK;}
	if(nine[1][2][0] == BLOCK) {nine[0][2][0] = BLOCK; nine[2][2][0] = BLOCK;}
	if(nine[2][1][0] == BLOCK) {nine[2][0][0] = BLOCK; nine[2][2][0] = BLOCK;}
}

/********************************************************
                       MAP CHECK
*********************************************************/
int ab_cmp(int a, int b)
{
	if(a == b) return 0;
	if(a > b) return 1; else return -1;
}

/* sx,sy: 始点 */
int xy_map(int map[][YMAX], int x, int y, int sx, int sy)
{
//	printf("check sx:%d, sy:%d, x:%d, y:%d\n", sx, sy, x, y);
	return map[sx + x][sy + y];
}

int xy_map2(int bmap[][YMAX][2], int x, int y, int sx, int sy)
{
//	printf("check sx:%d, sy:%d, x:%d, y:%d\n", sx, sy, x, y);
	return bmap[sx + x][sy + y][1]; /* return smell */
}

/* 0:壁ではない
   1:斜めでない
  -1:壁がある
*/
int chk_oblique(int map[][YMAX], int x, int y, int sx, int sy)
{
/*	int px, py;
	
	px = ab_cmp(x, sx);
	py = ab_cmp(y, sy);
	
	if(px == 0 || py == 0) return 1;
	if(map[sx+px][sy] == BLOCK || map[sx][sy+py] == BLOCK) return -1;
	return 0;
*/
	if(x == 0 || y == 0) return 1;
	if(map[sx+x][sy] == BLOCK || map[sx][sy+y] == BLOCK) return -1;
	return 0;
}

/********************************************************
                      MOVE OBJECT
*********************************************************/
void move_map_obj(int map[][YMAX], int x, int y, int sx, int sy, int obj_type)
{
//	printf("go sx:%d, sy:%d, x:%d, y:%d\n", sx, sy, x, y);
	map[sx][sy] = NONE;
	map[sx + x][sy + y] = obj_type;
}

int swap_map_obj(int map[][YMAX], int x, int y, int obj_type)
{
	int t;
	t = map[x][y];
	map[x][y] = obj_type;
	return t;
}

/********************************************************
                         RANDOM
*********************************************************/
/* 0～n-1までの乱数を取得 */
int get_random(int n)
{
	return (int)(n*genrand_real2());
}

/* nからm-1の範囲内で乱数を取得(n>=0, n<m-1) */
int get_random_range(int n, int m)
{
	int x;
	
	x = get_random(m - n);
	x = x + n;
	return x;
}

/********************************************************
                      GET ANY DATA
*********************************************************/
int get_flcnt_id(int fl)
{
	int s, h;
	
	h = fl / 10;
	s = fl - h * 10;
	return s * 1000 + h * 100;
}

int get_direction(int x, int y)
{
	if(x == -1 && y == 0) return NORTH;
	else if(x == -1 && y ==  1) return NORTH_EAST;
	else if(x ==  0 && y ==  1) return EAST;
	else if(x ==  1 && y ==  1) return EAST_SOUTH;
	else if(x ==  1 && y ==  0) return SOUTH;
	else if(x ==  1 && y == -1) return SOUTH_WEST;
	else if(x ==  0 && y == -1) return WEST;
	else return WEST_NORTH;
}
