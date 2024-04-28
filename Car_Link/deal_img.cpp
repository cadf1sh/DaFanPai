/*
 * deal_img.c
 *
 *  Created on: 2023年7月7日
 *      Author:蛋仔
 */
#include "deal_img.h"
#ifdef MY_QT_DEBUG
EulerAngleTypedef SystemAttitude = {0};
EulerAngleTypedef SystemAttitudeRate = {0};
EulerAngleTypedef angle = {0};
extern int reset_flag;
int ABS(int data)
{
  if(data <= 0)
    data = -data;

  return data;
}

float relativeYaw;
#endif
float my_Yaw;
float offset;//淇
float keep_Yaw;
float infraredrayDis ;
uint8 img_threshold_group[3]={0};//鍒掑垎闃堝�煎尯鍩�   涓婁腑涓�
uint8 mid_height;
uint8 distinguish_delay = 0; //浣滀负妯柇鏂矾鍥惧儚鎴愬姛鍚庣殑涓存椂鍙橀噺 鍥惧儚鎴愮珛涔嬪悗 寤舵椂涓�灏忔鏃堕棿锛岀湅鐪嬫縺鍏夋槸涓嶆槸鏈夊�� 鏈夊�煎氨鍒や负妯柇 娌″�煎氨鍒や负鏂矾
uint8 loseroadok = 0 ;
uint8 inum_all_threshold = 45;  //鏂戦┈绾块粦鐐规暟闃堝��
uint8 nowThreshold = 0;
uint16_t edgeThreshold = 30;
uint16_t minThreshold = 0;
uint16_t maxThreshold = 255;
uint16_t startRow = 0;
uint16_t endRow = 0;
uint16_t numCnt;
uint8 t_right_flag = 1;//灏忓皬鏍囧織浣�
uint8 tt_right_flag = 0;//濡傛灉妫�鏌ュ埌涓�娆℃弧瓒宠鏄庝箣鍚庤偗瀹氫細婊¤冻
uint8 t_left_flag = 1;
uint8 tt_left_flag = 0;//濡傛灉妫�鏌ュ埌涓�娆℃弧瓒宠鏄庝箣鍚庝竴瀹氭弧瓒宠繛绾�
uint16_t histogram[256];
uint8    dealimg_finish_flag;                                           // 一场图像采集完成标志位
uint8    dealimg_send_flag;       // wifi发送的数组号数
uint8 imgsend_1[MT9V03X_H][MT9V03X_W]; //wifi需要发送的图像数组一号
uint8 imgsend_2[MT9V03X_H][MT9V03X_W]; //wifi需要发送的图像数组一号
uint8 imgGray[MT9V03X_H][MT9V03X_W];
uint8 (*imgBinary)[COL] = imgGray;
uint8 basemap[YM][XM];
uint8 leftmap[YM][XM];
uint8 rightmap[YM][XM];
uint8 allmap[YM][XM];
uint8 insidemap[YM][XM];
uint8 deletemap[YM][XM];
uint8 linemap[YM][XM];
uint8 midline[YM / 2];
uint16_t delaytime_obstacle_threshold_1;//瀹炲垯鐘舵��1璺▼
uint16_t delaytime_obstacle_threshold_2;//瀹炲垯鐘舵��2璺▼
uint16_t delaytime_ramp[5];
uint16_t delaytime_Annulus;
uint16_t delaytime_inloseroad;
uint16_t delaytime_outloseroad;
uint16_t protect_time;
uint8 previousThreshold = 0;
static uint8 ramp_tip = 1;
uint8 straight_clear = 0;
uint8 tips_temp ;
//float leftline[YM] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
//                      2, 2, 2, 2, 3, 3, 3, 3, 4, 4,
//                      4, 4, 5, 5, 5, 6, 6, 6, 6, 7,
//                      7, 7, 8, 8, 8, 8, 9 ,9, 9, 9,
//                      10, 10, 10, 10, 11 ,11, 11, 12, 12, 12,
//                      13, 13, 13, 13, 14, 14, 14, 14, 15, 15,
//                      15, 16, 16, 16, 16, 17, 17, 17, 17, 18};
//float leftline[YM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//                      1, 1, 1, 1, 2, 2, 2, 2, 3, 3,
//                      3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
//                      6, 6, 6, 6, 7, 7, 7 ,8, 8, 8,
//                      8, 9, 9, 9, 9, 10, 10, 10, 11, 11,
//                      11, 11, 12, 12, 12, 12, 13, 13, 13, 14,
//                      14, 14, 15, 15, 15, 16, 16, 16, 16, 17};
//float leftline[YM] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 2,
//                      2, 2, 2, 3, 3, 3, 3, 4, 4, 4,
//                      4, 5, 5, 5, 5, 6, 6, 6, 6, 7,
//                      7, 7, 8, 8, 8, 8, 9 ,9, 9, 9,
//                      10, 10, 10, 10, 11, 11, 11, 12, 12, 12,
//                      12, 13, 13, 13, 13, 14, 14, 14, 14, 15,
//                      15, 15, 15, 16, 16, 16, 16, 16, 17, 17};
//float leftline[YM] = {6, 6, 7, 7, 7, 7, 7, 7, 8, 8,
//                      8, 8, 8, 8, 9, 9, 9, 9, 10, 10,
//                      10, 10, 10, 11, 11, 11, 11, 11, 12, 12,
//                      12, 12, 12, 12, 13, 13, 13 ,13, 13, 14,
//                      14, 14, 14, 14, 15, 15, 15, 15, 15, 16,
//                      16, 16, 16, 16, 16, 17, 17, 17, 17, 17,
//                      18, 18, 18, 18, 18, 19, 19, 19, 19, 19};




float leftline[YM] = {2, 2,
                      3, 3, 3,
                      4, 4, 4,
                      5, 5, 5,
                      6, 6,
                      7, 7, 7,
                      8, 8,
                      9, 9, 9,
                      10, 10, 10,
                      11, 11, 11 ,
                      12, 12, 12,
                      13, 13, 13,
                      14, 14, 14,
                      15, 15, 15,
                      16, 16, 16,
                      17, 17, 17,
                      18, 18, 18,
                      19, 19, 19,
                      20, 20, 20,
                      21};// 閫熷害瑙勫垝绾�


float rightline[YM];
float k1[YM];




//float k2[YM] = {23.5, 23.8, 24.2, 24.8, 25.3, 25.9, 26.4, 26.8, 27.3, 27.7,
//                28.2, 28.7, 29.3, 29.8, 30.4, 31.1, 31.7, 32.3, 32.9, 33.6,
//                34.2, 34.8, 35.4, 36.0, 36.7, 37.4, 38.1, 28.8, 29.6, 40.5,
//                41.4, 42.4, 43.4, 44.5, 45.6, 46.7, 47.9, 49.1, 50.4, 51.7,
//                53.1, 54.5, 55.9, 57.5, 59.1, 60.8, 62.6, 64.5, 66.5, 68.7,
//                70.9, 73.3, 75.9, 78.7, 81.6, 84.7, 88.1, 91.9, 96.0, 100.6,
//                105.9, 112.1, 119.2, 127.5, 137.2, 148.2, 160.9, 174.9, 189.9, 205.4};
//float k2[YM] = {23.5, 23.9, 24.1, 24.3, 24.5, 24.8, 25.2, 25.6, 26.1, 26.7,
//                27.3, 27.9, 28.5, 29.1, 29.7, 30.3, 30.8, 31.4, 32.0, 32.6,
//                33.2, 33.9, 34.5, 35.2, 35.9, 36.6, 37.4, 38.2, 39.0, 39.8,
//                40.7, 41.6, 42.6, 43.6, 44.7, 45.9, 47.1, 48.4, 49.9, 51.4,
//                53.1, 54.8, 56.7, 58.7, 60.8, 63.0, 65.3, 67.6, 70.1, 72.5,
//                75.0, 77.6, 80.3, 83.1, 86.1, 89.2, 92.8, 96.7, 101.2, 106.5,
//                112.7, 120.1, 128.8, 139.2, 151.4, 165.6, 182.0, 200.7, 221.6, 244.7};


float k2[YM] = {
        25, 25.6, 26.3, 26.9, 27.6,                 //1
        28.5,  29.54, 30.5, 31.5 , 32.5,            //2
        33.8, 35.2, 37.9, 39.2, 40.5,               //3
        42.2, 43.9, 45.6, 47.3 , 49,                //4
        50.3 ,   51.1 ,   52.5 , 53.6  , 54.7 ,     //5
        56.1, 57.2, 59.2,    61.6 , 64.0,           //6
        65.0, 66.4,  68.8,70.0, 71.2,               //7
        73.9, 76.6, 79.4, 82.1, 84.9,               //8
        90.9, 94.0, 97.1,  100.2, 103.2,            //9
        106.7, 110.0 , 113.5 , 117.0, 120.4,        //10
        124.2, 128.0, 131.8, 135.6, 140,            //11
    };


//float k2[YM] = {0};


AnnulusDEV AD;
AnnulusDEV AD_INIT = {0};
ImgInfo imgInfoInit;
ImgInfo imgInfo;
ImgFlag imgFlag = {0};
ImgFlag LAST_IF = {0};

uint8 speedlineLeft[YM];
uint8 speedlineRight[YM];
int8_t bottom_line = 0;
int8_t top_line = 55; //鎴浘鑼冨洿
PtStack noDown;
uint8 leftlineZero = 0;
uint8 rightlineZero = 0;
uint8 deleteforleftflag = 0;
uint8 deleteforrightflag = 0;
uint8 ob_direction;//障碍方向
uint16_t nowAnnulusNum = 0;
uint16_t nowGarageNum = 0;
uint16_t nowRampNum = 0;
uint16_t _nowRampNum = 1;
uint16_t nowobstacleNum = 0;
uint16_t nowloseroadNum = 0;



/******************************************************************************
* 鍑芥暟鍚嶇О     : standard
* 鎻忚堪        : 涓�绯诲垪鍒濆鐨勬爣鍑嗗寲澶勭悊
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void standard(void)
{
    float x;
    uint8 speedX;
    for (uint8 j = 0; j < YM; ++j)
    {
    rightline[j] = XX - leftline[j];  //鍙宠竟绾胯祴鍊�//杈圭晫瑙勫垝绾�
    k1[j] = XM / (rightline[j] - leftline[j] + 1);  //鐢ㄦ潵鐭绐佸彉杈圭晫//妯悜鐣稿彉鐭鏁扮粍
    speedX = 0;
    speedlineLeft[j] = 23 - speedX;
    speedlineRight[j] = 23 + speedX;  //閫熷害瑙勫垝绾�
    if (!leftlineZero && leftline[j] >= 0)
    {
    //宸﹁竟绾胯捣濮嬭鏁扮‘瀹�
    leftlineZero = j;
    }
    if (!rightlineZero && rightline[j] <= XX)
    {
    //鍙宠竟绾胯捣濮嬭鏁扮‘瀹�
    rightlineZero = j;
    }
    }
    imgInfoInit.num_lm = 0;
    imgInfoInit.num_rm = 0;



    memset(imgInfoInit.start_lm, 0, sizeof(imgInfoInit.start_lm));
    memset(imgInfoInit.start_rm, 0, sizeof(imgInfoInit.start_rm));


    for (uint8 i = 0; i < 5; ++i)
    {
    imgInfoInit.left_bottom[i].x = XX;
    imgInfoInit.right_bottom[i].x = 0;
    imgInfoInit.left_top[i].x = XX;
    imgInfoInit.right_top[i].x = 0;

    imgInfoInit.bottom_left[i].x = XX;
    imgInfoInit.bottom_right[i].x = 0;
    imgInfoInit.top_left[i].x = XX;
    imgInfoInit.top_right[i].x = 0;

    imgInfoInit.left_bottom[i].y = YY;
    imgInfoInit.right_bottom[i].y = YY;
    imgInfoInit.left_top[i].y = 0;
    imgInfoInit.right_top[i].y = 0;

    imgInfoInit.bottom_left[i].y = YY;
    imgInfoInit.bottom_right[i].y = YY;
    imgInfoInit.top_left[i].y = 0;
    imgInfoInit.top_right[i].y = 0;

    imgInfoInit.d_left_bottom[i].x = XX;
    imgInfoInit.d_right_bottom[i].x = 0;
    imgInfoInit.d_left_bottom[i].y = YY;
    imgInfoInit.d_right_bottom[i].y = YY;
    }
    imgInfoInit.d_bottom.x = XX;
    imgInfoInit.d_bottom.y = YY;  //deletemap瑙掔偣
    imgInfoInit.left_x = XM;
    imgInfoInit.right_x = 0;
    imgInfoInit.left_y = YM;
    imgInfoInit.right_y = YM;

    imgInfoInit._top = 0;
    imgInfoInit.top = 0;
    imgInfoInit.bottom = YY;
    imgInfoInit.speedTop = YM;
    imgInfoInit.annulusTop = YM;

    imgInfoInit.stop_line = STOP_LINE; //鍋忓樊鎴绾匡紝楂樹簬杩欐潯绾跨殑琛屾暟涓嶅弬涓庡亸宸绠�
    imgInfoInit.limit_line = YM;
    imgInfoInit.step = STEP; //鎼滃浘鎴绾�
    imgInfoInit.line_forbid = NONELINE;
    imgInfoInit.repeatNum = 0;
    imgInfoInit.dnum_top = 0;
    imgInfoInit.bnum_all = 0;
    imgInfoInit.lnum_all = 0;
    imgInfoInit.rnum_all = 0;
    imgInfoInit.dnum_all = 0;
    memset(II.lnum, 0, sizeof(II.lnum));
    memset(II.rnum, 0, sizeof(II.rnum));

    imgInfoInit.lmin = 0;
    imgInfoInit.rmin = 0;
    imgInfoInit.leftnum = 0;
    imgInfoInit.rightnum = 0;
    imgInfoInit.midnum = 0;

}



/******************************************************************************
* 鍑芥暟鍚嶇О     : map_init
* 鎻忚堪        : 鍦板浘涓�绯诲垪鍒濆
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void map_init(void)
{
    memset(histogram, 0, sizeof(histogram));
    memset(basemap, 1, sizeof(basemap));
    memset(leftmap, 0, sizeof(leftmap));
    memset(rightmap, 0, sizeof(rightmap));
    memset(insidemap, 1, sizeof(insidemap));
    memset(deletemap, 0, sizeof(deletemap));
    memset(midline, 0, sizeof(midline));
    memset(linemap, 0, sizeof(linemap));
    noDown.num = 0;
    II = imgInfoInit;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : getGrayscaleHistogram
* 鎻忚堪        : 鐏板害鐩存柟鍥剧粺璁�
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void getGrayscaleHistogram(void)
{
    uint8 *ptr = &imgGray[MT9V03X_H-YM][0];
    uint8 *ptrEnd = &imgGray[MT9V03X_H-1][_COL] + 1;
    while (ptr != ptrEnd)
    {
        ++histogram[*ptr++];
    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : getOSTUThreshold
* 鎻忚堪        : 澶ф触娉�
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 getOSTUThreshold(void)//澶ф触娉�
{
    uint32_t sum = 0, valueSum = 0;
    uint64_t sigma = 0, maxSigma = 0;
    float w1 = 0, w2 = 0;
    int32_t u1 = 0, u2 = 0;
    uint8 min = 0, max = 255;
    for (min = 0; histogram[min] == 0 && min < 255; ++min)
        ;
    for (max = 255; histogram[max] == 0 && max > 0; --max)
        ;
    if (min < minThreshold)
    {
        min = minThreshold;
    }
    if (max > maxThreshold)
    {
        max = maxThreshold;
    }
    uint32_t lowSum[256] = {0};
    uint32_t lowValueSum[256] = {0};
    /*浼樺寲鍚庣殑澶ф触娉曪紝鏃堕棿澶嶆潅搴�  n^2  鍙樹负  2n  */
    for (uint16_t i = min; i <= max; ++i)
    {
        sum += histogram[i];
        valueSum += histogram[i] * i;
        lowSum[i] = sum;
        lowValueSum[i] = valueSum;
    }
    for (uint16_t i = min; i < max + 1; ++i)
    {
        w1 = (float)lowSum[i] / sum;
        w2 = 1 - w1;
        u1 = (int32_t)(lowValueSum[i] / w1);
        u2 = (int32_t)((valueSum - lowValueSum[i]) / w2);
        sigma = (uint64_t)(w1 * w2 * (u1 - u2) * (u1 - u2));
        if (sigma >= maxSigma)
        {
            maxSigma = sigma;
            nowThreshold = i;
        }
        else
        {
            break;
        }
    }
    if(nowThreshold<117)
        nowThreshold=117;
    previousThreshold = nowThreshold;
    return nowThreshold;
}


