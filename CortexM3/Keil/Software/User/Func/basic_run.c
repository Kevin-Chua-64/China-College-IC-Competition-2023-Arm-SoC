#include <stdlib.h>
#include <stdio.h>
#include "CortexM3.h"
#include "basic_draw.h"
#include "draw.h"
#include "basic_run.h"

void switch_my_act(int my_plane[2],int my_buttles[20][2]){
	int i;
	uint8_t key = 0xff;
	
	// 按键还是手套
	if (USE_GLOVE){
		for(i=0;i<60;i++){
			if(UART->STATE & UART_STATE_RXBF_Msk) key = UART->DATA-0x30; // ascii转16进制
			else delay_ms(10);
		}
	}
	else{
		key = keyScan(1);
		delay_ms(500);
	}
	
	//清除之前所画
	//LCD_Fill(my_plane[0]-14,my_plane[1]-14,	my_plane[0]+14,my_plane[1]+14,BACK_COLOR);
	
	switch(key)
		{
			// right
			case KEY0_PRES:
				LCD_Fill(my_plane[0]-14,my_plane[1]-14,	my_plane[0]+14,my_plane[1]+14,BACK_COLOR);
				if(my_plane[0]+10 < right_limit - 14){
				my_plane[0] = my_plane[0]+10;
			}	break;
				
			//left
			case KEY2_PRES:
				LCD_Fill(my_plane[0]-14,my_plane[1]-14,	my_plane[0]+14,my_plane[1]+14,BACK_COLOR);
				if(my_plane[0]-10 > left_limit + 14){
				my_plane[0] = my_plane[0]-10;
			}	break;
				
			// attack
			case KEY1_PRES:				
				for(i=0; i<20; i++){
				if(my_buttles[i][0] == 0){
					my_buttles[i][0] = my_plane[0];
					my_buttles[i][1] = my_plane[1]-15;	
					break;	
				}
			}	break;
			default: break;
		}
}

void update_my_buttles(int my_buttles[20][2]){
	int i;
	
	for(i=0; i<20; i++){
		if(my_buttles[i][0] != 0){
			//清除之前所画
			LCD_Fill(my_buttles[i][0]-1,my_buttles[i][1]-1,	my_buttles[i][0]+1,my_buttles[i][1]+1,BACK_COLOR);
			
			my_buttles[i][1] = my_buttles[i][1] - 10;
			
			//当到达边界时销毁
			if(my_buttles[i][1] < up_limit){				
				my_buttles[i][0] = 0;
				my_buttles[i][1] = 0;
			}
		}
	}
}

void update_enemy_buttles(int enemy_buttles[50][2], int level){
	int i;
	
	for(i=0; i<50; i++){
		if(enemy_buttles[i][0] != 0){
			//清除之前所画
			LCD_Fill(enemy_buttles[i][0]-1,enemy_buttles[i][1]-1,	enemy_buttles[i][0]+1,enemy_buttles[i][1]+1,BACK_COLOR);
			
			enemy_buttles[i][1] = enemy_buttles[i][1] + level + 4;
			
			//当到达边界时销毁
			if(enemy_buttles[i][1] > down_limit){				
				enemy_buttles[i][0] = 0;
				enemy_buttles[i][1] = 0;
			}
		}
	}
}

void update_enemy_planes(int enemy_planes[5][2], int level){
	int i;
	
	for(i=0; i<5; i++){
		if(enemy_planes[i][0] != 0){
			//清除之前所画
			LCD_Fill(enemy_planes[i][0]-14,enemy_planes[i][1]-14,	enemy_planes[i][0]+14,enemy_planes[i][1]+14,BACK_COLOR);
			
			enemy_planes[i][1] = enemy_planes[i][1] + level + 3;
			//当到达边界时销毁
			if(enemy_planes[i][1] > down_limit-38){			
				enemy_planes[i][0] = 0;
				enemy_planes[i][1] = 0;
			}
		}
	}
}

//一次只生成一个飞机
//生成飞机概率 => 对应关卡
void generate_enemy_planes(int level, int enemy_planes[5][2]){
	int i;
	
	// 随机生成地敌方飞机
	for(i=0; i<5; i++){
		if(enemy_planes[i][0] == 0){
			if(rand()%100 < level + 6){
				enemy_planes[i][0] = rand()%210 + 14;
				enemy_planes[i][1] = 61;
			} break;
		}
	}
}

void generate_enemy_buttles(int level, int enemy_plane[5][2],  int enemy_buttles[50][2]){
	int i, j;
	
	for(i=0; i<5; i++){	
		if(enemy_plane[i][0] != 0 && rand()%200 < level + 1){
			for(j=0; j<50; j++){
				if(enemy_buttles[j][0] == 0){
					enemy_buttles[j][0] = enemy_plane[i][0];
					enemy_buttles[j][1] = enemy_plane[i][1] + 15;
					break;
				}
			}
		}
	}
}

void generate_boss_buttles(int boss[2],  int enemy_buttles[50][2]){
	int i, j;
	
	for(i=0; i<11; i++){
		if(rand()%200 < 3){
			for(j=0; j<50; j++){
				if(enemy_buttles[j][0] == 0){
					enemy_buttles[j][0] = boss[0] - 30 + i * 6;
					enemy_buttles[j][1] = boss[1] + 15;
					break;
				}
			}	
		}
	}
}

