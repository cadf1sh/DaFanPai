#ifndef HEADFILE_H
#define HEADFILE_H

#define T_REX_QT_DEBUG 1
#define MY_QT_DEBUG 1

#define qout qDebug() << __FILE__ << __LINE__ << __FUNCTION__
#define MULTIPLE 4   //图片放大倍数
#define WifiBuffer_AllNum 13400

#include <QFileDialog>
#include <QStorageInfo>
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QKeyEvent>
#include <iostream>
#include <QRegExp>
#include <QDateTime>
#include <opencv2/opencv.hpp>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <QThread>

#define PI 3.1415926

#define BYTE0(x)       (*((char *)(&x)))
#define BYTE1(x)       (*((char *)(&x) + 1))
#define BYTE2(x)       (*((char *)(&x) + 2))
#define BYTE3(x)       (*((char *)(&x) + 3))

#define TRACK 0.178//轮距，米
#define SPD_CONVERT 57.8f//1m/s 5783个脉冲/s 57.83个脉冲/s

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int16_t int16;
typedef int32_t int32;

typedef struct Test
{
    uint8_t test0 = 0;
    uint8_t test1 = 0;
    uint8_t test2 = 0;
    uint8_t test3 = 0;
    uint8_t test4 = 0;
    float test5 = 0;
    float test6 = 0;
    float test7 = 0;
    float test8 = 0;
    float test9 = 0;
} Test;

typedef struct
{
  float Pitch;  //俯仰角
  float Yaw;    //偏航角
  float Roll;   //翻滚角
} EulerAngleTypedef;

typedef enum LCDState
{
    ADJUST_PARAM,
    DISPLAY_PARAM,
    DISPLAY_RUNNING_INFO,
    DO_NOT_DISPLAY,
    REVIEW,
}LCDState;

typedef enum RunState
{
    STOP,
    BALANCE,
    READY,
    RUNNING,
    BRAKING,
} RunState;

/* 小车运行模式枚举类型 */
typedef enum RunMode
{
    NORMAL_RUN,
    TIMING_RUN,
} RunMode;

typedef enum RunSpeedMode
{
    VARIABLE_SPEED,
    CONSTANT_SPEED,
    NORMAL_SHIFT_SPEED,
    ADC_SPEED,
} RunSpeedMode;

typedef enum StopReason
{
    HaventStarted = 0,
    TimeUp,
    RunOutLine,
    ParkingDetected,
    StallProtect,
} StopReason;

typedef struct ProgramRunTime
{
    uint16_t whileTime;
    uint16_t _whileTime;
    uint16_t maxWhileTime;
    uint16_t aveWhileTime;
    uint16_t goTime;
    uint16_t maxGoTime;
    uint16_t aveGoTime;
} ProgramRunTime;

typedef struct RunningInfo
{
    ProgramRunTime programRunTime;
    uint8_t annulusNum;
    uint8_t crossRoadNum;
    uint8_t rampNum;
    uint8_t srampNum;
} RunningInfo;

typedef struct Point
{
     uint8_t x;
     uint8_t y;
} Point;

typedef enum ControlLine
{
    BOTHLINE,
    LEFTLINE,
    RIGHTLINE,
    MIDLINE,
    NONELINE,
} ControlLine;

/*自己的头文件*/
#include "deal_img.h"
#include "Udp_recv.h"

#endif // HEADFILE_H
