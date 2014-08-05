#ifndef _SET_OBJECT_H_
#define _SET_OBJECT_H_

#define BUF_LEN 7

int input_key(void);
int input_num_key(void);
int input_char_key(void);
int input_yn_key(void);
void input_enter(void);

void change_key_to_xy(int *, int *, int key);
void change_i_to_ch32(int , char *);

void map_cpy(int map[][YMAX], int bmap[][YMAX][2]);
void map_cpy2(int map[][YMAX][2], int bmap[][YMAX][2]);

void set_object(int *, int *, struct _list *, int map[][YMAX], int );
void set_bmap_smell(int bmap[][YMAX][2], int , int , int );
void set_bmap_smell_down(int bmap[][YMAX][2], int , int , int );
void set_nine_floor(int bmap[][YMAX][2], int , int , int nine[][3][2]);

int ab_cmp(int , int );
int xy_map(int map[][YMAX],int ,int ,int ,int );
int xy_map2(int bmap[][YMAX][2],int ,int ,int ,int );
int chk_oblique(int map[][YMAX],int , int , int , int );

void move_map_obj(int map[][YMAX],int ,int ,int ,int ,int );
int swap_map_obj(int map[][YMAX], int , int , int );

int get_random(int );
int get_random_range(int ,int );
int get_flcnt_id(int );
int get_direction(int , int );

#endif
