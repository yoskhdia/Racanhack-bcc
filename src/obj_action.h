#ifndef _OBJECT_ACTION_H_
#define _OBJECT_ACTION_H_

enum{
	MUSH_01_MAX = 5,
	MUSH_02_MAX = 1,
	
	MUSH_ARRAY = MUSH_01_MAX + MUSH_02_MAX,
};

enum{
	S_W = 0,
	S_B,
};

void init_mush(int map[][YMAX], struct _list *);
void eat_mush(LPSPLAYER , int );
void find_mush_01(LPSPLAYER );
void find_mush_02(LPSPLAYER );
void delete_mush(int , int );
task_func grow_mush(LPTCB );

void show_map(int map[][YMAX]);
void show_map2(int map[][YMAX][2]);
void show_map3(int map[][YMAX][2]);

void show_mmap(int map[][YMAX], int );
void show_game_map(int map[][YMAX], int ,int );

void show_equip(int , int );

#endif
