/*
 * deal_img.c
 *
 *  Created on: 2023年7月7日
 *      Author:蛋仔
 */
#include "deal_img.h"
uint16 histogram[256];

uint8 nowThreshold = 0 ;
uint8 previousThreshold = 0 ;
uint8 minThreshold = 0 ;
uint8 maxThreshold = 255 ;
uint8 Exiacn[my_row][my_col];
uint8 _Exiacn[my_row][my_col];
uint8 imgGray[my_row][_my_col];
uint8  l_found=0;
uint8  r_found=0;
uint8  start_point_l[2];// [0]存储x [1]存储y
uint8  start_point_r[2];
uint8  adaptThreshold;//sobel算子阈值
uint8  Thresholdcompare;
uint8 points_l_num ;
uint8 points_r_num ;
uint8 points_l[USE_num][2];//左线
uint8 points_r[USE_num][2];//右线
uint8 points_c[USE_num][2];//中线
uint8 lstep=0;
uint8 rstep=0;
uint8 points_real_l_num=0;
uint8 points_real_r_num=0;

/*******************************
函数名称： Threshold_init
函数作用： Threshold初始化
输入值 ：  void
返回值 ：  void

*******************************/
void Threshold_init(void)
{
    adaptThreshold=100;
    Thresholdcompare=100;
}

/*******************************
函数名称： new_get_start_point
函数作用： 迷宫法找到开始起点
输入值 ：  void
返回值 ：  找到1  没找到0

*******************************/

    uint8 new_get_start_point(void)
    {
        l_found = 0; // 左找到起点标志位
        r_found = 0; // 右找到起点标志位

        start_point_l[1] = my_row - 1; // 左起点的y值设为my_row - 1
        start_point_r[1] = my_row - 1; // 右起点的y值设为my_row - 1
        for (uint8 i = my_col/2+1; i>1; i--) // 从左边界开始逐列搜索
        {
            if (Exiacn[start_point_l[1]][i - 2] == Black &&Exiacn[start_point_l[1]][i - 1] == Black && Exiacn[start_point_l[1]][i] == White) // 差比和为真
            {
                start_point_l[0] = i;
                l_found = 1;
                break;
            }
//            _Exiacn[start_point_l[1]][i] = 250;//上位机显示！！！！！！！！
        }

        for (uint8 i = my_col/2 - 1; i<my_col-1-1; i++) // 从右边界开始逐列搜索
        {
            if (Exiacn[start_point_r[1]][i]==White && Exiacn[start_point_r[1]][i+1] ==Black && Exiacn[start_point_r[1]][i+2] ==Black) // 差比和为真
            {
                start_point_r[0] = i;
                r_found = 1;
                break;
            }
//            _Exiacn[start_point_l[1]][i] = 252;//上位机显示！！！！！！！！
        }

        if (!l_found)
        {
            start_point_l[0] = 0; // 左起点的x值设为0，即左边界
        }

        if (!r_found)
        {
            start_point_r[0] = my_col - 1; // 右起点的x值设为my_col - 1，即右边界
        }

        return (l_found || r_found) ? 1 : 0;
    }

