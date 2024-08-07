#ifndef __MUSIC_H
#define __MUSIC_H

#define DO_L 262
#define RE_L 294
#define MI_L 330
#define FA_L 349
#define SO_L 392
#define LA_L 440
#define SI_L 494

#define DO_M 523
#define RE_M 587
#define MI_M 659
#define FA_M 698
#define SO_M 784
#define LA_M 880
#define SI_M 988

#define DO_H 1046
#define RE_H 1175
#define MI_H 1318
#define FA_H 1397
#define SO_H 1568
#define LA_H 1760
#define SI_H 1976

// ����
const int starSky_key[43]= {LA_M, MI_M, MI_M, MI_M, RE_M,
									MI_M, MI_M, DO_M, RE_M, MI_M, SO_M,
									LA_M, MI_M, MI_M, MI_M, RE_M,
									DO_M, DO_M, DO_M, RE_M, MI_M, SO_M,
									RE_M, RE_M, RE_M, RE_M, DO_M,
									RE_M, RE_M, SI_L, DO_M, RE_M, SO_M,
									MI_M, RE_M, MI_M, DO_H, MI_M,
									RE_M, DO_M, RE_M, SI_M, SO_M};  // ��ʱ������ = 0.5 * 1/Ƶ�ʣ�װ��ֵ = 50M * T
const int starSky_beat[43]={2, 2, 2, 1, 1,
									2, 2, 1, 1, 1, 1,
									2, 2, 2, 1, 1,
									2, 2, 1, 1, 1, 1,
									2, 2, 2, 1, 1,
									2, 2, 1, 1, 1, 1,
									1, 1, 2, 2, 2,
									1, 1, 2, 2, 2}; // 2Ϊ0.5�룬1Ϊ0.25��

#endif
