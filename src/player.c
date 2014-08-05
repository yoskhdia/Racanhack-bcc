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

/********************************************************
                        INIT
*********************************************************/
void init_player(LPSPLAYER pl)
{
	pl->x = 0;                  /* x位置 */
	pl->y = 0;                  /* y位置 */
	pl->pturn = 1;
	
	pl->lv = 1;                 /* level */
	pl->ep = 0;                 /* experience point */
	pl->hp = 30;                /* hit point */
	pl->hpmax = 30;
	pl->mp = 20;                /* magic point */
	pl->mpmax = 20;
	pl->learn = 3;              /* 習得済み波紋 */
	pl->p = 5;                  /* power */
	pl->d = 5;                  /* diffence */
	pl->s = OFF;
	pl->equip.weapon = W_NONE;  /* 装備武器 */
	pl->equip.body   = B_NONE;  /* 装備体 */
}

/********************************************************
                    TASK FUNCTION
*********************************************************/
/* p[0] = map, p[1] = &player */
task_func player_turn(LPTCB t)
{
	int key;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	pl->s = OFF;
	
	key = -1;
	while(key == -1){
		printf("key ->");
		key = input_num_key();
		if(key >= ONE && key <= NINE) player_move(t, &key);
		if(key == ZERO) player_menu(t, &key);
//		if(key == KEY_z){show_map(t->p[0]); input_key();}
	}
}

/********************************************************
                          MENU
*********************************************************/
/* p[0] = map, p[1] = &player */
task_func player_menu(LPTCB t, int *key)
{
	LPSPLAYER pl;
	int i, mnum;
	char str[][30] = {
		"0.キャンセル",
		"1.cola fire",
		"2.逃げる",
		"3.地図",
		"4.dodge next",
		"5.cracker",
		"6.overdrive A",
		"7.overdrive B",
		"8.special attack",
		"9.allround attack"
	};
	int mps[] = {5, 10, 16, 20, 24, 29, 33, 1, 2};
	
	pl = (LPSPLAYER)t->p[1];
	printf("M E N U:\n");
	for(i = 1; i <= pl->learn; i++){
		printf("%s", str[i]);
		if(i == 4) printf("(%d)\n", pl->pturn / 20 + 5);
		else if(i == 8) printf("(%d)\n", pl->mp);
		else printf("(%d)\n", mps[i-1]);
	}
	printf("%s\n", str[0]);
	
	printf("input menu number ->");
	mnum = input_num_key();
	if(mnum > pl->learn+ZERO) mnum = ZERO;
	switch(mnum){
	case ONE:
		write_map(t);
		if(pm_cola(t) == -1){
			*key = -1;
			write_map(t);
		}
		break;
	case TWO:
		write_map(t);
		pm_escape(t);
		break;
	case THREE:
		write_map(t);
		pm_call_map(t);
		break;
	case FOUR:
		write_map(t);
		pm_next_s(t);
		break;
	case FIVE:
		write_map(t);
		pm_cracker(t);
		break;
	case SIX:
		write_map(t);
		if(pm_sun_yellow_h(t) == -1){
			*key = -1;
			write_map(t);
		}
		break;
	case SEVEN:
		write_map(t);
		pm_blue_t_h(t);
		break;
	case EIGHT:
		write_map(t);
		if(pm_eija(t) == -1){
			*key = -1;
			write_map(t);
		}
		break;
	case NINE:
		write_map(t);
		pm_h_purple(t);
		break;
	default:
		*key = -1;
		write_map(t);
		break;
	}
}

/********************************************************
                      MENU FUNCTION
*********************************************************/
/* p[0] = map, p[1] = &player */
int pm_cola(LPTCB t)
{
	int ar;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	if(pl->mp >= 5){
		printf("0.キャンセル\n");
		printf("arrow ->");
		ar = input_num_key();
		if(ar >= ONE && ar <= NINE){
			printf("cola fire!");
			input_enter();
			player_attacked(t, ar, 1.3);
			pl->mp -= 5;
			if(pl->mp < 0) pl->mp = 0;
			return 0;
		} else return -1;
	} else {
		printf("MPが足りない... ");
		input_enter();
		return 0;
	}
}

/* p[0] = map, p[1] = &player */
void pm_escape(LPTCB t)
{
	int x, y;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	
	if(pl->mp >= 10){
		printf("escape ");
		input_enter();
		for(x = get_random_range(1, XMAX), y = get_random_range(1, YMAX);
			xy_map(t->p[0], 0, 0, x, y) != NONE;
			x = get_random_range(1, XMAX), y = get_random_range(1, YMAX));
		move_map_obj(t->p[0], 0, 0, pl->x, pl->y, NONE);
		move_map_obj(t->p[0], 0, 0, x, y, PLAYER);
		pl->x = x;
		pl->y = y;
		pl->mp -= 10;
		if(pl->mp < 0) pl->mp = 0;
	} else {
		printf("MPが足りない... ");
		input_enter();
	}
}