/*******************************
函数名称： adaptive_deal
函数作用： 自适应_sobel_边缘检测并进行二值化
输入值 ：  x y 灰度
返回值 ：  黑 白

*******************************/
uint8 adaptive_deal(uint8 x,uint8 y)
{
        //{-1 , 0 , 1}
        //{-2 , 0 , 2}//[0]纵向梯度
        //{-1 , 0 , 1}

        //{-1, -2, -1}
        //{ 0,  0,  0}//[1]横向梯度
        //{ 1,  2,  1}
        short i, j;
        short temp[2];
        //adaptive_kernel(x,y,&kernel);
        i = y;
        j = x;
        temp[0] = -1*(short)imgGray[i - 1][j - 1] + (short) imgGray[i - 1][j + 1]     //{-1 , 0 , 1}
        -2* (short) imgGray[i][j - 1] + 2*(short) imgGray[i][j + 1]                   //{-2 , 0 , 2}
        -1*(short) imgGray[i + 1][j - 1] + (short) imgGray[i + 1][j + 1];             //{-1 , 0 , 1}
        temp[1] = -1*(short) imgGray[i - 1][j - 1] + (short) imgGray[i + 1][j - 1]    //{-1, -2, -1}
        -2*(short) imgGray[i - 1][j] + 2*(short) imgGray[i + 1][j]                    //{ 0,  0,  0}//横向梯度
        -1*(short) imgGray[i - 1][j + 1] + (short) imgGray[i + 1][j + 1];             //{ 1,  2,  1}

        temp[0] = abs(temp[0]);
        temp[1] = abs(temp[1]);

        temp[0]=(temp[0] < temp[1])?temp[1]:temp[0];        //哪个取哪个

        Exiacn[i][j]=(temp[0] > adaptThreshold)?Black:White;//如果梯度大于了经验阈值 也就是说该点周围灰度变化较大为边界点为了巡边界定为黑点

        return Exiacn[i][j];
}
/*******************************
函数名称： compare
函数作用： 差/和 判断是否黑白跳跃点 (x-y)/(x+y) 表现了 x与y 的差异
输入值 ：  x y 灰度
返回值 ：

*******************************/
uint8 compare(uint8 white, uint8 black)
{
    int c ,d ;
        float e ;
        c= ((int)white - (int)black);
        if(c <= 0 )
            {
                return 0;
            }
        d = ((int)white + (int)black);
        e = (d * 100)/ c;
        if(e >= Thresholdcompare)
            {
            return 1; //是跳变点
            }
        else
            {
            return 0;
            }
}
/*******************************
函数名称： Along_Wall
函数作用： 左手法制 右手法则 沿着墙走巡线
输入值 ：
返回值 ：

*******************************/
int dir_front[4][2] = {{0,  -1},{1,  0},{0,  1},{-1, 0}};
int dir_frontleft[4][2] = {{-1, -1},{1,  -1},{1,  1},{-1, 1}};
int dir_frontright[4][2] = {{1,  -1},{1,  1},{-1, 1},{-1, -1}};
void Along_Wall(void)
{
       uint8 lx = start_point_l[0];uint8 ly = start_point_l[1];
       uint8 rx = start_point_r[0];uint8 ry = start_point_r[1];
       points_l_num = sizeof(points_l) / sizeof(points_l[0]);//左边的边界点数
       points_r_num = sizeof(points_r) / sizeof(points_r[0]);//右边的边界点数

        uint8 LL_front_value;
        uint8 LL_frontleft_value;
        uint8 RL_front_value;
        uint8 RL_frontright_value;

        lstep = 0;
        rstep = 0;
            int  ldir = 0, lturn = 0;   // step表示前进的步数；dir通过改变索引改变当前小人朝向的方向
            int  rdir = 0, rturn = 0;
    while (lstep < points_l_num  &&  rstep < points_r_num &&
            lturn < 4 &&  rturn < 4 &&
             0 < ly &&
             0 < ry  )
            {

        _Exiacn[ly][lx] = 2;//上位机显示！！！！！！！！
        _Exiacn[ry][rx] = 3;//上位机显示！！！！！！！！

        LL_front_value = Exiacn[ly + dir_front[ldir][1]][lx + dir_front[ldir][0]];
        LL_frontleft_value = Exiacn[ly + dir_frontleft[ldir][1]][lx + dir_frontleft[ldir][0]];
        if (LL_front_value == Black)  // 前进方向像素为黑色
        {
            ldir = (ldir + 1) % 4;   // 遇到前方为黑色需要右转一次
            lturn++;
        }
        else if (LL_frontleft_value == Black) // 前方像素为白色，且左前方像素为黑色
        {
            lx += dir_front[ldir][0];
            ly += dir_front[ldir][1];
            points_l[lstep][0] = lx;
            points_l[lstep][1] = ly;
            lstep++;
            lturn = 0;
        }
        else  // 前方为白色，左前方为白色（墙角）
        {
            lx += dir_frontleft[ldir][0];   // 遇到墙角要斜着走
            ly += dir_frontleft[ldir][1];
            ldir = (ldir + 3) % 4;   // 遇到墙角要左转一次
            points_l[lstep][0] = lx;
            points_l[lstep][1] = ly;
            lstep++;
            lturn = 0;
         }


 ////////////////////////////////////////////////////////////////////////////////////
        RL_front_value= Exiacn[ry + dir_front[rdir][1]][rx + dir_front[rdir][0]];
        RL_frontright_value = Exiacn[ry + dir_frontright[rdir][1]][rx + dir_frontright[rdir][0]];
        if (RL_front_value == Black ) {
            rdir = (rdir + 3) % 4;
            rturn++;
        } else if (RL_frontright_value == Black) {
            rx += dir_front[rdir][0];
            ry += dir_front[rdir][1];
            points_r[rstep][0] = rx;
            points_r[rstep][1] = ry;
            rstep++;
            lturn = 0;
        } else {
            rx += dir_frontright[rdir][0];
            ry += dir_frontright[rdir][1];
            ldir = (ldir + 1) % 4;
            points_r[rstep][0] = rx;
            points_r[rstep][1] = ry;
            rstep++;
            rturn = 0;
        }


           }


    points_l_num = lstep;
    points_r_num = rstep;


}