/******************************************************************************
* 鍑芥暟鍚嶇О     : H(x, y)
* 鎻忚堪        : 鍨傜洿鏂瑰悜鐨勪竴闃跺樊鍒嗭紝姊害
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
#define H(x, y)         (imgGray[y][x] > imgGray[y + 1][x + 1] ?                                \
    imgGray[y][x] - imgGray[y + 1][x + 1] : imgGray[y + 1][x + 1] - imgGray[y][x])



/******************************************************************************
* 鍑芥暟鍚嶇О     : V(x, y)
* 鎻忚堪        : 姘村钩鏂瑰悜鐨勪竴闃跺樊鍒嗭紝姊害
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
#define V(x, y)         (imgGray[y][x + 1] > imgGray[y + 1][x] ?                                \
    imgGray[y][x + 1] - imgGray[y + 1][x] : imgGray[y + 1][x] - imgGray[y][x + 1])



/******************************************************************************
* 鍑芥暟鍚嶇О     : robertJudge
* 鎻忚堪        : robert杈圭紭妫�娴�
* 杩涘叆鍙傛暟     : x, y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
#define robertJudge(x, y)                                                               \
{                                                                                       \
    num = 0;                                                                            \
    sum = 0;                                                                            \
    for (uint8 i = 0; i < 4; ++i)                                                     \
    if (imgGray[y][x + i] < nowThreshold)                 ++sum;                        \
    else if (H(x + i, y) + V(x + i, y) >= edgeThreshold)  sum += 2;                     \
    if (sum >= 4)     ++num;                                                            \
    sum = 0;                                                                            \
    for (uint8 i = 0; i < 4; ++i)                                                     \
    if (imgGray[y + 1][x + i] < nowThreshold)                     ++sum;                \
    else if (H(x + i, y + 1) + V(x + i, y + 1) >= edgeThreshold)  sum += 2;             \
    if (sum >= 4)     ++num;                                                            \
    if (num == 0)             {allmap[j][i] = 0; continue;}                             \
    else if (num >= 2)        {allmap[j][i] = 1; continue;}                             \
    sum = 0;                                                                            \
    for (uint8 i = 0; i < 4; ++i)                                                     \
    if (imgGray[y + 2][x + i] < nowThreshold)                     ++sum;                \
    else if (H(x + i, y + 2) + V(x + i, y + 2) >= edgeThreshold)  sum += 2;             \
    if (sum >= 4)     ++num;                                                            \
    if (num >= 2)     {allmap[j][i] = 1; continue;}                                     \
    else              {allmap[j][i] = 0; continue;}                                     \
    }


/******************************************************************************
* 鍑芥暟鍚嶇О     : robertJudge_Y
* 鎻忚堪        : robert杈圭紭妫�娴嬮槻姝鏂瑰悜瓒婄晫鍋氱殑鐗规畩澶勭悊
* 杩涘叆鍙傛暟     : x, y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
#define robertJudge_Y(x, y)                                                             \
{                                                                                       \
    num = 0;                                                                            \
    sum = 0;                                                                            \
    for (uint8 i = 0; i < 4; ++i)                                                     \
    if (imgGray[y][x + i] < nowThreshold)                 ++sum;                    \
    else if (H(x + i, y) + V(x + i, y) >= edgeThreshold)  sum += 2;                 \
    if (sum >= 4)     ++num;                                                            \
    sum = 0;                                                                            \
    for (uint8 i = 0; i < 4; ++i)                                                     \
    if (imgGray[y + 1][x + i] < nowThreshold)                     ++sum;            \
    else if (H(x + i, y + 1) + V(x + i, y + 1) >= edgeThreshold)  sum += 2;         \
    if (sum >= 4)     ++num;                                                            \
    if (num == 0)             {allmap[j][i] = 0; continue;}                             \
    else if (num >= 2)        {allmap[j][i] = 1; continue;}                             \
    if (imgGray[y + 2][x] < nowThreshold &&                                             \
    imgGray[y + 2][x + 1] < nowThreshold &&                             \
    imgGray[y + 2][x + 2] < nowThreshold &&                             \
    imgGray[y + 2][x + 3] < nowThreshold)                               \
{                                                                                   \
    ++num;                                                                          \
    }                                                                                   \
    if (num >= 2)     {allmap[j][i] = 1; continue;}                                     \
    else              {allmap[j][i] = 0; continue;}                                     \
    }



/******************************************************************************
* 鍑芥暟鍚嶇О     : robertJudge_X
* 鎻忚堪        : robert杈圭紭妫�娴嬮槻姝鏂瑰悜瓒婄晫鍋氱殑鐗规畩澶勭悊
* 杩涘叆鍙傛暟     : x, y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
#define robertJudge_X(x, y)                                                             \
{                                                                                       \
    num = 0;                                                                            \
    sum = 0;                                                                            \
    for (uint8 i = 0; i < 3; ++i)                                                     \
    if (imgGray[y][x + i] < nowThreshold)                 ++sum;                    \
    else if (H(x + i, y) + V(x + i, y) >= edgeThreshold)  sum += 2;                 \
    if (imgGray[y][x + 3] < nowThreshold)       ++sum;                                  \
    if (sum >= 4)     ++num;                                                            \
    sum = 0;                                                                            \
    for (uint8 i = 0; i < 3; ++i)                                                     \
    if (imgGray[y + 1][x + i] < nowThreshold)                     ++sum;            \
    else if (H(x + i, y + 1) + V(x + i, y + 1) >= edgeThreshold)  sum += 2;         \
    if (imgGray[y + 1][x + 3] < nowThreshold)       ++sum;                              \
    if (sum >= 4)     ++num;                                                            \
    if (num == 0)             {allmap[j][i] = 0; continue;}                             \
    else if (num >= 2)        {allmap[j][i] = 1; continue;}                             \
    for (uint8 i = 0; i < 3; ++i)                                                     \
    if (imgGray[y + 2][x + i] < nowThreshold)                     ++sum;            \
    else if (H(x + i, y + 2) + V(x + i, y + 2) >= edgeThreshold)  sum += 2;         \
    if (imgGray[y + 2][x + 3] < nowThreshold)       ++sum;                              \
    if (sum >= 4)     ++num;                                                            \
    if (num >= 2)     {allmap[j][i] = 1; continue;}                                     \
    else              {allmap[j][i] = 0; continue;}                                     \
    }



/******************************************************************************
* 鍑芥暟鍚嶇О     : robertJudge_XY
* 鎻忚堪        : robert杈圭紭妫�娴嬮槻姝鍜寉鏂瑰悜鍚屾椂瓒婄晫鍋氱殑鐗规畩澶勭悊
* 杩涘叆鍙傛暟     : x, y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
#define robertJudge_XY(x, y)                                                            \
{                                                                                       \
    num = 0;                                                                            \
    sum = 0;                                                                            \
    for (uint8 i = 0; i < 3; ++i)                                                     \
    if (imgGray[y][x + i] < nowThreshold)                 ++sum;                    \
    else if (H(x + i, y) + V(x + i, y) >= edgeThreshold)  sum += 2;                 \
    if (imgGray[y][x + 3] < nowThreshold)       ++sum;                                  \
    if (sum >= 4)     ++num;                                                            \
    sum = 0;                                                                            \
    for (uint8 i = 0; i < 3; ++i)                                                     \
    if (imgGray[y + 1][x + i] < nowThreshold)                     ++sum;            \
    else if (H(x + i, y + 1) + V(x + i, y + 1) >= edgeThreshold)  sum += 2;         \
    if (imgGray[y + 1][x + 3] < nowThreshold)       ++sum;                              \
    if (sum >= 4)     ++num;                                                            \
    if (num == 0)             {allmap[j][i] = 0; continue;}                             \
    else if (num >= 2)        {allmap[j][i] = 1; continue;}                             \
    if (imgGray[y + 2][x] < nowThreshold &&                                             \
    imgGray[y + 2][x + 1] < nowThreshold &&                             \
    imgGray[y + 2][x + 2] < nowThreshold &&                             \
    imgGray[y + 2][x + 3] < nowThreshold)                               \
{                                                                                   \
    ++num;                                                                          \
    }                                                                                   \
    if (num >= 2)     {allmap[j][i] = 1; continue;}                                     \
    else              {allmap[j][i] = 0; continue;}                                     \
    }



/******************************************************************************
* 鍑芥暟鍚嶇О     : ostuJudge
* 鎻忚堪        : 澶ф触娉曞垽鏂�
* 杩涘叆鍙傛暟     : uint8 x, uint8 y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 ostuJudge(uint8 x, uint8 y)
{
    uint8 cnt = 0;
    uint8 threshold = 0;
    if (y <= 15+18)//这里的y是逐飞原图的y，小的时候是远景，大的时候是近景。是未转置图像的上半部分我说的远近是与车子的距离跟哪个图像无关
    {
    threshold = img_threshold_group[0];
    }
    else if (y > 20+15 && y <= 45+15)//这里的y是逐飞原图的y，小的时候是远景，大的时候是近景。是未转置图像的上半部分我说的远近是与车子的距离跟哪个图像无关
    {
    threshold = img_threshold_group[1];
    }
    else
    {
        threshold = img_threshold_group[2];
    }
//    threshold = nowThreshold;
    for(int i = 0; i < 4; i++)//鎼滀粬鍙充晶鍥涙牸鐨勫儚绱犵偣棰滆壊
    {
        if(imgGray[y][x + i] <threshold)//灏忎簬闃堝�煎垯鏄粦鐐癸紝鐩稿悓闃堝�间笅搴曢儴澶氬嚭鏉ヤ簡涓�浜涚櫧鐐�
            cnt++;
    }
//    if(cnt >= 1)
//        {
//        return 0;//baise
//        }
//    else
//        {
//        return 1;
//        }
    if(cnt >= 3)
        {
        return 1;//榛戣壊  234涓粦鐐圭殑鍒や负榛戝嵆涓変釜鐧界偣
        }
    else
        {
        return 0;
        }
}
//uint8 ostuJudge(uint8 x, uint8 y,uint8 threshold)
//{
//    uint8 cnt = 0;
//    for(int i = 0; i < 4; i++)//鎼滀粬鍙充晶鍥涙牸鐨勫儚绱犵偣棰滆壊
//    {
//        if(imgGray[y][x + i] <threshold)//灏忎簬闃堝�煎垯鏄粦鐐癸紝鐩稿悓闃堝�间笅搴曢儴澶氬嚭鏉ヤ簡涓�浜涚櫧鐐�
//            cnt++;
//    }
////    if(cnt >= 1)
////        {
////        return 0;//baise
////        }
////    else
////        {
////        return 1;
////        }
//    if(cnt >= 3)
//        {
//        return 1;//榛戣壊  234涓粦鐐圭殑鍒や负榛戝嵆涓変釜鐧界偣
//        }
//    else
//        {
//        return 0;
//        }
//}


/******************************************************************************
* 鍑芥暟鍚嶇О     : OSTU_ROBERT
* 鎻忚堪        : 闃冲厜绠楁硶2
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void OSTU_ROBERT(void)//
{
    uint8 _x;
    uint8 _y;
    int y = -1;

    for (uint8 j = bottom_line; j < top_line; ++j)//3-73
    {
        _y = (_ROW - j);
        y++;
        for (uint8 i = 0; i < XM; ++i)
        {
            _x = i << 2;
            allmap[y][i] = ostuJudge(_x, _y);//濡傛灉璇ョ偣鍙充晶鍥涙牸鑷冲皯鏈�2涓粦鐐癸紝鍒欑疆涓洪粦鑹�
        }
    }
}
//void OSTU_ROBERT(void)//
//{
//    uint8 _x;
//    uint8 _y;
//    int y = -1;
//    uint8 threshold = img_threshold_group[2];
//    for (uint8 j = bottom_line; j < top_line; ++j)//3-73
//    {
//        _y = (_ROW - j);
//        y++;
//        if (y == 18)
//        {
//        threshold = img_threshold_group[1];
//        }
//        else if (y == 45)//这里的y是逐飞原图的y，小的时候是远景，大的时候是近景。是未转置图像的上半部分我说的远近是与车子的距离跟哪个图像无关
//        {
//        threshold = img_threshold_group[0];
//        }
//        for (uint8 i = 0; i < XM; ++i)
//        {
//            _x = i << 2;
//            allmap[y][i] = ostuJudge(_x, _y, threshold);//濡傛灉璇ョ偣鍙充晶鍥涙牸鑷冲皯鏈�2涓粦鐐癸紝鍒欑疆涓洪粦鑹�
//        }
//    }
//}


/******************************************************************************
* 鍑芥暟鍚嶇О     : searchleftmapPoint
* 鎻忚堪        : 宸﹀浘鐗瑰緛鐐瑰鎵� RB(i), BR(i), TR(i)//BR鏄乏涓嬭竟绾跨殑搴曢儴锛孯B鏄乏涓嬭竟绾跨殑椤堕儴
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
#define searchleftmapPoint(x, y)                                                                \
{                                                                                               \
    if (x > RB_X(II.num_lm)&&y < (RB_Y(II.num_lm)+20))                                          \
    {         /*濡傛灉宸︾嚎鍚戝乏閫掕繘锛屽垯涓嶆柇鏇存柊RB鍜孯T  */                                                     \
    RB_X(II.num_lm) = x;                                                                        \
    RB_Y(II.num_lm) = y;                                                                        \
    RT_X(II.num_lm) = x;                                                                        \
    RT_Y(II.num_lm) = y;                                                                        \
    }                                                                                            \
    else if (x == RB_X(II.num_lm)&&y < (RB_Y(II.num_lm)+20))  /*濡傛灉鍒楁暟娌℃湁鏀瑰彉锛屽垯琛屾暟鍑忓皬鏃舵洿鏂癛B锛岃鏁板澶ф椂鏇存柊RT*/          \
{                                                           \
    if (y < RB_Y(II.num_lm))                                \
{                                                           \
    RB_Y(II.num_lm) = y;                                    \
    }                                                       \
    else if (y > RT_Y(II.num_lm))                           \
{                                                           \
    RT_Y(II.num_lm) = y;                                    \
    }                                                       \
    }                                                       \
    if (y < BR_Y(II.num_lm))    /*濡傛灉杩樻湁杈圭嚎鍦˙R涓嬫柟鍒欐洿鏂癇R*/    \
{                                                           \
    BR_X(II.num_lm) = x;                                    \
    BR_Y(II.num_lm) = y;                                    \
    }                                                       \
    else if (y == BR_Y(II.num_lm) && x > BR_X(II.num_lm))   \
{                                                           \
    BR_X(II.num_lm) = x;                                    \
    }                                                       \
    if (y > TR_Y(II.num_lm))     /*濡傛灉鏈夊湪TR涓婃柟鐨勭偣鍒欐洿鏂癟R  */      \
{                                                           \
    TR_X(II.num_lm) = x;                                    \
    TR_Y(II.num_lm) = y;                                    \
    }                                                           \
    else if (y == TR_Y(II.num_lm) && x > TR_X(II.num_lm))       \
{                                                           \
    TR_X(II.num_lm) = x;                                    \
    }                                                       \
                                                             \
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : searchleftPoint
* 鎻忚堪        : 鍔熻兘涓巗earchleftmapPoint绫讳技锛屽尯鍩熶俊鎭噰闆�
* 杩涘叆鍙傛暟     : x, y, r值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
#define searchleftPoint(x, y, r)                                \
{                                                               \
    if (x > r->right_bottom.x)                                  \
{                                                           \
    r->right_bottom.x = x;                                  \
    r->right_bottom.y = y;                                  \
    r->right_top.x = x;                                     \
    r->right_top.y = y;                                     \
    }                                                           \
    else if (x == r->right_bottom.x)                            \
{                                                           \
    if (y < r->right_bottom.y)                              \
{                                                       \
    r->right_bottom.y = y;                              \
    }                                                       \
    else if (y > r->right_top.y)                            \
{                                                       \
    r->right_top.y = y;                                 \
    }                                                       \
    }                                                           \
    if (y < r->bottom_right.y)                                  \
{                                                           \
    r->bottom_right.x = x;                                  \
    r->bottom_right.y = y;                                  \
    }                                                           \
    else if (y == r->bottom_right.y && x > r->bottom_right.x)   \
{                                                           \
    r->bottom_right.x = x;                                  \
    }                                                           \
    if (y > r->top_right.y)                                     \
{                                                           \
    r->top_right.x = x;                                     \
    r->top_right.y = y;                                     \
    }                                                           \
    else if (y == r->top_right.y && x > r->top_right.x)         \
{                                                           \
    r->top_right.x = x;                                     \
    }                                                           \
    }



/******************************************************************************
* 鍑芥暟鍚嶇О     : searchrightmapPoint
* 鎻忚堪        : //鍙冲浘鐗瑰緛鐐瑰鎵撅紝 LB(i), BL(i), TL(i)锛孡T(i)
* 杩涘叆鍙傛暟     : x, y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
#define searchrightmapPoint(x, y)                               \
{                                                               \
    if (x < LB_X(II.num_rm)&&y < (LB_Y(II.num_rm)+20))            /*2022-1-1鍔狅紝鍥犱笁宀斿弶澶凩B涓嶭T鏇存柊娣蜂贡*/         \
{                                                          \
    LB_X(II.num_rm) = x;                                    \
    LB_Y(II.num_rm) = y;                                    \
    LT_X(II.num_rm) = x;                                    \
    LT_Y(II.num_rm) = y;                                    \
    }                                                           \
    else if (x == LB_X(II.num_rm)&&y < (LB_Y(II.num_rm)+20))                              \
{                                                           \
    if (y < LB_Y(II.num_rm))                                \
{                                                       \
    LB_Y(II.num_rm) = y;                                \
    }                                                       \
    else if (y > LT_Y(II.num_rm))                           \
{                                                       \
    LT_Y(II.num_rm) = y;                                \
    }                                                       \
    }                                                           \
    if (y < BL_Y(II.num_rm))                                    \
{                                                           \
    BL_X(II.num_rm) = x;                                    \
    BL_Y(II.num_rm) = y;                                    \
    }                                                           \
    else if (y == BL_Y(II.num_rm) && x < BL_X(II.num_rm))       \
{                                                           \
    BL_X(II.num_rm) = x;                                    \
    }                                                           \
    if (y > TL_Y(II.num_rm))                                    \
{                                                           \
    TL_X(II.num_rm) = x;                                    \
    TL_Y(II.num_rm) = y;                                    \
    }                                                           \
    else if (y == TL_Y(II.num_rm) && x < TL_X(II.num_rm))       \
{                                                           \
    TL_X(II.num_rm) = x;                                    \
    }                                                           \
    }
//鐢辩浜屼釜榛戣壊鍖哄煙寮�濮嬫壘锛屽鏋滀笂鏂硅繕鏈夌櫧鑹茬偣璇存槑鏄竴涓嫭绔嬪尯鍩燂紝鍗宠繑鍥�1




