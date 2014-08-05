#include<stdio.h>
#include<stdlib.h>
#include<time.h>
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

static int floor_cnt, moarray[ENEMY_MAX];

/********************************************************
                         MAIN
*********************************************************/
int main(void)
{
	int map[XMAX][YMAX], bmap[XMAX][YMAX][2];
	SPLAYER player;
	LPTCB task;
	
	floor_cnt = 1;
	init_genrand((unsigned long)time(NULL)); // 必ずmainに入れること
	init_task();
	init_monster();
	
	task = create_task(title_load, NULL, PRIO_00);
	task->p[0] = &player;
	task->p[1] = &floor_cnt;
	
	task = create_task(game_start, NULL, PRIO_00);
	task->p[0] = map;
	task->p[1] = &player;
	task->p[2] = bmap;
	
	loop_task();
	
//	printf("taskcnt =%d\n", count_task());
	
	return 0;
}

/********************************************************
                     TASK FUNCTION
*********************************************************/
/* 実行時初期化: 
   p[0] = map, p[1] = &player, p[2] = bmap */
task_func game_start(LPTCB t)
{
	LPTCB task;
	
	/* 初期タスク設定 */
	task = create_task(floor_load, NULL, PRIO_00);
	task->p[0] = t->p[0];
	task->p[1] = t->p[1];
	task->p[2] = t->p[2];
	
	task = create_task(write_map, NULL, PRIO_01);
	task->p[0] = t->p[0];
	task->p[1] = t->p[1];
	
	task = create_task(player_turn, NULL, PRIO_02);
	task->p[0] = t->p[0];
	task->p[1] = t->p[1];
	
	task = create_task(set_player_smell, NULL, PRIO_02);
	task->p[0] = t->p[1];
	task->p[1] = t->p[2];
	
	task = create_task(check_monster_alive, NULL, PRIO_06);
	task->p[0] = t->p[0];
	task->p[1] = t->p[1];
	task->p[2] = t->p[2];
	
	task = create_task(pl_level_up, NULL, PRIO_06);
	task->p[1] = t->p[1];
	
	task = create_task(monster_turn, NULL, PRIO_06);
	task->p[0] = t->p[0];
	
	task = create_task(more_monster, NULL, PRIO_06);
	task->p[0] = t->p[0];
	task->p[1] = t->p[1];
	task->p[2] = t->p[2];
	
	task = create_task(grow_mush, NULL, PRIO_08);
	task->p[0] = t->p[0];
	task->p[1] = t->p[1];
	
	kill_task(t);
}

/* game終了処理
 呼び出し元タスクtの次にtailtaskがあるなら終了処理のみ */
void game_end(void)
{
	switch_taskflg();
	kill_all_monster();
	kill_all_task();
	printf("Thank you for playing.\n");
}

/* p[1] = player, 他不定 */
void game_over_end(LPTCB t)
{
	fuck_save(t);
	switch_taskflg();
	kill_all_monster();
	kill_all_task();
	printf("GAME OVER.\nto be continued...\n");
}

void game_clear_end(LPTCB t)
{
	switch_taskflg();
	kill_all_monster();
	kill_all_task();
	printf("JOJO beated Curz!!\nG A M E   C L E A R\n");
	input_enter();
	printf("SAVE clear data?(y/n)-> ");
	if(input_yn_key() == KEY_Y) save_clear(t);
	printf("\nThank you for playing.\n");
}