/* p[0] = map, p[1] = &player */
void pm_call_map(LPTCB t)
{
	int flg;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	flg = KEY_N;
	
	if(pl->mp >= 16+10*pl->lv){
		printf("全ての情報を表示しますか？MP消費%d追加(Y/N) ->", 10*pl->lv);
		flg = input_yn_key();
		if(flg == KEY_Y) pl->mp -= 10*pl->lv;
//		if(pl->mp < 0) pl->mp = 0;
	}
	if(pl->mp >= 16){
		show_mmap(t->p[0], flg);
		input_enter();
		pl->mp -= 16;
		if(pl->mp < 0) pl->mp = 0;
	} else {
		printf("MPが足りない... ");
		input_enter();
	}
}

/* p[0] = map, p[1] = &player */
void pm_next_s(LPTCB t)
{
	int n;
	LPSPLAYER pl;
	char say[5][50] = {
		"hey hey hey",
		"fxxx!",
		"oh, my god",
		"help me"
	};
	
	pl = (LPSPLAYER)t->p[1];
	
	if(pl->mp >= pl->pturn / 20 + 5){
		printf("I can see your next ");
		input_enter();
		n = get_random_range(0, 5);
		printf("「%s」 ", say[n]);
		printf("isnt it!? ");
		input_enter();
		printf("１ターンの間１度だけ全ての攻撃を避ける ");
		input_enter();
		pl->s = ON;
		pl->mp -= pl->pturn / 20 + 5;
		if(pl->mp < 0) pl->mp = 0;
	} else {
		printf("MPが足りない... ");
		input_enter();
	}
}

/* p[0] = map, p[1] = &player */
void pm_cracker(LPTCB t)
{
	int i;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	
	if(pl->mp >= 24){
		printf("全方位に攻撃！ ");
		input_enter();
		printf("cracker fire! ");
		input_enter();
		for(i = ONE; i <= NINE; i++){
			player_attacked(t, i, 0.5);
		}
		pl->mp -= 24;
		if(pl->mp < 0) pl->mp = 0;
	} else {
		printf("MPが足りない... ");
		input_enter();
	}
}

/* p[0] = map, p[1] = &player */
int pm_sun_yellow_h(LPTCB t)
{
	int ar;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	
	if(pl->mp >= 29){
		printf("0.キャンセル\n");
		printf("arrow ->");
		ar = input_num_key();
		if(ar >= ONE && ar <= NINE){
			printf("overdirve! ");
			input_enter();
			player_attacked(t, ar, 2.0);
			pl->mp -= 29;
			if(pl->mp < 0) pl->mp = 0;
			return 0;
		} else return -1;
	} else {
		printf("MPが足りない... ");
		input_enter();
		return 0;
	}
}

/* p[0] = map, p[1] = &player */
void pm_blue_t_h(LPTCB t)
{
	int i;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	
	if(pl->mp >= 33){
		printf("全方位に攻撃！ ");
		input_enter();
		printf("overdirve! ");
		input_enter();
		for(i = ONE; i <= NINE; i++){
			player_attacked(t, i, 0.8);
		}
		pl->mp -= 33;
		if(pl->mp < 0) pl->mp = 0;
	} else {
		printf("MPが足りない... ");
		input_enter();
	}
}

/* p[0] = map, p[1] = &player */
int pm_eija(LPTCB t)
{
	int ar;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	
	if(pl->mp >= 1){
		printf("0.キャンセル\n");
		printf("arrow ->");
		ar = input_num_key();
		if(ar >= ONE && ar <= NINE){
			printf("special attack! ");
			input_enter();
			player_attacked(t, ar, pl->mp / 10);
			pl->mp = 0;
			return 0;
		} else return -1;
	} else {
		printf("MPが足りない... ");
		input_enter();
		return 0;
	}
}

/* p[0] = map, p[1] = &player */
void pm_h_purple(LPTCB t)
{
	int i;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	
	if(pl->mp >= 2){
		printf("全方位に攻撃！ ");
		input_enter();
		for(i = ONE; i <= NINE; i++){
			player_attacked(t, i, 2.0);
		}
		pl->mp -= 2;
		if(pl->mp < 0) pl->mp = 0;
	} else {
		printf("MPが足りない... ");
		input_enter();
	}
}

/********************************************************
                         ATTACK
*********************************************************/
/* p[0] = map, p[1] = &player */
task_func player_attacked(LPTCB t, int ar, double extra)
{
	int w, damage;
	double mper;
	LPSPLAYER pl;
	LPSMONSTER mo;
	
	pl = (LPSPLAYER)t->p[1];
	mper = genrand_real2();
	
	if(ar != -1 && mper > 0.05){
		if((mo = check_attack(pl->x, pl->y, ar)) != NULL){
			w = pl->equip.weapon / 100;
			damage = ((pl->p+pl->equip.weapon-w*100) * 10 / mo->d) * extra;
			printf("player attack [%c]%s! %4d DAMAGE! ", ar, mo->name, damage);
			input_enter();
			mo->hp -= damage;
		} else {
			printf("miss!\n");
		}
	} else printf("miss!\n");
}

