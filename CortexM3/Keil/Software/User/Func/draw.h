#ifndef __DRAW_H
#define __DRAW_H

#define SCORE_COLOR         BLUE
#define LEVEL_COLOR         BRRED
#define LIFE_COLOR          RED
#define MY_PLANE_COLOR      BLUE
#define ENEMY_PLANE_COLOR   BROWN
#define BOSS_COLOR          BRED
#define MY_BULLET_COLOR     BLACK
#define ENEMY_BULLET_COLOR  RED

//��ʼ��������һ��һ����Ϸ��ֻ��ʹ��һ��
void draw_start_page(void);
void init_play_interface(void);
void draw_end(int *score,int *score_2, int *score_3);

//����Ⱦ����Ⱦ�������
//��Щ�����ҲҪ��Ⱦ����һ���̶��Ͻ���Ч��
void draw_play_all(int score,int level,int life, int my_plane[2], int my_buttles[20][2], int boss[2], int enemy_planes[5][2], int enemy_buttles[50][2]);

#endif
