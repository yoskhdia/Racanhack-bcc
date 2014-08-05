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

static TCB monsterwork[MONSTER_MAX];
static SMONSTER mon[MONSTER_MAX];
static LPTCB monstop;
static LPTCB monstail;
static int monscnt;

struct _list *ma_list;

/********************************************************
                        INIT
*********************************************************/
void init_monster(void)
{
	int i;
	
	for(i = 0; i < MONSTER_MAX; i++){
		monsterwork[i].state = TASK_OFF; /* monster task off */
		monsterwork[i].func = NULL;     /* 自身の行動設定関数 */
		monsterwork[i].prev = NULL;     /* 前tcb初期化 */
		monsterwork[i].next = NULL;     /* 次tcb初期化 */
		monsterwork[i].des  = NULL;      /* デストラクタ初期化 */
		monsterwork[i].p[0] = &mon[i];
		mon[i].x = 0;
		mon[i].y = 0;
	}
	
	monstop = &monsterwork[MONSTER_MAX-2]; /* ghost */
	monstail = &monsterwork[MONSTER_MAX-1]; /* ghost */
	
	monstop->state = TASK_SYS;
	monstop->prio = PRIO_TOP;
	monstop->prev = NULL;
	monstop->next = monstail;
	
	monstail->state = TASK_SYS;
	monstail->prio = PRIO_BOTTOM;
	monstail->prev = monstop;
	monstail->next = NULL;
}

/********************************************************
                    MONSTER TASK

                       CREATE
*********************************************************/
LPTCB create_monster(void *func, void *des, int prio)
{
	int i, j;
	LPTCB t, tp;
	
	tp = monstop;
	
	for(i = 0; i < MONSTER_MAX; i++){
		t = &monsterwork[i];
		if(t->state == TASK_OFF){
			t->state = TASK_ON;
			t->func = func;
			t->des = des;
			t->prio = prio;
			for(j = 1; j < TASK_POINTER; j++){
				t->p[j] = NULL;
			}
			
			while(tp != NULL){
				if(tp->prio > prio){
					t->prev = tp->prev;
					t->next = tp;
					tp->prev->next = t;
					tp->prev = t;
					break;
				}
				tp = tp->next;
			}
			return t;
		}
	}

	return NULL;
}

void* create_boss_monster(int id)
{
	id = id - id/10000*10000;
	switch(id){
	case BOSS01:
		return monster_move2;
	case BOSS02:
	case BOSS03:
	case BOSS04:
	case BOSS05:
	case BOSS06:
	case BOSS07:
		return monster_move1;
	default:
		printf("error: cannot find boss_move function.\n");
		return NULL;
	}
}

void* get_des(int id)
{
	id = id - id/10000*10000;
	switch(id){
	case BOSS01:
	case BOSS02:
	case BOSS03:
	case BOSS04:
	case BOSS05:
	case BOSS06:
	case BOSS07:
		return NULL;
	default:
		return NULL;
	}
}

int read_monster_area(int fl, int *moarray)
{
	int i, c, idcnt, mocnt;
	char buf[32], fltmp[32], idtmp[8];
	FILE *fp;
	
	if((fp = fopen("../data/moarea.dat", "r")) == NULL){
		perror("error: cannnot open moarea.dat file.\n");
		return -1;
	}
	
	for(i = 0; i < ENEMY_MAX; i++){
		moarray[i] = 0;
	}
	mocnt = 0;
	
	fltmp[0] = 'F';
	fltmp[1] = '\0';
	change_i_to_ch32(fl, buf);
	strcat(fltmp, buf);
	strcat(fltmp, ":\n");
//	printf("fltmp:%s\n", fltmp);
	
	while((fgets(buf, sizeof(buf), fp)) != NULL){
		if(buf[0] != '\n'){
			if(strcmp(fltmp, buf) == 0){
				/*********
				boss: 99xxxx, lvup enemy: LVxxxx, enemy: 00xxxx
				先頭2文字で判別
				*********/
				idcnt = 0;
				while((c = fgetc(fp)) != EOF){
					if(c == '\n') continue;
//					printf("%d, %c\n", c, c);
					idtmp[idcnt++] = (char)c;
					if(c == ','){
						idcnt = 0;
						idtmp[6] = '\0';
//						printf("[%d]idtmp:%s\n", mocnt, idtmp);
						moarray[mocnt++] = atoi(idtmp);
					} else if(c == ':'){
						if(idcnt > 1){
							idtmp[6] = '\0';
//							printf("[%d]idtmp:%s\n", mocnt, idtmp);
							moarray[mocnt++] = atoi(idtmp);
						}
						idcnt = 0;
					} else if(c == ';'){
						if(idcnt > 1){
							idtmp[6] = '\0';
//							printf("[%d]idtmp:%s\n", mocnt, idtmp);
							moarray[mocnt++] = atoi(idtmp);
						}
						break;
					}
				}
				break;
			}
		}
	}
	
	fclose(fp);
	if(mocnt > 0) return 0;
	else return -1;
}

