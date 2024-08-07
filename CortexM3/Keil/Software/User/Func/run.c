#include "CortexM3.h"
#include "draw.h"
#include "basic_run.h"
#include "run.h"

// 结构体记录数据
struct
{
	int score;       // 本局得分
	int score_2;     // 上局得分
	int score_3;     // 最佳得分
	
	int life;        // 剩余生命值
	
	int my_plane[2]; // 自己飞机的位置
	int my_buttles[my_buttle_max][2]; // 自己子弹的位置

	int level;       // 难度等级
	int boss[2];     // boss位置
	int boss_life;   // boss血量
	int pace;        // boss左右位移量
	
	int enemy_planes[enemy_plane_max][2];   // 敌机位置
	int enemy_buttles[enemy_buttle_max][2]; // 敌机子弹位置
	
	int come_boss;   // 1表示boss来临
}interface;
											
void key0_to_skip(void){
	while(1){
		if(keyScan(0) == KEY0_PRES) break;
		else delay_ms(10);
	}
}

void init_interface_info(void){
	int i=0;
	
	interface.score = 0;
	interface.score_2 = 0;
	interface.score_3 = 0;
	
	interface.life = 3;
	
	interface.my_plane[0] = 120;	interface.my_plane[1] = 290;
	
	for(i=0; i<my_buttle_max; i++){
		interface.my_buttles[i][0] = 0;
		interface.my_buttles[i][1] = 0;
	}
	
	interface.level = 1;
	interface.boss[0] = 0;
	interface.boss[1] = 0;
	interface.boss_life = 10;
	interface.pace = 3;
	interface.come_boss = 0;	
	for(i=0; i<enemy_plane_max; i++){
		interface.enemy_planes[i][0] = 0;
		interface.enemy_planes[i][1] = 0;
	}
	
	for(i=0; i<enemy_buttle_max; i++){
		interface.enemy_buttles[i][0] = 0;
		interface.enemy_buttles[i][1] = 0;
	}
}

void init_interface_info_second_time(void){
	int i=0;
	
	interface.life = 3;
	
	interface.my_plane[0] = 120;	interface.my_plane[1] = 290;
	
	for(i=0; i<my_buttle_max; i++){
		interface.my_buttles[i][0] = 0;
		interface.my_buttles[i][1] = 0;
	}
	
	interface.level = 1;
	interface.boss[0] = 0;
	interface.boss[1] = 0;
	interface.boss_life = 10;
	interface.pace = 3;
	interface.come_boss = 0;
		
	for(i=0; i<enemy_plane_max; i++){
		interface.enemy_planes[i][0] = 0;
		interface.enemy_planes[i][1] = 0;
	}
	
	for(i=0; i<enemy_buttle_max; i++){
		interface.enemy_buttles[i][0] = 0;
		interface.enemy_buttles[i][1] = 0;
	}
}

void draw_all_through_struct(void){
	draw_play_all(interface.score,interface.level,interface.life,	interface.my_plane,interface.my_buttles, interface.boss,	interface.enemy_planes, interface.enemy_buttles);
}

void all_init(void){
	//初始界面初始化
	LCD_Clear(GBLUE);
	draw_start_page(); 
	
	Timer_Init();
	
	key0_to_skip();
	LCD_Clear(BACK_COLOR);
	
	//游玩界面数据初始化
	init_interface_info();
	
	//游玩界面初始化
	init_play_interface();
	
	draw_all_through_struct();
	delay_ms(500);
}

void single_game(void){
	while(1){
		// 游戏结束
		if(interface.life == 0 || interface.boss_life == 0){break;}
		
		// boss
		if(interface.score >= 40 && interface.come_boss == 0){
			interface.boss[0] = 127;
			interface.boss[1] = 85;
			interface.come_boss = 1;
			
			clear_all_enemy_plane(interface.my_plane,	interface.enemy_planes, interface.my_buttles, interface.enemy_buttles);
		}
		// 升级难度
		if(interface.score % 10 == 0){
			interface.level = interface.score/10 + 1;

			if(interface.level > 5){
				interface.level = 5;
			}
		}
		
		// 更新按键或手套
		switch_my_act(interface.my_plane,interface.my_buttles);
		
		// 更新画面
		update_my_buttles(interface.my_buttles); // LCD_Fill
		if(interface.boss[0] == 0){
			update_enemy_planes(interface.enemy_planes, interface.level); // LCD_Fill
			update_enemy_buttles(interface.enemy_buttles, interface.level); // LCD_Fill
			
			generate_enemy_planes(interface.level,interface.enemy_planes);
			generate_enemy_buttles(interface.level, interface.enemy_planes, interface.enemy_buttles);
			
			check_my_plane_to_enemy_plane_collide(&(interface.score),&(interface.life),	interface.my_plane,interface.enemy_planes); // LCD_Fill
			check_my_plane_to_enemy_buttles_collide(&(interface.life),	interface.enemy_buttles, interface.my_plane); // LCD_Fill
			check_my_buttles_to_enemy_plane_collide(&(interface.score),	interface.my_buttles, interface.enemy_planes); // LCD_Fill
		}
		else{
			update_enemy_buttles(interface.enemy_buttles, 1); // LCD_Fill
			move_boss(interface.boss, &(interface.pace)); // LCD_Fill
			generate_boss_buttles(interface.boss, interface.enemy_buttles);
			check_my_plane_to_enemy_buttles_collide(&(interface.life),	interface.enemy_buttles, interface.my_plane); // LCD_Fill
			check_my_buttles_to_boss_collide(&(interface.score), &(interface.boss_life),	interface.my_buttles, interface.boss); // LCD_Fill
		}
		
		draw_all_through_struct();	 
		delay_ms(10);
	}
}

void run_game(void){
	single_game();
	
	draw_end(&(interface.score),&(interface.score_2),&(interface.score_3));		
	key0_to_skip(); 
	LCD_Clear(GBLUE);
	//初始界面初始化
	draw_start_page(); 
	key0_to_skip();
	LCD_Clear(BACK_COLOR);
	
	//游玩界面数据初始化
	init_interface_info_second_time();
	
	//游玩界面初始化
	init_play_interface();
	
	draw_all_through_struct();
	delay_ms(500);
}
