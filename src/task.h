#ifndef _TASK_H_
#define _TASK_H_

#define TASK_MAX 256
#define TASK_WSIZE 256
#define TASK_POINTER 3

/* 優先順位 */
enum{
	PRIO_TOP,
	PRIO_00,
	PRIO_01,
	PRIO_02,
	PRIO_03,
	PRIO_04,
	PRIO_05,
	PRIO_06,
	PRIO_07,
	PRIO_08,
	PRIO_09,
	SYSTEM_PRIO, /* system priority */
	PRIO_LAST,
	PRIO_BOTTOM,
};

enum{
	TASK_SYS,
	TASK_ON,
	TASK_OFF,
	TASK_KILL,
	TASK_SLEEP,
};

typedef void task_func;

typedef struct _tcb{
	int state; /* task statement */
	int type; /* task type */
	int prio; /* 優先順位 */
	
	void *func; /* 処理関数へのポインタ */
	void *p[TASK_POINTER]; /* タスクワーク */
	struct _tcb *prev; /* 前のTCB */
	struct _tcb *next; /* 次のTCB */
	void *des; /* デストラクタ */
} TCB, *LPTCB;

/* タスク初期化 */
void init_task(void);

/* タスク登録 */
LPTCB create_task(void *func, void *des, int prio);

/* タスク削除 */
void kill_task(LPTCB );
void kill_all_task(void);
void kill_prio_task(int prio);

/* タスク実行 */
void loop_task(void);

/* タスク数カウント */
int count_task(void);
int *get_taskcnt_p(void);

/* トップ・テイルタスク取得 */
LPTCB get_top_task(void);
LPTCB get_tail_task(void);

/* タスクループ終了(フラグ切替) */
void switch_taskflg(void);

/* 寝ているタスクを起こす */
void wakeup_task(LPTCB );
void wakeup_all_task(void);
void wakeup_prio_task(int );

#endif
