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
                    MONSTER TYPE 01

ƒ‚ƒ“ƒXƒ^[‚ÍL‚¢‚Ì‹­‚¢•û‚ÖŒü‚©‚Á‚Ä“®‚­(d‚È‚ç‚È‚¢‚æ‚¤‚É)
*********************************************************/
/* p[0] = &mo[i], p[1] = player, p[2] = bmap */
task_func monster_move1(LPTCB t)
{
	int i, j, x, y, flg;
	int nine[3][3][2];
	LPSMONSTER mo;
	
	mo = (LPSMONSTER)t->p[0];
	
//	show_map3(t->p[2]);
	set_nine_floor(t->p[2], mo->x, mo->y, nine);
	flg = OFF;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			if(nine[i][j][0] == PLAYER) flg = ON;
		}
	}
	if(flg == ON){
//		printf("attack!\n");
		monster_attacked(t, 1.0);
	} else {
		if(check_room(t) != ON){
			R_hand_walk(nine, &mo->direction, 4);
			x = 1; y = 1;
			for(i = 0; i < 3; i++){
				for(j = 0; j < 3; j++){
					if(nine[i][j][0] == MARKER){
						x = i;
						y = j;
					}
				}
			}
			set_bmap_smell(t->p[2], mo->x, mo->y, SMELL_01);
			mo->x = mo->x - 1 + x;
			mo->y = mo->y - 1 + y;
			set_bmap_smell(t->p[2], mo->x, mo->y, SMELL_MO);
		} else {
			x = mo->x; y = mo->y;
			walk_to_player(t, nine, &x, &y);
			set_bmap_smell(t->p[2], mo->x, mo->y, SMELL_01);
			mo->x = x;
			mo->y = y;
			set_bmap_smell(t->p[2], mo->x, mo->y, SMELL_MO);
		}
	}
}

/********************************************************
                    MONSTER TYPE 02

*********************************************************/
/* p[0] = &mo[i], p[1] = player, p[2] = bmap */
task_func monster_move2(LPTCB t)
{
	int i, j, x, y, flg;
	int nine[3][3][2];
	LPSMONSTER mo;
	
	mo = (LPSMONSTER)t->p[0];
	
//	show_map3(t->p[2]);
	set_nine_floor(t->p[2], mo->x, mo->y, nine);
	flg = chk_boss_site(t, 2); /* ”¼Œa2 */
	if((flg == 1 || flg == 2) && (get_random_range(0, 5) == 3)){
		printf("special attack");
		input_enter();
		if(mo->mp >= 5){
			monster_attacked(t, 1.5);
			mo->mp -= 5;
		} else {
			printf("enemy gains mp");
			input_enter();
			input_enter();
			mo->mp = mo->mpmax;
		}
	} else if(flg == 1){
		printf("bomb!\n");
		monster_attacked(t ,1.0);
	} else {
		if(check_room(t) != ON){
			R_hand_walk(nine, &mo->direction, 4);
			x = 1; y = 1;
			for(i = 0; i < 3; i++){
				for(j = 0; j < 3; j++){
					if(nine[i][j][0] == MARKER){
						x = i;
						y = j;
					}
				}
			}
			set_bmap_smell(t->p[2], mo->x, mo->y, SMELL_02);
			mo->x = mo->x - 1 + x;
			mo->y = mo->y - 1 + y;
			set_bmap_smell(t->p[2], mo->x, mo->y, SMELL_BOSS);
		} else {
			x = mo->x; y = mo->y;
			walk_to_player(t, nine, &x, &y);
			set_bmap_smell(t->p[2], mo->x, mo->y, SMELL_02);
			mo->x = x;
			mo->y = y;
			set_bmap_smell(t->p[2], mo->x, mo->y, SMELL_BOSS);
		}
	}
}

