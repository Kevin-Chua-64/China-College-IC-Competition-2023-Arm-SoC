#ifndef __BASIC_RUN_H
#define __BASIC_RUN_H	

#define left_limit  2
#define right_limit 238
#define up_limit    37
#define down_limit  318

//检测操作来更新
void switch_my_act(int my_plane[2],int my_buttles[50][2]);

//更新画面
void update_my_buttles(int my_buttles[20][2]);
void update_enemy_buttles(int enemy_buttles[50][2], int level);
void update_enemy_planes(int enemy_planes[5][2], int level);

//生成敌机和子弹
void generate_enemy_planes(int level, int enemy_planes[5][2]);
void generate_enemy_buttles(int level, int enemy_plane[5][2],  int enemy_buttles[50][2]);
void generate_boss_buttles(int boss[2],  int enemy_buttles[50][2]);

//检查撞击
void check_my_plane_to_enemy_plane_collide(int *score,int *life,	int my_plane[2], int enemy_planes[5][2]);
void check_my_plane_to_enemy_buttles_collide(int *life,	int enemy_buttles[50][2], int my_plane[2]);
void check_my_buttles_to_enemy_plane_collide(int *score,	int my_buttles[50][2], int enemy_planes[5][2]);
void check_my_buttles_to_boss_collide(int *score, int *boss_life,	int my_buttles[20][2], int boss[2]);

//清除所有敌机
void clear_all_enemy_plane(int my_plane[2],	int enemy_plane[5][2], int my_buttles[20][2], int enemy_buttles[50][2]);

//Boss运动
void move_boss(int boss[2], int *pace);

#endif
