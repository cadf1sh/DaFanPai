/*
 * go.c
 *
 *  Created on: 2020年10月29日
 *      Author: HP
 */
#include "headfile.h"
#include "deal_img.h"
#include <QTime>
#include <time.h>
#include "widget.h"
extern uint8 nowThreshold;
uint8 mid = 23;
extern int reset_flag;
uint8 close_Threshold = 5;
uint8 far_Threshold = 8;
uint8 mid_Threshold = 0;
uint8 always_camera_flag = 0;
uint8 always_camera_delay = 0;
uint8 toptemp;
uint8 speedtype;
uint8 delay_garage_flag = 0;
uint8 delay_garage = 0;
void go(void)
{
    map_init();//鍒濆鍖�
    getGrayscaleHistogram();//鑾峰彇鐏板害鍥�
    if( IF.ramp )//坡道的时候锁住最后一帧的阈值，防止摄像头抬头导致图像紊乱
    {
    nowThreshold= nowThreshold +1 -1 ;
    }
    else
    {
    nowThreshold = getOSTUThreshold();/*大津法获取阈值*/
    img_threshold_group[2]=nowThreshold + close_Threshold; //近景       //近景。 少加，能看清最近
    img_threshold_group[1]=nowThreshold - mid_Threshold;   //中景
    img_threshold_group[0]=nowThreshold - far_Threshold;   //远景       ///远景。，多降，能清晰识别更远
    }
    OSTU_ROBERT();//澶ф触娉�
    II.last_top = toptemp;
    for (uint8 i = 0; i < XM; ++i)//閬嶅巻鍥惧儚涓嬫柟绗竴琛� i鏄垪鏁�
    {
        if (mid + i < XM  && !allmap[0][mid + i])//鍥惧儚宸︿晶濡傛灉閬囧埌涓�涓櫧鐐瑰垯杩涘叆searching
        {
            searchimg(mid + i, 0);
            break;
        }
        if (mid >= i && !allmap[0][mid - i])//鍥惧儚鍙充晶濡傛灉閬囧埌涓�涓櫧鐐瑰垯杩涘叆searching
        {
            searchimg(mid - i, 0);
            break;
        }
    }

    if (IF.annulus )//圆环特有的step调整功能
    {
        uint8 down = getDown();//鎵嚎琛�
        II.step = down < 50 ? down : 50;
    }
    if(II.bnum_all)//濡傛灉basemap琚亶鍘嗚繃鍚庡浘瀛樺湪鐧界偣
    {
        for(uint8 j = 0; j < II.step; ++j)
        {
            if(basemap[j][0] && !leftmap[j][0] && !rightmap[j][0] &&//濡傛灉basemap鏄粦锛屽苟涓斿乏鍙冲浘閮芥槸榛樿鐨勭櫧
               (II.num_lm == 0 || (II.num_lm > 0 && II.num_lm < 5 && isSearch(0, j, rightmap))))//褰擄紙绗竴娆″惎鍔�    鎴�    宸﹀尯鍩熷ぇ浜�0涓斿皬浜�5鐨勬椂鍊欙級锛屽苟涓斿湪 骞朵笖鍦ㄧ0鍒楁壘鍒颁簡榛戠櫧璺冲彉鐐广��
            {
                //绗竴娆¤繘鍏earchleftmap鏃朵负锛岀0鍒楋紝鏈�宸︿笅榛戣壊鍖哄煙鐨勬渶涓嬩竴琛�
                searchleftmap(0, j);//瀵规瘡涓�涓乏閮ㄥ尯鍩熻繘琛岄亶鍘嗭紝鑾峰彇leftmap锛屽苟鍦╨eftmap涓彁鍙栫壒寰佺偣
                II.lnum_all += II.lnum[II.num_lm];//宸﹀浘榛戠偣鏁�
                II.start_lm[II.num_lm] = j;//璁板綍涓嬪乏杈规瘡涓粦鑹插皝闂尯鍩熺殑鏈�搴曚笅涓�琛�
                ++II.num_lm;//宸﹂粦鑹插尯鍩熸暟鍔犱竴
                ++II._num_lm;
            }
            if(basemap[j][XX] && !leftmap[j][XX] && !rightmap[j][XX] &&
               (II.num_rm == 0 || (II.num_rm > 0 && II.num_rm < 5 && isSearch(XX, j, leftmap))))//褰擄紙绗竴娆″惎鍔�    鎴�    鍙冲尯鍩熷ぇ浜�0涓斿皬浜�5鐨勬椂鍊欙級锛屽苟涓斿湪 骞朵笖鍦ㄧ鏈�鍚庝竴鍒�46鍒楁壘鍒颁簡榛戠櫧璺冲彉鐐广��
            {
                searchrightmap(XX, j);//绗竴娆¤繘鍏earchrihjtmap鏃朵负锛屾渶鍚庝竴鍒楋紝鏈�鍙充笅榛戣壊鍖哄煙鐨勬渶涓嬩竴琛�
                II.rnum_all += II.rnum[II.num_rm];//宸﹀浘榛戠偣鏁�
                II.start_rm[II.num_rm] = j;//璁板綍涓嬪彸杈规瘡涓粦鑹插皝闂尯鍩熺殑鏈�搴曚笅涓�琛�
                ++II.num_rm;//鍙抽粦鑹插尯鍩熸暟鍔犱竴
                ++II._num_rm;
            }
        }
    }


    uint8 left = 0;
    uint8 right = XX;
    for (int8_t i = mid; i >= 0; --i)
    {
        if (basemap[0][i])//    濡傛灉杩欎釜鐐规槸榛戣壊鐨�
        {
            left = i;
            break;
        }
    }


    for (uint8 i = mid; i < XM; ++i)
    {
        if (basemap[0][i])//    濡傛灉杩欎釜杩欎釜鐐规槸榛戣壊
        {
            right = i;//        鍒欒繖0琛岀殑鍙崇偣鏄繖涓�
            break;
        }
    }


    mid = (left + right) / 2;// 姹備竴涓嬬闆惰鐨勪腑绾�

    if (II.bnum_all)//basemap瀛樺湪鐧界偣鏁帮紙璇″紓鐨勫湴鏂癸級
    {
        if (!IF.ramp)//娌℃湁鍧￠亾鐨勮瘽
        {
            crossroadforleft();  // 十字元素左侧边线的删减，同时包含上下区域
            crossroadforright(); // 十字元素右侧边线的删减，同时包含上下区域
        }
        deleteline();      //急弯情况下的回环删线，控制弯道路径的重点
        Get_insideMap();//搜出斑马线
        getSpeedTop();
    }
        //鍦嗙幆
        if ( ! IF.ramp )
        {
            IF.annulus = goAnnulus();//宸�01 2 3 4 5 6      鍙�6 7 8 9 10 11 12
        }

        //鍧￠亾
        if ( !( IF.annulus || IF.annulusDelay ) )
        {
        IF.ramp = goRamp();
        }
        if(IF.annulus==0&&IF.ramp==0)
          IF.obstacle = goObstacle();
    directionControl();
    speedtype = getSpeedType();
    toptemp = II.top;

    if(reset_flag)
    {
        IF.ramp = 0 ;
        IF.startline = 0;
        IF.obstacle =0;
        IF.annulus = 0 ;
        IF.annulusDelay = 0 ;
        reset_flag = 0;
    }
}
