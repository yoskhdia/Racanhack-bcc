#ifndef _MONSTER_MOVE_
#define _MONSTER_MOVE_

task_func monster_move1(LPTCB );
task_func monster_move2(LPTCB );

int check_room(LPTCB );
void search_pl(LPTCB , int *, int , int , int , int , int );
void R_hand_walk(int nine[][3][2], int *, int );
void walk_to_player(LPTCB , int nine[3][3][2], int *, int *);
void walk_by_direction(int nine[][3][2], int , int , int *, int *, int *, int);
int direct(int dmap[], int nine[][3][2], int , int *, int *);
int chk_boss_site(LPTCB ,int );

task_func monster_attacked(LPTCB , double);

#endif