LPSMONSTER check_attack(int px, int py, int ar)
{
	int i, x, y;
	LPSMONSTER mo;
	mo = get_mo_p();
	
	change_key_to_xy(&x, &y, ar);
	for(i = 0; i < MONSTER_MAX; i++){
		if(((mo+i)->x == px+x) && ((mo+i)->y == py+y)) return (mo+i);
	}
	
	return NULL;
}

/********************************************************
                          MOVE
*********************************************************/
/* p[0] = map, p[1] = &player */
task_func player_move(LPTCB t, int *ar)
{
	int x, y, objflg, *f;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	change_key_to_xy(&x, &y, *ar);
	if(*ar != -1 && chk_oblique(t->p[0], x, y, pl->x, pl->y) != -1){
		
		objflg = xy_map(t->p[0], x, y, pl->x, pl->y);
//		if(ar == FIVE) objflg = STAIRS;
		switch(objflg){
		case NONE:
			move_map_obj(t->p[0], x, y, pl->x, pl->y, PLAYER);
			(pl->x) += x;
			(pl->y) += y;
			break;
		case STAIRS:
			move_map_obj(t->p[0], x, y, pl->x, pl->y, PLAYER);
			(pl->x) += x;
			(pl->y) += y;
			f = get_flcnt_p();
			(*f)++;
			down_stairs(t);
			break;
		case MONSTER:
		case MO_BOSS:
			player_attacked(t, *ar, 1.0);
			break;
		case MUSH_01:
		case MUSH_02:
			move_map_obj(t->p[0], x, y, pl->x, pl->y, PLAYER);
			(pl->x) += x;
			(pl->y) += y;
			eat_mush(pl, objflg);
			break;
		case BLOCK:
			*ar = -1;
			write_map(t);
			break;
		}
	} else {
		*ar = -1;
		write_map(t);
	}
}

/* p[0] = map, p[1] = &player */
task_func down_stairs(LPTCB t)
{
	printf("\ndo you wanna SAVE?(Y/N) ->");
	if(input_yn_key() == KEY_Y) data_save(t);
	
	kill_all_monster();
	wakeup_prio_task(PRIO_00);
}

/********************************************************
                     OTHER TASK
*********************************************************/
/* p[0] = &player, p[1] = bmap */
task_func set_player_smell(LPTCB t)
{
	int i, j;
	int bmap[XMAX][YMAX][2];
	LPSPLAYER pl;
	
	map_cpy2(t->p[1], bmap);
	pl = (LPSPLAYER)t->p[0];
	for(i = 0; i < XMAX; i++){
		for(j = 0; j < YMAX; j++){
			if(bmap[i][j][1] > SMELL_00){
				set_bmap_smell_down(t->p[1], i, j, 1);
			}
		}
	}
	set_bmap_smell(t->p[1], pl->x, pl->y, SMELL_PL);
}

/* p[1] = &player, 他不定 */
task_func pl_level_up(LPTCB t)
{
	LPSPLAYER pl;
	int lv_e[99] = {
		      0,       8,      20,      50,     100,     180,     290,     500,     800,    1200,
		   1800,    2500,    3300,    4100,    5000,    6100,    7300,    8600,   10000,   11500,
		  14000,   17000,   20000,   24000,   29000,   35000,   42000,   50000,   60000,   70000,
		  85000,  100000,  120000,  140000,  160000,  180000,  200000,  230000,  260000,  290000,
		 330000,  370000,  410000,  460000,  510000,  560000,  620000,  680000,  750000,  820000,
		 900000, 1000000, 1100000, 1200000, 1300000, 1400000, 1500000, 1600000, 1700000, 1800000,
		1900000, 2000000, 2100000, 2200000, 2300000, 2400000, 2500000, 2600000, 2700000, 2800000,
		2900000, 3100000, 3300000, 3500000, 3700000, 3900000, 4100000, 4300000, 4500000, 4700000,
		4900000, 5100000, 5300000, 5500000, 5700000, 5900000, 6100000, 6300000, 6500000, 6700000,
		6900000, 7100000, 7300000, 7500000, 7700000, 7900000, 8400000, 9000000, 9999999
	};
	
	pl = (LPSPLAYER)t->p[1];
	if(pl->ep >= lv_e[pl->lv]){
		printf("LEVEL UP！ ");
		input_enter();
		pl->lv += 1;
		if((pl->hpmax += 15) > 999) pl->hpmax = 999;
		if((pl->mpmax += 8) > 999) pl->mpmax = 999;
		if((pl->p += 1) > 99) pl->p = 99;
		if((pl->d += 1) > 99) pl->d = 99;
	}
}

/* p[0] = map, p[1] = &player, p[2] = bmap */
task_func pl_learn_up(LPTCB t, int id)
{
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	id = id - id/10000*10000;
	switch(id){
	case BOSS01:
	case BOSS02:
	case BOSS03:
	case BOSS04:
	case BOSS05:
	case BOSS06:
	case BOSS07:
		pl->learn++;
	}
}