int read_monster(int id, LPSMONSTER monster)
{
	int i, m, n, flg;
	char buf[256], idtmp[5], kidtmp[5], mon[7][31];
	FILE *fp;
	
	if((fp = fopen("../data/monster.csv", "r")) == NULL){
		perror("error: cannnot open monster.dat file.\n");
		return -1;
	}
	
	change_i_to_ch32(id - id/10000*10000, kidtmp);
	memset(idtmp, '\0', sizeof(idtmp));
	strcat(idtmp, kidtmp);
	while(strlen(idtmp) < 4){
		for(i = 4; i > 0; i--){
			idtmp[i] = idtmp[i-1];
		}
		idtmp[0] = ZERO;
	}
//	printf("id:%s\n", idtmp);
	
	flg = OFF;
	m = n = 0;
	while((fgets(buf, sizeof(buf), fp)) != NULL){
		if(buf[0] != '\n'){
			if(strncmp(idtmp, buf, 4) == 0){
				for(i = 5; buf[i] != '\n'; i++){
					if(buf[i] == '"') continue;
					if(buf[i] != ','){
						mon[m][n] = buf[i];
						n++;
					} else {
						mon[m][n] = '\0';
						m++;
						n = 0;
					}
				}
				flg = ON;
				break;
			}
		}
	}
	
	if(flg == ON){
		monster->id = id;
		strcpy(monster->name, mon[0]);
		monster->hp = monster->hpmax = atoi(mon[1]);
		monster->mp = monster->mpmax = atoi(mon[2]);
		monster->p  = atoi(mon[3]);
		monster->d  = atoi(mon[4]);
		monster->ep = atoi(mon[5]);
		monster->direction = get_random_range(0, 8);
		
		if(id/10000 > 0 && id/10000 < 99){
			mo_level_up(monster, id/10000);
		}
		
		fclose(fp);
		return atoi(mon[6]);
	} else {
		perror("error: cannot find monster data.\n");
		fclose(fp);
		return -1;
	}
}

/********************************************************
                          KILL
*********************************************************/
/* タスク削除 */
void kill_monster(LPTCB t)
{
	void (*func)(TCB *);
	LPSMONSTER mo;
	
	mo = (LPSMONSTER)t->p[0];
	mo->x = 0;
	mo->y = 0;
	if(t->des != NULL){
		func = (void (*)(LPTCB))t->des;
		func(t);
		t->des = NULL;
	}
	t->state = TASK_KILL;
}

void kill_all_monster(void)
{
	LPTCB t = monstop->next;
	
	while(t != NULL){
		if(t->state == TASK_ON || t->state == TASK_SLEEP) kill_monster(t);
		t = t->next;
		if(t == monstail) break;
	}
}

/********************************************************
                         LOOP
*********************************************************/
void loop_monster(int map[][YMAX])
{
	LPTCB t;
	
	t = monstop;
	while(t != NULL){
		if(t->state == TASK_KILL){
			monscnt--;
			t->state = TASK_OFF;
			t->prev->next = t->next;
			t->next->prev = t->prev;
		}
		t = t->next;
	}
	
	t = monstop;
	while(t != NULL){
		if(t->state == TASK_ON){
			if(t->func != NULL){
				LPSMONSTER mo;
				void (*func)(LPTCB);
				func = (void (*)(LPTCB))t->func;
				mo = (LPSMONSTER)t->p[0];
				map[mo->x][mo->y] = NONE;
				func(t);
				if(mo->id > 990000) map[mo->x][mo->y] = MO_BOSS;
				else map[mo->x][mo->y] = MONSTER;
			}
		}
		t = t->next;
	}
}

