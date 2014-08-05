#include<stdio.h>
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

static TCB taskwork[TASK_MAX];
static LPTCB tasktop;
static LPTCB tasktail;
static int taskcnt;
static int taskflg;

/* タスク初期化 */
void init_task(void)
{
	int i;
	
	taskcnt = 0;
	taskflg = ON;
	
	for(i = 0; i < TASK_MAX; i++){
		taskwork[i].state = TASK_OFF; /* task off */
		taskwork[i].func = NULL;     /* 実行関数初期化 */
		taskwork[i].prev = NULL;     /* 前tcb初期化 */
		taskwork[i].next = NULL;     /* 次tcb初期化 */
		taskwork[i].des = NULL;      /* デストラクタ初期化 */
	}
	
	tasktop = &taskwork[0];
	tasktail = &taskwork[1];
	
	tasktop->state = TASK_SYS;
	tasktop->prio = PRIO_TOP;
	tasktop->prev = NULL;
	tasktop->next = tasktail;
	taskcnt++;
	
	tasktail->state = TASK_SYS;
	tasktail->prio = PRIO_BOTTOM;
	tasktail->prev = tasktop;
	tasktail->next = NULL;
	taskcnt++;
}

/* タスク登録 */
LPTCB create_task(void *func, void *des, int prio)
{
	int i, j;
	LPTCB t, top;
	
	top = tasktop;
	
	for(i = 0; i < TASK_MAX; i++){
		t = &taskwork[i];
		if(t->state == TASK_OFF){
			t->state = TASK_ON;
			t->func = func;
			t->des = des;
			t->prio = prio;
			for(j = 0; j < TASK_POINTER; j++){
				t->p[j] = NULL;
			}
			
			while(top != NULL){
				if(top->prio > prio){
					t->prev = top->prev;
					t->next = top;
					top->prev->next = t;
					top->prev = t;
					break;
				}
				top = top->next;
			}
			taskcnt++;
			return t;
		}
	}
	
	return NULL;
}

/* タスク削除 */
void kill_task(LPTCB t)
{
	void (*func)(TCB *);
	
	if(t->des != NULL){
		func = (void (*)(LPTCB))t->des;
		func(t);
		t->des = NULL;
	}
	t->state = TASK_KILL;
}

void kill_all_task(void)
{
	LPTCB t = tasktop->next;
	
	while(t != NULL){
		if(t->state == TASK_ON || t->state == TASK_SLEEP) kill_task(t);
		t = t->next;
		if(t == tasktail) break;
	}
}

void kill_prio_task(int prio)
{
	LPTCB t = tasktop;
	
	while(t != NULL){
		if(t->state == TASK_ON){
			if(t->prio == prio) kill_task(t);
		}
		t = t->next;
	}
}

/* タスク実行 */
void loop_task(void)
{
	LPTCB t;
	while(taskflg != OFF){
		t = tasktop;
		
		while(t != NULL){
			if(t->state == TASK_ON){
				if(t->func != NULL){
					void (*func)(LPTCB);
					func = (void (*)(LPTCB))t->func;
					func(t);
				}
			}
			t = t->next;
		}
		
		t = tasktop;
		while(t != NULL){
			if(t->state == TASK_KILL){
				taskcnt--;
				t->state = TASK_OFF;
				t->prev->next = t->next;
				t->next->prev = t->prev;
			}
			t = t->next;
		}
	}
}

/* タスク数カウント */
int count_task(void)
{
	return taskcnt;
}

int *get_taskcnt_p(void)
{
	return &taskcnt;
}

/* トップ・テイルタスク取得 */
LPTCB get_top_task(void)
{
	return tasktop;
}

LPTCB get_tail_task(void)
{
	return tasktail;
}

/* タスクループ終了(フラグ切替) */
void switch_taskflg(void)
{
	if(taskflg == ON) taskflg = OFF;
	else taskflg = ON;
}

/* 寝ているタスクを起こす */
void wakeup_task(LPTCB t)
{
	if(t->state == TASK_SLEEP) t->state = TASK_ON;
}

void wakeup_all_task(void)
{
	LPTCB t = tasktop->next;
	
	while(t != NULL){
		if(t->state == TASK_SLEEP) wakeup_task(t);
		t = t->next;
		if(t == tasktail) break;
	}
}

void wakeup_prio_task(int prio)
{
	LPTCB t = tasktop;
	
	while(t != NULL){
		if(t->state == TASK_SLEEP){
			if(t->prio == prio) wakeup_task(t);
		}
		t = t->next;
		if(t == tasktail) break;
	}
}
