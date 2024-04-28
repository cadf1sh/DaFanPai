/*
 * @Description:
 * @Autor: Tim
 * @Date: 2021-03-09 21:27:06
 * @LastEditors: Tim
 * @LastEditTime: 2021-03-09 22:17:47
 */
#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include "headfile.h"
#define LSMI leastSquareMethodInfo
#define DP deviationParam

typedef struct LeastSquareMethodInfo
{
    float K;//最小二乘法拟合斜率
    float B;//最小二乘法拟合截距

    float DK;
    float lastK;
    float lastB;

    float nowDeviation;
    float lastDeviation[8];
    float aveDeviation;
    float aveDeviationLeft;
    float aveDeviationRight;
    uint16 absDeviation[5];//閸嬪繐妯婄紒婵嗩嚠閸婏拷
    int16 diffDeviation[5];//瑜版挸澧犻崑蹇撴▕-娑撳﹥顐奸崑蹇撴▕閿涘苯浜曢崚锟�
} LeastSquareMethodInfo;

typedef struct DeviationParam
{
    uint32 k;//閺傛粎宸肩化缁樻殶
    uint32 b;//閹搭亣绐涚化缁樻殶
    uint16 annulusK[5];
    uint16 annulusB[5];
    uint16 rampK;
    uint16 rampB;
    uint16 obK;
    uint16 obB;
    uint16 PakingK;
    uint16 PakingB;
    uint16 obOffset;
} DeviationParam;

extern uint16_t outGarage_pwm;
extern DeviationParam deviationParam;
extern LeastSquareMethodInfo leastSquareMethodInfo;
extern void directionControl();
extern void deviationParam_init();
extern uint8 getline(float kb[2], ControlLine cl);
extern float getAveDeviation(ControlLine cl);
extern uint8 getrampline(float kb[2],ControlLine cl);
extern float get_ave(float p[],uint8 len);
extern ControlLine controlLine ;
extern uint8 delay_left_line_flag;
extern uint8 delay_right_line_flag;
#endif
