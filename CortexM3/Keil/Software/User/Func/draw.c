#include "CortexM3.h"
#include "basic_draw.h"
#include "draw.h"

//初始化函数，一般一次游戏中只会使用一次
void draw_start_page(void){
	LCD_ShowString_color(70,80,200,24,24,"AIRCRAFT",BRRED,GBLUE);
	LCD_ShowString_color(85,103,200,24,24,"BATTLE",BRRED,GBLUE);		
	
	draw_my_plane(90,180,POINT_COLOR);
	draw_my_plane(120,180,POINT_COLOR);
	draw_my_plane(150,180,POINT_COLOR);
	
	LCD_ShowString_color(60,263,200,12,12,"Press KEY0 to start",BLACK,GBLUE);
}

void init_play_interface(void){
	//设置基础界面
	LCD_DrawLine(1,1,239,1);
	LCD_DrawLine(1,35,239,35);
	
	LCD_DrawLine(1,1,1,35);
	LCD_DrawLine(239,1,239,35);
	
	LCD_DrawLine(78,1,78,35);
	LCD_DrawLine(150,1,150,35);

	LCD_ShowString_color(5,8,90,24,16,"SCROE:",SCORE_COLOR,BACK_COLOR);
	LCD_ShowString_color(85,8,90,24,16,"LEVEL:",LEVEL_COLOR,BACK_COLOR);
	LCD_ShowString_color(155,8,90,24,16,"LIFE",LIFE_COLOR,BACK_COLOR);
}

void draw_end(int *score,int *score_2, int *score_3){
	LCD_Clear(GBLUE);
	
	LCD_ShowString_color(70,40,200,24,24,"GAME OVER",RED,GBLUE);
	LCD_ShowString_color(40,93,200,24,24,"This SCORE:",BROWN,GBLUE);
	LCD_ShowxNum_color(170,93,*score,2,24,0,BROWN,GBLUE);	
	
	LCD_ShowString_color(40,143,200,24,24,"Last SCORE:",BROWN,GBLUE);
	LCD_ShowxNum_color(170,143,*score_2,2,24,0,BROWN,GBLUE);

	*score_3 = *score_3>( *score>*score_2 ? *score : *score_2 ) ? *score_3 : ( *score>*score_2 ? *score : *score_2 );	// 取最大值
	*score_2 = *score;
	*score = 0;
	
	LCD_ShowString_color(40,203,200,24,24,"Best SCORE:",MAGENTA,GBLUE);
	LCD_ShowxNum_color(170,203,*score_3,2,24,0,MAGENTA,GBLUE);
	
	LCD_ShowString_color(50,290,200,12,12,"Press KEY0 to continue",BLACK,GBLUE);
	
	draw_my_plane(90,260,POINT_COLOR);
	draw_my_plane(120,260,POINT_COLOR);
	draw_my_plane(150,260,POINT_COLOR);
	
	draw_star(200,300);
	draw_star(20,130);
	draw_star(50,230);
	
	draw_star(150,120);
	draw_star(80,30);
}

//渲染函数，帧动画调用渲染
//包含分数/生命渲染
//我方子弹渲染
//敌方飞机渲染
//敌方子弹渲染

//其他渲染可调用 basic_draw.h
//1. 我方飞机渲染
//2. boss渲染
//3. cross渲染
void draw_play_title(int score, int level, int life){
	//填充信息
	int i=0;
	int start_x = 200;
	
	//LCD_Fill(75,8,130,32,GRAY);
	LCD_ShowxNum_color(55,8,score,2,16,0,SCORE_COLOR,BACK_COLOR);		//显示分数
	LCD_ShowxNum_color(135,8,level,1,16,0,LEVEL_COLOR,BACK_COLOR);		//显示等级
	
	LCD_Fill(195,10,231,20,BACK_COLOR);
	for(i=0; i<life; i++){	//显示生命值
		draw_heart(start_x+i*13, 15, LIFE_COLOR);
	}
}

//#define my_buttle_max 100
//#define enemy_buttle_max 300
//#define enemy_plane_max 10

//过滤 0，0
void draw_all_my_buttles(int my_buttles[20][2]){
	int i;
	for(i=0; i<20; i++){
		if(my_buttles[i][0] != 0){
			draw_my_bullet(my_buttles[i][0],my_buttles[i][1],MY_BULLET_COLOR);
		}
	}
}

//过滤 0，0
void draw_all_enemy_planes(int enemy_planes[5][2]){
	int i;
	for(i=0; i<5; i++){
		if(enemy_planes[i][0] != 0){
			draw_enemy_plane(enemy_planes[i][0],enemy_planes[i][1],ENEMY_PLANE_COLOR);
		}
	}
}

//过滤 0，0
void draw_all_enemy_buttles(int enemy_buttles[50][2]){
	int i;
	for(i=0; i<50; i++){
		if(enemy_buttles[i][0] != 0){
			draw_enemy_bullet(enemy_buttles[i][0],enemy_buttles[i][1],ENEMY_BULLET_COLOR);
		}
	}
}

//懒渲染，渲染所有组件，
//有些不变的也要渲染，会降低效率
void draw_play_all(int score,int level,int life, int my_plane[2], int my_buttles[20][2], int boss[2], int enemy_planes[5][2], int enemy_buttles[50][2]){
	draw_play_title(score,level,life);
	
	draw_my_plane(my_plane[0],my_plane[1],MY_PLANE_COLOR);
	draw_all_my_buttles(my_buttles);
	if(boss[0]!= 0){
		draw_boss(boss[0],boss[1],BOSS_COLOR);
	}
	draw_all_enemy_planes(enemy_planes);
	draw_all_enemy_buttles(enemy_buttles);
}
