/*
 * @Description:
 * @Autor: Tim
 * @Date: 2021-03-09 21:27:16
 * @LastEditors: Tim
 * @LastEditTime: 2021-03-09 22:23:59
 */
#include "direction.h"
extern ImgInfo imgInfo;
extern ImgFlag imgFlag;
extern AnnulusDEV AD;
extern float k1[YM];
extern float k2[YM];
extern uint8 leftlineZero;
extern uint8 rightlineZero;
extern uint16 nowAnnulusNum;
//extern EulerAngleTypedef      SystemAttitudeRate;
//extern SpeedInfo speedInfo;
extern volatile RunState runState;
LeastSquareMethodInfo leastSquareMethodInfo;
DeviationParam deviationParam;
ControlLine controlLine = BOTHLINE;
uint16 yaw_ramp_k = 30;
float kbleft[2] = {0};
float kbright[2] = {0};
float kbmid[2] = {0};
uint8 delay_left_line_flag;
uint8 delay_right_line_flag;
uint16_t outGarage_pwm ;

void directionControl()
{
    if(IF.ramp)
    {
        II.midnum = getrampline(kbmid, MIDLINE);
    }
    II.leftnum = getline(kbleft, LEFTLINE);
    II.rightnum = getline(kbright, RIGHTLINE);
    /*********/
    uint16 lnum = II.lnum_all;
    uint16 rnum = II.rnum_all;
    if (II.leftnum < 10)//左边边界点过少，直接把左边黑点数置0
    {
        lnum = 0;
    }
    if (II.rightnum < 10)//同理
    {
        rnum = 0;
    }
    if (II.leftnum < 10 && II.rightnum < 10)
    {
        lnum = rnum = 0;
    }
    if (II.line_forbid == LEFTLINE)//如果左边线禁用，左边黑点数为0
    {
        lnum = 0;
    }
    else if (II.line_forbid == RIGHTLINE)
    {
        rnum = 0;
    }
    else if (II.line_forbid == BOTHLINE)//路障，应该不用考虑
    {
        lnum = rnum = 0;
    }
//针对特殊情况对黑点数做处理，为后续控制线判断做准备
    if (lnum > rnum + 900)
    {
        controlLine = LEFTLINE;
    }
    else if (rnum > lnum + 900)
    {
        controlLine = RIGHTLINE;
    }
    else
    {
        if (lnum >= rnum && rnum <= 80 && lnum >= 80)
        {
            controlLine = LEFTLINE;
        }
        else if (rnum >= lnum && lnum <= 80 &&
                 rnum >= 80)
        {
            controlLine = RIGHTLINE;
        }
        else if (rnum >= 80 && lnum >= 80)
        {
            controlLine = BOTHLINE;
        }
        else
        {
            controlLine = NONELINE;
        }
    }
    if (II.line_forbid == NONELINE)//标准线里初始值就是NONELINE
    {
        if (IF.annulus > 6)//右圆环左线控制
        {
            controlLine = LEFTLINE;
        }
        else if (IF.annulus > 0)//左圆环右线控制
        {
            controlLine = RIGHTLINE;
        }
    }
    if (IF.ramp)
    {
        controlLine = MIDLINE;
        if (II.midnum <= 5)
        {
            controlLine = NONELINE;
        }

    }

    //********************************************************************
    //********************************************************************
    //********************************************************************


    if(IF.annulusDelay && (ABS(getMapXMax_Row(54,basemap,0) - 21)<=2 && BR_X(0)  <9))//左边界和左规划线差不多重合 )//右边界和右规划线差不多重合
         {
            delay_left_line_flag = 1;
         }
    if(IF.annulusDelay && (ABS(getMapXMin_Row(54,basemap,0) - 25)<=2 && BL_X(0) > 34))
        {
            delay_right_line_flag = 1;
        }
    if(IF.annulusDelay &&  delay_left_line_flag && Annulus_dir == goLeft)
    {
        controlLine = RIGHTLINE;
    }
    if(IF.annulusDelay &&  delay_right_line_flag && Annulus_dir == goRight)
    {
        controlLine = LEFTLINE ;
    }

    //********************************************************************
    //********************************************************************
    //********************************************************************


    //K越大反应越剧烈，B越小反应越敏感。//控制线选择完成//一般赛道的KK和bb
    float kk = DP.k / 10.000, bb = DP.b / 100.000;
    if((II.num_lm && II.num_rm && II.start_lm[0] > 30 && II.start_rm[0] > 30)
            || (II.num_lm && !II.num_rm && II.start_lm[0] > 30)
            || (II.num_rm && !II.num_lm && II.start_rm[0] > 30))
    {
        bb *= 0.8;//变得更灵敏
    }

   //标志位是圆环的话//圆环的KK和bb
    if (IF.annulus)
    {
        if(nowAnnulusNum>3)
            {
            nowAnnulusNum=7 - nowAnnulusNum;
            }
        kk = DP.annulusK[nowAnnulusNum - 1] / 10.000;
        bb = DP.annulusB[nowAnnulusNum - 1] / 100.000;
    }

    //爬坡//坡道的kk和bb
    else if (IF.ramp)
    {
        kk = DP.rampK / 10.000;
        bb = DP.rampB / 100.000;
    }
    if (IF.obstacle == OBRight)
    {
        kbleft[1] -= DP.obOffset / 10.0;
        kbright[1] -= DP.obOffset / 10.0;
        bb = DP.obB / 10.0;
    }
    else if (IF.obstacle == OBLeft)
    {
        kbleft[1] += DP.obOffset / 10.0;
        kbright[1] += DP.obOffset / 10.0;
        bb = DP.obB / 10.0;
    }
    if (controlLine == BOTHLINE)
    {
        LSMI.K = (kbleft[0] + kbright[0]) / 2;
        LSMI.B = (kbleft[1] + kbright[1]) / 2;
        LSMI.nowDeviation = LSMI.K * kk + LSMI.B * bb;
        LSMI.DK = LSMI.K * 100;
    }
    else if (controlLine == LEFTLINE)
    {
        LSMI.K = kbleft[0];
        LSMI.B = kbleft[1];
        LSMI.nowDeviation = LSMI.K * kk + LSMI.B * bb;
        LSMI.DK = LSMI.K * 100;
    }
    else if (controlLine == RIGHTLINE)
    {
        LSMI.K = kbright[0];
        LSMI.B = kbright[1];
        LSMI.nowDeviation =   LSMI.K * kk   +   LSMI.B * bb  ;
        LSMI.DK = LSMI.K * 100;
    }
    else if (controlLine == MIDLINE)
    {
        LSMI.K = kbmid[0];
        LSMI.B = kbmid[1];
        //坡道加上偏航角阻尼
        LSMI.nowDeviation = LSMI.K * kk + LSMI.B * bb ;
        LSMI.DK = LSMI.K * 100;
    }
//    Curvature = -1*PID_Realize_Curvature(&Turn_PIDcal, Turn_PIDparam[0], LSMI.nowDeviation, TAR_DIFF);
    if (!(LSMI.nowDeviation == LSMI.nowDeviation && LSMI.K == LSMI.K &&
          LSMI.B == LSMI.B))
    {
        LSMI.K = LSMI.lastK;
        LSMI.B = LSMI.lastB;
    }
    LSMI.lastDeviation[7]=LSMI.lastDeviation[6];
    LSMI.lastDeviation[6]=LSMI.lastDeviation[5];
    LSMI.lastDeviation[5]=LSMI.lastDeviation[4];
    LSMI.lastDeviation[4]=LSMI.lastDeviation[3];
    LSMI.lastDeviation[3]=LSMI.lastDeviation[2];
    LSMI.lastDeviation[2]=LSMI.lastDeviation[1];
    LSMI.lastDeviation[1]=LSMI.lastDeviation[0];
    LSMI.lastDeviation[0] = LSMI.nowDeviation;
    LSMI.lastK = LSMI.K;
    LSMI.lastB = LSMI.B;

    if (IF.annulus)
    {
        if (AD.flag == 3)//status=5//确认入环
        {
            LSMI.nowDeviation = AD.sumDev / AD.cnt;
            LSMI.DK = AD.sumDK / AD.cnt;
        }
        else if (AD.flag == 4)//出圆环补线=4//即将出环
        {
            LSMI.DK = AD.sumDK / AD.cnt;
        }
    }

    if (AD.flag == 1)//status=3
    {
        AD.sumDev += LSMI.nowDeviation;
        AD.sumDK += LSMI.DK;
        ++AD.cnt;
    }
    LSMI.aveDeviationLeft = getAveDeviation(LEFTLINE);//30-50左右的平均偏差
    LSMI.aveDeviationRight = getAveDeviation(RIGHTLINE);

    if (LSMI.aveDeviationLeft != 10000 && LSMI.aveDeviationRight != 10000)//正常情况
    {
        LSMI.aveDeviation =
            (LSMI.aveDeviationLeft + LSMI.aveDeviationRight) / 2;//总的平均偏差
    }
    else if (LSMI.aveDeviationLeft == 10000 && LSMI.aveDeviationRight != 10000)
    {
        LSMI.aveDeviation = LSMI.aveDeviationRight;
    }
    else if (LSMI.aveDeviationRight == 10000 && LSMI.aveDeviationLeft != 10000)
    {
        LSMI.aveDeviation = LSMI.aveDeviationLeft;
    }

    LSMI.absDeviation[4] = LSMI.absDeviation[3];
    LSMI.absDeviation[3] = LSMI.absDeviation[2];
    LSMI.absDeviation[2] = LSMI.absDeviation[1];
    LSMI.absDeviation[1] = LSMI.absDeviation[0];
    LSMI.absDeviation[0] = abs((int)LSMI.aveDeviation);


    LSMI.diffDeviation[4] = LSMI.diffDeviation[3];
    LSMI.diffDeviation[3] = LSMI.diffDeviation[2];
    LSMI.diffDeviation[2] = LSMI.diffDeviation[1];
    LSMI.diffDeviation[1] = LSMI.diffDeviation[0];
    LSMI.diffDeviation[0] = LSMI.absDeviation[0] - LSMI.absDeviation[1];
}


/*图像偏差参数*/
void deviationParam_init()
{
    DP.k = 630;//590
    DP.b = 75;//120
    DP.annulusK[0] = 630;//580
    DP.annulusB[0] = 75;//120
    DP.annulusK[1] = 630;
    DP.annulusB[1] = 75;
    DP.annulusK[2] = 630;
    DP.annulusB[2] = 75;
    DP.annulusK[3] = 592;
    DP.annulusB[3] = 105;
    DP.annulusK[4] = 592;
    DP.annulusB[4] = 105;
    DP.rampK = 620;
    DP.rampB = 70;
}
