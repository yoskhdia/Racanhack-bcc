#ifndef _MAIN_H_
#define _MAIN_H_

#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define MAX(a, b) (((a) >= (b)) ? (a) : (b))

#define FALSE (0)
#define TRUE (!FALSE)

enum{
	XMAX = 36, /* mapãs•ûŒü */
	YMAX = 38, /* map‰¡•ûŒü */
};

enum{
	ENTER = 10,
	ZERO = 48,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
};

enum{
	KEY_A = 65,
	KEY_L = 76,
	KEY_M = 77,
	KEY_N = 78,
	KEY_S = 83,
	KEY_Y = 89,
	KEY_Z = 90,
};

enum{
	KEY_a = 97,
	KEY_l = 108,
	KEY_m = 109,
	KEY_n = 110,
	KEY_s = 115,
	KEY_y = 121,
	KEY_z = 122,
};

enum{
	ON,
	OFF,
	UNKNOWN,
};

typedef int GBOOLEAN;

task_func game_start(LPTCB );
void game_end(void);
void game_over_end(LPTCB );
void game_clear_end(LPTCB );

task_func floor_load(LPTCB );
task_func write_map(LPTCB );

int* get_flcnt_p(void);
int* get_moarray_p(void);

#endif