/********************************************************
                     FLOOR LOAD
*********************************************************/
/* p[0] = map, p[1] = &player, p[2] = bmap */
task_func floor_load(LPTCB t)
{
	int i, x, y, monscnt, id, bosscnt[2];
	struct _list *room_list;
	LPSPLAYER pl;
	LPSMONSTER mo;
	
	/* floor設定 */
	room_list = init_floor(t->p[0], t->p[2]);
	set_object(&x, &y, room_list, t->p[0], STAIRS);
	set_bmap_smell(t->p[2], x, y, SMELL_ST);
//	show_map2(t->p[2]);
	
	/* player設定 */
	pl = (LPSPLAYER)t->p[1];
	set_object(&x, &y, room_list, t->p[0], PLAYER);
	pl->x = x;
	pl->y = y;
	pl->fturn = 1;
	set_bmap_smell(t->p[2], x, y, SMELL_PL);
	
	/* monster設定 */
	chose_monscnt();
	monscnt = get_monscnt();
//	printf("monscnt: %d\n", monscnt);
	mo = get_mo_p();
	if(read_monster_area(floor_cnt, moarray) == 0){
		int chk;
		LPTCB mt;
		
		bosscnt[0] = bosscnt[1] = 0;
		for(i = 0; i < ENEMY_MAX; i++){
//			printf("%d: %d\n", i, moarray[i]);
			if(moarray[i] > 990000) bosscnt[0]++;
			else if(moarray[i] == 0) break;
			bosscnt[1]++;
		}
		
		/* ボス設定 */
		for(i = 0; i < bosscnt[0]; i++){
			if((chk = read_monster(moarray[i], mo+i)) == -1){
				list_free(room_list);
				game_end();
				return;
			}
			mt = create_monster(create_boss_monster(moarray[i]), get_des(moarray[i]), chk);
			mt->p[1] = t->p[1];
			mt->p[2] = t->p[2];
			
			set_object(&x, &y, room_list, t->p[0], MO_BOSS);
			(mo+i)->x = x;
			(mo+i)->y = y;
			set_bmap_smell(t->p[2], x, y, SMELL_BOSS);
		}
		
		/* 標準/Level monster設定 */
		for(i = bosscnt[0]; i < monscnt; i++){
			id = bosscnt[0] + get_random_range(0, bosscnt[1] - bosscnt[0]);
			if((chk = read_monster(moarray[id], mo+i)) == -1){
				list_free(room_list);
				game_end();
				return;
			}
			mt = create_monster(monster_move1, NULL, chk);
			mt->p[1] = t->p[1];
			mt->p[2] = t->p[2];
/*			printf("id: %d, name: %s, hp: %d, mp: %d, p: %d, d: %d\n",
				id, (mo+i)->name,(mo+i)->hp,(mo+i)->mp,(mo+i)->p,(mo+i)->d);
*/
			set_object(&x, &y, room_list, t->p[0], MONSTER);
			(mo+i)->x = x;
			(mo+i)->y = y;
			set_bmap_smell(t->p[2], x, y, SMELL_MO);
		}
	}
	
	/* キノコ設定 */
	init_mush(t->p[0], room_list);
	
	list_free(room_list);
	t->state = TASK_SLEEP;
}

/********************************************************
                      WRITE MAP
*********************************************************/
/* p[0] = map, p[1] = &player */
task_func write_map(LPTCB t)
{
	int n, m;
	LPSPLAYER pl;
	
	pl = (LPSPLAYER)t->p[1];
	
	system("cls");
	printf("%03dF", floor_cnt);
	printf("   (%2d, %2d)", pl->x, pl->y);
	printf(" -%4dTURN(%4dTURN)\n", pl->fturn, pl->pturn);
	printf("LEVEL:%2d HP:%3d/%3d MP:%3d/%3d ", pl->lv, pl->hp, pl->hpmax, pl->mp, pl->mpmax);
	printf("P:%2d D:%2d EXP:%d\n", pl->p, pl->d, pl->ep);
	n = pl->equip.weapon/100;
	m = pl->equip.body/100;
	printf("武器:[");
	show_equip(S_W, n);
	printf("](%d) 防具:[", pl->equip.weapon - 100*n);
	show_equip(S_B, m);
	printf("](%d)\n", pl->equip.body - 100*m);
	show_game_map(t->p[0], pl->x, pl->y);
}

/********************************************************
                   OTHER FUNCTION
*********************************************************/
int* get_flcnt_p(void)
{
	return &floor_cnt;
}

int* get_moarray_p(void)
{
	return moarray;
}