/********************************************************
               OTHER MONSTER TASK FUNC
*********************************************************/
void chose_monscnt(void)
{
	monscnt = get_random_range(MONSTER_MIN, MONSTER_MAX - 1);
}

int get_monscnt(void)
{
	return monscnt;
}

LPSMONSTER get_mo_p(void)
{
	return mon;
}

/********************************************************
                           ROAD
*********************************************************/
void make_monster_road(int bmap[][YMAX][2], struct _list* room_list)
{
	int i, j, midx, midy, pmx, pmy;
	struct _list *li;
	LPROOM room;
	LPMROAD ma;
	
	ma_list = NULL;
	
	for(li = room_list; li != NULL; li = li->next){
		room = (LPROOM)li->data;
		
		/* 中点 */
		midx = (room->lx + room->hx) / 2;
		midy = (room->ly + room->hy) / 2;
		
		for(i = room->lx; i <= room->hx; i++){
			if(i == room->lx || i == room->hx){
				for(j = room->ly; j <= room->hy; j++){
					if(bmap[i][j][0] == MARKER){
						if(ma_add(i, j, midx, midy) == NULL){
							list_free(room_list);
							list_free(ma_list);
							exit(-1);
						}
					}
				}
			} else {
				if(bmap[i][room->ly][0] == MARKER){
					if(ma_add(i, room->ly, midx, midy) == NULL){
						list_free(room_list);
						list_free(ma_list);
						exit(-1);
					}
				} else if(bmap[i][room->hy][0] == MARKER){
					if(ma_add(i, room->hy, midx, midy) == NULL){
						list_free(room_list);
						list_free(ma_list);
						exit(-1);
					}
				}
			}
		}
	}
	
	for(li = ma_list; li != NULL; li = li->next){
		ma = (LPMROAD)li->data;
		pmx = ab_cmp(ma->x, ma->midx);
		pmy = ab_cmp(ma->y, ma->midy) * -1;
/*		pmx = 0; pmy = 0;
		if(ma->x < ma->midx) pmx = -1; else pmx = 1;
		if(ma->y < ma->midy) pmy = 1; else pmy = -1;
*/
		
		if(pmx != 0){
			for(i = ma->midx; i != ma->x; i += pmx){
				bmap[i][ma->midy][0] = M_ROAD;
			}
		}
		if(pmy != 0){
			for(i = ma->y; i != ma->midy; i += pmy){
				bmap[ma->x][i][0] = M_ROAD;
			}
		}
		bmap[ma->x][ma->midy][0] = M_ROAD;
	}
	
	list_free(ma_list);
}

LPMROAD ma_add(int x, int y, int midx, int midy)
{
	LPMROAD ma;
	ma = (LPMROAD)malloc(sizeof(MROAD));
	if(ma == NULL){
		perror("error: failed memory allocation.\n");
		return NULL;
	}
	
	ma->x = x;
	ma->y = y;
	ma->midx = midx;
	ma->midy = midy;
	ma_list = list_append(ma_list, ma);
	if(ma_list == NULL){
		perror("error: failed memory allocation - ma_list.\n");
		free(ma);
		exit(-1);
	}
	
	return ma;
}

/********************************************************
                     TASK FUNCTION
*********************************************************/
/* p[0] = map */
task_func monster_turn(LPTCB t)
{
	loop_monster(t->p[0]);
}

/********************************************************
                      OTHER TASK
*********************************************************/
void mo_level_up(LPSMONSTER mo, int lv)
{
	double lv_e[5] = {0.5, 0.3, 0.4, 0.4, 0.7};
	char chLv[3];
	
	change_i_to_ch32(lv, chLv);
	strcat(mo->name, "(+");
	strcat(mo->name, chLv);
	strcat(mo->name, ")");
	
	mo->hp = mo->hpmax += mo->hpmax*((double)lv*lv_e[0]);
	mo->mp = mo->mpmax += mo->mpmax*((double)lv*lv_e[1]);
	mo->p  += mo->p*((double)lv*lv_e[2]);
	mo->d  += mo->d*((double)lv*lv_e[3]);
	mo->ep += mo->ep*((double)lv*lv_e[4]);
}