/*******************************
函数名称： do_black_board
函数作用： 施加黑色边框
输入值 ：
返回值 ：

*******************************/
void do_black_board(void)
{
    for (int i = 0; i < my_row; i++) {
        for (int j = 0; j < my_col; j++) {
            if (i == 0 || i == my_row - 1 || j == 0 || j == my_col - 1) {
                imgGray[i][j] = 0;
            }
        }
    }
}


/******************************************************************************
* 鍑芥暟鍚嶇О     : getGrayscaleHistogram
* 鎻忚堪        : 鐏板害鐩存柟鍥剧粺璁�
* 杩涘叆鍙傛暟     : void值
* 杩斿洖鍙傛暟     : void
******************************************************************************/
void getGrayscaleHistogram(void)
{
    memset(histogram, 0, sizeof(histogram));
    uint8 *ptr = &imgGray[0][0];
    uint8 *ptrEnd = &imgGray[my_row - 1][_my_col - 1] + 1;
    while (ptr != ptrEnd)
    {
        ++histogram[*ptr++];
    }
}





/*******************************
函数名称
函数作用：
输入值 ：
返回值 ：

*******************************/
uint8 getExiacn(void)//澶ф触娉�
{
    getGrayscaleHistogram();

    uint32_t sum = 0, valueSum = 0;
    uint64_t sigma = 0, maxSigma = 0;
    float w1 = 0, w2 = 0;
    int32_t u1 = 0, u2 = 0;
    uint8 min = 0, max = 255;
    for (min = 0; histogram[min] == 0 && min < 255; ++min)
        ;
    for (max = 255; histogram[max] == 0 && max > 0; --max)
        ;

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
    previousThreshold = nowThreshold;
    if (nowThreshold < minThreshold)
    {
        nowThreshold = minThreshold;
    }
    if (nowThreshold > maxThreshold)
    {
        nowThreshold = maxThreshold;
    }



    for(uint8 i = 0 ; i < _my_col/2 ; i++)
    {
        for(uint8 j = 0 ; j < my_row ; j++)
        {
            if(imgGray[j][i*2] + imgGray[j][i*2-1] < 2*nowThreshold)
            {
                Exiacn[j][i] = 0;
            }
            else
            {
                Exiacn[j][i] = 255;
            }
        }
    }

    memcpy(_Exiacn,Exiacn,sizeof(Exiacn));
    return nowThreshold;
}

/*******************************
函数名称 ips114_draw_nine_point
函数作用：画点
输入值 ：
返回值 ：

*******************************/
void ips114_draw_nine_point(uint8 x ,uint8 y,const uint16 color)
{
    {
        int dx; int dy;
        for ( dx = -1; dx <= 1; dx++)
        {
             for ( dy = -1; dy <= 1; dy++)
                {
                         if(x + dx >= 0 && x + dx <=188)
                         { if(y+dy >=0&&y+dy<=120){
//                                 ips114_draw_point(x + dx, y + dy, color);
                             }
                         }
                }
        }

    }
}