void check_my_plane_to_enemy_plane_collide(int *score,int *life,	int my_plane[2], int enemy_planes[5][2]){
	int i, temp;
	
	for(i=0; i<5; i++){
		if(enemy_planes[i][0] != 0){
			//盒模型碰撞检测
			if(abs(enemy_planes[i][0] - my_plane[0]) <= 28){
				if(abs(enemy_planes[i][1] - my_plane[1]) <= 28){
					LCD_Fill(my_plane[0]-14,my_plane[1]-14,	my_plane[0]+14,my_plane[1]+14,BACK_COLOR);
					LCD_Fill(enemy_planes[i][0]-14,enemy_planes[i][1]-14,	enemy_planes[i][0]+14,enemy_planes[i][1]+14,BACK_COLOR);
				
					my_plane[0] = 120;
					my_plane[1] = 290;
					
					enemy_planes[i][0] = 0;
					enemy_planes[i][1] = 0;
					
					temp = (*score);
					(*score) = temp+1;
					
					temp = (*life);
					(*life) = temp-1;
					
					LED0_ON;
					delay_ms(20);
					LED0_OFF;
					break;
				}
			}
		}
	}
}

void check_my_plane_to_enemy_buttles_collide(int *life,	int enemy_buttles[50][2], int my_plane[2]){
	int i, temp;
	
	for(i=0; i<50; i++){
		if(enemy_buttles[i][0] != 0){
			if(abs(enemy_buttles[i][0] - my_plane[0]) <= 15){
				if(abs(enemy_buttles[i][1] - my_plane[1]) <= 15){
					LCD_Fill(my_plane[0]-14,my_plane[1]-14,	my_plane[0]+14,my_plane[1]+14,BACK_COLOR);
					
					my_plane[0] = 120;
					my_plane[1] = 290;
					
					enemy_buttles[i][0] = 0;
					enemy_buttles[i][1] = 0;
					
					temp = (*life);
				 (*life) = temp-1;
					
					LED0_ON;
					delay_ms(20);
					LED0_OFF;
					break;
				}
			}
		}
	}
}

void check_my_buttles_to_enemy_plane_collide(int *score,	int my_buttles[20][2], int enemy_planes[5][2]){
	int i, j, temp;
	
	for(i=0; i<5; i++){
		if(enemy_planes[i][0] != 0){
			for(j=0; j<20; j++){
				//碰撞检测
				if(abs(my_buttles[j][0] - enemy_planes[i][0]) <= 15){
					if(abs(my_buttles[j][1] - enemy_planes[i][1]) <= 15){
						LCD_Fill(enemy_planes[i][0]-14,enemy_planes[i][1]-14,	enemy_planes[i][0]+14,enemy_planes[i][1]+14,BACK_COLOR);

						my_buttles[j][0] = 0;
						my_buttles[j][1] = 0;
						
						enemy_planes[i][0] = 0;
						enemy_planes[i][1] = 0;
						
						temp = (*score);
						(*score) = temp+1;
						
						LED1_ON;
					  delay_ms(20);
					  LED1_OFF;
						break;
					}
				}
			}
		}
	}
}

void check_my_buttles_to_boss_collide(int *score, int *boss_life,	int my_buttles[20][2], int boss[2]){
	int i, temp;
	
	for(i=0; i<20; i++){
				//碰撞检测
		if(my_buttles[i][0] != 0){
			if(abs(my_buttles[i][0] - boss[0]) <= 41){
				if(abs(my_buttles[i][1] - boss[1]) <= 41){
					LCD_Fill(boss[0]-40, boss[1] - 40, boss[0] + 40, boss[1] + 40,BACK_COLOR);
				
					my_buttles[i][0] = 0;
					my_buttles[i][1] = 0;
					
					temp = (*score);
					(*score) = temp+1;
					
					temp = (*boss_life);
					(*boss_life) = temp-1;
					
					LED1_ON;
					delay_ms(20);
					LED1_OFF;
					break;
				}
			}
		}
	}
}

void clear_all_enemy_plane(int my_plane[2],	int enemy_plane[5][2], int my_buttles[20][2], int enemy_buttles[50][2]){
	int i;
	
	for(i=0; i<5; i++){
		enemy_plane[i][0] = 0;
		enemy_plane[i][1] = 0;
	}
	
	for(i=0; i<20; i++){
		if(my_buttles[i][0] != 0){
			my_buttles[i][0] = 0;
			my_buttles[i][1] = 0;
		}
	}
	
	for(i=0; i<50; i++){
		if(enemy_buttles[i][0] != 0){
			enemy_buttles[i][0] = 0;
			enemy_buttles[i][1] = 0;
		}
	}
	
	LCD_Fill(1,41,239,318,BACK_COLOR);
	draw_my_plane(my_plane[0],my_plane[1], MY_PLANE_COLOR);
}

void move_boss(int boss[2], int *pace){
			
			int temp;
	    LCD_Fill(boss[0] - 40, boss[1] - 40, boss[0] + 40, boss[1] + 40, BACK_COLOR);
			
			boss[0] = boss[0] + (*pace);
			
			if(boss[0] < 43 || boss[0] > 197){				
				temp = (*pace);
				(*pace) = -temp;
			}
}
