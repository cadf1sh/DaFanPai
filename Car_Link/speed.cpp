
#include "speed.h"
SpeedInfo speedInfo;
SpeedParam speedParam;
/**
 * @description:速度信息初始化
 * @param {None}
 * @return {None}
 * @author: Tim
 */
void speed_init()
{
    SpeedInfo_Init();
    speedParam_init();
}


void SpeedInfo_Init()
{
    SI.varL[0] = 0;
    SI.varL[1] = 0;
    SI.varL[2] = 0;

    SI.varR[0] = 0;
    SI.varR[1] = 0;
    SI.varR[2] = 0;

    SI.nowSpeedL = 0;
    SI.nowSpeedR = 0;
    SI.nowSpeedM=0;
    SI.aimSpeedL = 0;
    SI.aimSpeedR = 0;
    SI.aimSpeed = 0;

    SI.speedTrueL = 0.0f;
    SI.speedTrueR = 0.0f;
    SI.speedTrueM = 0.0;
    SI.speedLimitThreshold = 600;
    SI.speedMin = 0;

    SI.motorPWML = 0;
    SI.motorPWMR = 0;
    SI.differential = 42;
    SI.encoderThreshold = 140;
    SI.speed_limit=3;

    SP.constantSpeed=100;
    SI.fuzzyspeed=0;
}



uint16_t getAimSpeed()
{
    uint16_t aimSpeed;


        if(speedtype == StraightL)
        {
            aimSpeed = SP.straightSpeedL;
        }
        else if(speedtype == StraightS)
        {
            aimSpeed = SP.straightSpeedS;
        }
        else if(speedtype == StraightBreak)
        {
            aimSpeed = SP.straightBreakSpeed;
        }
        else if(speedtype == Curve)
        {
            aimSpeed = SP.curveSpeed;
        }
        else if(speedtype == Annulus)
        {
            aimSpeed = SP.annulusSpeed[nowAnnulusNum-1];
        }
        else if (speedtype == Ramp)
        {
        aimSpeed  = SP.rampSpeed;
        }
        else if(speedtype == Obstacle)
        {
            aimSpeed = SP.ObstacleSpeed;
        }
}



uint8 getSpeedType()
{
    uint8 set_speedTop = 50;//设定速度最高行
    uint8 straight_ade = 10;
    static uint8 type = 0;
    static uint8 straight_long_delay = 0;

//    if (type == StraightBreak && speed_TrueM < SP.straightBreakSpeed)//涓婃鏄洿閬撻��鍑猴紝涓棿鐪熷疄閫熷害灏忎簬鐩撮亾閫�鍑洪�熷害
//    {
//        type = Curve;
//        straight_long_delay = 0;
//    }
//    else if (type == StraightBreak && (II.top < 50 || II.speedTop < 45 || LSMI.absDeviation[0] > 15))//涓婃鏄洿閬撻��鍑猴紝璧涢亾鏈�楂樼偣浣庝簬60琛寍|涓棿鍒楁渶浣庨粦鐐逛綆浜�50琛寍|杩欐鍥惧儚涓婂崐閮ㄥ垎骞冲潎鍋忓樊澶т簬20
//    {
//        type = Curve;
//        straight_long_delay = 0;
//    }
//    else if (IF.annulus)
//    {
//        type = Annulus;
//        straight_long_delay = 0;
//    }
//    else if (IF.ramp)
//    {
//        type = Ramp;
//        straight_long_delay = 0;
//    }
//    else if (type != StraightBreak && type != StraightL &&
//             II.top >= 53 && II.speedTop >= set_speedTop &&
//             LSMI.absDeviation[0] < straight_ade && LSMI.absDeviation[1] < straight_ade && LSMI.absDeviation[2] < straight_ade)
//    {
//        type = StraightS;
//        straight_long_delay++;
//        if(straight_long_delay > 5)
//        {
//            type = StraightL;
//        }
//    }
//    else if ((II.top < YM - 7 || II.speedTop < set_speedTop || LSMI.absDeviation[0] > straight_ade))
//    {
//        if(type != StraightBreak)
//        {
//            if((type == StraightL || type == StraightS) && speed_TrueM > SP.straightBreakSpeed)//涓婃鏄暱鐩撮亾鎴栫煭鐩撮亾锛屼腑闂寸湡瀹為�熷害澶т簬鐩撮亾閫�鍑洪�熷害
//            {
//                type = StraightBreak;
//            }
//            else
//            {
//                type = Curve;
//            }
//        }
//        straight_long_delay = 0;
//    }

     return type;
}

////////////////////////////////////////////////////////
void speedParam_init()
{
    SP.constantSpeed = 200;
    SP.straightSpeedL = 250;
    SP.straightSpeedS = 250;
    SP.straightBreakSpeed = 250;
    SP.curveSpeed = 250;
    SP.annulusSpeed[0] = 250;
    SP.annulusSpeed[1] = 250;
    SP.annulusSpeed[2] = 250;
    SP.annulusSpeed[3] = 250;
    SP.annulusSpeed[4] = 250;
    SP.rampSpeed = 250;
}