/******************************************************************************
* 鍑芥暟鍚嶇О     : searchrightPoint
* 鎻忚堪        : 鍔熻兘涓巗earchrightmapPoint绫讳技锛屽尯鍩熶俊鎭噰闆�
* 杩涘叆鍙傛暟     : x, y, r值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
#define searchrightPoint(x, y, r)                               \
{                                                               \
    if (x < r->left_bottom.x)                                   \
{                                                           \
    r->left_bottom.x = x;                                   \
    r->left_bottom.y = y;                                   \
    r->left_top.x = x;                                      \
    r->left_top.y = y;                                      \
    }                                                           \
    else if (x == r->left_bottom.x)                             \
{                                                           \
    if (y < r->left_bottom.y)                               \
{                                                       \
    r->left_bottom.y = y;                               \
    }                                                       \
    else if (y > r->left_top.y)                             \
{                                                       \
    r->left_top.y = y;                                  \
    }                                                       \
    }                                                           \
    if (y < r->bottom_left.y)                                   \
{                                                           \
    r->bottom_left.x = x;                                   \
    r->bottom_left.y = y;                                   \
    }                                                           \
    else if (y == r->bottom_left.y && x < r->bottom_left.x)     \
{                                                           \
    r->bottom_left.x = x;                                   \
    }                                                           \
    if (y > r->top_left.y)                                      \
{                                                           \
    r->top_left.x = x;                                      \
    r->top_left.y = y;                                      \
    }                                                           \
    else if (y == r->top_left.y && x < r->top_left.x)           \
{                                                           \
    r->top_left.x = x;                                      \
    }                                                           \
    }
uint8 isSearch(uint8 x, uint8 y, uint8 map[][XM])//从下往上找第一个basemap是黑但map是白的点，找不到返回0找得到返回1
{
    for (uint8 k = y + 1; k < YM; ++k)
    {
        if (!basemap[k][x])//basemap涓虹櫧鐐规椂杩涘叆if锛屾壘鍒癰asemap涓竴涓櫧鑹茬殑鐐归��鍑哄惊鐜紝鍗虫壘杈圭晫
        {
            break;
        }
        if (map[k][x])//濡傛灉base涓槸榛戯紝map涓篃鏄粦鍒欒繑鍥�0
        {
            return 0;
        }
    }
    return 1;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : searchimg
* 鎻忚堪        : 閫氳繃allmap灏哹asemap涓殑鐧界偣鎻忕粯鍑烘潵锛屽苟灏哸llmap涓櫧榛戝垎鐣岀偣鐢ㄧ孩鑹叉弿缁樺嚭鏉�
* 杩涘叆鍙傛暟     : uint8 x, uint8 y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void searchimg(uint8 x, uint8 y)
{
    Point a;
    a.x = x; a.y = y;
    stack s;
    Point data[MAXSIZE]; //MAXSIZE閫掑綊娣卞害
    InitStack(&s,MAXSIZE,data);
    PushStack(&s,x,y);
    while(!EmptyStack(&s))//绌烘爤杩斿洖1锛屽嵆涓嶆槸绌烘爤鐨勮瘽杩涘叆while
    {
        Point a;
        a = s.data[s.top];
        if(!allmap[a.y][a.x])//濡傛灉allmap涓繖涓偣鏄櫧鐐�
        {
            if(basemap[a.y][a.x]==1)//濡傛灉basemap閲岃繖涓偣娌℃悳杩�  //骞朵笖basemap涓繖涓偣鏄粦鑹�
            {
                ++II.bnum_all;//basemap榛戠偣鏁板姞涓�
                insidemap[a.y][a.x] = 0;
                basemap[a.y][a.x] = 0;
                if(II.top < a.y)
                {
                II.top = a.y;
                }
                if(a.y<II.top-30 && IF.ramp)
                {
                    return;
                }
            }//涓婁釜鐐�  涓婁笅宸﹀彸鍝噷鏈夐粦鐐瑰摢閲屽叆鏍�
            if (a.x > 0 && basemap[a.y][a.x - 1] == 1)//宸�
                PushStack(&s,a.x-1,a.y);
            else if (a.x < XX && basemap[a.y][a.x + 1] == 1)//鍙�
                PushStack(&s,a.x+1,a.y);
            else if (a.y > 0 && basemap[a.y - 1][a.x] == 1)//涓�
                PushStack(&s,a.x,a.y-1);
            else if (a.y < YY && basemap[a.y + 1][a.x] == 1)//涓�
                PushStack(&s,a.x,a.y+1);

            else PopStack(&s);
        }
        else
        {
            basemap[a.y][a.x] = 2;  //绾㈣壊杈圭晫
            PopStack(&s);

        }

    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : searchleftmap
* 鎻忚堪        : 鎵惧嚭宸﹀浘鐨勫舰鐘�
* 杩涘叆鍙傛暟     : uint8 x, uint8 y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void searchleftmap(uint8 x, uint8 y)
{
    Point a;
    a.x = x;
    a.y = y;
    stack s;
    Point data[MAXSIZE];
    InitStack(&s,MAXSIZE,data);
    if (basemap[a.y][a.x])PushStack(&s,x,y);
    else leftmap[y][x] = 2;
    while(!EmptyStack(&s))
    {
        Point a;
        a = s.data[s.top];
        if(basemap[a.y][a.x])//濡傛灉鏄粦鐐�
        {
            if(leftmap[a.y][a.x] == 0)//濡傛灉leftmap涓繖涓偣娌¤閬嶅巻杩囷紝鍥犱负鍒濆鍏ㄧ櫧锛屼絾鏄細缁忚繃澶勭悊
            {
                searchleftmapPoint(a.x, a.y);   //鍖哄煙淇℃伅鎼滅储
                leftmap[a.y][a.x] = 1;  //leftmap琚亶鍘嗕簡
                insidemap[a.y][a.x] = 0;//inside鍒濆涓�1榛戣壊锛岄�氳繃杩欎竴姝ュ皢閬嶅巻鍒扮殑鐐圭疆0鍙樼櫧鑹诧紝鍗砳nsidemap涓病鏈夎閬嶅巻鍒扮殑鐐逛负榛戣壊 //insidemap琚亶鍘嗕簡
                if(II._top < a.y)
                {
                    II._top = a.y;
                }
                if(rightmap[a.y][a.x])
                {
                    ++II.repeatNum;//宸﹀彸涓ゅ浘涓繖涓偣鍧囦负榛戣壊鍒欏垽瀹氳繖涓偣涓洪噸澶�
                }
                ++II.lnum[II.num_lm];
            }

            if (a.x > 0 && !leftmap[a.y][a.x - 1])//濡傛灉宸﹁竟鏈変釜鐧界偣锛堝嵆宸﹁竟杩樻湁涓偣娌￠亶鍘嗗埌锛夛紝杩涙爤
                PushStack(&s,a.x-1,a.y);
            else if (a.x < XX && !leftmap[a.y][a.x + 1])
                PushStack(&s,a.x+1,a.y);
            else if (a.y < YY && !leftmap[a.y + 1][a.x])
                PushStack(&s,a.x,a.y+1);
            else if (a.y > 0 && !leftmap[a.y - 1][a.x])
            {
                if(a.y <= II.top)
                    PushStack(&s,a.x,a.y-1);
                else if(basemap[a.y - 1][a.x] && noDown.num < XM)
                {  //闃叉宸﹀彸鍖哄煙杩為�氾紝濡傛灉瓒呰繃浜哹asemap鐨勬渶楂樿鏁板氨绂佹涓嬪
                    noDown.point[noDown.num].x = a.x;
                    noDown.point[noDown.num].y = a.y - 1;
                    ++noDown.num;
                    PopStack(&s);
                }
                else if(!basemap[a.y - 1][a.x])//濡傛灉basemap涓繖涓偣涓虹櫧鑹�
                {
                    leftmap[a.y - 1][a.x] = 2;
                }
                else
                {
                    PopStack(&s);
                }
            }

            else PopStack(&s);
        }
        else
        {
            leftmap[a.y][a.x] = 2;
            PopStack(&s);

        }

    }

}



/******************************************************************************
* 鍑芥暟鍚嶇О     : searchrightmap
* 鎻忚堪        : 鎵惧嚭鍙冲浘鐨勫舰鐘�
* 杩涘叆鍙傛暟     : uint8 x, uint8 y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void searchrightmap(uint8 x, uint8 y)
{
    Point a;
    a.x = x;
    a.y = y;
    stack s;
    Point data[MAXSIZE];
    InitStack(&s,MAXSIZE,data);
    if (basemap[a.y][a.x])PushStack(&s,x,y);
    else rightmap[y][x] = 2;
    while(!EmptyStack(&s))
    {
        Point a;
        a = s.data[s.top];
        if(basemap[a.y][a.x])
        {
            if(rightmap[a.y][a.x] == 0)
            {
                searchrightmapPoint(a.x, a.y);
                rightmap[a.y][a.x] = 1;
                insidemap[a.y][a.x] = 0;
                if(II._top < a.y )
                {
                    II._top = a.y;
                }
                if(leftmap[a.y][a.x])
                {
                    ++II.repeatNum;
                }
                ++II.rnum[II.num_rm];
            }
            if (a.x > 0 && !rightmap[a.y][a.x - 1])
                PushStack(&s,a.x-1,a.y);
            else if (a.x < XX && !rightmap[a.y][a.x + 1])
                PushStack(&s,a.x+1,a.y);
            else if (a.y < YY && !rightmap[a.y + 1][a.x])
                PushStack(&s,a.x,a.y+1);
            else if (a.y > 0 && !rightmap[a.y - 1][a.x])
            {
                if(a.y <= II.top)
                    PushStack(&s,a.x,a.y-1);
                else if(basemap[a.y - 1][a.x] && noDown.num < XM)
                {
                    noDown.point[noDown.num].x = a.x;//閫愭笎鍚戜笂鐨勬儏鍐典笉浼氳璁板綍锛屽洖鐜細琚褰�
                    noDown.point[noDown.num].y = a.y - 1;
                    ++noDown.num;
                    PopStack(&s);
                }
                else if(!basemap[a.y - 1][a.x])
                {
                    rightmap[a.y-1][a.x] = 2;
                }
                else
                {
                    PopStack(&s);
                }
            }
            else
            {
                PopStack(&s);
            }
        }
        else
        {
            rightmap[a.y][a.x] = 2;
            PopStack(&s);
        }
    }

}



/******************************************************************************
* 鍑芥暟鍚嶇О     : searchdeletemap
* 鎻忚堪        : 鎵惧嚭闄ゅ幓宸﹀彸鍥剧殑鍒犻櫎鍥剧殑褰㈢姸
* 杩涘叆鍙傛暟     : uint8 x, uint8 y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void searchdeletemap(uint8 x, uint8 y)
{
    Point a;
    a.x = x;
    a.y = y;
    stack s;
    Point data[MAXSIZE];
    InitStack(&s,MAXSIZE,data);
    PushStack(&s,x,y);
    while(!EmptyStack(&s))
    {
        Point a;
        a = s.data[s.top];
        if(basemap[a.y][a.x])
        {
            if(deletemap[a.y][a.x]==0)
            {
                ++II.dnum_all;
                deletemap[a.y][a.x] = 1;
                insidemap[a.y][a.x] = 0;
                if(II.d_bottom.y > a.y)
                {  //瀵绘壘deletemap瑙掔偣
                    II.d_bottom.y = a.y;
                    II.d_bottom.x = a.x;
                }
            }
            if (a.x > 0 && deletemap[a.y][a.x - 1] == 0)
                PushStack(&s,a.x-1,a.y);
            else if (a.x < XX && deletemap[a.y][a.x + 1] == 0)
                PushStack(&s,a.x+1,a.y);
            else if (a.y > 0 && deletemap[a.y - 1][a.x] == 0)
                PushStack(&s,a.x,a.y-1);
            else if (a.y < YY && deletemap[a.y + 1][a.x] == 0)
                PushStack(&s,a.x,a.y+1);

            else PopStack(&s);
        }
        else
        {
            deletemap[a.y][a.x] = 2;
            PopStack(&s);

        }

    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : searchdeletemap2
* 鎻忚堪        : 棰濆鎵惧嚭闄ゅ幓宸﹀彸鍥剧殑鍒犻櫎鍥剧殑褰㈢姸
* 杩涘叆鍙傛暟     : uint8 x, uint8 y值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void searchdeletemap2(uint8 x, uint8 y)
{
    Point a;
    a.x = x;
    a.y = y;
    stack s;
    Point data[MAXSIZE];
    InitStack(&s,MAXSIZE,data);
    PushStack(&s,x,y);
    while(!EmptyStack(&s))
    {
        Point a;
        a = s.data[s.top];
        if(basemap[a.y][a.x])//濡傛灉basemap杩欓噷鏄粦鐐�
        {
            if(deletemap[a.y][a.x]==0)//濡傛灉deletemap娌℃湁琚亶鍘嗚繃
            {
                ++II.dnum_all;
                ++II.dnum_top;
                deletemap[a.y][a.x] = 254;
                insidemap[a.y][a.x] = 0;    //insidemap琚亶鍘嗚繃浜�   //insidemap鍙樻垚榛戣壊
                //                if(II.d_bottom.y > a.y)
                //                {  //瀵绘壘deletemap瑙掔偣
                //                    II.d_bottom.y = a.y;
                //                    II.d_bottom.x = a.x;
                //                }
            }
            if (a.x > 0 && !deletemap[a.y][a.x - 1] && leftmap[a.y][a.x - 1] != 1 &&
                    rightmap[a.y][a.x - 1] != 1)
                PushStack(&s,a.x-1,a.y);
            else if (a.x < XX && !deletemap[a.y][a.x + 1] && leftmap[a.y][a.x + 1] != 1 &&
                     rightmap[a.y][a.x + 1] != 1)
                PushStack(&s,a.x+1,a.y);
            else if (a.y < YY && !deletemap[a.y + 1][a.x] && leftmap[a.y + 1][a.x] != 1 &&
                     rightmap[a.y + 1][a.x] != 1)
                PushStack(&s,a.x,a.y+1);
            else if (a.y > 0 &&!deletemap[a.y - 1][a.x] && leftmap[a.y - 1][a.x] != 1 &&
                     rightmap[a.y - 1][a.x] != 1)
                PushStack(&s,a.x,a.y-1);

            else {
                PopStack(&s);
            }
        }
        else
        {
            deletemap[a.y][a.x] = 253;
            PopStack(&s);

        }
    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : searchCountmap
* 鎻忚堪        : 鏃�
* 杩涘叆鍙傛暟     : uint8 x, uint8 y, uint8 src[][XM]值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void searchCountmap(uint8 x, uint8 y, uint8 src[][XM])
{
    Point a;
    a.x = x;
    a.y = y;
    stack s;
    Point data[MAXSIZE];
    InitStack(&s,MAXSIZE,data);
    PushStack(&s,x,y);
    while(!EmptyStack(&s))
    {
        Point a;
        a = s.data[s.top];
        if(basemap[a.y][a.x])//basemap鏄櫧鐐�
        {
            if(src[a.y][a.x]==0)//骞朵笖琚悳绱㈢殑鍦板浘涔熸槸鐧借壊
            {
                ++numCnt;
                src[a.y][a.x] = 1;
            }
            if (a.x > 0 && !src[a.y][a.x - 1])
                PushStack(&s,a.x-1,a.y);
            else if (a.x < XX && !src[a.y][a.x + 1])
                PushStack(&s,a.x+1,a.y);
            else if (a.y > 0 && !src[a.y - 1][a.x])
                PushStack(&s,a.x,a.y-1);
            else if (a.y < YY && !src[a.y + 1][a.x])
                PushStack(&s,a.x,a.y+1);

            else PopStack(&s);
        }
        else
        {
            src[a.y][a.x] = 2;
            PopStack(&s);

        }

    }

}



/******************************************************************************
* 鍑芥暟鍚嶇О     : ResearchDeletemap
* 鎻忚堪        : 閲嶆柊鎼渄eletemap
* 杩涘叆鍙傛暟     : uint8 x, uint8 y, uint8 src[][XM]值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void ResearchDeletemap(uint8 x, uint8 y, uint8 src[][XM])
{
    Point a;
    a.x = x;
    a.y = y;
    stack s;
    Point data[MAXSIZE];
    InitStack(&s,MAXSIZE,data);
    PushStack(&s,x,y);
    while(!EmptyStack(&s))
    {
        Point a;
        a = s.data[s.top];
        if(basemap[a.y][a.x])
        {
            if(src[a.y][a.x]==0)
            {
                ++numCnt;
                src[a.y][a.x] = 1;
                if(II.d_bottom.y > a.y)
                {  //瀵绘壘deletemap瑙掔偣
                    II.d_bottom.y = a.y;
                    II.d_bottom.x = a.x;
                }
            }
            if (a.x > 0 && !src[a.y][a.x - 1])
            {
                PushStack(&s,a.x-1,a.y);
                }
            else if (a.x < XX && !src[a.y][a.x + 1])
            {
                PushStack(&s,a.x+1,a.y);
                }
            else if (a.y > 0 && !src[a.y - 1][a.x])
            {
                PushStack(&s,a.x,a.y-1);
                }
            else if (a.y < YY && !src[a.y + 1][a.x])
            {
                PushStack(&s,a.x,a.y+1);
                }

            else {PopStack(&s);}
        }
        else
        {
            src[a.y][a.x] = 2;
            PopStack(&s);

        }
    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : searchmap
* 鎻忚堪        : 鎼滃嚭src鐨刴ap
* 杩涘叆鍙傛暟     : uint8 x, uint8 y, uint8 src[][XM]值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void searchmap(uint8 x, uint8 y, uint8 src[][XM])
{
    Point a;
    a.x = x;
    a.y = y;
    stack s;
    Point data[MAXSIZE];
    InitStack(&s,MAXSIZE,data);
    PushStack(&s,x,y);
    while(!EmptyStack(&s))
    {
        Point a;
        a = s.data[s.top];
        if(basemap[a.y][a.x])
        {
            if(src[a.y][a.x]==0)
            {
                src[a.y][a.x] = 1;
            }
            if (a.x > 0 && !src[a.y][a.x - 1])
                PushStack(&s,a.x-1,a.y);
            else if (a.x < XX && !src[a.y][a.x + 1])
                PushStack(&s,a.x+1,a.y);
            else if (a.y > 0 && !src[a.y - 1][a.x])
                PushStack(&s,a.x,a.y-1);
            else if (a.y < YY && !src[a.y + 1][a.x])
                PushStack(&s,a.x,a.y+1);

            else PopStack(&s);
        }
        else
        {
            src[a.y][a.x] = 2;
            PopStack(&s);

        }

    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : cntMap
* 鎻忚堪        : 鏃�
* 杩涘叆鍙傛暟     : uint8 x, uint8 y值
* 杩斿洖鍙傛暟     : numCnt
******************************************************************************/
uint16_t cntMap(uint8 x, uint8 y)
{
    numCnt = 0;
    uint8 map[YM][XM] = {0};
    searchCountmap(x, y, map);
    return numCnt;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : cntDeleteMap
* 鎻忚堪        : 鏃�
* 杩涘叆鍙傛暟     : uint8 x, uint8 y, uint8 map[][XM]值
* 杩斿洖鍙傛暟     : numCnt
******************************************************************************/
uint16_t cntDeleteMap(uint8 x, uint8 y, uint8 map[][XM])
{
    numCnt = 0;
    ResearchDeletemap(x, y, map);
    return numCnt;
}



uint8_t getMapYMin_Col(uint8_t x, uint8_t map[][XM], uint8_t value)//第x列从下往上，如果map[i][x]==value,就返回行数
{
    for (uint8_t i = 0; i < YM; ++i)
    {
        if (map[i][x] == value)
        {
            return i;
        }
    }
    return YM;
}

uint8_t getMapYMax_Col(uint8_t x, uint8_t map[][XM], uint8_t value)//第x列从上往下，如果map[i][x]==value,就返回行数
{
    for (uint8_t i = 0; i < YM; ++i)
    {
        if (map[YY - i][x] == value)
        {
            return YY - i;
        }
    }
    return YM;
}
uint8_t getMapYMin_Col2(uint8_t x, uint8_t y, uint8_t map[][XM])//x列第i行从下往上，如果map[i][x]是白点返回最大值，如果是边界点则返回边界坐标
{
    for (uint8_t i = y; i < YM; ++i)
    {
        if (map[i][x] == 1)
            break;
        if (map[i][x] == 2)
            return i;
    }
    return YM;
}

uint8_t getMapXMax_Row(uint8_t y, uint8_t map[][XM], uint8_t value)//输入y行数，从左向右返回第一次在该行map值为value的列数
{
    for (uint8_t i = 0; i < XM; ++i)
    {
        if (map[y][XX - i] == value)
        {
            return XX - i;
        }
    }
    return XM;
}

uint8_t getMapXMin_Row(uint8_t y, uint8_t map[][XM], uint8_t value)//在y行从左往右，获取满足条件值的最小列数
{
    for (uint8_t i = 0; i < XM; ++i)
    {
        if (map[y][i] == value)
        {
            return i;
        }
    }
    return XM;
}


/******************************************************************************
* 鍑芥暟鍚嶇О     : deleteforleft
* 鎻忚堪        :
* 杩涘叆鍙傛暟     : uint8 undelete_flag值
* 杩斿洖鍙傛暟     :
******************************************************************************/
uint8 deleteforleft(uint8 undelete_flag)
{
    if (deleteforleftflag)
    {
        return 0;
    }
    deleteforleftflag = 1;
    uint8 right = 0;
    for (uint8 i = 0; i < II.num_lm; ++i)
    {
        if (RIGHT(i) > right)
        {
            right = RIGHT(i);
        }
    }
    for (uint8 i = 0; i <= right; ++i)
    {
        for (uint8 j = 0; j <= II.top; ++j)
        {
            if (leftmap[j][i] == 1)
            {
                for (uint8 k = j + 1; k <= II.top; ++k)
                {
                    if (leftmap[k][i] == 2)
                    {
                        if (!undelete_flag && II.right_y > k + 5)
                        {
                            break;
                        }
                        for (uint8 m = k; m <= II.top; ++m)
                        {
                            if (leftmap[m][i] == 0)
                            {
                                for (uint8 n = m + 1; n <= II.top; ++n)
                                {
                                    if (leftmap[n][i] == 1)
                                    {
                                        for (uint8 a = n + 1; a <= II.top; ++a)
                                        {
                                            if (leftmap[a][i] == 2)
                                            {
                                                leftmap[a][i] = 0;
                                            }
                                        }
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    return 1;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : crossroadforright
* 鎻忚堪        : 鍙冲叆鍗佸瓧
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     :
******************************************************************************/
uint8 crossroadforright(void)
{
    if (!IF.annulus && II._num_lm + II._num_rm == 0)
    {
        for (uint8 j = II.step; j < STOP_LINE; ++j)
        {
            if (basemap[j][XX])
            {
                if (!leftmap[j][XX] && !rightmap[j][XX] &&
                        isSearch(XX, j, leftmap))
                {
                    searchrightmap(XX, j);
                    II.rnum_all += II.rnum[II.num_rm];
                    II.start_rm[II.num_rm++] = j;
                }
                break;
            }
        }
        if (!II.num_rm)
        {
            return 0;
        }
    }

    uint8 width = 3;

    uint8 undelete_flag = 0;
    uint8 leftX = 255;
    uint8 leftTop = 255;
    uint8 leftBottom = 255;

    uint8 rightX = 255;
    uint8 rightTop = 255;
    uint8 rightBottom = 255;
    for (uint8 i = 0, flag = 0; i < XM; ++i)
    {
        for (uint8 j = 0; j <= II.top; ++j)
        {
            if (rightmap[j][i] == 1)
            {
                leftBottom = j;
                for (uint8 k = j + 1; k <= II.top; ++k)
                {
                    if (rightmap[k][i] != 1)
                    {
                        leftX = i;
                        leftTop = k - 1;
                        flag = 1;
                        break;
                    }
                }
                break;
            }
        }
        if (flag == 1)
        {
            break;
        }
    }
    uint16_t pointnum = 0;
    uint16_t mynum = 0;
    if (leftX == 255 || leftTop == 255 || leftBottom == 255 ||
            leftTop > leftBottom + 15)
    {
        goto DELETE;
    }
    for (uint8 i = leftTop; i < YM; ++i)
    {
//        if (leftmap[i][leftX] == 1)
//        {
//            qout << 2;
//            goto DELETE;
//        }
        if (i > leftTop + (YM - leftTop) / 1.2 || undelete_flag)
        {
            break;
        }
    }
    if (IF.annulus == AR1)
    {
        return 0;
    }
    do
    {
        if (leftX + width > XX)
        {
            undelete_flag = 1;
            break;
        }
        rightX = leftX + width;
        for (uint8 j = 0; j <= II.top; ++j)
        {
            if (rightmap[j][rightX] == 1)
            {
                rightBottom = j;
                for (uint8 k = j + 1; k <= II.top; ++k)
                {
                    if (rightmap[k][rightX] != 1)
                    {
                        rightTop = k - 1;
                        break;
                    }
                }
                break;
            }
        }
        if (rightBottom == 255 || rightTop == 255 ||
                rightTop > rightBottom + 40)
        {
            goto DELETE;
        }
        for (uint8 j = rightBottom; j <= rightTop; ++j)
        {
            for (uint8 i = leftX; i <= rightX; ++i)
            {
                if (rightmap[j][i] == 1)
                {
                    ++pointnum;
                }
            }
        }
        mynum = (rightTop - rightBottom + 1) * (rightX - leftX + 1) / 2;
        mynum += (rightTop - rightBottom + 1) * 1.2 + 5;
        if (mynum <= pointnum)
        {
            goto DELETE;
        }
        II.left_x = leftX;
        II.left_y = leftTop;
    } while (0);
    if (!IF.annulus && II._num_rm == 1 && II.start_rm[0] < 25 && TOP_RM(0) < 40 && getMapYMin_Col(23, basemap, 1) > 60)
    {
        for (uint8 j = 5; j < YM; ++j)//(YM - leftTop) * 3 / 4 + 3
        {
            if (j + leftTop > YY || leftX < 3)
            {
                break;
            }
            if (basemap[j + leftTop][leftX - 3])
            {
                if (leftmap[j + leftTop][leftX - 3] != 1 &&
                        rightmap[j + leftTop][leftX - 3] != 1 &&
                        !(II.start_lm[0] < 25 && TOP_LM(0) > j + leftTop &&
                          k2[TOP_LM(0)] > k2[TOP_RM(0)] + 30) &&
                        isSearch(leftX - 3, j + leftTop, leftmap))
                {
                    searchrightmap(leftX - 3, j + leftTop);
                    II.rnum_all += II.rnum[II.num_rm];
                    II.start_rm[II.num_rm++] = j + leftTop;
                }
                break;
            }
        }
    }
    deleteforright(undelete_flag);
    for (uint8 j = leftTop + 1; j <= TOP_RM(0) + 1 && j <= II.top; ++j)
    {
        for (int8_t i = XX; i >= leftX; --i)
        {
            if (rightmap[j][i] == 2)
            {
                rightmap[j][i] = 0;
            }
        }
    }
DELETE:
    if (!IF.ramp && !((IF.annulusDelay || IF.annulus == AL5 || IF.annulus == AR5) &&
                      II.num_lm + II.num_rm == 1))
    {
        for (uint8 n = 0; n < II.num_rm; ++n)
        {
            if (BOTTOM_RM(n) == 0)
            {
                continue;
            }
            Point bottom_left;
            bottom_left.x = BL_X(n);
            bottom_left.y = BL_Y(n) - 1;
            Point last[100];
            uint8 num = 0;
            if (rightmap[bottom_left.y][XX] == 2)
            {
                rightmap[bottom_left.y][XX] = 0;
                last[num].x = XX;
                last[num].y = bottom_left.y;
                ++num;
            }
            for (uint8 j = bottom_left.y; j < bottom_left.y + 10 && j < YM; ++j)
            {
                for (uint8 i = 0, flagRight = 0, flagLeft = 0; i < XM; ++i)
                {
                    if (!flagRight && bottom_left.x + i <= XX)
                    {
                        if (rightmap[j][bottom_left.x + i] == 2)
                        {
                            rightmap[j][bottom_left.x + i] = 0;
                        }
                    }
                    //                    else
                    //                    {
                    //                        flagRight = 1;
                    //                    }
                    if (!flagLeft && bottom_left.x >= i &&
                            rightmap[j][bottom_left.x - i])
                    {
                        if (rightmap[j][bottom_left.x - i] == 2)
                        {
                            rightmap[j][bottom_left.x - i] = 0;
                            if (rightmap[j][bottom_left.x - i - 1] == 0)
                            {
                                last[num].x = bottom_left.x - i;
                                last[num].y = j;
                                ++num;
                                //                                flagLeft = 1;
                            }
                        }
                    }
                    //                    else if (bottom_left.x + 1 == i)
                    //                    {
                    //                        flagLeft = 1;
                    //                    }
                    //                    if (flagRight && flagLeft)
                    //                    {
                    //                        break;
                    //                    }
                }
            }
            for (int i = num - 1; i >= 0; --i)
            {
                if ((last[i].y + 1 < YM &&
                     rightmap[last[i].y + 1][last[i].x] == 2) ||
                        (last[i].y + 1 < YM && last[i].x >= 1 &&
                         rightmap[last[i].y + 1][last[i].x - 1] == 2) ||
                        (last[i].x >= 1 &&
                         rightmap[last[i].y][last[i].x - 1] == 2) ||
                        (last[i].y + 1 < YM && last[i].x + 1 < XM &&
                         rightmap[last[i].y + 1][last[i].x + 1] == 2) ||
                        (last[i].x + 1 < XM &&
                         rightmap[last[i].y][last[i].x + 1] == 2) ||
                        (last[i].y >= 1 &&
                         rightmap[last[i].y - 1][last[i].x] == 2) ||
                        (last[i].y >= 1 && last[i].x + 1 < XM &&
                         rightmap[last[i].y - 1][last[i].x + 1] == 2) ||
                        (last[i].y >= 1 && last[i].x >= 1 &&
                         rightmap[last[i].y - 1][last[i].x - 1] == 2))
                {
                    //                    if ((last[i].x == 0 && last[i].y == 0)
                    //                    ||
                    //                        (last[i].x == XX && last[i].y ==
                    //                        0))
                    //                    {
                    //                        break;
                    //                    }
                    rightmap[last[i].y][last[i].x] = 2;
                }
            }
        }
    }
    return 0;
}




/******************************************************************************
* 鍑芥暟鍚嶇О     : crossroadforleft
* 鎻忚堪        : 宸﹀叆鍗佸瓧
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 crossroadforleft(void)//
{
//***********************************十字中的看不到下角时对于上角的寻找***************************************************
    if (!IF.annulus && !IF.obstacle && !IF.startline && II._num_lm + II._num_rm == 0) ////不是元素且leftmap与rightmap都没有区域
    {
        for (uint8 j = II.step; j < STOP_LINE; ++j)//step行到STOP_LINE的搜线
        {
            if (basemap[j][0])//第0列为黑点时
            {
                if (!leftmap[j][0] && !rightmap[j][0] &&//左右图的第零列都是白
                        isSearch(0, j, rightmap))  //判断条件从左往右判断，这样写可以减少计算量
                {
                    searchleftmap(0, j);
                    II.lnum_all += II.lnum[II.num_lm];
                    II.start_lm[II.num_lm++] = j;
                }
                break;
            }
        }
        if (!II.num_lm)//濡傛灉娌℃湁鎼滃埌鍖哄煙锛屽垯涓嶆槸鍗佸瓧锛屼笉闇�瑕佸垹绾挎搷浣�
        {
            return 0;
        }
    }
//***************************寻找下角点*************************************************
    uint8 width = 3;
    uint8 undelete_flag = 0;
    uint8 rightX = 255;//左角点的X坐标q
    uint8 rightTop = 255;//左角点的上边沿Y坐标q     角点可能在同一列有上下好几行，所以需要记录上下边沿
    uint8 rightBottom = 255;//左角点的下边沿Y坐标q
    uint8 leftX = 255;
    uint8 leftTop = 255;
    uint8 leftBottom = 255;
    for (uint8 i = 0, flag = 0; i < XM; ++i)//自右向左（因为后面是XX-i）
    {
        for (uint8 j = 0; j <= II.top; ++j)//自下向上
        {
            if (leftmap[j][XX - i] == 1)//遇到黑点，应该就是角点
            {
                rightBottom = j;//记录下角点的下边沿Y坐标
                for (uint8 k = j + 1; k <= II.top; ++k)//涓婄Щ涓�涓粦鐐癸紝缁х画鎼滃
                {
                    if (leftmap[k][XX - i] != 1)//杩涗竴姝ュ垽鏂紝濡傛灉涓婄Щ涓嶆槸1锛堥粦锛夛紝灏辨槸瑙掔偣涓婅竟娌�
                    {
                        rightX = XX - i;//记录角点的X坐标
                        rightTop = k - 1;//记录角点的上边沿Y坐标
                        flag = 1;//只有搜到下角点，才会置1，否则为0
                        break;
                    }
                }
                break;
            }
        }
        if (flag == 1)
        {
            break;
        }
    }
//根据下角点是否存在。判断现在是否有下边界，没有下边界的话则通过goto跳出多层for循环直接进行上边界的删线
    uint16_t pointnum = 0;//涓夎褰㈠尯鍩熷唴鐨勯粦鐐规暟
    uint16_t mynum = 0;//涓夎褰㈤潰绉�
    if (rightX == 255 || rightTop == 255 || rightBottom == 255 ||
            rightTop > rightBottom + 40)//如果满足这个条件，说明只有上边界，只要删上边界就行
    {
        goto DELETE;
    }
    for (uint8 i = rightTop; i < YM; ++i)
    {
//        if (rightmap[i][rightX] == 1)//搴旇鏄槻姝㈠集閬撹璇垽涓哄崄瀛楄�屽啓鐨勫垽鏂潯浠�
//        {
//            goto DELETE;
//        }
        if (i > rightTop + (YM - rightTop) / 1.2 || undelete_flag)
        {
            break;  //娌′粈涔堢敤
        }
    }
    if (IF.annulus == AL1)//鍦嗙幆鐘舵��1骞朵笖涓嶆槸鎻愭棭璇嗗埆鐨勬儏鍐典笉鍒犵嚎锛坆oom3娉ㄩ噴锛�
    {
        return 0;
    }
    do
    {
        if (rightX < width)
        {
            undelete_flag = 1;//缃�1浠ｈ〃涓嶅垹绾�
            break;
        }
        leftX = rightX - width;//涓嬫柟鎼滃绗竴涓粦鐐圭殑妯潗鏍�
        for (uint8 j = 0; j <= II.top; ++j)//浠庝笅寰�涓�
        {
            if (leftmap[j][leftX] == 1)//鎼滃榛戠偣
            {
                leftBottom = j;//璁板綍閬囧埌鐨勭涓�涓粦鐐圭殑Y鍧愭爣
                for (uint8 k = j + 1; k <= II.top; ++k)//缁х画鍚戜笂鎼滃
                {
                    if (leftmap[k][leftX] != 1)//濡傛灉鎼滃埌闈為粦鑹茬殑鐐�
                    {
                        leftTop = k - 1;//璁板綍绗竴涓潪榛戣壊鐐圭殑Y鍧愭爣
                        break;
                    }
                }
                break;
            }
        }
        if (leftBottom == 255 || leftTop == 255 || leftTop > leftBottom + 30)//锛堜笌涓婇潰鐨勭被浼硷級 //绉绘鐨勬椂鍊�30闇�瑕佽嚜宸卞ソ濂借皟涓�璋�
        {
            goto DELETE;  //鍙兘鏄崄瀛楀洖鐜儏鍐�
        }
        for (uint8 j = leftBottom; j <= leftTop; ++j)
        {
            for (uint8 i = leftX; i <= rightX; ++i)//涓や釜for寰幆鏋勬垚涓�涓笁瑙掑舰鍖哄煙
            {
                if (leftmap[j][i] == 1)
                {
                    ++pointnum;//鏁板嚭涓夎褰㈠尯鍩熷唴鐨勯粦鐐规暟
                }
            }
        }
        mynum = (leftTop - leftBottom + 1) * (rightX - leftX + 1) / 2;//璁＄畻涓夎褰㈢殑闈㈢Н
        mynum += (leftTop - leftBottom + 1) * 1.2 + 5;//鍔犱笂涓夎褰㈢殑宸﹁竟绾匡紝骞惰繘琛岄�傚綋鐨勬墿澶�
        if (mynum <= pointnum)//绁栦紶鍒ゆ柇锛屽闀夸篃涓嶇煡閬撲负浠�涔�
        {
            goto DELETE;
        }
        II.right_x = rightX;
        II.right_y = rightTop;//璁板綍涓嬪乏瑙掔偣鐨勬í绾靛潗鏍�
    } while (0);    //do while(0)鐨勫啓娉曟槸涓轰簡鍙互break


    /*以下是对于十字上部边线的处理*/
    if (!IF.annulus && II._num_lm == 1 && II.start_lm[0] < 25 && TOP_LM(0) < 40 && getMapYMin_Col(23, basemap, 1) > 60)
    {                                                                          //涓嶆槸鍏冪礌锛孴R鍦�40琛屼互涓嬶紝寮�濮嬭鍦�25琛屼互涓嬶紝涓棿涓�鍒楁渶浣庨粦鐐归珮浜�60琛�
        for (uint8 j = 5; j < YM; ++j)//涓嶣OOM3绫讳技
        {
            if (j + rightTop > YY || rightX + 3 > XX)//防止越界
            {
                break;
            }
            if (basemap[j + rightTop][rightX + 3])//宸︿笅瑙掔偣鍚戝彸涓夊垪
            {
                if (leftmap[j + rightTop][rightX + 3] != 1 &&
                        rightmap[j + rightTop][rightX + 3] != 1 &&
                        !(II.start_rm[0] < 25 && TOP_RM(0) > j + rightTop &&//鍙宠竟鐣岃捣鐐瑰湪25琛屼互涓婃垨TL鍦ㄨ鐐逛笂鏂�
                          k2[TOP_RM(0)] > k2[TOP_LM(0)] + 30) &&
                        isSearch(rightX + 3, j + rightTop, rightmap))//鍒ゆ柇鍙冲浘鏄惁鎼滆繃
                {
                    searchleftmap(rightX + 3, j + rightTop);//閲嶆柊瀵昏竟鐣岀嚎  杩囧渾鐜椂闃叉瀵荤嚎杩囬珮锛屾妸鍙︿竴鏉＄嚎鐨勮鐐规壘浜嗚繘鏉�
                    II.lnum_all += II.lnum[II.num_lm];
                    II.start_lm[II.num_lm++] = j + rightTop;
                }
                break;
            }//鎰熻鍘绘帀涔熸病浠�涔堝奖鍝嶏紝鍏蜂綋搴旂敤鐨勬椂鍊欏啀璋�
        }
    }
    deleteforleft(undelete_flag);//?????????杩欓噷杩涜浜嗗垹绾挎搷浣滐紝鍒犵殑鏄笂杈圭晫鐨勪竴閮ㄥ垎鍜屽叏閮ㄧ殑涓嬭竟鐣�
//下面这个for循环删掉了下半角点向外撇出去的部分
    for (uint8 j = rightTop + 1; j <= TOP_LM(0) + 1 && j <= II.top; ++j)
    {
        for (uint8 i = 0; i <= rightX; ++i)
        {
            if (leftmap[j][i] == 2)
            {
                leftmap[j][i] = 0;//鍒犵嚎锛屽垹闄ょ洿閬撲笂鐨勫皬寮洸
            }
        }
    }//杩欎竴鍧楀幓鎺夊ソ鍍忔病鏈夊奖鍝�
DELETE://删除上边界水平多余部分，以及只有上边界的情况下的上边界
    if (!IF.ramp && !((IF.annulusDelay || IF.annulus == AL5 || IF.annulus == AR5) &&//涓嶆槸鍧￠亾鍜屽嚭鍦嗙幆闃舵
                      II.num_lm + II.num_rm == 1))//宸﹀彸鍖哄煙鏁扮浉鍔犱笉绛変簬1
    {
        for (uint8 n = 0; n < II.num_lm; ++n)
        {
            if (BOTTOM_LM(n) == 0)//BR涓嶅湪0琛�
            {
                continue;
            }
            Point bottom_right;
            bottom_right.x = BR_X(n);
            bottom_right.y = BR_Y(n) - 1;
            Point last[200];
            uint8 num = 0;
            if (leftmap[bottom_right.y][0] == 2)
            {
                leftmap[bottom_right.y][0] = 0;
                last[num].x = 0;
                last[num].y = bottom_right.y;
                ++num;
            }//璁板綍涓媗ast[0]
            for (uint8 j = bottom_right.y; j < bottom_right.y + 10 && j < YM; ++j)//BR鍚戜笂10琛岋紙鏈変簤璁級
            {
                for (uint8 i = 0, flagLeft = 0, flagRight = 0; i < XM; ++i)//浠R_X涓鸿捣鐐瑰悜宸﹀垹绾�
                {
                    if (!flagLeft && bottom_right.x >= i)
                    {
                        if (leftmap[j][bottom_right.x - i] == 2)
                        {
                            leftmap[j][bottom_right.x - i] = 0;// 鍒犵嚎锛屾搷浣�
                        }
                    }
                    //                    else
                    //                    {
                    //                        flagLeft = 1;
                    //                    }
                    if (!flagRight && bottom_right.x + i <= XX &&
                            leftmap[j][bottom_right.x + i])//浠R_X涓鸿捣鐐瑰悜鍙冲垹绾�
                    {
                        if (leftmap[j][bottom_right.x + i] == 2)
                        {
                            leftmap[j][bottom_right.x + i] = 0;
                            if (leftmap[j][bottom_right.x + i + 1] == 0)//姣忚璺濈璧涢亾鏈�杩戠殑鐐癸紝璁板綍涓嬫潵
                            {
                                last[num].x = bottom_right.x + i;
                                last[num].y = j;
                                ++num;
                                //                                flagRight = 1;
                            }
                        }
                    }
                    //                    else if (bottom_right.x + i == XM)
                    //                    {
                    //                        flagRight = 1;
                    //                    }
                    //                    if (flagRight && flagLeft)
                    //                    {
                    //                        break;
                    //                    }
                }
            }
            for (int i = num - 1; i >= 0; --i)//鎭㈠澶氬垹鐨勫瀭鐩撮儴鍒嗙殑绾�
            {
                if ((last[i].y + 1 < YM &&
                     leftmap[last[i].y + 1][last[i].x] == 2) ||
                        (last[i].y + 1 < YM && last[i].x + 1 < XM &&
                         leftmap[last[i].y + 1][last[i].x + 1] == 2) ||
                        (last[i].x + 1 < XM &&
                         leftmap[last[i].y][last[i].x + 1] == 2) ||
                        (last[i].y + 1 < YM && last[i].x >= 1 &&
                         leftmap[last[i].y + 1][last[i].x - 1] == 2) ||
                        (last[i].x >= 1 &&
                         leftmap[last[i].y][last[i].x - 1] == 2) ||
                        (last[i].y >= 1 &&
                         leftmap[last[i].y - 1][last[i].x] == 2) ||
                        (last[i].y >= 1 && last[i].x + 1 < XM &&
                         leftmap[last[i].y - 1][last[i].x + 1] == 2) ||
                        (last[i].y >= 1 && last[i].x >= 1 &&
                         leftmap[last[i].y - 1][last[i].x - 1] == 2))
                {
                    //                    if ((last[i].x == 0 && last[i].y == 0)
                    //                    ||
                    //                        (last[i].x == XX && last[i].y ==
                    //                        0))
                    //                    {
                    //                        break;
                    //                    }
                    leftmap[last[i].y][last[i].x] = 2;
                }
            }
        }
    }
    return 0;
}




/******************************************************************************
* 鍑芥暟鍚嶇О     : deleteforright
* 鎻忚堪        :
* 杩涘叆鍙傛暟     : uint8 undelete_flag值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 deleteforright(uint8 undelete_flag)
{
    if (deleteforrightflag)
    {
        return 0;
    }
    deleteforrightflag = 1;
    uint8 left = XX;
    for (uint8 i = 0; i < II.num_rm; ++i)
    {
        if (left > LEFT(i))
        {
            left = LEFT(i);
        }
    }
    for (int8_t i = XX; i >= left; --i)
    {
        for (uint8 j = 0; j <= II.top; ++j)
        {
            if (rightmap[j][i] == 1)
            {
                for (uint8 k = j + 1; k <= II.top; ++k)
                {
                    if (rightmap[k][i] == 2)
                    {
                        if (!undelete_flag && II.left_y > k + 5)
                        {
                            break;
                        }
                        for (uint8 m = k; m <= II.top; ++m)
                        {
                            if (rightmap[m][i] == 0)
                            {
                                for (uint8 n = m + 1; n <= II.top; ++n)
                                {
                                    if (rightmap[n][i] == 1)
                                    {
                                        for (uint8 a = n + 1; a <= II.top; ++a)
                                        {
                                            if (rightmap[a][i] == 2)
                                            {
                                                rightmap[a][i] = 0;
                                            }
                                        }
                                        break;
                                    }
                                    else if (rightmap[n][i] == 2)
                                    {
                                        rightmap[n][i] = 0;
                                    }
                                }
                                break;
                            }
                            else if (rightmap[m][i] == 2)
                            {
                                rightmap[m][i] = 0;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    return 1;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : deleteline
* 鎻忚堪        : 鍒犵嚎
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void deleteline(void)//閸ョ偟骞嗛崚鐘靛殠
{
    if (IF.annulus == AL1 || IF.annulus == AR1)
    {
        return;
    }
#define DELETE_NUM 0
/*****************************************************向左拐的回环************************************************/
    uint8 down = YM;
    for (uint8 i = TOP_LM(0); i > 1; --i)//浠嶵R_Y寰�涓�
    {
        if (X_WBW_Detect(0, XX, i, leftmap, goRight))//搴旇鏄乏鍥炵幆//搴旇鏄乏杞�
            down = i;//鎵惧埌鍥炵幆搴曢儴  //goRight鐨勬剰鎬濇槸WBW鍒ゆ柇鐨勬椂鍊欎粠宸﹀線鍙冲垽鏂�
        else
            break;
    }
    uint8 minDown;//鍒嗕袱閮ㄥ垎鍒犵嚎锛宒own-mindown,mindown-up
    if (down > RT_Y(0) + 1)
    {
        minDown = down;
    }
    else
    {
        minDown = RT_Y(0) + 1;
        for (uint8 j = down; j < RT_Y(0) + 1 && j < YM; ++j)//绗竴閮ㄥ垎鍒犵嚎
        {
            for (uint8 i = 0; i < getMapXMax_Row(j, leftmap, 1) && i != XM; ++i)
            {
                if (leftmap[j][i] == 2)
                    leftmap[j][i] = 0;
            }
        }
    }
    uint8 up = TOP_LM(0) + 1 < YY ? TOP_LM(0) + 1 : YY;
    if (minDown != YM && up >= DELETE_NUM + minDown)//绗簩閮ㄥ垎鍒犵嚎
    {
        for (uint8 j = minDown; j + DELETE_NUM <= up; ++j)
        {
            for (uint8 i = 0; i <= getMapXMax_Row(j, leftmap, 2) && i < XM; ++i)
            {
                if (leftmap[j][i] == 2)
                    leftmap[j][i] = 0;
            }
        }
    }//鎰熻涓ら儴鍒嗗彲浠ュ悎骞舵垚涓�閮ㄥ垎锛岀浜岄儴鍒嗛�氳繃璋僁ELETE_NUM搴旇鏈夌壒娈婄殑浣滅敤,浣嗘槸濡傛灉鏄�0鐨勮瘽灏辨病浠�涔堝叧绯�
/*****************************************************向右拐的回环************************************************/
    down = YM;
    for (uint8 i = TOP_RM(0); i > 1; --i)
    {
        if (X_WBW_Detect(XX, 0, i, rightmap, goLeft))//搴旇鏄彸鍥炵幆
            down = i;//鍥炵幆鐨勫簳閮╕鍧愭爣  //goLeft鐨勬剰鎬濆簲璇ユ槸WBW鍒ゆ柇鐨勬椂鍊欎粠鍙冲線宸﹀垽鏂�
        else
            break;
    }
    if (down > LT_Y(0) + 1)
    {
        minDown = down;
    }
    else
    {
        minDown = LT_Y(0) + 1;
        for (uint8 j = down; j < LT_Y(0) + 1 && j < YM; ++j)
        {
            for (int8_t i = XX; i > getMapXMin_Row(j, rightmap, 1) && i != XM; --i)
            {
                if (rightmap[j][i] == 2)
                    rightmap[j][i] = 0;
            }//杩欓噷鍒犵殑鏄痬inDown涓嬮儴鐨勫洖鐜竟绾�
        }
    }
    up = TOP_RM(0) + 1 < YY ? TOP_RM(0) + 1 : YY;//鍥炵幆椤堕儴
    if (minDown != YM && up >= DELETE_NUM + minDown)
    {
        for (uint8 j = minDown; j + DELETE_NUM <= up; ++j)
        {
            for (uint8 i = getMapXMin_Row(j, rightmap, 2); i < XM; ++i)
            {
                if (rightmap[j][i] == 2)
                    rightmap[j][i] = 0;
            }//杩欓噷鍒犵殑鍒爉indown涓婇儴鐨勫洖鐜竟绾�
        }
    }
    if (II.num_lm == 0 && II.num_rm)
    {
        deleteforright(1);
    }
    else if (II.num_rm == 0 && II.num_lm)
    {
        deleteforleft(1);
    }//杩欎袱涓狪f鎰熻鏈夌偣澶氫綑锛屽墠闈㈢殑鍒犵嚎宸茬粡寰堝交搴曚簡
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : X_B2W_Num
* 鎻忚堪        : 鎵惧埌鐩稿簲y鑼冨洿鍐� 浠庨粦鍒扮櫧鐨勭偣鏁�
* 杩涘叆鍙傛暟     : int y1, int y2, uint8 map[][XM], uint8 dir值
* 杩斿洖鍙傛暟     : 榛戝埌鐧界殑鐐规暟閲�
******************************************************************************/
uint8 X_B2W_Num(int y1, int y2, uint8 map[][XM], uint8 dir)  //
{
  // uint8 flag=0;
  uint8 num = 0;
  if (dir == goRight)
    for (uint8 i = y1; i < y2; i++)
    {
      for (uint8 j = 0; j < 31; j++)
      {
        if (map[i][j] != 0 && map[i][j + 1] == 0)
        {
          num++;
          break;
        }
      }
    }

  else if (dir == goLeft)
    for (uint8 i = y1; i < y2; i++)
    {
      for (uint8 j = XX; j > 15; j--)
      {
        if (map[i][j] != 0 && map[i][j - 1] == 0)
        {
          num++;
          break;
        }
      }
    }
  return num;
}




/******************************************************************************
* 鍑芥暟鍚嶇О     : X_WBW_Detect
* 鎻忚堪        : 鏍规嵁flag妫�娴嬫寚瀹氳浠庡乏寰�鍙虫垨浠庡彸寰�宸︼紝鏄惁瀛樺湪榛戠櫧榛�(X_WBW)鎯呭喌锛寈1<x2
* 杩涘叆鍙傛暟     : int8_t x1, int8_t x2, uint8 y, uint8 map[][XM],uint8 flag值
* 杩斿洖鍙傛暟     : 杩欎竴琛屾槸鍚﹀瓨鍦ㄧ櫧榛戠櫧鎯呭喌
******************************************************************************/
uint8 X_WBW_Detect(int8_t x1, int8_t x2, uint8 y, uint8 map[][XM],
                     uint8 flag)//
{
    if (flag == goRight)
        for (int8_t j = x1; j <= x2; ++j)
        {
            if (map[y][j] != 1)
            {
                for (int8_t k = j + 1; k <= x2 && k < XM; ++k)
                {
                    if (map[y][k] == 1)
                    {
                        for (int8_t m = k + 1; m <= x2; ++m)
                        {
                            if (map[y][m] != 1)
                                return 1;//杩欎竴琛屼粠宸﹀線鍙崇櫧榛戠櫧杩斿洖涓�
                        }
                        break;
                    }
                }
                break;
            }
        }
    if (flag == goLeft)
        for (int8_t j = x1; j >= x2 && j >= 0; --j)
        {
            if (map[y][j] != 1)
            {
                for (int8_t k = j - 1; k >= x2; --k)
                {
                    if (map[y][k] == 1)
                    {
                        for (int8_t m = k - 1; m >= x2; --m)
                        {
                            if (map[y][m] != 1)
                                return 1;
                        }
                        break;
                    }
                }
                break;
            }
        }
    return 0;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : Y_WBW_Detect
* 鎻忚堪        : 鎸囧畾鍒椾粠涓嬪線涓妝1鍒皔2锛屾壘鍒版槸鍚﹀瓨鍦ㄧ櫧榛戠櫧鎯呭喌
* 杩涘叆鍙傛暟     : uint8 y1, uint8 y2, uint8 x, uint8 map[][XM]值
* 杩斿洖鍙傛暟     : 杩欎竴鍒椾粠涓嬪埌涓婃槸鍚﹀瓨鍦ㄧ櫧榛戠櫧鎯呭喌
******************************************************************************/
uint8 Y_WBW_Detect(uint8 y1, uint8 y2, uint8 x, uint8 map[][XM])//
{
    for (char i= y1; i <= y2; ++i)
    {
        if (map[i][x] == 0)
        {
            for (char k = i + 1; k <= y2; ++k)
            {
                if (map[k][x] == 1 || map[k][x] == 2)
                {
                    for (char m = k + 1; m <=y2; ++m)
                    {
                        if (map[m][x] == 0)
                        {
                            return 1;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
    return 0;
}




/******************************************************************************
* 鍑芥暟鍚嶇О     : Y_BWB_Detect
* 鎻忚堪        : 鎸囧畾鍒椾粠涓嬪線涓妝1鍒皔2锛屾壘鍒版槸鍚﹀瓨鍦ㄩ粦鐧介粦鎯呭喌
* 杩涘叆鍙傛暟     : uint8 y1, uint8 y2, uint8 x, uint8 map[][XM]值
* 杩斿洖鍙傛暟     : 杩欎竴鍒椾粠y1鍒皔2涓嬪線涓婃槸鍚﹀瓨鍦ㄩ粦鐧介粦鎯呭喌
******************************************************************************/
uint8 Y_BWB_Detect(uint8 y1, uint8 y2, uint8 x, uint8 map[][XM])
{
    for (char i= y1; i <= y2; ++i)
    {
        if (map[i][x] > 0)//榛戞垨鍏朵粬杈圭晫
        {
            for (char k = i + 1; k <= y2; ++k)
            {
                if (map[k][x] == 0)//鐧�
                {
                    for (char m = k + 1; m <=y2; ++m)
                    {
                        if (map[m][x] > 0)//榛戞垨鍏朵粬杈圭晫
                        {
                            return 1;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
    return 0;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : Get_insideMap
* 鎻忚堪        : 鑾峰彇insidemap
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void Get_insideMap(void)
{
    static uint8 Startline_delay=0,delay_flag=0;
    if (II.bnum_all + II.lnum_all + II.rnum_all == XM * YM + II.repeatNum)
    {  //濡傛灉宸﹀彸鍥剧殑榛戠偣鏁伴噺鍔犱笂basemap鐨勭櫧鐐规暟閲忕瓑浜庡浘鍍忔墍鏈夌偣鐨勬暟閲忓姞涓婂乏鍙冲浘閲嶅鐨勭偣鐨勬暟閲�
        //鍥犱负杩欓噷灏辩浉褰撲簬insidemao榛戠偣鏁颁负0锛屼笉瀛樺湪insidemap
        return;
    }
    for (int8_t i = 0; i < noDown.num ; ++i)
    {  //searchdeletemap2鎼滅殑鏄彇娑堜笅瀵昏�屽垹闄ょ殑鍖哄煙  缁胯壊
        if (leftmap[noDown.point[i].y][noDown.point[i].x] != 1 &&
                rightmap[noDown.point[i].y][noDown.point[i].x] != 1 &&
                !deletemap[noDown.point[i].y][noDown.point[i].x])
                 searchdeletemap2(noDown.point[i].x, noDown.point[i].y);
    }
    for (uint8 i = 0; i < YM; ++i)
    {  //searchdeletemap鎼滅殑鏄甯告悳鍥炬病鎼滃埌鐨勫尯鍩�
        if (basemap[i][0] && !leftmap[i][0] && !rightmap[i][0] &&
                !deletemap[i][0])
        {
            searchdeletemap(0, i);
        }
        if (basemap[i][XX] && !leftmap[i][XX] && !rightmap[i][XX] &&
                !deletemap[i][XX])
        {
            searchdeletemap(XX, i);
        }

    }
    for (uint8 i = 0; i < XM; ++i)
    {  //searchdeletemap鎼滅殑鏄甯告悳鍥炬病鎼滃埌鐨勫尯鍩�
        if (basemap[YY][i] && !leftmap[YY][i] && !rightmap[YY][i] &&
                !deletemap[YY][i])
        {
            searchdeletemap(i, YY);
        }
    }

}


/******************************************************************************
* 鍑芥暟鍚嶇О     : leftAnnulusDetect
* 鎻忚堪        : 鏄惁宸﹀叆鍦嗙幆鐨勬娴�
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 leftAnnulusDetect(void)
{
    uint8 up = 0;
    //    if (II.num_lm != 1 || II.num_rm != 1 || II.lnum[0] + II.rnum[0] < 250 ||
            //                    TOP_RM(0) < 50 || TOP_RM(0) < II.start_rm[0] + 25 || TOP_LM(0) < 20 ||
            //                    TOP_LM(0) > 55 || II.start_rm[0] > 35 || II.start_lm[0] > 25 ||
            //                    RIGHT(0) > 23 || BR_X(0) > 15 || k2[TOP_RM(0)] - k2[TOP_LM(0)] < 25 ||
            //                    II.top < 55 || II.speedTop < 50) // adjust II.speedTop//涓�绯诲垪鍒ゆ柇锛屽厛鐢ㄥ啀璇�
            if (II.num_lm == 0 || II.num_rm == 0 ||
                    II.lnum[0] + II.rnum[0] < 150 ||
                    TOP_RM(0) < 25 || TOP_RM(0) < II.start_rm[0] + 10||
                    TOP_LM(0) > 50 || TOP_LM(0) < 20 ||
                    II.start_rm[0] > 35 || II.start_lm[0] > 30||
                    RIGHT(0) > 20 || BR_X(0) > 10||//鏈夊紓璁�
                    II.top < 50 || II.speedTop < 40 ||
                    !II.lnum[0]) // adjust II.speedTop//涓�绯诲垪鍒ゆ柇锛屽厛鐢ㄥ啀璇绰仿�
        {
                qout<<"       begin              ";
                qout<<"II.num_lm == 0 "<<(II.num_lm == 0);
                qout<<"II.num_rm == 0"<<(II.num_rm == 0);
                qout<<"II.lnum[0] + II.rnum[0] < 150"<<(II.lnum[0] + II.rnum[0] < 150);
                qout<<"TOP_RM(0) < 25"<<(TOP_RM(0) < 25);
                qout<<"TOP_RM(0) < II.start_rm[0] + 10"<<(TOP_RM(0) < II.start_rm[0] + 10);
                qout<<"TOP_LM(0) < 20"<<(TOP_LM(0) < 20);
                qout<<"II.start_lm[0] > 35 "<<(II.start_lm[0] > 35);
                qout<<"II.start_rm[0] > 35 "<<(II.start_rm[0] > 35) ;
                qout<<"(BR_X(0) < 3 &&  II.lnum[0] > 180)"<<(BR_X(0) < 3 &&  II.lnum[0] > 180) ;
                qout<<"(BR_X(0) < 3)"<<(BR_X(0) < 3) ;
                qout<<"II.top < 50"<<(II.top < 50);
                qout<<"II.speedTop < 40 "<<(II.speedTop < 40);
                qout<<"II.top < 50"<<(II.top < 50);
                qout<<"!II.lnum[0]"<<(!II.lnum[0] );
                qout<<"II.dnum_all > 370 || II.dnum_all < 50"<<(II.dnum_all > 370 || II.dnum_all < 10);
                qout<<"getMapYMin_Col(15, basemap, 1)  >= 53"<<(getMapYMin_Col(15, basemap, 1)  >= 53);
                qout<<"        end                "<<(II.num_lm == 0 || II.num_rm == 0 ||
                                                      II.lnum[0] + II.rnum[0] < 150 ||
                                                      TOP_RM(0) < 25 || TOP_RM(0) < II.start_rm[0] + 10||
                                                      TOP_LM(0) > 50 || TOP_LM(0) < 20 ||
                                                      II.start_rm[0] > 35 || II.start_lm[0] > 35||
                                                      RIGHT(0) > 20 || BR_X(0) > 10||//有异议
                                                       (BR_X(0) < 6 &&  II.lnum[0] > 180)||//有异议
                                                      (BR_X(0) < 6 &&  II.lnum[0] < 10)||//有异议
                                                      II.top < 50 || II.speedTop < 40 ||
                                                      !II.lnum[0]  || II.dnum_all > 370 || II.dnum_all < 10  ||
                                                      getMapYMin_Col(14, basemap, 2)  >= 53);
            return 0;
        }
    uint8 deleteTop = getMapYMin_Col(0, deletemap, 254);  //杩欓噷鏄彇娑堜笅瀵荤殑deletemap锛屽鏋渄eletemap娌℃湁缁胯壊鍒欎负55
    uint8 min = getMapYMin_Col(0, deletemap, 1);  //杩欓噷鏄甯告悳鍥炬病鎼滃嚭鏉ョ殑deletemap
    uint8 end = getMapYMax_Col(0, leftmap, 1);
    uint8 yhTop;//鍦嗙幆椤�
    if (deleteTop != YY && min == YY && II.top >= 47)//delete为绿色进这个
    {
        if (II.dnum_top <= 40)
        {
//            qout<<II.dnum_top;
            return 0;
        }
        if (getMapYMin_Col(4, deletemap, 254) > deleteTop + 3)
        {
            return 0;
        }
        if (getMapYMin_Col(8, deletemap, 254) > deleteTop + 4)
        {  //鍦嗙幆椤惰秼鍔挎槸骞崇殑
            return 0;
        }
        for (uint8 i = 0; i < XM; ++i)
        {
            uint8 j = getMapYMin_Col(i, deletemap, 253);
            if (j == YM)
            {
//                qout<<j;
                return 0;
            }
            if (deletemap[j + 1][i] == 253)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        yhTop = deleteTop; //圆环顶
    }
    else//delete为黑色进这个
    {
        if (deleteTop < min)
        {  //如果满足这个条件说明不是圆环顶
            return 0;
        }

        if (min != YM && cntMap(0, min) <= 25)
        {  //杩欎釜鏉′欢瑕佸湪鐞嗚В涓�涓�
            return 0;
        }

//        if (getMapYMin_Col(4, deletemap, 1) > min + 3)//2022-1-1鍥犻噰闆嗙殑鍥惧儚鏈夐棶棰樻敞閲婏紝姝ゆ潯浠朵负鍒ゆ柇鍦嗙幆椤朵负骞崇殑锛岃嫢涓嶅钩鍒欐帓闄ゅ渾鐜�
//        {
//            qout << '1' ;
//            return 0;
//        }
//        if (getMapYMin_Col(8, deletemap, 1) > min + 4)
//        {
//            qout << '1' ;
//            return 0;
//        }
        yhTop = min;  //鍦嗙幆椤�
    }
    if (end == YM ||
        end < 18 ||//67  55
        yhTop > end + 27  ||
        yhTop < 40 ||
        yhTop < TOP_LM(0) ||
        end + 2 > yhTop)
        {
        return 0;
        }
    if (II.start_rm[0] > 0)
    {
        for (uint8 j = XX - 1; j > 20; --j)
        {
            if (rightmap[II.start_rm[0]][j] == 1)
            {  //鍙兘鏄帓闄ゅ崄瀛楃殑骞叉壈
                return 0;
            }
        }
    }
    for (int8_t i = RB_X(0); i >= 0; --i)//2-24鏀瑰搴︼紝鍘熸湰RT
    {
        if (leftmap[RB_Y(0)][i] != 1)
        {
//            qout<<"i"<<i;
//            qout<<"leftmap[RB_Y(0)][i]"<<leftmap[RB_Y(0)][i];
            return 0;
        }
    }
    uint8 down = YY;
    int8_t stopX = getMapXMax_Row(0, leftmap, 1);
    if (stopX == XM)
    {
        stopX = 0;
    }
    uint8 n = 0;
    for (int8_t i = RIGHT(0); i >= stopX; --i)
    {  //保证一个下降的趋势，防止出现十字上拐点那样的平线

        uint8 temp = getMapYMin_Col(i, leftmap, 1);//判断BR-TR之间的列数是否为持续减
        if (down < temp)
        {
//            qout<<"down"<<down;
//            qout<<"temp"<<temp;
            return 0;
        }
        else if (down == temp)
        {
            ++n;
        }
        else
        {
            down = temp;
        }
    }
    if (n > 4)//2022-1-1鏀癸紝杩欓噷鐨刵鏄湪涓婃柟鍒ゆ柇鎸佺画鍑忕殑杩囩▼涓嚭鐜板垪鏁颁笉鍙樼殑娆℃暟鍘熸湰涓�2
    {
//        qout<<n;
        return 0;//杩涗竴姝ュ垽鏂紝闃叉璇垽
    }
    if (RB_Y(0) + 5 > TOP_LM(0))
    {
//        qout<<"RB_Y(0)"<<RB_Y(0);
//        qout<<"TOP_LM(0)"<<TOP_LM(0);
        return 0;
    }
//    if (TOP_LM(0) <   RT_Y(0) + (uint8)((RIGHT(0) + 1) / 1.5 > 3 ?
//                                        3 : (RIGHT(0) + 1) / 1.5))
//    {
//        qout <<'7';
//        return 0;
//    }
    if (II.start_lm[0] == 0 || RIGHT(0) <= 3)
    {  //淇濊瘉涓�涓笂鍗囩殑瓒嬪娍
        up = 0;
        for (int8_t i = RIGHT(0); i > 0; --i)
        {
            uint8 temp = getMapYMax_Col(i, leftmap, 1);
            if (up > temp)
            {
                return 0;
            }
            else
            {
                up = temp;
            }
        }
    }
    else
    {
        uint8 topLeft = getMapXMin_Row(TOP_LM(0), leftmap, 1);

        if (topLeft > ((RIGHT(0) + 1) / 4 < 4 ? (RIGHT(0) + 1) / 4 : 4))
        {  //鏍规嵁瀹為檯鍥惧儚鍐欑殑鍒ゆ柇
            int cnt = 0;
            for(int j = topLeft; j < XM / 2; j++)
            {
                if(leftmap[TOP_LM(0)][j])
                {
                    cnt++;
                }
                else
                {
                    break;
                }
            }
            if(cnt > 8)//2022-2-24鏀癸紝鍒ゆ柇鍦嗙幆鍐呭湀涓婃柟鏄笉鏄钩鐨勶紝濡傛灉澶钩锛屽垯涓嶆槸鍦嗙幆锛屽師鏈负4,6
            {
//                qout<<cnt;
                return 0;
            }
            else
            {
//                if(TOP_LM(0) - 1 > 0)
//                {
//                    topLeft = getMapXMin_Row(TOP_LM(0) - 1, leftmap, 1);
//                    if (topLeft > ((RIGHT(0) + 1) / 4 < 4 ? (RIGHT(0) + 1) / 4 : 4))//???//2-24鏀瑰搴�
//                    {
//                        qout <<'1';
//                        return 0;
//                    }
//                }
            }
        }
        uint8 end = getMapYMax_Col(0, leftmap, 1);
        if (TOP_LM(0) > end + 8)//2-24鏀瑰搴� 3-25
        {
//            qout<<"TOP_LM(0)"<<TOP_LM(0);
//            qout<<"end"<<end;
            return 0;
        }
    }
    if (TOP_LM(0) > BOTTOM_LM(0) + 20 && RIGHT(0) > 5 && BR_X(0) <= XX - 1 &&
            RT_X(0) <= XX - 1)
    {
        uint8 x1, y1, x2, y2;
        x1 = BR_X(0) - 1;
        y1 = BR_Y(0) ;
        x2 = TR_X(0) ;
        y2 = TR_Y(0) ;//2023骞�7鏈�5鏃�18:22:01
        if (strJudge(x1, y1, x2, y2, leftmap, y1, y2, 1, 2))
        {  //宸﹀崐杈硅竟鐣岀洿绾垮垽鏂紝濡傛灉鏄渾鐜氨涓嶅彲鑳芥槸鐩寸嚎
//            qout<<strJudge(x1, y1, x2, y2, leftmap, y1, y2, 1, 2);
            return 0;
        }
    }
    uint8 x1 = 255, y1 = 255, x2 = 255, y2 = 255;
    if (BL_X(0) <= 0)
    {
        return 0;
    }
    x1 = BL_X(0) - 1;
    y1 = BL_Y(0);
    for (uint8 i = 0; i < XM; ++i)
    {
        if (rightmap[yhTop][i] == 1)
        {
            x2 = i - 1;
            y2 = yhTop ;
            break;
        }
        if (i == XX)
        {
            return 0;
        }
    }
    return strJudge(x1, y1, x2, y2, rightmap, y1, y2, 1, (y2 - y1) / 10);
}





/******************************************************************************
* 鍑芥暟鍚嶇О     : rightAnnulusDetect
* 鎻忚堪        : 鏄惁鍙冲叆鍦嗙幆鐨勬娴�
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 rightAnnulusDetect(void)
{
    uint8 up = 0;
    //    if (II.num_rm != 1 || II.num_lm != 1 || II.rnum[0] + II.lnum[0] < 250 ||
    //                    TOP_LM(0) < 50 || TOP_LM(0) < II.start_lm[0] + 25 || TOP_RM(0) < 20 ||
    //                    TOP_RM(0) > 55 || II.start_lm[0] > 35 || II.start_rm[0] > 25 ||
    //                    LEFT(0) < 23 || BL_X(0) < XX - 15 ||
    //                    k2[TOP_LM(0)] - k2[TOP_RM(0)] < 25 || II.top < 55 || II.speedTop < 50)
    if (II.num_rm != 1 || II.num_lm != 1 ||
            II.rnum[0] + II.lnum[0] < 150 ||
            TOP_LM(0) < 50 ||
            TOP_LM(0) < II.start_lm[0] + 10 ||
            TOP_RM(0) > 50 ||
            TOP_RM(0) < 20 ||
            II.start_lm[0] > 35 ||
            II.start_rm[0] > 30 ||
            LEFT(0) < 20 ||
            BL_X(0) < XX - 10 ||//鏈夊紓璁�
            II.top < 50 ||
            II.speedTop < 40 ||
            !II.rnum[0] )
    {
        return 0;
    }
    uint8 deleteTop = getMapYMin_Col(XX, deletemap, 254);
    uint8 min = getMapYMin_Col(XX, deletemap, 1);
    uint8 end = getMapYMax_Col(XX, rightmap, 1);
    uint8 yhTop;
    if (deleteTop != YY && min == YY && II.top >= 47)
    {
        if (II.dnum_top <= 40)
        {
            return 0;
        }
        if (getMapYMin_Col(XX - 4, deletemap, 254) > deleteTop + 3)
        {
            return 0;
        }
        if (getMapYMin_Col(XX - 8, deletemap, 254) > deleteTop + 4)
        {
            return 0;
        }
        for (uint8 i = 0; i < XM; ++i)
        {
            uint8 j = getMapYMin_Col(XX - i, deletemap, 253);
            if (j == YY)
            {
                return 0;
            }
            if (deletemap[j + 1][XX - i] == 253)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        yhTop = deleteTop;
    }
    else
    {
        if (deleteTop <= min)
        {
            return 0;
        }
        if (min != YM && cntMap(XX, min) <= 25)
        {
            return 0;
        }
//        if (getMapYMin_Col(XX - 4, deletemap, 1) > min + 3)
//        {
//            return 0;
//        }
//        if (getMapYMin_Col(XX - 8, deletemap, 1) > min + 4)
//        {
//            return 0;
//        }
        yhTop = min;
    }
    if (end == YM ||
        end < 18 ||//67
        yhTop > end + 27 ||
        yhTop < 40 ||
        yhTop  < TOP_RM(0) ||
        end + 2 > yhTop)
    {
        return 0;
    }
    if (II.start_lm[0] > 0)
    {
        for (uint8 j = 1; j < XX - 20; ++j)
        {
            if (leftmap[II.start_lm[0]][j] == 1)
            {
                return 0;
            }
        }
    }
    for (uint8 i = LB_X(0); i < XM; ++i)
    {
        if (rightmap[LB_Y(0)][i] == 0 )
        {
            return 0;
        }
    }
    uint8 down = YY;
    uint8 stopX = getMapXMin_Row(0, rightmap, 1);
    if (stopX == XM)
    {
        stopX = XX;
    }
    uint8 n = 0;
    for (uint8 i = LEFT(0); i <= stopX; ++i)
    {
        uint8 temp = getMapYMin_Col(i, rightmap, 1);
        if (down < temp)
        {
            return 0;
        }
        else if (down == temp)
        {
            ++n;
        }
        else
        {
            down = temp;
        }
    }
    if (n > 4)
    {
        return 0;
    }
    if (LB_Y(0) + 5 > TOP_RM(0))
    {
        return 0;
    }
//    if (TOP_RM(0) < LT_Y(0) + (uint8)((XX - LEFT(0) + 1) / 1.5 > 3 ?
//                                        3 : (XX - LEFT(0) + 1) / 1.5))
//    {
//        return 0;
//    }
    if (II.start_rm[0] == 0 || LEFT(0) >= XX - 3)
    {//淇濊瘉涓�涓笂鍗囩殑瓒嬪娍
        up = 0;
        for (uint8 i = LEFT(0); i < XM; ++i)
        {
            uint8 temp = getMapYMax_Col(i, rightmap, 1);
            if (up > temp)
            {
                return 0;
            }
            else
            {
                up = temp;
            }
        }
    }
    else
    {
        uint8 topRight = getMapXMax_Row(TOP_RM(0), rightmap, 1);
        if (topRight < XX - ((XX - LEFT(0) + 1) / 4 < 4 ? (XX - LEFT(0) + 1) / 4 : 4))
        {
            int cnt = 0;
            for(int j = topRight; j > XM / 2; j--)
            {
                if(rightmap[TOP_RM(0)][j])
                {
                    cnt++;
                }
                else
                {
                    break;
                }
            }
            if(cnt > 8)//TL閹� 閸︺劏顢戦惃鍕拨閻愯鏆�
            {
                return 0;
            }
            else
            {
//                if(TOP_RM(0) - 1 > 0)
//                {
//                    topRight = getMapXMax_Row(TOP_RM(0) - 1, rightmap, 1);
//                    qout<<XX - ((XX - LEFT(0) + 1) / 4 < 4 ? (XX - LEFT(0) + 1) / 4 : 4);
//                    if (topRight < XX - ((XX - LEFT(0) + 1) / 4 < 4 ? (XX - LEFT(0) + 1) / 4 : 4))//閿涚噦绱甸敍
//                    {
//                        qout<<'f';
//                        return 0;
//                    }
//                }
            }
        }
        uint8 end = getMapYMax_Col(XX, rightmap, 1);
        if (TOP_RM(0) > end + 8)//TL濮ｆ梹娓堕崣鍐插灙閺� 妤傛﹢绮﹂悙褰掔彯6鐞�
        {
            return 0;
        }
    }
    if (TOP_RM(0) > BOTTOM_RM(0) + 20 && LEFT(0) < XX - 5 && BL_X(0) >= 1 &&
            LT_X(0) >= 1)
    {  //TODO:閸欘垵鍏橀張澶愭６妫版﹫绱濋弽鍥唶娑� 娑�
        uint8 x1, y1, x2, y2;
        x1 = BL_X(0) - 1;
        y1 = BL_Y(0);
        x2 = TL_X(0) ;
        y2 = TL_Y(0);
        if (strJudge(x1, y1, x2, y2, rightmap, y1, y2, 1, 2))
        {//鍙冲崐杈硅竟鐣岀洿绾垮垽鏂紝濡傛灉鏄渾鐜氨涓嶅彲鑳芥槸鐩寸嚎
            return 0;
        }
    }

    uint8 x1 = 255, y1 = 255, x2 = 255, y2 = 255;
    if (BR_X(0) >= XX)
    {
        return 0;
    }
    x1 = BR_X(0) + 1;
    y1 = BR_Y(0);
    for (uint8 i = 0; i < XM; ++i)
    {
        if (leftmap[yhTop][XX - i] == 1)
        {
            //  if (rightmap[yhTop][j] == 1)
            //      x2 = j-1;
            // else
           x2 = XX - i + 1;
            y2 = yhTop ;
            break;
        }
        if (i == XX)
        {
            return 0;
        }
    }
    if (x1 == 255 || y1 == 255 || x2 == 255 || y2 == 255)
    {
        return 0;
    }
    return strJudge(x1, y1, x2, y2, leftmap, y1, y2, 1, (y2 - y1) / 10);
}





/******************************************************************************
* 鍑芥暟鍚嶇О     : strJudge
* 鎻忚堪        : 缁欏嚭x1,x2,y1,y2杩欎袱鐐瑰潗鏍囦箣闂存槸鍚︽槸鐩寸嚎
* 杩涘叆鍙傛暟     : uint8 x1, uint8 y1, uint8 x2, uint8 y2,uint8 map[][XM], uint8 sy1, uint8 sy2, int8_t limit,uint8 errorNum值
* 杩斿洖鍙傛暟     : 鏄惁鏄洿绾�
******************************************************************************/
uint8 strJudge(uint8 x1, uint8 y1, uint8 x2, uint8 y2,
                 uint8 map[][XM], uint8 sy1, uint8 sy2, int8_t limit,
                 uint8 errorNum)
{
    int8_t x;
    uint8 num = 0;
    if (y1 >= y2 || sy1 < y1 || sy2 > y2)
    {
        return 0;
    }
    float k = (float)(x2 - x1) / (y2 - y1);
    for (uint8 i = sy1; i < sy2; ++i)
    {
        x = (uint8)(x1 + k * (i - y1) + 0.5);
        if (map[i][x] == 2)
        {
            continue;
        }
        else
        {
            uint8 flag = 0;
            for (int8_t j = -2; j <= 2; ++j)
            {
                if (x + j < 0 || x + j > XX)
                {
                    continue;
                }
                if (map[i][x + j] == 2)
                {
                    flag = 1;
                    if (j < -limit || j > limit)
                    {
                        ++num;
                    }
                    break;
                }
            }
            if (flag == 0)//濡傛灉涓ょ偣鐨勮繛绾块檮杩戦兘娌℃湁杈圭晫锛屽垯杈圭晫鏍规湰涓嶅彲鑳芥槸鐩寸嚎
            {
#ifdef MY_QT_DEBUG
                for (uint8 i = y1; i < y2; ++i)
                {
                    map[i][(uint8)(x1 + k * (i - y1))] = 250;
                }
#endif
                return 0;
            }
        }
    }
    if (num > errorNum)//杈圭晫瓒呰繃浜嗚寖鍥撮檺瀹氳揪鍒颁簡涓�瀹氭暟閲忥紝鍒欒竟鐣屼篃涓嶅彲鑳芥槸鐩寸嚎
    {
#ifdef MY_QT_DEBUG
        for (uint8 i = y1; i < y2; ++i)
        {
            map[i][(uint8)(x1 + k * (i - y1))] = 250;
        }
#endif
        return 0;
    }
#ifdef MY_QT_DEBUG
    for (uint8 i = y1; i < y2; ++i)
    {
        map[i][(uint8)(x1 + k * (i - y1))] = 254;
    }
#endif
    return 1;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : strJudge_X
* 鎻忚堪        :
* 杩涘叆鍙傛暟     : uint8 x1, uint8 y1, uint8 x2, uint8 y2,uint8 map[][XM], uint8 sx1, uint8 sx2, int8_t limit,uint8 errorNum值
* 杩斿洖鍙傛暟     :
******************************************************************************/
uint8 strJudge_X(uint8 x1, uint8 y1, uint8 x2, uint8 y2,
                   uint8 map[][XM], uint8 sx1, uint8 sx2, int8_t limit,
                   uint8 errorNum)
{
    int8_t y;
    uint8 num = 0;
    if (x1 >= x2 || sx1 < x1 || sx2 > x2)
    {
        return 0;
    }
    float k = (float)(y2 - y1) / (x2 - x1);

    for (uint8 i = sx1; i < sx2; ++i)
    {
        y = (uint8)(y1 + k * (i - x1) + 0.5);
        if (map[y][i] == 2)
        {
            continue;
        }
        else
        {
            uint8 flag = 0;
            for (int8_t j = -2; j <= 2; ++j)
            {
                if (y + j < 0 || y + j > YY)
                {
                    continue;
                }
                if (map[y + j][i] == 2)
                {
                    flag = 1;
                    if (j < -limit || j > limit)
                    {
                        ++num;
                    }
                    break;
                }
            }
            if (flag == 0)
            {
                //#ifdef BOOM4_MY_QT_DEBUG
                //                for (uint8 i = x1; i < x2; ++i)
                //                {
                //                    map[(uint8)(y1 + k * (i - x1) + 0.5)][i]
                //                    = 255;
                //                }
                //#endif
                return 0;
            }
        }
    }
    //#ifdef BOOM4_MY_QT_DEBUG
    //    for (uint8 i = x1; i < x2; ++i)
    //    {
    //        map[(uint8)(y1 + k * (i - x1) + 0.5)][i] = 254;
    //    }
    //#endif
    if (num > errorNum)
    {
        return 0;
    }
    return 1;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : drawline
* 鎻忚堪        : 鍦╩ap鍥句笂浠巟1,y1鍒皒2,y2鐢讳竴鏉＄洿绾�
* 杩涘叆鍙傛暟     : uint8 x1, uint8 y1, uint8 x2, uint8 y2,uint8 map[][XM]值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void drawline(uint8 x1, uint8 y1, uint8 x2, uint8 y2,
              uint8 map[][XM]) // y1>y2
{
    if (y1 == y2)
    {
        return;
    }
    float k = (float)(x1 - x2) / (y1 - y2);
    for (uint8 i = y2; i <= y1; ++i)// y1>y2
    {
        map[i][(uint8)(x2 + k * (i - y2))] = 2;
    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : drawline2
* 鎻忚堪        : 鍦╩ap鍥句笂浠巟1,y1鍒皒2,y2鐢讳竴鏉＄敱n鍐冲畾寮у害鐨勭嚎
* 杩涘叆鍙傛暟     : uint8 x1, uint8 y1, uint8 x2, uint8 y2,uint8 map[][XM], uint8 n值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void drawline2(uint8 x1, uint8 y1, uint8 x2, uint8 y2,
               uint8 map[][XM], uint8 n) // x1>x2      // y1寰楀皬浜巠2
{
    uint8 lasty = YM;
    if (x1 == x2) //濡傛灉琛ョ嚎鐨勮捣鐐瑰拰缁堢偣鐩稿悓鐨勮瘽锛宺eturn
    {
        return;
    }
    float k = (float)(y1 - y2) / (x1 - x2); //琛ョ嚎鐨勮捣鐐瑰拰缁堢偣鐨勬枩鐜�
    float mid = (float)(x1 + x2) / 2;  //琛ョ嚎鐨勪腑鐐�
    uint8 climax = (uint8)n * (x1 - x2) / XM; // 12.0
    for (uint8 j = x2; j <= x1; ++j)
    {
        uint8 temp = (uint8)((j - x2) * k + y2 + climax -
                                 climax * (2 * fabs(j - mid) / (x1 - x2)) *
                                 (2 * fabs(j - mid) / (x1 - x2)));
        if (temp > YY)
        {
            continue;
        }
        map[temp][j] = 3;
        int8_t gap = (int8_t)temp - lasty;
        if (gap > 1 && lasty != YM)
        {
            while (--gap)
            {
                map[temp - gap][j] = 3;
            }
        }
        if (gap < -1 && lasty != YM)
        {
            while (++gap)
            {
                map[temp - gap][j - 1] = 3;
            }
        }
        lasty = temp;
    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : drawline3
* 鎻忚堪        : 鍦╩ap鍥句笂浠巟1,y1鍒皒2,y2鐢讳竴鏉″姬绾�
* 杩涘叆鍙傛暟     : uint8 x1, uint8 y1, uint8 x2, uint8 y2,uint8 map[][XM]值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void drawline3(uint8 x1, uint8 y1, uint8 x2, uint8 y2,
               uint8 map[][XM]) // x1>x2
{
    uint8 lasty = YM;
    if (x1 == x2)
        return;
    float k = (float)(y1 - y2) / (x1 - x2);
    float mid = (float)(x1 + x2) / 2;
    uint8 climax = (uint8)12 * (x1 - x2) / XM; // 12.0
    for (uint8 j = x2; j <= x1; ++j)
    {
        uint8 temp = (uint8)((j - x2) * k + y2 - climax +
                                 climax * (2 * fabs(j - mid) / (x1 - x2)) *
                                 (2 * fabs(j - mid) / (x1 - x2)));
        if (temp > YY)
        {
            continue;
        }
        map[temp][j] = 3;
        int8_t gap = (int8_t)temp - lasty;
        if (gap > 1 && lasty != YM)
        {
            while (--gap)
            {
                map[temp - gap][j] = 3;
            }
        }
        if (gap < -1 && lasty != YM)
        {
            while (++gap)
            {
                map[temp - gap][j - 1] = 3;
            }
        }
        lasty = temp;
    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : goAnnulus
* 鎻忚堪        : 鍦嗙幆鐨勬�诲鐞嗗垽鏂嚱鏁�
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 draw_line = 0;
uint8 draw_line_y;
uint8 draw_line_x;
uint8 goAnnulus(void)//
{
    static uint8 status = 0;
    static uint16_t time = 0;
    static uint16_t in = 0;
    if(reset_flag)
    {
        status = 0 ;
        nowAnnulusNum = 0;
    }
    //insidemap鍐呯殑榛戠偣鏁�
    uint16_t inum_all = XM * YM + II.repeatNum - II.bnum_all - II.lnum_all - II.rnum_all - II.dnum_all;
    if( !status && inum_all > 5) //鍑哄ぇ鍦嗙幆宸︿笅瑙掓湁涓�灏忛儴鍒嗚繘鍏nsidemap
    {
        return 0;
    }

    //宸﹀彸鍏ョ幆妫�娴�
    if (status == 0 && leftAnnulusDetect())
    {
        status = 1;
        Annulus_dir = goLeft;
        time = 0;
        in = 0;
        AD = AD_INIT;
        ++nowAnnulusNum;
    }
    else if (status == 0 && rightAnnulusDetect())
    {
        status = 1;
        Annulus_dir = goRight;
        time = 0;
        in = 0;
        AD = AD_INIT;
        ++nowAnnulusNum;
    }


    if (status == 1)
    {
        do
        {
            if (isEnter(Annulus_dir)/*鎵惧埌涓変釜浠ヤ笂鐨勬枒椹嚎鐐癸紝濡傛灉鍏佽姝ゆ柟鍚戝叆搴�*/)
            {
                status = 2;
                break;
            }
            AnnulusDeal(Annulus_dir, status);

        } while (0);
    }
    //qout<<status;
    if (status == 2)
    {
        do
        {
            if ((Annulus_dir == goLeft  && getMapYMin_Col(0, deletemap, 1) == YM &&     //图像最左侧一列全为白时
                                LT_X(0) == 0 &&
                              II.top != YY)  ||
                            (Annulus_dir == goRight && getMapYMin_Col(XX, deletemap, 1) == YM &&//鍥惧儚鏈�鍙充晶涓�鍒楀叏涓虹櫧
                                  RT_X(0) == XX &&
                              II.top !=  YY)
                              )
            {

                status = 3;
                AD.flag = 1;
                break;
            }
            AnnulusDeal(Annulus_dir, status);//鐘舵�佸ぇ浜�2鏃朵笉杩涜澶勭悊
        } while (0);
    }

    if (status == 3)
    {
        if ((Annulus_dir == goRight && II.right_y <= 50 && II.right_x < XX - 10 && II.right_x != 0) ||
                (Annulus_dir == goLeft && II.left_y <= 50 && II.left_x > 10  && II.left_x != XM))//鍒ゆ柇鏄惁瑕佸嚭鍦嗙幆浜�
        {  // 50 5
            status = 4;
            AD.flag = 2;
        }
    }
    if (status == 4)
    {
        do
        {
            if ((Annulus_dir == goRight && II.num_lm == 0) ||
                    (Annulus_dir == goLeft && II.num_rm == 0))
            {
                status = 5;
                AD.flag = 3;
                break;
            }
            AD.flag = 2;
            leave(Annulus_dir);
        } while (0);
    }
    if (status == 5)
    {
    #define MIN_DOWN 25
        if (Annulus_dir == goRight)
        {
            uint8 lmin = getMapYMin_Col(0, deletemap, 1);
            if (lmin == YM && II.num_lm == 1 && II.start_lm[0] <= MIN_DOWN)
            {
                status = 6;
            }
        }
        else if (Annulus_dir == goLeft)
        {
            uint8 rmin = getMapYMin_Col(XX, deletemap, 1);//鐭ラ亾鏈�宸﹁竟鐨勯粦鑹茬偣鍦ㄦ渶涓婃柟
            if (rmin == YM && II.num_rm == 1 && II.start_rm[0] <= MIN_DOWN)
            {
                status = 6;
            }
        }
    }
    if (status == 6)
    {
        IF.annulusDelay = 1;
    }
    else
    {
        IF.annulusDelay = 0;
    }
    if (status == 6)
            {
                ++time;
                if (time > delaytime_Annulus)
                {
                    status = 0;
                    draw_line = 0;
                }
            }

    if (status && status != 6)
    {
        return (Annulus_dir - 1) * 6 + status;
    }
    else
    {
        return 0;
    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : getSpeedTop
* 鎻忚堪        : 寰楀埌speedtop
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void getSpeedTop(void)
{
    for (uint8 i = 0; i < YM; ++i)
    {
        if (basemap[i][speedlineLeft[i]] && !insidemap[i][speedlineLeft[i]])
            //rightmap[i][speedlineLeft[i]]==1)
            {
            II.speedTop = i;
            break;
        }
        if (basemap[i][speedlineRight[i]] && !insidemap[i][speedlineRight[i]])
            //leftmap[i][speedlineRight[i]]==1)
            {
            II.speedTop = i;
            break;
        }
    }
    if(II.speedTop > 54)II.speedTop=54;

}




/******************************************************************************
* 鍑芥暟鍚嶇О     : isEnter
* 鎻忚堪        : 鍒ゆ柇鏄惁鍙互杩涘叆浠ir鏂瑰悜鐨勫渾鐜�
* 杩涘叆鍙傛暟     : uint8 dir值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 isEnter(uint8 dir)//
{
    if(dir == goLeft && II.num_rm > 0)
    {
        return 0;
    }
    if(dir == goRight && II.num_lm > 0)
    {
        //qout<<'f';
        return 0;
    }

    uint8 num = 0;

    for(uint8 i = 0; i < 35; ++i) //39
    {    //deletemap 0-50
        if(num >= 3) // 鍦╠eletemap鐨勫浘涓煇涓�鐭╁舰鍖哄煙涓壘鍒颁笁涓互涓婄殑榛戠櫧榛戠偣銆傚氨璇存槑鎵惧埌浜�
        {
            return 1;
        }
        for(uint8 j = 0; j < XM; ++j)
        {
            if(deletemap[i][j] == 0)//鐧借壊
            {
                for (uint8 k = j + 1; k < XM; ++k)
                {
                    if (deletemap[i][k])//鐧借壊
                    {
                        for (uint8 m = k + 1; m < XM; ++m)
                        {
                            if (deletemap[i][m] == 0)//鐧借壊
                            {
                                ++num;
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : leave
* 鎻忚堪        : 鍒ゆ柇鏄惁鍙互绂诲紑浠ir鏂瑰悜鐨勫渾鐜�
* 杩涘叆鍙傛暟     : uint8 dir值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 leave(uint8 dir)
{
    if (dir == goLeft && II.left_x >= 5)
    {
        if (II.left_x == XM && TOP_RM(0) == YY && LEFT(0) < XM / 2)
        {
            return 0;
        }
        else if (II.left_x >= XX - 1)
        {
            II.line_forbid = BOTHLINE;
            return 0;
        }
        else
        {
            uint8 min = getMapYMin_Col(0, deletemap, 1);
            if (min == YY)
            {
                min = getMapYMin_Col(0, deletemap, 254);
            }
            if(cntMap(0, min) < 30)
            {
                min = getMapYMax_Col(3, deletemap, 2) + 1;
            }
            if (min < YM && min > II.left_y)
            {
                uint8 minx = getMapXMin_Row(min, deletemap, 2);
                drawline2(II.left_x, II.left_y, minx, min, rightmap, 12);
                AD.flag = 4;
            }
            else
            {
                II.line_forbid = BOTHLINE;
                return 0;
            }
        }
    }
    else if (dir == goRight && II.right_x <= XX - 5)
    {
        if (II.right_x == XM && TOP_LM(0) == YY && RIGHT(0) > XM / 2)
        {
            return 0;
        }
        else if (II.right_x <= 1) //5
        {
            II.line_forbid = BOTHLINE;
            return 0;
        }
        else
        {
            uint8 min = getMapYMin_Col(XX, deletemap, 1);
            if (min == YY)
            {
                min = getMapYMin_Col(XX, deletemap, 254);
            }
            if(cntMap(XX, min) < 30)
            {
                min = getMapYMax_Col(XX - 3, deletemap, 2) + 1;
            }
            if (min < YM && min > II.right_y)
            {

                uint8 upd = getMapXMax_Row(min, deletemap, 1);
                drawline2(XX - 3, min, II.right_x, upd, leftmap, 12);
                //                uint8 upd = getMapYMin_Col(II.right_x, leftmap, 4);
                //                drawline2(minx, min, II.right_x, upd, leftmap, 12);
                AD.flag = 4;
            }
            else
            {
                II.line_forbid = BOTHLINE;
                return 0;
            }
        }
    }
    return 1;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : AnnulusDeal
* 鎻忚堪        : 鍦嗙幆鐨勫鐞嗗嚱鏁�
* 杩涘叆鍙傛暟     : uint8 ADir, uint8 status值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 AnnulusDeal(uint8 ADir, uint8 status)
{
#define yhmap deletemap//yhmap锛堝渾鐜痬ap) 褰撳仛 deletemap
    if (ADir == 0 || status > 2)
    {  //濡傛灉鐘舵�佸凡缁忓ぇ浜�2     //娌℃湁鍏ョ幆鏂瑰悜
        return 0;
    }
    uint8 downY = YM, downX = XM;
    uint8 ret = 0; //鏄惁琛ョ嚎瀹屾垚鐨勬爣蹇�
    if (ADir == goLeft)
    {
        uint8 value_1 = 1;
        uint8 value_2 = 2;
        uint8 min = getMapYMin_Col(0, deletemap, 1);//鍒犲浘涓0鍒楁渶浣庨粦鐐�
        if (min == YM)
        {
            min = getMapYMin_Col(0, deletemap, 254);
            value_1 = 254;
            value_2 = 253;  //TODO:鏍囪涓�涓�
            if (min == YM)
            {
                return 0;
            }
        }
        if (II.top > min - 1)
        {  //閲嶆柊璁剧疆鍙栨秷涓嬪鐨勯珮搴�
            II.top = min - 1;
        }
        uint8 myflag = 0;
        if (value_1 == 1)
        {
            //閲嶆柊鎼渄eletemap锛岃繕涓嶆槸寰堟噦
            //閲嶆柊鎼渄eletemap锛屽洜涓洪噸鏂拌缃簡鍙栨秷涓嬪鐨勯珮搴�
            memset(deletemap, 0, sizeof(deletemap));
            searchmap(0, min, yhmap);
        }
        for (uint8 i = 0; i < XX; ++i)
        {//杩欎釜鍦版柟鏈韩娌℃湁闂锛屽彧鏄噰闆嗙殑鍥惧儚鍦嗙幆椤朵笉骞筹紝杩欓噷鏈剰鏄�氳繃鎵惧垪鏂瑰悜涓婅繛缁袱涓竟鐣屾潵纭畾鍦嗙幆椤剁殑鎷愮偣锛屽鏋滅‘瀹炰笉鏄浘鍍忕殑闂鍙互閫氳繃璋冭妭i<3鏉ュ鍔犺竟鐣岀殑瀹借搴�
            for (uint8 j = 0; j < YY ; ++j)
            {
                if (yhmap[j][i] == value_2 &&
                        (yhmap[j + 1][i] != value_2 || i <= 3))
                {   //鎶奷eletmap閲岄潰鐨勭嚎璧嬪�肩粰rightmap  //绗簩涓潯浠舵槸涓轰簡鍘绘帀deletemap閲岄潰鍚戜笂鐨勮竟鐣岀嚎锛堝乏杈圭嚎锛�
                    rightmap[j][i] = 3;
                    downY = j;
                    downX = i;
                    break;
                }
                else if (yhmap[j][i] == value_2 &&
                         yhmap[j + 1][i] == value_2)
                {
                    if (i < XX && getMapYMin_Col(i + 1, yhmap, value_1) > j &&
                            myflag == 0)
                    {
                        myflag = 1;
                        break;
                    }
                    else
                    {
                        rightmap[j][i] = 3;
                        rightmap[j + 1][i] = 3;
                        downY = j;
                        downX = i;
                        break;
                    }
                }
            }
            if (myflag)
            {  //鎵惧埌浜哾eletemap閲岄潰鐨勮鐐�
                break;
            }
        }
        if (downY < II.speedTop)
        {
            II.speedTop = downY;  //搴旇涔熸槸鐢ㄦ潵鎺у埗閫熷害
        }
        if (downY > 51 || downX < 7)//濡傛灉瑙掔偣澶潬宸︿笂鐨勮瘽锛屽氨涓嶈ˉ浜�
        {
            for (uint8 i = min - 10; i < downY + 5 && i < YM; ++i)
            {
                for (uint8 j = 0; j <= downX; ++j)
                {
                    if (rightmap[i][j] == 3)
                    {
                        rightmap[i][j] = 0;
                    }
                }
            }
            return 0;
        }
        uint8 flag2 = 0;
        int x, y;
        float kk = 1.4;
        if (II.num_rm && II.start_rm[0] < 25)
        { //杩欓噷鎺у埗琛ョ嚎鐨勬枩鐜�
            for (uint8 i = 1; i < XM; ++i)
            {
                x = downX + i;
                y = (int)(downY - i * kk);
                if (x > XX || y < 0)
                {
                    break;
                }
                if (rightmap[y][x])
                {
                    kk = 1.1;
                }
            }
        }
        else
        {
            kk = 1.7;
        }

        for (uint8 i = 1; i < XM; ++i)
        {
            x = downX + i;
            y = (int)(downY - i * kk);
            if (x > XX || y < 0)
            {
                break;
            }
            if (rightmap[y][x] || x == XX || y == 0)
            {
                flag2 = 1;
                uint8 bo = y;
                if (rightmap[y][x] == 2)
                {
                    bo = y + 1;
                }
                for (uint8 k = bo; k < YM; ++k)
                {
                    for (uint8 m = 0; m < XM; ++m)
                    {
                        if (rightmap[k][m] == 2)
                        {//鍒犵嚎锛屽垹闄よˉ寰楃嚎涓庤竟绾夸氦鐐瑰線涓婄殑杈圭嚎
                            rightmap[k][m] = 0;
                        }
                    }
                }
                break;
            }

        }
        if (flag2)
        {
            if (II.num_rm)
            {
                drawline2(x, y, downX, downY, rightmap, 11); //杩欓噷鐨勫弬鏁拌皟涓�璋冭兘璁╄ˉ绾垮钩婊戣繛缁竴鐐�

            }
            else
            {
                drawline2(x, y, downX, downY, rightmap, 11);
                //                drawline2(XX, rightlineZero, downX, downY, rightmap, 12);
            }
            ret = 1;  //ret=1琛ㄧず琛ョ嚎瀹屾垚
        }
        //        else if (flag2 == 2)
        //        {
        //            drawline2(XX, 0, downX, downY, rightmap);
        //            //            drawline2(XX, rightlineZero, downX,
        //            downY, rightmap); ret = 1;
        //        }
    }
    else if (ADir == goRight)
    {
        uint8 value_1 = 1;
        uint8 value_2 = 2;
        uint8 min = getMapYMin_Col(XX , deletemap, 1);
        if (min == YM)
        {
            min = getMapYMin_Col(XX, deletemap, 254);
            value_1 = 254;
            value_2 = 253;
            if (min == YM)
            {
                return 0;
            }
        }
        if (II.top > min - 1)
        {
            II.top = min - 1;
        }

        uint8 myflag = 0;
        if (value_1 == 1)
        {
            memset(deletemap, 0, sizeof(deletemap));
            searchmap(XX, min, yhmap);
        }
        for (uint8 i = 0; i < XM - 1; ++i)
        {
            for (uint8 j = 0; j < YY; ++j)
            {
                if (yhmap[j][XX - i] == value_2 &&
                        (yhmap[j + 1][XX - i] != value_2 || i <= 3))
                {
                    leftmap[j][XX - i] = 3;
                    downY = j;
                    downX = XX - i;
                    break;
                }
                else if (yhmap[j][XX - i] == value_2 &&
                         yhmap[j + 1][XX - i] == value_2)
                {
                    if (i < XX &&
                            getMapYMin_Col(XX - i - 1, yhmap, value_1) > j &&
                            myflag == 0)
                    {
                        myflag = 1;
                        break;
                    }
                    else
                    {
                        leftmap[j][XX - i] = 3;
                        leftmap[j + 1][XX - i] = 3;
                        downY = j;
                        downX = XX - i;
                        break;
                    }
                }
            }
            if (myflag)
            {
                break;
            }
        }
        if (downY < II.speedTop)
        {
            II.speedTop = downY;
        }
        if (downY > 51 || downX > XX - 7)
        {
            for (uint8 i = min - 10; i < downY + 5 && i < YM; ++i)
            {
                for (int8_t j = XX; j >= downX; --j)
                {
                    if (leftmap[i][j] == 3)
                    {
                        leftmap[i][j] = 0;
                    }
                }
            }
            return 0;
        }
        uint8 flag2 = 0;
        int x, y;
        float kk = 1.4;
        if (II.num_lm && II.start_lm[0] < 25)
        {
            for (uint8 i = 1; i < XM; ++i)
            {
                x = downX - i;
                y = (int)(downY - i * kk);
                if (x < 0 || y < 0)
                {
                    break;
                }
                if (leftmap[y][x])
                {
                    kk = 1.1;
                }
            }
        }
        else
        {
            kk = 1.7;
        }
        for (uint8 i = 1; i < XM; ++i)
        {
            x = downX - i;
            y = (int)(downY - i * kk);
            if (x < 0 || y < 0)
            {
                break;
            }
            if (leftmap[y][x] || x == 0 || y == 0)
            {
                flag2 = 1;
                uint8 bo = y;
                if (leftmap[y][x] == 2)
                {
                    bo = y + 1;
                }
                for (uint8 k = bo; k < YM; ++k)
                {
                    for (uint8 m = 0; m < XM; ++m)
                    {
                        if (leftmap[k][m] == 2)
                        {
                            leftmap[k][m] = 0;
                        }
                    }
                }
                break;
            }
        }
        if (flag2)
        {
            if (II.num_lm)
            {
                drawline2(downX, downY, x, y, leftmap, 9);
            }
            else
            {
                drawline2(downX, downY, x, y, leftmap, 9);
                //                drawline2(downX, downY, 0, leftlineZero, leftmap, 12);
            }
            //            drawline2(downX, downY, 0, 0, leftmap);
            ret = 1;
        }
        //        else if (flag2 == 2)
        //        {
        ////            drawline2(downX, downY, 0, 0, leftmap);
        //                        drawline2(downX, downY, 0, y,
        //                        leftmap);
        //            ret = 1;
        //        }
    }
    return ret;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : getDown
* 鎻忚堪        : 寰楀埌鎵嚎琛�
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : down鎵嚎琛屾暟
******************************************************************************/
uint8 getDown(void)
{
    if (IF.annulus == AL1||IF.annulus == AR1)
    {
        return 35;
    }
    if (IF.annulus == AR2)
    {
        uint8 min = getMapYMin_Col2(XX, 0, basemap);
        if (min > 20)
            for (uint8 i = XX; i > XX - 23; --i)
            {
                if (basemap[min][i] == 2)
                {
                    return  min + 2;
                }
            }
        return   min + 2;
    }
    else if (IF.annulus == AL2)
    {
        uint8 min = getMapYMin_Col2(0, 0, basemap);
        if (min > 20)
            for (uint8 i = 0; i < 23; ++i)
            {
                if (basemap[min-1][i] == 2)
                {
                    return min + 2;
                }
            }
        return  min + 2;
        //        return 10;
    }
    else if(IF.annulus == AL3)
    {
        uint8 min = getMapYMin_Col(XX, basemap, 1);
        return min > 35 ? min : 35;
    }
    else if(IF.annulus == AR3)
    {
        uint8 min = getMapYMin_Col(0, basemap, 1);
        return min > 35 ? min : 35;
    }
    else if(IF.annulus == AL4)
    {
        return 20;
    }
    else if(IF.annulus == AR4)
    {
        return 20;
    }
    else if(IF.annulus == AL5)
    {
        uint8 min = getMapYMin_Col(XX, basemap, 1);
        return min > 45 ? min : 45;
    }
    else if(IF.annulus == AR5)
    {
        uint8 min = getMapYMin_Col(0, basemap, 1);
        return min > 45 ? min : 45;
    }
    else if (IF.annulus && IF.annulus != AR1&&IF.annulus != AL1)//鏉╂瑩鍣烽惃鍕拷鏄忕帆娴狅綀銆冪�瑰啫绻�妞ょ粯鏂侀張锟介崥锟�
    {
        return 35;   //閸﹀棛骞嗛悩鑸碉拷锟�2姒涙顓籨own = 30
    }
    return YY;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : getline
* 鎻忚堪        : 鏈�灏忎簩涔樻硶鎷熷悎宸﹀彸涓ょ嚎鐨勬枩鐜囧拰鎴窛
* 杩涘叆鍙傛暟     : float kb[2], ControlLine cl值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 getline(float kb[2], ControlLine cl)//鏈�灏忎簩涔樻硶鎷熷悎
{
    float sumx = 0;
    float sumy = 0;
    float sumxy = 0;
    float sumx2 = 0;
    uint8 n = 0;
    float x;
    float y;
    uint8 lastx = 0, llastx = 0;
    uint8 lasty = 0;
    if (cl == LEFTLINE)
    {
        lastx = 0;
        lasty = 0;

        for (uint8 i = 0; i < II.stop_line; ++i)
        {
            for (uint8 j = 0; j < XX; ++j)
            {
                if ((leftmap[i][j] == 2 &&
                     (leftmap[i][j + 1] == 0 || leftmap[i][j + 1] == 2))/*璇ョ偣鐨勫彸鐐规槸鍚︽槸  鐧界偣鎴栬�呯孩鐐�  */ ||
                     leftmap[i][j] == 3)//=3鏄繃鐜矝鐨勬椂鍊欒ˉ鐨勭嚎
                {
                    //                    if (i == lasty &&
                    //                                    (abs((int8_t)j - (int8_t)lastx) > 1 ||
                    //                                     abs((int8_t)llastx - (int8_t)j) > 3) &&
                    //                                    leftmap[i][j] != 3 &&
                    //                                    i > 0 && i + 3 < II.top)
                    //                    {
                    //                        break;
                    //                    }
                    leftmap[i][j] = 4;//QT閲岄潰宸﹀彸鍥捐竟鐣�4鐨勬潵婧愶紝鍙備笌璁＄畻鐨勮竟鐣岀嚎鏍囪涓�4
                    ++n;//璁板綍杈圭晫鐐规暟閲�
                    y = (j - leftline[i]) * k1[i];//涔樹笂涓�涓暩鍙樻牎姝ｇ郴鏁帮紝杩樺師鐪熷疄鐨勫亸宸�

                    x = k2[i];//绠楀嚭鐪熷疄鐨剎鍧愭爣锛堣繖閲岀殑x,y鏄畻鍋忓樊鍙﹀寤虹殑绯伙級
                    sumx += x;//x鍧愭爣鐨勭疮鍔�
                    sumy += y;//y鍧愭爣鐨勭疮鍔�
                    sumxy += x * y;//x*y鐨勭疮鍔�
                    sumx2 += x * x;//x骞虫柟鐨勭疮鍔�

                    lastx = j;//璁板綍涓嬫渶鍚庣殑y鍧愭爣
                    lasty = i;//璁板綍涓嬫渶鍚庣殑x鍧愭爣
                }
            }
            if ((leftmap[i][XX - 1] == 4 || (i < YY && leftmap[i + 1][XX - 1] == 4) || (i > 0 && leftmap[i - 1][XX - 1] == 4)) && (leftmap[i][XX] == 2 || leftmap[i][XX] == 3))
            {
                leftmap[i][XX] = 4;//杩欓噷搴旇鏄湪鍒ゆ柇x=xx鐨勬椂鍊欑殑杈圭嚎鎯呭喌
                ++n;
            }
            llastx = lastx;
        }
    }

    else if (cl == RIGHTLINE)
    {
        lastx = XX;
        lasty = 0;
        for (uint8 i = 0; i < II.stop_line; ++i)
        {
            for (uint8 j = XX; j > 0; --j)
            {
                if ((rightmap[i][j] == 2 &&
                     (rightmap[i][j - 1] == 0 || rightmap[i][j - 1] == 2 )) ||
                        rightmap[i][j] == 3)
                {

                    //                    if (i == lasty &&
                    //                                    (abs((int8_t)j - (int8_t)lastx) > 1 ||
                    //                                     abs((int8_t)llastx - (int8_t)j) > 3) &&
                    //                                    rightmap[i][j] != 3 &&
                    //                                    i > 0 && i + 3 < II.top)
                    //                    {
                    //                        break;
                    //                    }
                    rightmap[i][j] = 4;
                    ++n;
                    y = (j - rightline[i]) * k1[i];
                    x = k2[i];
                    sumx += x;
                    sumy += y;
                    sumxy += x * y;
                    sumx2 += x * x;

                    lastx = j;
                    lasty = i;
                }
            }
            if ((rightmap[i][1] == 4 || (i < YY && rightmap[i + 1][1] == 4) || (i > 0 && rightmap[i - 1][1] == 4)) &&
                    (rightmap[i][0] == 2 || rightmap[i][0] == 3))
            {
                rightmap[i][0] = 4;
                ++n;
            }
            llastx = lastx;
        }
    }
    kb[0] = (n * sumxy - sumx * sumy) / (n * sumx2 - sumx * sumx);//鍙傝�冪櫨搴︾櫨绉戞渶灏忎簩涔樻硶鐨勫叕寮忥紝绠楀嚭璧涢亾杈圭晫涓庢爣鍑嗙嚎涔嬮棿鐨勫亸宸拰瀵瑰簲鐨勮鏁颁箣闂寸殑鍑芥暟鍏崇郴
    kb[1] = (sumy  - kb[0] * sumx )/ n;//kb[0]鏄枩鐜囷紝kb[1]鏄父鏁� 鎴窛
    return n;//杩斿洖杈圭晫鐐圭殑鏁伴噺
}

float getAveDeviation(ControlLine cl) // leftmap, leftline
{
    float sum = 0;
    float x;
    int n = 0;
    uint8 d_line = 30;
    uint8 stop = II.top - 1 < 50 ? II.top - 1 : 50;//榛戣壊鍖哄煙鐨勯《閮ㄤ笌stopline杩涜姣旇緝
    if (cl == LEFTLINE)
    {
        for (uint8 j = d_line; j < stop; ++j)
        {
            for (uint8 i = 0; i < XM; ++i)
            {
                if (leftmap[j][i] >= 2)
                {
                    ++n;//30-stop涔嬮棿鐨勮竟鐣岀偣鏁伴噺
                    x = (i - leftline[j]) * k1[j];//姣忎釜杈圭晫鐐瑰搴旂殑鍋忓樊鍊�
                    sum += x;//绠楀嚭鎬荤殑鍋忓樊鍊�
                }
            }
        }
    }
    if (cl == RIGHTLINE)
    {
        for (uint8 j = d_line; j < stop; ++j)
        {
            for (int8_t i = XX; i >= 0; --i)
            {
                if (rightmap[j][i] >= 2)
                {
                    ++n;
                    x = (i - rightline[j]) * k1[j];
                    sum += x;
                }
            }
        }
    }
    if (n == 0)
        return 10000;
    else
        return (sum / n);//30-stop涔嬮棿鐨勫钩鍧囧亸宸�
}

/******************************************************************************
* 鍑芥暟鍚嶇О     : goRamp
* 鎻忚堪        : 鍧￠亾鐨勬�诲鐞嗗垽鏂嚱鏁�
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 goRamp(void)
{
    static uint8 status = 0;
    static uint16_t delay_time = 0;
    if(reset_flag)
    {
        status = 0 ;
        nowRampNum = 0;
    }
    if(!status && ramp())
    {
        status = 1;
        nowRampNum++;
    }
    if(status == 1)
    {
        delay_time++;
//        qout<<delay_time;
        if(delay_time >= 40)//帧数
        {
            status = 0;
            delay_time = 0;
        }
    }
    return status;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : findRampLine_L
* 鎻忚堪        : 瀵绘壘鍧￠亾宸﹁竟杞﹂亾绾挎槸鍚︽槸绗﹀悎涓婂崌瓒嬪娍鐨勭嚎
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 findRampLine_L(void)
{
    uint8 num = 0;
    for(uint8 j = 0; j < XM; j++)
    {
        for(uint8 i = II.start_lm[0];i < YM; i++)
        {
            if(leftmap[i][j] == 2)
            {
                if(i + 10 < YY && leftmap[i + 10][j] == 2)
                {
                    qout<<i <<j;
                    return 0;
                }
                if(i + 4 < YY && leftmap[i + 4][j] == 2)
                {
                    num+=2;
                }
                else if(i + 2 < YY && leftmap[i + 2][j] == 2)//鐗瑰緛浜� 绔栫嚎鍙橀暱
                {
                    num++;
                }
                break;
            }
        }
//        qout<<"L_num="<<num;
        if(num > 10)//&&IS.top_lm[0]>55
        {
            return 1;
        }
    }
    return 0;
}



/******************************************************************************
* 鍑芥暟鍚嶇О     : findRampLine_R
* 鎻忚堪        : 瀵绘壘鍧￠亾鍙宠竟杞﹂亾绾挎槸鍚︽槸绗﹀悎涓婂崌瓒嬪娍鐨勭嚎
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 findRampLine_R()
{
    uint8 num = 0;
    for(uint8 j = XX; j > 0; j--)
    {
        for(uint8 i = II.start_rm[0];i < YM; i++)
        {
            if(rightmap[i][j] == 2)
            {
                if(i + 10 < YY && rightmap[i + 10][j] == 2)
                {  //闃叉鍥犱负鐣稿彉浜х敓鐨勪竴鏉＄珫绾胯�岃鍒ゆ垚鍧￠亾
                    return 0;
                }
                if(i + 4 < YY && rightmap[i + 4][j] == 2)
                {
                    num+=2;
                }
                else if(i + 2 < YY && rightmap[i + 2][j] == 2)//鐗瑰緛浜� 绔栫嚎鍙橀暱
                {
                    num++;
                }
                break;
            }
        }
//        qout<<"R_num="<<num;
        if(num > 10)//&&IS.top_lm[0]>55
        {
            return 1;
        }
    }
    return 0;
}

/******************************************************************************
* 鍑芥暟鍚嶇О     : ramp
* 鎻忚堪        : 鍒ゆ柇鍧￠亾鐨勫嚱鏁�
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 ramp(void)
{
    if( II.top != YY||II.dnum_all) //白赛道顶高，无deletemap图
      {
          return 0 ;
      }
    if(II.num_lm == 2 || II.num_rm == 2)//左右区属于连通区域
    {
        return 0;
    }
    tips_temp=getMapXMax_Row(YY, basemap, 0)-getMapXMin_Row(YY, basemap, 0);//最顶行白点总数
//    qout<<"tips_temp="<<tips_temp;
    if(tips_temp<=16)   //顶端白点异常增大
    {
        return 0;
    }
    if( II.num_lm && II.num_rm && straightramp())//左右都有区域，直道入坡
    {
//        if(findRampLine_L() || findRampLine_R() || straightramp())
//        {
            return 1;
//    }
    }
    else if(((II.start_lm[0]>15 && RT_Y(0) == YY)||!II.num_lm )&& LT_Y(0) == YY &&
               findRampLine_R())//无左图，左拐入坡
    {
//                qout<<"start_lm[0]="<<II.start_lm[0];
        uint8 last_y = 0,y = YY;
        uint8 flag = 0;
        for(uint8 j = 0;j < XM; j++)
        {
            for(uint8 i = 0;i < YM; i++)
            {
                if(rightmap[i][j] == 2)
                { //保障右边区域的边线的一个上升趋势
                    y = i;
                    if(i <= last_y && j>0)
                        {
//                        qout<<"last_y="<<last_y;
                            return 0;
                        }
                    last_y = y;
                    break;
                }
                if(i == YY && tips_temp > 13 ) flag = 1;
            }
            if(flag) break;
        }
//        qout<<"right";
        return 1;
    }
    else if(((II.start_rm[0]>15 && LT_Y(0) == YY)||!II.num_lm)&& RT_Y(0) == YY &&
            findRampLine_L())//无右图，右拐入坡
    {
//                qout<<"start_rm[0]="<<II.start_rm[0];
        uint8 last_y = 0, y = YY;
        uint8 flag = 0;
        for(uint8 j = XX; j > 0; j--)
        {
            for(uint8 i = 0; i < YM; i++)
            {
                if(leftmap[i][j] == 2)
                { //保障左边区域的边线的一个上升趋势
                    y = i;
                    if(i <= last_y && j < XX)
                    {
//                        qout<<"last_y="<<last_y;
                        return 0;
                    }
                    last_y = y;
                    break;
                }
                if(i == YY && tips_temp > 13 ) flag = 1;
            }
            if(flag ) break;
        }
//        qout<<"left";
        return 1;
    }
    return 0;
}


/******************************************************************************
* 鍑芥暟鍚嶇О     : getrampline
* 鎻忚堪        : 鍦ㄥ潯閬撴椂锛屽緱鍒板潯閬撶殑宸﹀彸绾挎枩鐜囧拰鎴窛
* 杩涘叆鍙傛暟     : float kb[2], ControlLine cl值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 getrampline(float kb[2], ControlLine cl)
{
//    for (uint8 j = 0, flag = 0; j < YM / 3.5; ++j)//涓嬪崐閮ㄥ垎
        for (uint8 j = 0; j < YM / 3.5; ++j)//涓嬪崐閮ㄥ垎
    {
        uint8 l = 0;
        uint8 r = XX;
        for (uint8 i = 0; i < XM; ++i)//浠庡彸鍒板乏
        {
            if (leftmap[j][XX - i] == 2)
            {
                l = XX - i;//宸﹁竟鐣�
                break;
            }
        }
        for (uint8 i = 0; i < XM; ++i)//浠庡乏鍒板彸
        {
            if (rightmap[j][i] == 2)
            {
                r = i;//鍙宠竟鐣�
                break;
            }
        }
//        if (flag == 0 && l != 0 && r != XX)
//        {
//            flag = 1;//宸﹀彸杈圭晫閮芥悳鍒�
//        }
//        else if (flag == 1 && (l == 0 || r == XX))
//        {
//            continue;
//        }
        midline[j] = l + r;
    }

    float sumx = 0;
    float sumy = 0;
    float sumxy = 0;
    float sumx2 = 0;
    uint8 n = 0;
    float x;
    float y;
    uint8 lasty = 255;

    for (uint8 i = 0; i < YM / 3.5; ++i)
    {
        if (midline[i] == 0)
        {
            continue;
        }
        if (lasty != 255)
        {
            if (midline[i] > midline[lasty] + 2 || midline[i] + 2 < midline[lasty])//涓庝笂涓�琛屽樊鍊煎ぇ浜�2锛屽垯璇ヨ涓嶈繘琛屾枩鐜囪繍绠�
            {
                continue;
            }
        }
        lasty = i;
        ++n;
        y = (midline[i] - XX) * k1[i];

        x = k2[i];
        sumx += x;
        sumy += y;
        sumxy += x * y;
        sumx2 += x * x;
    }

    kb[0] = (n * sumxy - sumx * sumy) / (n * sumx2 - sumx * sumx);
    kb[1] = sumy / n - kb[0] * sumx / n;
    return n;
}




extern ImgFlag IIF;
#define dealmap deletemap


/******************************************************************************
* 鍑芥暟鍚嶇О     : detectStartline
* 鎻忚堪        : 鍒犻櫎鍏ュ簱绾�
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 detectStartline(void)
{
    uint16_t inum_all = XM * YM + II.repeatNum
            - II.bnum_all - II.lnum_all
            - II.rnum_all - II.dnum_all;

    if(inum_all < 80 || inum_all > 500) //80  500
    {
        return 0;
    }
    uint8 startMap[YM][XM] = {0};
    uint8 num = 0;
    uint16_t pixelNum = 0;
    for(uint8 j = 0; j < YM / 2; ++j)
    {
        for(uint8 i = 0; i < XX; ++i)
        {
            if(insidemap[j][i])
            {
                for(uint8 k = i; k < XX; ++k)
                {
                    if(insidemap[j][k] && !startMap[j][k])
                    {
                        numCnt = 0;
                        searchCountmap(k, j, startMap);
                        if(numCnt > 5)
                        {
                            ++num;
                            pixelNum += numCnt;
                        }
                    }
                }
                break;
            }
        }
    }
    if(num >= 5 && pixelNum > 80)
    {
        return 1;
    }
    return 0;
}
/******************************************************************************
* 鍑芥暟鍚嶇О     : cnt_WBW_num
* 鎻忚堪        : 璁＄畻map涓瓂琛屼粠x1鍒皒2鐨勭櫧榛戠櫧鐐圭殑鏁伴噺
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 cnt_WBW_num(uint8 x1, uint8 x2, uint8 y, uint8 map[][XM])
{
    if(x1 > XX || x2 > XX || y > YY)
    {
        return 255;
    }
    uint8 _cnt = 0;
    uint8 color = 0;
    uint8 j = y;
    if(x1 < x2)
    {
        uint8 i = x1;
        do
        {
            if(map[j][i] != color)
            {
                ++_cnt;
                color = !color;
            }
            i++;
        } while(i <= x2);
        return _cnt / 2;
    }
    else if(x1 > x2)
    {
        int8_t i = x2;
        do
        {
            if(map[j][i] != color)
            {
                ++_cnt;
                color = !color;
            }
            i--;
        } while(i <= x1);
        return _cnt / 2;
    }
}




/******************************************************************************
* 鍑芥暟鍚嶇О     : detectStartline_v2
* 鎻忚堪        : 妫�娴嬫枒椹嚎
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
uint8 detectStartline_v2(void)
{
    /*鏂戦┈绾块粦鐐规暟閲廼nsidemap鐨勯粦鐐规暟*/uint16_t inum_all = XM * YM + II.repeatNum//鎬诲儚绱犵偣+宸﹀彸閲嶅鐨勯粦鐐�
                - II.bnum_all - II.lnum_all  //basemap涓殑榛戠偣鏁� + leftmap涓殑榛戠偣鏁�
                - II.rnum_all - II.dnum_all;    //rightmap涓殑榛戠偣鏁�

    if(inum_all < 5 || inum_all > 500) //鑷垜淇濇姢锛宨nsidemap鍐呴粦鐐规暟澶鎴栧お灏戠洿鎺ョ粨鏉熸鏌�
    {
        return 0;
    }
    uint8 startMap[YM][XM] = {0}; //涓存椂澶勭悊鍦板浘
    uint8 num = 0;//鎬诲叡鍥惧儚鐨勮繛閫氶粦鑹插尯鍩熷潡鏁伴噺
    uint16_t pixelNum = 0;//鎵�鏈夎繛閫氶粦鑹插尯鍩熺殑榛戠偣鏁�
    for(uint8 j = YM / 4; j < YM; ++j)
    {
        for(uint8 i = 0; i < XX; ++i)
        {
            if(insidemap[j][i])//褰撳墠insidemap鐨勬槸榛戠偣
            {
                for(uint8 k = i; k < XX; ++k)
                {
                    if(insidemap[j][k] && !startMap[j][k])//insidemap璇ョ偣涓洪粦
                    {
                        numCnt = 0;//鎼滅储鍒版瘡涓粦鑹插尯鍩熺殑榛戠偣鏁�
                        searchCountmap(k, j, startMap); //鎼滅储insidemap鑼冨洿涓璪asemap榛戣壊鍖哄煙鍐呴粦鐐规暟  //瀵箂tartmap澶勭悊
                        if(numCnt >= 2)  //榛戠偣鏁板ぇ浜�2锛屽垽瀹氫负鏈夋晥榛戣壊鍖哄煙
                        {
                            ++num;//璇存槑鏈夐粦鑹插尯鍩�
                            pixelNum += numCnt; //缁熻鎵�鏈夐粦鑹插尯鍩熺殑榛戠偣鏁�
                        }
                    }
                }
                break;
            }
        }
    }
    if(num >= 4 && pixelNum >= 12)
    {  //鏈�4涓嫭绔嬬殑榛戣壊鍖哄煙锛堥棶棰橈紝鍙互鍚堝苟绱ф尐鍦ㄤ竴璧凤級锛屽苟涓旈粦鐐规暟鐩稿姞澶т簬15
        return 1;
    }
    return 0;
}
uint8 goObstacle()
{
    static int16 obNum[67] = { 240, 237, 234, 230, 227, 223, 218, 214, 209, 204, 199, 194, 189, 184, 179, 173, 168, 163, 158, 153,
        148, 144, 139, 134, 130, 126, 121, 117, 113, 109, 105, 102, 98, 94, 91, 88, 84, 81, 78, 75,
        72, 69, 66, 63, 60, 57, 54, 52, 49, 46, 44, 41, 39, 36, 34, 32, 29, 27, 26, 24,
        22, 21, 20, 19, 18, 18, 18 };
    int left = 0;
    int right = XX;
    int downy = 0;
    char start_only_ones = 0;
    uint8 obstaclemap[YM][XM] = { 0 };
    uint16_t insideNum = XM*YM + II.repeatNum - II.bnum_all - II.rnum_all - II.lnum_all - II.dnum_all;//insidemap中的黑点数
    IF.startline = 0;
    for (uint8 i = 0; i <= YY; i++)//从底部到顶部一次识别，基本贯穿整个函数
    {
        uint8 myflag = 0;
        if (insideNum < 8) break;
        for (char m = XX - 1; m >= 0; --m)//优先使用左右图找
        {
            if (leftmap[i][m] == 1)//从右向左找第一个黑点
            {
                left = m + 1;
                myflag = 1;
                break;
            }
        }
        if (myflag == 0)//如果这一行leftmap没有一个黑点，则取basemap中最左侧白点记为left
        for (uint8 m = 0; m <= XX; ++m)
        {
            if (basemap[i][m] == 0)//白的
            {
                left = m;
                break;
            }
        }
        myflag = 0;
        for (char n = 1; n<XM; ++n)
        {
            if (rightmap[i][n] == 1)//黑的
            {
                right = n - 1;
                myflag = 1;
                break;
            }
        }
        if (myflag == 0)
        for (int n = XX; n>0; --n)
        {
            if (basemap[i][n] == 0)//白的
            {
                right = n;
                break;
            }
        }


        for (uint8 j = left; j <= right; j++)
        {
            if (insidemap[i][j])//遇到insidemap的黑点
            {
                /* ****************起跑线识别程序*******************/
                if (start_only_ones<1 && i<30)//求联通区域的个数，一副图像检测1次
                {
                    uint8 startmap[YM][XM] = { 0 };
                    uint8 s_cnt = 0;//insidemap中搜到的黑色区域数
                    for (uint8 m = i; m<i + 20; m++)//搜insidemap上面二十行的区域
                    {
                        for (uint8 n = 0; n<XM; n++)
                        {
                            if (!startmap[m][n] && insidemap[m][n])
                            {
                                searchmap(n, m, startmap);
                                s_cnt++;
                            }
                        }
                    }
                    uint8 snum = 0;//从左向右黑白跳变点的数量
                    if (s_cnt>4)
                    {
                        for (uint8 m = i; m < i + 20; ++m)
                        {
                            snum = 0;
                            for (uint8 n = left; n < right; ++n)
                            {
                                if (startmap[m][n] != 1 && startmap[m][n + 1] == 1)
                                    ++snum;
                            }
                            if (snum >4) break;
                        }
                    }
                    if (snum >4)//跳变点有四个以上则是斑马线
                    {
                        IF.startline = 1;
                        ++start_only_ones;
                        break;
                    }
                    else
                        IF.startline = 0;
                }
                else if (start_only_ones == 0)
                    IF.startline = 0;
                downy = i;
                int ob_num;
                numCnt = 0;
                if (obstaclemap[i][j] == 0)
                    searchCountmap(j, i, obstaclemap);
                else continue;
                ob_num = numCnt;
                insideNum -= ob_num;
                //开始判障碍了
                //if (IF.obstacle != 0) ob_num = (int)(ob_num*1.1);
//                if (downy <= 65 && ob_num > obNum[downy] * 0.8 - 10 && ob_num < obNum[downy] * 1.3 + 25)//对黑块的数量进行限制
//                {  //如果是障碍或之前已经识别出障碍了
//                    int max = 4;
//                    //  int direction=0;
//                    ob_direction = 0;
//                    /*      if (IF.obstacle == 1) ob_direction = 1;
//                    else if (IF.obstacle == 2) ob_direction = 2;*/
//                    if (ob_direction == 0)
//                    {
//                        for (uint8 k = downy + 5; k <= YY; k++)
//                        {
//                            if (left == 0 && ob_direction == 0)//如果左边没有左边界，且并没有判过障碍（当前行之前）左斜入障碍
//                            {
//                                max = 10;
//                                uint8 obs_right = right;
//                                for (char m = XX; m>0; m--)//向左搜索从basemap找到第一个白点
//                                {
//                                    if (basemap[k][m] == 0)
//                                    {
//                                        obs_right = m;
//                                        break;
//                                    }
//                                }
//                                for (uint8 m = 1; m<max; m++)
//                                {
//                                    if (obs_right  < m)
//                                        break;
//                                    if (obstaclemap[k][obs_right - m])
//                                    {
//                                        ob_direction = 2;
//                                        if (k1[k] * m>23)
//                                            ob_direction = 1;
//                                        break;
//                                    }
//                                }
//                            }
//                            else if (right == XX&&ob_direction == 0)//如果右边没有右边界，且并没有判过障碍（当前行之前）右斜入障碍
//                            {
//                                max = 10;
//                                uint8 obs_left = left;
//                                for (char m = 0; m<XX; m++)
//                                {
//                                    if (basemap[k][m] == 0)
//                                    {
//                                        obs_left = m;
//                                        break;
//                                    }
//                                }
//                                for (uint8 m = 1; m < max; ++m)
//                                {
//                                    if (obs_left + m > XX)
//                                        break;
//                                    if (obstaclemap[k][obs_left + m])
//                                    {
//                                        ob_direction = 1; //障碍在左边
//                                        if (k1[k] * m>23)
//                                            ob_direction = 2;
//                                        break;
//                                    }
//                                }
//                            }
//                            else if (ob_direction == 0 && left != 0 && right != XX)//左右边都有边界，且并没有判过障碍（当前行之前）
//                            {
//                                max = 8;
//                                for (uint8 m = 1; m < max; ++m)
//                                {
//                                    if (right - m  < 0 || left + m > XX)
//                                        break;
//                                    if (obstaclemap[k][right - m])
//                                    {
//                                        ob_direction = 2; //障碍在右边
//                                        break;
//                                    }
//                                    if (obstaclemap[k][left + m])
//                                    {
//                                        ob_direction = 1; //障碍在左边
//                                        break;
//                                    }
//                                }
//                            }
//                        }
//                    }
//                    if (ob_direction == 0)
//                    {
//                        if (left == 0) ob_direction = 1;
//                        else if (right == XX) ob_direction = 2;
//                    }
//                    if (ob_direction == 1)   //之前已经已经判断出前方有障碍，且障碍在左边
//                    {
//                        return 1;  //表示有障碍物
//                    }
//                    //障碍物在右边
//                    else if (ob_direction == 2)
//                    {
//                        return 2;  //表示有障碍物
//                    }
//                }
                break;//-----------------------------------调试添加
            }

        }

        /////////////////////////////////////////////

    }
    return 0;
}
uint8_t straightramp()
{
    if(II.num_lm != 1 || II.num_rm != 1)//左右区域都为一个
    {
        return 0;
    }
    if( II.start_lm[0]<=5 || II.start_rm[0]<=5 || II.start_lm[0]>=30 || II.start_rm[0]>=30)  //低端有一定高度
    {
//        qout<<"start_lm="<<II.start_lm[0];
//        qout<<"start_rm="<<II.start_rm[0];
        return 0;
    }
    if(II.top_left[0].y != YY || II.top_right[0].y != YY)//TR、TL都在最高行
    {
        return 0;
    }
    int8_t y = 0;
    uint8_t num = 0;
    y = II.start_lm[0] >= II.start_rm[0] ? II.start_lm[0] : II.start_rm[0];//左右起始行高的
    for(int8_t i = YM; i > y; i--)//从上到下
    {
        for(int8_t j = 0; j < XM; j++)//从左到右
        {
            if(leftmap[i][j] == 2)
            {
                if(i > 10 && leftmap[i - 10][j] == 2)
                {  //因为畸变产生的一条竖线（高10行）
//                    qout<<"out1"<<i<<j;
                    return 0;
                }
                for(uint8_t k = j + 1; k < XM; k++)
                {
                    if(rightmap[i][k] == 2)
                    {
                        if(i > 10 && rightmap[i - 10][k] == 2)
                        {  //因为畸变产生的一条竖线（高10行）
//                            qout<<"out2"<<i<<j;
                            return 0;
                        }
                        if(k - j > rightline[i] - leftline[i] + 6)//比普通直道超宽6列
                        {
//                            qout<<k<<j<<i;
                            num++;
//                            qout << "num = " << num;
                            if(num > 10)//>6行的超宽
                            {
                                //IF.sramp = 1;
//                                qout << "straightramp";
                                return 1;
                            }
                        }
                        //                        else
                        //                        {
                        //                            return 0;
                        //                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    return 0;
}
