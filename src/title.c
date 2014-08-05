#include<stdio.h>
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

/* p[0] = &player, p[1] = &floor_cnt */
task_func title_load(LPTCB t)
{
	int key;
	char buf[85];
	FILE *fp;
	
	if((fp = fopen("../data/title01.dat", "r")) == NULL){
		perror("error: cannot open title.\n");
		exit(-1);
	}
	
	while((fgets(buf, sizeof(buf)-1, fp)) != NULL){
		printf("%s", buf);
	}
	fclose(fp);
	
	init_player(t->p[0]);
	
	printf("\nchoose ->");
	key = input_char_key();
	if(key == KEY_L || key == KEY_l) data_load(t);
	
	kill_task(t);
}

/********************************************************
                          SAVE
*********************************************************/
/* p[0] = map, p[1] = player */
task_func data_save(LPTCB t)
{
	int *f;
	FILE *fp;
	LPSPLAYER pl;
	
	if((fp = fopen("../data/save.dat", "wb")) == NULL){
		printf("error: cannnot open save file.\n");
		return;
	}
	
	pl = (LPSPLAYER)t->p[1];
	f = get_flcnt_p();
	
	fwrite(f, sizeof(int), 1, fp);
	fwrite(&pl->pturn, sizeof(int), 1, fp);
	fwrite(&pl->lv, sizeof(int), 1, fp);
	fwrite(&pl->ep, sizeof(int), 1, fp);
	fwrite(&pl->hp, sizeof(int), 1, fp);
	fwrite(&pl->hpmax, sizeof(int), 1, fp);
	fwrite(&pl->mp, sizeof(int), 1, fp);
	fwrite(&pl->mpmax, sizeof(int), 1, fp);
	fwrite(&pl->learn, sizeof(int), 1, fp);
	fwrite(&pl->p, sizeof(int), 1, fp);
	fwrite(&pl->d, sizeof(int), 1, fp);
	fwrite(&pl->equip.weapon, sizeof(int), 1, fp);
	fwrite(&pl->equip.body, sizeof(int), 1, fp);
	
	fclose(fp);
	
	printf("done save.\nEXIT GAME.\n");
	game_end();
}

/* p[1] = &player, ‘¼•s’è */
task_func fuck_save(LPTCB t)
{
	int *f;
	FILE *fp;
	LPSPLAYER pl;
	
	if((fp = fopen("../data/save.dat", "wb")) == NULL){
		printf("error: cannnot open save file.\n");
		return;
	}
	
	pl = (LPSPLAYER)t->p[1];
	init_player(pl);
	f = get_flcnt_p();
	*f = 1;
	
	fwrite(f, sizeof(int), 1, fp);
	fwrite(&pl->pturn, sizeof(int), 1, fp);
	fwrite(&pl->lv, sizeof(int), 1, fp);
	fwrite(&pl->ep, sizeof(int), 1, fp);
	fwrite(&pl->hp, sizeof(int), 1, fp);
	fwrite(&pl->hpmax, sizeof(int), 1, fp);
	fwrite(&pl->mp, sizeof(int), 1, fp);
	fwrite(&pl->mpmax, sizeof(int), 1, fp);
	fwrite(&pl->learn, sizeof(int), 1, fp);
	fwrite(&pl->p, sizeof(int), 1, fp);
	fwrite(&pl->d, sizeof(int), 1, fp);
	fwrite(&pl->equip.weapon, sizeof(int), 1, fp);
	fwrite(&pl->equip.body, sizeof(int), 1, fp);
	
	fclose(fp);
}

/* p[1] = &player, ‘¼•s’è */
task_func save_clear(LPTCB t)
{
	int *f;
	FILE *fp;
	LPSPLAYER pl;
	
	if((fp = fopen("../data/save.dat", "wb")) == NULL){
		printf("error: cannnot open save file.\n");
		return;
	}
	
	pl = (LPSPLAYER)t->p[1];
	init_player(pl);
	f = get_flcnt_p();
	*f = 1;
	pl->hp = pl->hpmax = 200;
	pl->mp = pl->mpmax = 200;
	pl->learn = 9;
	
	fwrite(f, sizeof(int), 1, fp);
	fwrite(&pl->pturn, sizeof(int), 1, fp);
	fwrite(&pl->lv, sizeof(int), 1, fp);
	fwrite(&pl->ep, sizeof(int), 1, fp);
	fwrite(&pl->hp, sizeof(int), 1, fp);
	fwrite(&pl->hpmax, sizeof(int), 1, fp);
	fwrite(&pl->mp, sizeof(int), 1, fp);
	fwrite(&pl->mpmax, sizeof(int), 1, fp);
	fwrite(&pl->learn, sizeof(int), 1, fp);
	fwrite(&pl->p, sizeof(int), 1, fp);
	fwrite(&pl->d, sizeof(int), 1, fp);
	fwrite(&pl->equip.weapon, sizeof(int), 1, fp);
	fwrite(&pl->equip.body, sizeof(int), 1, fp);
	
	fclose(fp);
}

/********************************************************
                         LOAD
*********************************************************/
/* p[0] = &player, p[1] = &floor_cnt */
task_func data_load(LPTCB t)
{
	FILE *fp;
	LPSPLAYER pl;
	
	if((fp = fopen("../data/save.dat", "rb")) == NULL){
		printf("error: cannnot open save file.\n");
		printf("continue?(y/n) ->");
		if(input_yn_key() == KEY_Y) data_load(t);
		return;
	}
	
	pl = (LPSPLAYER)t->p[0];
	
	fread(t->p[1], sizeof(int), 1, fp);
	fread(&pl->pturn, sizeof(int), 1, fp);
	fread(&pl->lv, sizeof(int), 1, fp);
	fread(&pl->ep, sizeof(int), 1, fp);
	fread(&pl->hp, sizeof(int), 1, fp);
	fread(&pl->hpmax, sizeof(int), 1, fp);
	fread(&pl->mp, sizeof(int), 1, fp);
	fread(&pl->mpmax, sizeof(int), 1, fp);
	fread(&pl->learn, sizeof(int), 1, fp);
	fread(&pl->p, sizeof(int), 1, fp);
	fread(&pl->d, sizeof(int), 1, fp);
	pl->s = OFF;
	fread(&pl->equip.weapon, sizeof(int), 1, fp);
	fread(&pl->equip.body, sizeof(int), 1, fp);
	
	fclose(fp);
}
