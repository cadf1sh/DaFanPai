

#ifndef DEAL_IMG_H
#define DEAL_IMG_H
#include "headfile.h"

#define my_col 94
#define my_row 70
#define _my_col 188


//
//宏定义
#define Black 0
#define White 255
#define USE_num (3*(my_row))
extern uint8 nowThreshold;
extern uint8 previousThreshold;
extern uint8 minThreshold;
extern uint8 maxThreshold;
extern uint16 histogram[256];
//全局变量
extern uint8 Exiacn[my_row][my_col];
extern uint8 _Exiacn[my_row][my_col];
extern uint8 imgGray[my_row][_my_col];
extern uint8 Thresholdcompare;
extern uint8 points_l[USE_num][2];
extern uint8 points_r[USE_num][2];
extern uint8 points_l_num;
extern uint8 points_r_num;
//函数声明
uint8 getExiacn(void);
void getGrayscaleHistogram(void);
uint8 new_get_start_point(void);
uint8 adaptive_deal(uint8 x,uint8 y);
uint8 compare(uint8 white, uint8 black);
void Along_Wall(void);
void Threshold_init(void);
void do_black_board(void);
void ips114_draw_nine_point(uint8 x ,uint8 y,const uint16 color);
#endif /* CODE_MIGONG_H_ */