/* p[0] = map, p[1] = &player, p[2] = bmap */
task_func check_monster_alive(LPTCB t)
{
	LPTCB tmo;
	LPSPLAYER pl;
	LPSMONSTER mo;
	
	tmo = monstop;
	pl = (LPSPLAYER)t->p[1];
	
	while(tmo != NULL){
		mo = (LPSMONSTER)tmo->p[0];
		if(tmo->state == TASK_ON && mo->hp < 1){
			printf("%sを倒した.%4dの経験値を獲得 ", mo->name, mo->ep);
			input_enter();
			if((pl->ep += mo->ep) > 9999999) pl->ep = 9999999;
			if(mo->id >= 990000) pl_learn_up(t, mo->id);
			move_map_obj(t->p[0], 0, 0, mo->x, mo->y, NONE);
			set_bmap_smell(t->p[2], mo->x, mo->y, SMELL_02);
			if(mo->id == 990000+BOSS07) game_clear_end(t);
			kill_monster(tmo);
		}
		tmo = tmo->next;
		if(tmo == monstail) break;
	}
	
	if((pl->pturn % 8) == 0){
		pl->hp = pl->hp + pl->hpmax * 0.15;
		if(pl->hp > pl->hpmax) pl->hp = pl->hpmax;
	}
	if((pl->pturn % 12) == 0){
		pl->mp = pl->mp + pl->mpmax * 0.15;
		if(pl->mp > pl->mpmax) pl->mp = pl->mpmax;
	}
	
	write_map(t);
	
	switch(pl->fturn){
	case 500:
		printf("どこからか風が吹いてきた...\n");
		input_enter();
		break;
	case 750:
		printf("風が強くなってきた...\n");
		input_enter();
		break;
	case 900:
		printf("駄目だ！今にも飛ばされそうだ...\n");
		input_enter();
		break;
	case 1000:
		printf("%4d DAMAGE!!\n", pl->hpmax);
		input_enter();
		pl->hp = 0;
		game_over_end(t);
		break;
	default:
		break;
	}
	
	pl->pturn += 1;
	pl->fturn += 1;
}

/* p[0] = map, p[1] = &player, p[2] = bmap */
task_func more_monster(LPTCB t)
{
	int i, j, x, y, id, chk, *moarray, bosscnt[2];
	LPTCB mt, mowork;
	LPSPLAYER pl;
	LPSMONSTER mo;
	
	pl = (LPSPLAYER)t->p[1];
	mowork = monstop;
	mo = get_mo_p();
	if(((pl->fturn % 8) == 0) && (monscnt < MONSTER_MAX - 2)){
		for(i = 0; i < MONSTER_MAX; i++){
			mowork = &monsterwork[i];
			if(mowork->state == TASK_OFF){
				moarray = get_moarray_p();
				bosscnt[0] = bosscnt[1] = 0;
				for(j = 0; j < ENEMY_MAX; j++){
					if(moarray[j] > 990000) bosscnt[0]++;
					else if(moarray[j] == 0) break;
					bosscnt[1]++;
				}
				
				id = bosscnt[0] + get_random_range(0, bosscnt[1] - bosscnt[0]);
				if((chk = read_monster(moarray[id], mo+i)) == -1){
					break;
				}
				mt = create_monster(monster_move1, NULL, chk);
				mt->p[1] = t->p[1];
				mt->p[2] = t->p[2];
				
				for(x = get_random_range(1, XMAX), y = get_random_range(1, YMAX);
					xy_map(t->p[0], 0, 0, x, y) != NONE;
					x = get_random_range(1, XMAX), y = get_random_range(1, YMAX));
				(mo+i)->x = x;
				(mo+i)->y = y;
				move_map_obj(t->p[0], 0, 0, x, y, MONSTER);
				set_bmap_smell(t->p[2], x, y, SMELL_MO);
				break;
			}
		}
	}
}

