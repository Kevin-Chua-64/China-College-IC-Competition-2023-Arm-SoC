#ifndef __BASICDRAW_H
#define __BASICDRAW_H 	

//x,y用来标识中心点位置
//所以以下尺寸都按照单数来计算
//仅作说明，与实现无关
#define cross_width   15
#define cross_height  15
#define heart_width   11
#define heart_height  11
#define buttle_width  3 
#define buttle_height 3
#define star_width    5
#define star_height   9

//21*41
void draw_my_plane(int x, int y, int color);
void draw_enemy_plane(int x ,int y, int color);
void draw_boss(int x, int y, int color);
void draw_my_bullet(int x, int y, int color);
void draw_enemy_bullet(int x, int y, int color);
void draw_cross(int x, int y);
void draw_heart(int x, int y, int color);
void draw_star(int x,int y);

#endif
