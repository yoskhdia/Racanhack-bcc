#ifndef _PLAYER_H_
#define _PLAYER_H_

enum{
	PLAYER_SITE = 5,
	G_MAP = PLAYER_SITE * 2 + 1,
};

void init_player(LPSPLAYER );

task_func player_turn(LPTCB );

task_func player_menu(LPTCB , int *);
int pm_cola(LPTCB );
void pm_escape(LPTCB );
void pm_call_map(LPTCB );
void pm_next_s(LPTCB );
void pm_cracker(LPTCB );
int pm_sun_yellow_h(LPTCB );
void pm_blue_t_h(LPTCB );
int pm_eija(LPTCB );
void pm_h_purple(LPTCB );

task_func player_attacked(LPTCB , int , double );
LPSMONSTER check_attack(int , int , int );

task_func player_move(LPTCB , int *);
task_func down_stairs(LPTCB );

task_func set_player_smell(LPTCB );
task_func pl_level_up(LPTCB );
task_func pl_learn_up(LPTCB , int );

#endif
