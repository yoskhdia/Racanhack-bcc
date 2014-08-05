#ifndef _MONSTER_H_
#define _MONSTER_H_

#define MONSTER_SITE 5 /* モンスターからXマス */
/* #define WALK_MAP (MONSTER_SITE * 2 + 1) */

enum{
	MONSTER_MIN = 4,
	MONSTER_MAX = 12, /* 2つのシステムタスク含む */
	ENEMY_MAX = 6, /* 1floorのmonster(敵)の種類 */
};

enum{
	NORTH = 0,
	NORTH_EAST,
	EAST,
	EAST_SOUTH,
	SOUTH,
	SOUTH_WEST,
	WEST,
	WEST_NORTH,
};

/* boss ID */
enum{
	BOSS01 = 7,
	BOSS02 = 10,
	BOSS03 = 12,
	BOSS04 = 13,
	BOSS05 = 16,
	BOSS06 = 17,
	BOSS07 = 18,
};

typedef struct _mroad{
	int x;
	int y;
	int midx;
	int midy;
} MROAD, *LPMROAD;

void init_monster(void);

LPTCB create_monster(void *, void *, int );
void* create_boss_monster(int );
void* get_des(int );
int read_monster_area(int, int *);
int read_monster(int , LPSMONSTER );

void kill_monster(LPTCB );
void kill_all_monster(void);
void loop_monster(int map[][YMAX]);

void chose_monscnt(void);
int get_monscnt(void);
LPSMONSTER get_mo_p(void);

void make_monster_road(int bmap[][YMAX][2], struct _list* );
LPMROAD ma_add(int , int , int , int );

task_func monster_turn(LPTCB );

void mo_level_up(LPSMONSTER , int );
task_func check_monster_alive(LPTCB );
task_func more_monster(LPTCB );

#endif
