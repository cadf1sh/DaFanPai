#ifndef SPEED_H
#define SPEED_H

#include "headfile.h"
#define SI speedInfo
#define SP speedParam

/* �ٶ���Ϣ�ṹ�� */
typedef struct SpeedInfo
{
    int16 varL[3];         //��¼��߱��������ٶ�ֵ
    int16 varR[3];         //��¼�ұ߱��������ٶ�ֵ

    int16 nowSpeedL;       //��������ĵ�ǰ�ٶ�
    int16 nowSpeedR;       //�ұ������ĵ�ǰ�ٶ�
    int16 nowSpeedM;       //�м�ĵ�ǰ�ٶ�

    float speedTrueL;     //�����ʵ�ٶ�
    float speedTrueR;     //�Ҳ���ʵ�ٶ�
    float speedTrueM;     //�м���ʵ�ٶ�
    uint16 speedLimitThreshold;//�����ٶȵ��޷�
    int16 speedMin;       //������С�ٶ�

    int16 aimSpeed;    //Ŀ���ٶ�
    int16 aimSpeedL;       //Ŀ���ٶ�
    int16 aimSpeedR;       //Ŀ���ٶ�

    int16 motorPWML;    //������PWMֵ
    int16 motorPWMR;    //������PWMֵ

    uint16 differential;
    uint16 encoderThreshold;
    uint8 speed_limit;//Ŀ���ٶȵ��޷�
    uint32 fuzzyspeed;

    int16 Length;     //距离
} SpeedInfo;

typedef struct SpeedParam
{
        uint16 constantSpeed;
        uint16 straightSpeedS;                     //����
        uint16 straightSpeedL;
        uint16 straightBreakSpeed;
        uint16 curveSpeed;         // ���ι�ʽ����ٶ�
        uint16 exitSpeed;
        uint16 maxExitSpeed;             //������ٵ�����ٶ�
        uint16 exitEnterSpeed;
        uint16 annulusSpeed[5];
        uint16 rampSpeed;         //���µ��ٶ�
        uint16 OutGarageSpeed;    //�����ٶ�
        uint16 ObstacleSpeed;     //����ٶ�
        uint16 ParkingSpeed;      //ͣ���ٶ�
        uint16 loseroadSpeed;
} SpeedParam;




typedef enum
{
    StraightL,
    StraightS,
    StraightBreak,
    Curve,
    Ramp,
    Annulus,
    Obstacle,
} SpeedType;

extern uint8 speedtype;
extern SpeedInfo speedInfo;
extern SpeedParam speedParam;
extern void speed_init();
extern void SpeedInfo_Init();
extern uint16_t getAimSpeed();
uint8 getSpeedType();
void speedParam_init();
#endif // SPEED_H