/********************************************************
                        MOVE
*********************************************************/
/* p[0] = &mo[i], p[1] = player, p[2] = bmap */
int check_room(LPTCB t)
{
	int i, px, py, obj;
	LPSMONSTER mo;
	LPSPLAYER pl;
	
	mo = (LPSMONSTER)t->p[0];
	pl = (LPSPLAYER)t->p[1];
	
	px = ab_cmp(pl->x, mo->x);
	py = ab_cmp(pl->y, mo->y);
/*	px = 0; py = 0;
	if(mo->x < pl->x) px = 1; else px = -1;
	if(mo->y < pl->y) py = 1; else py = -1;
*/
	
	if(px == 0){
		for(i = 1; i <= MONSTER_SITE; i++){
			obj = xy_map2(t->p[2], 0, i*py, mo->x, mo->y);
			if(obj == SMELL_BL) return OFF;
			else if(obj == SMELL_PL) return ON;
		}
		return OFF;
	} else if(py == 0){
		for(i = 1; i <= MONSTER_SITE; i++){
			obj = xy_map2(t->p[2], i*px, 0, mo->x, mo->y);
			if(obj == SMELL_BL) return OFF;
			else if(obj == SMELL_PL) return ON;
		}
		return OFF;
	} else {
		int flg;
		flg = UNKNOWN;
		search_pl(t, &flg, mo->x, mo->y, px, py, MONSTER_SITE);
		return flg;
	}
}

/* ‹t‚kŽšŒ^‚ÉÄ‹A“I‚ÉƒvƒŒƒCƒ„[‚ª‚¢‚é‚©’T‚· */
void search_pl(LPTCB t, int *flg, int x, int y, int px, int py, int size)
{
	int i, obj;
	
	if(size > 0){
		search_pl(t, flg, x, y, px, py, size-1);
		if(*flg == UNKNOWN){
			for(i = 1; i < size; i++){
				obj = xy_map2(t->p[2], 0, 0, x+(i*px), y+(size*py));
				if(obj == SMELL_BL){
					*flg = OFF;
					return;
				} else if(obj == SMELL_PL){
					*flg = ON;
					return;
				}
			}
			for(i = 1; i <= size; i++){
				obj = xy_map2(t->p[2], 0, 0, x+(size*px), y+(i*py));
				if(obj == SMELL_BL){
					*flg = OFF;
					return;
				} else if(obj == SMELL_PL){
					*flg = ON;
					return;
				}
			}
		}
	}
}

/* ‰EŽè–@‚ÅM_ROAD‚ði‚Þ
   ‚½‚¾‚µAŽü‚è‚É“¹‚ª–³‚¢‚Æ‚«‚ÍL‚¢‚Ì‹­‚¢•û‚Éi‚Þ */
void R_hand_walk(int nine[][3][2], int *direction, int n)
{
	if(n > 0){
		switch(*direction){
		case NORTH:
		case NORTH_EAST:
			if(nine[1][2][0] == M_ROAD){
				nine[1][2][0] = MARKER;
				*direction = EAST;
			} else {
				*direction = WEST;
				R_hand_walk(nine, direction, n-1);
			}
			break;
		case WEST:
		case WEST_NORTH:
			if(nine[0][1][0] == M_ROAD){
				nine[0][1][0] = MARKER;
				*direction = NORTH;
			} else {
				*direction = SOUTH;
				R_hand_walk(nine, direction, n-1);
			}
			break;
		case SOUTH:
		case SOUTH_WEST:
			if(nine[1][0][0] == M_ROAD){
				nine[1][0][0] = MARKER;
				*direction = WEST;
			} else {
				*direction = EAST;
				R_hand_walk(nine, direction, n-1);
			}
			break;
		case EAST:
		case EAST_SOUTH:
			if(nine[2][1][0] == M_ROAD){
				nine[2][1][0] = MARKER;
				*direction = SOUTH;
			} else {
				*direction = NORTH;
				R_hand_walk(nine, direction, n-1);
			}
			break;
		}
	} else {
		int i, x, y, s_x, s_y;
		s_x = 1;
		s_y = 1;
		i = 0;
		while(i < 5){
			x = get_random_range(0, 3);
			y = get_random_range(0, 3);
			if((nine[x][y][0] == NONE) && (nine[x][y][1] > nine[s_x][s_y][1])){
				s_x = x;
				s_y = y;
				nine[x][y][0] = BLOCK;
			}
			i++;
		}
		nine[s_x][s_y][0] = MARKER;
		*direction = get_random_range(0, 8);
	}
}

