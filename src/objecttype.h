#ifndef _OBJECT_TYPE_H_
#define _OBJECT_TYPE_H_

enum{ /* map object type */
	NONE,
	BLOCK,
	STAIRS,
	PLAYER,
	MONSTER,
	MO_BOSS,
	M_ROAD,
	
	MUSH_01,
	MUSH_02,
	
	TRAP_01,
	TRAP_02,
	
	MARKER = 99,
};

enum{ /* bmap[1] main person smell */
	SMELL_BL, /* block */
	SMELL_ST, /* stairs */
	SMELL_MO, /* monster */
	SMELL_BOSS, /* monster st */
	
	SMELL_00, /* player */
	SMELL_01,
	SMELL_02,
	SMELL_03,
	SMELL_PL,
};

/* ID+POWER(2Œ…) */
enum{ /* equip weapon */
	W_NONE = 0,
	W_KNIFE = 103,
	W_SORD = 206,
	W_HAMMER = 310,
	W_HAT = 415,
};

enum{ /* equip body */
	B_NONE = 0,
	B_ARMER = 104,
	B_BANDANA = 207,
};

typedef struct _equip{
	int weapon;
	int body;
} EQUIP, *LPEQUIP;

typedef struct _splayer{
	int x; /* xˆÊ’u */
	int y; /* yˆÊ’u */
	int pturn; /* player turn */
	int fturn; /* floor turn */
	
	int lv; /* level */
	int ep; /* experience point */
	int hp; /* hit point */
	int hpmax; /* hp max */
	int mp; /* magic point */
	int mpmax; /* mp max */
	int learn; /* learning magic */
	int p; /* power */
	int d; /* diffence */
	int s; /* for func pm_next_s() */
	EQUIP equip; /* ‘•”õ */
} SPLAYER, *LPSPLAYER;

typedef struct _smonster{
	int x;
	int y;
	
	int id;
	char name[32];
	int hp;
	int hpmax;
	int mp;
	int mpmax;
	int p;
	int d;
	int ep;
	int direction; /* •ûŠp */
} SMONSTER, *LPSMONSTER;

#endif
