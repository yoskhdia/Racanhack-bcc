#ifndef _TASK_H_
#define _TASK_H_

#define TASK_MAX 256
#define TASK_WSIZE 256
#define TASK_POINTER 3

/* �D�揇�� */
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
	int prio; /* �D�揇�� */
	
	void *func; /* �����֐��ւ̃|�C���^ */
	void *p[TASK_POINTER]; /* �^�X�N���[�N */
	struct _tcb *prev; /* �O��TCB */
	struct _tcb *next; /* ����TCB */
	void *des; /* �f�X�g���N�^ */
} TCB, *LPTCB;

/* �^�X�N������ */
void init_task(void);

/* �^�X�N�o�^ */
LPTCB create_task(void *func, void *des, int prio);

/* �^�X�N�폜 */
void kill_task(LPTCB );
void kill_all_task(void);
void kill_prio_task(int prio);

/* �^�X�N���s */
void loop_task(void);

/* �^�X�N���J�E���g */
int count_task(void);
int *get_taskcnt_p(void);

/* �g�b�v�E�e�C���^�X�N�擾 */
LPTCB get_top_task(void);
LPTCB get_tail_task(void);

/* �^�X�N���[�v�I��(�t���O�ؑ�) */
void switch_taskflg(void);

/* �Q�Ă���^�X�N���N���� */
void wakeup_task(LPTCB );
void wakeup_all_task(void);
void wakeup_prio_task(int );

#endif