/* Player‚ÉŒü‚©‚Á‚Äi‚Þ */
/* p[0] = &mo[i], p[1] = player, p[2] = bmap */
void walk_to_player(LPTCB t, int nine[3][3][2], int *x, int *y)
{
	int px, py, obj, direction;
	LPSMONSTER mo;
	LPSPLAYER pl;
	
	mo = (LPSMONSTER)t->p[0];
	pl = (LPSPLAYER)t->p[1];
	
	px = ab_cmp(pl->x, mo->x);
	py = ab_cmp(pl->y, mo->y);
	direction = get_direction(px, py);
	
	obj = nine[1+px][1+py][0];
	if(!(obj == NONE || obj == M_ROAD))
		walk_by_direction(nine, direction, direction, &px, &py, &direction, 0);
	
	*x += px;
	*y += py;
	mo->direction = direction;
}

void walk_by_direction(int nine[][3][2], int d1, int d2, int *px, int *py, int *d, int n)
{
	int dmap[16] = {
		-1,  0,
		-1,  1,
		 0,  1,
		 1,  1,
		 1,  0,
		 1, -1,
		 0, -1,
		-1, -1
	};
	
	if(n < 4){
		if(d1 > WEST_NORTH) d1 = NORTH;
		if(d2 < NORTH) d2 = WEST_NORTH;
		if(direct(dmap, nine, d1, px, py)) *d = d1;
		else if(direct(dmap, nine, d2, px, py)) *d = d2;
		else walk_by_direction(nine, d1+1, d2-1, px, py, d, n+1);
	}
}

int direct(int dmap[], int nine[][3][2], int d, int *px, int *py)
{
	int obj;
	
	obj = nine[1+(dmap[d*2])][1+(dmap[d*2+1])][0];
	if(obj == NONE || obj == M_ROAD){
		*px = dmap[d*2];
		*py = dmap[d*2+1];
		return 1;
	}
	return 0;
}

/* ”¼Œan‚Ü‚Å‚ÌŠÔ‚Å
   player‚Æ‚ÌŠÔ‚É‰½‚à–³‚¢A‚à‚µ‚­‚Ímonster‚Ìê‡player‚Æ‚Ì”¼Œa‚ð•Ô‚·
   Œ©‚Â‚©‚ç‚È‚©‚Á‚½ê‡0‚ð•Ô‚· */
/* p[0] = &mo[i], p[1] = player, p[2] = bmap */
int chk_boss_site(LPTCB t, int n)
{
	int i, j, px, py, obj;
	LPSMONSTER mo;
	LPSPLAYER pl;
	
	mo = (LPSMONSTER)t->p[0];
	pl = (LPSPLAYER)t->p[1];
	
	px = ab_cmp(pl->x, mo->x);
	py = ab_cmp(pl->y, mo->y);
	
	if((pl->x - mo->x)*px > n) return 0;
	if((pl->y - mo->y)*py > n) return 0;
	
	for(i = 1; i <= n; i++){
		for(j = 0; j < i; j++){
			obj = xy_map2(t->p[2], 0, 0, mo->x+(j*px), mo->y+(i*py));
			if(obj == SMELL_BL){
				return 0;
			} else if(obj == SMELL_PL){
				return i;
			}
		}
		for(j = 0; j <= i; j++){
			obj = xy_map2(t->p[2], 0, 0, mo->x+(i*px), mo->y+(j*py));
			if(obj == SMELL_BL){
				return 0;
			} else if(obj == SMELL_PL){
				return i;
			}
		}
	}
	
	return 0;
}

/********************************************************
                        ATTACK
*********************************************************/
/* p[0] = &mo[i], p[1] = player, p[2] = bmap */
task_func monster_attacked(LPTCB t, double extra)
{
	int b, damage;
	double mper;
	LPSMONSTER mo;
	LPSPLAYER pl;
	
	mo = (LPSMONSTER)t->p[0];
	pl = (LPSPLAYER)t->p[1];
	
	mper = genrand_real2();
	if(mper > 0.1 && pl->s != ON){
		b = pl->equip.body / 100;
		damage = (mo->p * 10 / (pl->d+pl->equip.body-b*100)) * extra;
		printf("%s attacked! %4d DAMAGE! ", mo->name, damage);
		input_enter();
		pl->hp -= damage;
		if(pl->hp < 1) game_over_end(t);
	} else {
		printf("%s attacked! miss! ", mo->name);
		input_enter();
		pl->s = OFF;
	}
}
