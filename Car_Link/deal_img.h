

#ifndef DEAL_IMG_H
#define DEAL_IMG_H
#include "headfile.h"
#define MT9V03X_W               (188)
#define MT9V03X_H               (70)
int ABS(int data);
#define II imgInfo  // 鍥惧儚淇℃伅缁撴瀯浣撶缉鍐�
#define IF imgFlag  // 鍥惧儚鏍囧織浣嶇粨鏋勪綋缂╁啓

#define LB(i) II.left_bottom[i]     // 宸︿笅鐐�
#define RB(i) II.right_bottom[i]    // 鍙充笅鐐�
#define LT(i) II.left_top[i]        // 宸︿笂鐐�
#define RT(i) II.right_top[i]       // 鍙充笂鐐�

#define BL(i) II.bottom_left[i]     // 宸︿笅鐐圭炕杞緱鍒扮殑鍙充笅鐐�
#define BR(i) II.bottom_right[i]    // 鍙充笅鐐圭炕杞緱鍒扮殑宸︿笅鐐�
#define TL(i) II.top_left[i]        // 宸︿笂鐐圭炕杞緱鍒扮殑鍙充笂鐐�
#define TR(i) II.top_right[i]       // 鍙充笂鐐圭炕杞緱鍒扮殑宸︿笂鐐�

#define LB_X(i) II.left_bottom[i].x     // 宸︿笅鐐圭殑妯潗鏍�
#define RB_X(i) II.right_bottom[i].x    // 鍙充笅鐐圭殑妯潗鏍�
#define LT_X(i) II.left_top[i].x        // 宸︿笂鐐圭殑妯潗鏍�
#define RT_X(i) II.right_top[i].x       // 鍙充笂鐐圭殑妯潗鏍�

#define BL_X(i) II.bottom_left[i].x  // 宸︿笅鐐圭炕杞緱鍒扮殑鍙充笅鐐圭殑妯潗鏍�
#define BR_X(i) II.bottom_right[i].x     // 鍙充笅鐐圭炕杞緱鍒扮殑宸︿笅鐐圭殑妯潗鏍�
#define TL_X(i) II.top_left[i].x    // 宸︿笂鐐圭炕杞緱鍒扮殑鍙充笂鐐圭殑妯潗鏍�
#define TR_X(i) II.top_right[i].x   // 鍙充笂鐐圭炕杞緱鍒扮殑宸︿笂鐐圭殑妯潗鏍�

#define LB_Y(i) II.left_bottom[i].y // 宸︿笅鐐圭殑绾靛潗鏍�
#define RB_Y(i) II.right_bottom[i].y    // 鍙充笅鐐圭殑绾靛潗鏍�
#define LT_Y(i) II.left_top[i].y    // 宸︿笂鐐圭殑绾靛潗鏍�
#define RT_Y(i) II.right_top[i].y   // 鍙充笂鐐圭殑绾靛潗鏍�

#define BL_Y(i) II.bottom_left[i].y  // 宸︿笅鐐圭炕杞緱鍒扮殑鍙充笅鐐圭殑绾靛潗鏍�
#define BR_Y(i) II.bottom_right[i].y     // 鍙充笅鐐圭炕杞緱鍒扮殑宸︿笅鐐圭殑绾靛潗鏍�
#define TL_Y(i) II.top_left[i].y    // 宸︿笂鐐圭炕杞緱鍒扮殑鍙充笂鐐圭殑绾靛潗鏍�
#define TR_Y(i) II.top_right[i].y   // 鍙充笂鐐圭炕杞緱鍒扮殑宸︿笂鐐圭殑绾靛潗鏍�

#define TOP_LM(i) TR_Y(i)   // 宸﹂粦绾挎渶楂樼偣鐨勭旱鍧愭爣
#define BOTTOM_LM(i) BR_Y(i)    // 宸﹂粦绾挎渶浣庣偣鐨勭旱鍧愭爣
#define TOP_RM(i) TL_Y(i)   // 鍙抽粦绾挎渶楂樼偣鐨勭旱鍧愭爣
#define BOTTOM_RM(i) BL_Y(i)    // 鍙抽粦绾挎渶浣庣偣鐨勭旱鍧愭爣
#define RIGHT(i) RT_X(i)    // 鏈�鍙宠竟鐨勭偣鐨勬í鍧愭爣
#define LEFT(i) LT_X(i) // 鏈�宸﹁竟鐨勭偣鐨勬í鍧愭爣

#define XM 47                                       //鍥惧儚鍘嬬缉浜�4鍊嶏紝鏈�澶х殑X鍧愭爣
#define YM 55  //70 - 15 2023骞�5鏈�30鏃�14:46:35         //鍥惧儚鍘嬬缉浜�4鍊嶏紝鏈�澶х殑Y鍧愭爣
#define XX 46                                       //鍥惧儚鍘嬬缉浜�4鍊嶏紝涓嬫爣妯潗鏍�
#define YY 54  //69 - 15 2023骞�5鏈�30鏃�14:46:39         //鍥惧儚鍘嬬缉浜�4鍊嶏紝涓嬫爣绾靛潗鏍�

#define COL 188//涓暟鍒�
#define ROW 70//涓暟琛�
#define _COL 187//涓嬫爣鍒�
#define _ROW 69//涓嬫爣琛�

#define _YES 1          //True
#define _NO 0           //False
#define noGarage 0        //鏃犺溅搴�
#define goLeft 1        //寰�宸﹁蛋
#define goRight 2       //寰�鍙宠蛋


#define NO_LINE 0       //鎵句笉鍒扮嚎
#define LEFT_LINE 1     //宸︾嚎妯″紡锛岀姸鎬�
#define RIGHT_LINE 2    //鍙崇嚎妯″紡锛岀姸鎬�
#define _LEFT_LINE 3
#define _RIGHT_LINE 4

//障碍
#define OBLeft 1
#define OBRight 2
//鏍囧織鈫撯啌鈫�
//妯柇鐘舵��
#define OL1 1
#define OL2 2
#define OL3 3

#define OR1 4
#define OR2 5
#define OR3 6

#define Out 1
#define Straight 2
#define Back 3
#define DELAY 4
//鍦嗙幆鐘舵��
#define AL1 1
#define AL2 2
#define AL3 3
#define AL4 4
#define AL5 5

#define AR1 7
#define AR2 8
#define AR3 9
#define AR4 10
#define AR5 11

//鍥炵幆鐘舵�乧ro
#define BAL1 1
#define BAL2 2
#define BAL3 3
#define BAL4 4

#define BAR1 6
#define BAR2 7
#define BAR3 8
#define BAR4 9

//宀旇矾鐘舵��
#define FL1 1
#define FL2 2
#define FL3 3
#define FL4 4
#define FL5 5

#define FR1 7
#define FR2 8
#define FR3 9
#define FR4 10
#define FR5 11

//鍏ュ簱鐘舵��
#define PL1 1
#define PL2 2
#define PL3 3
#define PL4 4

#define PR1 6
#define PR2 7
#define PR3 8
#define PR4 9

//鍑哄簱鐘舵��
#define GL1 1
#define GL2 2
#define GL3 3

#define GR1 6
#define GR2 7
#define GR3 8


//鏍囧織鈫戔啈鈫�
#define IMG_SIZE (ROW * COL)    // 鍥惧儚澶у皬
#define MAP_SIZE (YM * XM)  // 鍦板浘澶у皬

#define STEP 35
#define STOP_LINE 50
#define MAXSIZE 2800

typedef struct ImgInfo {
  uint8 num_lm; // 宸︽�诲尯鍩熸暟
  uint8 num_rm; // 鍙虫�诲尯鍩熸暟

  uint8 _num_lm;  // go鏂囦欢閲岄潰鎼滃嚭鏉ョ殑宸﹀尯鍩熸暟
  uint8 _num_rm;  // go鏂囦欢閲岄潰鎼滃嚭鏉ョ殑鍙冲尯鍩熸暟
uint8 num_dm;
  uint8 start_lm[5];    // 宸﹀尯鍩熺殑璧峰琛屾暟
  uint8 start_rm[5];    // 鍙冲尯鍩熺殑璧峰琛屾暟

  Point left_bottom[5];   // 宸︿笅鐐规暟缁�
  Point right_bottom[5];  // 鍙充笅鐐规暟缁�
  Point left_top[5];  // 宸︿笂鐐规暟缁�
  Point right_top[5]; // 鍙充笂鐐规暟缁�

  Point bottom_left[5];   // 宸︿笅鐐圭炕杞緱鍒扮殑鍙充笅鐐规暟缁�
  Point bottom_right[5];  // 鍙充笅鐐圭炕杞緱鍒扮殑宸︿笅鐐规暟缁�
  Point top_left[5];  // 宸︿笂鐐圭炕杞緱鍒扮殑鍙充笂鐐规暟缁�
  Point top_right[5]; // 鍙充笂鐐圭炕杞緱鍒扮殑宸︿笂鐐规暟缁�

  Point d_bottom;   // 鍙充笅瑙掍笉鑳藉埌杈鹃儴鍒嗙殑搴曢儴鐐�
  Point d_left_bottom[5];  // 鍙充笅瑙掍笉鑳藉埌杈鹃儴鍒嗙殑宸︿笅鐐规暟缁�
  Point d_right_bottom[5]; // 鍙充笅瑙掍笉鑳藉埌杈鹃儴鍒嗙殑鍙充笅鐐规暟缁�

  uint8 left_x;   // 宸︿笅瑙掔偣鐨勬í鍧愭爣
  uint8 left_y;   //宸﹁竟鍗佸瓧瑙掔偣

  uint8 right_x;  // 鍙充笅瑙掔偣鐨勬í鍧愭爣
  uint8 right_y;  //鍙宠竟瀛楄鐐�

  uint8 _top; // 鍗佸瓧浜ゅ弶浣嶇疆鐨勬í鍧愭爣
  uint8 top;  // 涓婇潰鐨勭洿绾夸綅缃殑琛屾暟
  uint8 last_top;//涓婁竴娆＄櫧鑹茶禌閬撴渶椤惰
  uint8 bottom;   // 涓嬮潰鐨勭洿绾夸綅缃殑琛屾暟
  uint8 speedTop; // 瓒呴�熷鐨勪綅缃殑琛屾暟
  uint8 annulusTop; // 鍦嗙幆浣嶇疆鐨勮鏁�

  uint8 stop_line;  // 鎼滃浘鎴琛屾暟
  uint8 limit_line; // 鎸℃澘浣嶇疆鐨勮鏁�
  uint8 step;   // 鎵弿绾夸箣闂寸殑琛屾暟
  uint8 line_forbid;  // 鍦ㄧ洿閬撲笂鏄惁鍙互鎵埌榛戠嚎鐨勬爣蹇椾綅
  uint16_t repeatNum;   //宸﹀彸涓ゅ浘涓繖涓偣鍧囦负榛戣壊鍒欏垽瀹氳繖涓偣涓洪噸澶�
  uint16_t dnum_top;    // 鍙充笅瑙掔偣鍒拌揪涓嶄簡鐨勯儴鍒嗕腑涓婇潰鐨勯粦鐐规暟閲�
  uint16_t bnum_all;    // basemap黑点数
  uint16_t lnum_all;    // leftmap黑点数
  uint16_t rnum_all;    // rightmap黑点数
  uint16_t dnum_all;    // detelemap黑点数
  uint16_t lnum[5]; // leftmap涓簲涓綋涓叾涓竴涓腑鐨勯粦鐐规暟
  uint16_t rnum[5]; // rightmap涓簲涓綋涓叾涓竴涓腑鐨勯粦鐐规暟

  uint16_t lmin;  // leftmap涓殑鏈�灏忛粦鐐规暟
  uint16_t rmin;  // rightmap涓殑鏈�灏忛粦鐐规暟
  uint16_t leftnum;   // 宸﹁竟鍖哄煙榛戠偣鏁伴噺鎬诲拰
  uint16_t rightnum;  // 鍙宠竟鍖哄煙榛戠偣鏁伴噺鎬诲拰
  uint16_t midnum;    // 涓棿鍖哄煙榛戠偣鏁伴噺鎬诲拰
  uint16_t meetTop;   // 浼氳溅鍖哄煙绔栫嚎鐨勭旱鍧愭爣
} ImgInfo;    // 鍥惧儚淇℃伅缁撴瀯浣�


typedef struct ImgFlag
{
    uint8 sramp;          //涓婂潯
    uint8 ramp;
    uint8 annulus;        //鍦嗙幆
    uint8 annulusDelay;   //鍑哄渾鐜欢鏃�
    uint8 obstacle;
    uint8 startline;      //璧风偣鏍囧織浣�
} ImgFlag;//鍥惧儚鏍囧織浣嶇粨鏋勪綋

typedef struct
{
    uint16_t numCnt;        //榛戠偣鏁伴噺
    Point right_bottom;     //宸﹀浘榛戣壊鍖哄煙鏈�鍙宠竟鐨勫簳閮ㄧ偣
    Point right_top;        //宸﹀浘榛戣壊鍖哄煙鏈�鍙宠竟鐨勯《閮ㄧ偣
    Point bottom_right;     //宸﹀浘榛戣壊鍖哄煙鏈�搴曢儴闈犲彸杈圭殑鐐�
    Point top_right;        //宸﹀浘榛戣壊鍖哄煙鏈�椤堕儴闈犲彸杈圭殑鐐�
} RegionInfoLeft;           //宸﹁竟鍖哄煙鐨勪俊鎭�

typedef struct
{
    uint16_t numCnt;        //榛戠偣鏁伴噺
    Point left_bottom;      //鍙冲浘榛戣壊鍖哄煙鏈�宸﹁竟鐨勫簳閮ㄧ偣
    Point left_top;         //鍙冲浘榛戣壊鍖哄煙鏈�宸﹁竟鐨勯《閮ㄧ偣
    Point bottom_left;      //鍙冲浘榛戣壊鍖哄煙鏈�搴曢儴闈犲彸杈圭殑鐐�
    Point top_left;         //鍙冲浘榛戣壊鍖哄煙鏈�椤堕儴闈犲彸杈圭殑鐐�
} RegionInfoRight;//鍙宠竟鍖哄煙鐨勪俊鎭�

typedef struct PointNode//鏍�
{
    uint8 num;
    Point point[XX];
} PtStack;

typedef struct
{
    uint16_t cnt;//鐜舰閬撳唴閮ㄥ寘鍚殑鍏冪礌涓暟
    float sumDev;//鐜舰閬撳唴鎵�鏈夊厓绱犵殑鎬诲亸宸�
    float sumDK;//鐜舰閬撳唴鎵�鏈夊厓绱犵殑鎬籇K鍊�
    uint8 flag;//鐜舰宀涚殑鏍囧織浣�
} AnnulusDEV;//鐜舰閬撶粨鏋勪綋
extern float my_Yaw;
extern float offset;
extern float keep_Yaw;
extern uint8 draw_line ;
extern uint8 draw_line_y;
extern uint8 draw_line_x;
static uint8 Annulus_dir;

extern uint8 img_threshold_group[3];//鍒掑垎闃堝�煎尯鍩�
extern uint8 inum_all_threshold ;   //鏂戦┈绾块粦鐐规暟闃堝��
extern uint8 previousThreshold;   //涓婁竴涓槇鍊�
extern uint8 nowThreshold;        //褰撳墠闃堝��
extern uint16_t edgeThreshold;      //闃堝��
extern uint16_t minThreshold;       //鏈�灏忛槇鍊�
extern uint16_t maxThreshold;       //鏈�澶ч槇鍊�
extern uint16_t startRow;           //璧峰琛�
extern uint16_t endRow;             //缁堟琛�
extern uint16_t numCnt;             //榛戠偣鏁伴噺

extern uint8 obstacle_deal_flag; //姝ｅ湪澶勭悊鍦嗙幆鏍囧織銆�
extern uint16_t histogram[256];     //鐏板害鍥惧儚鐨勭洿鏂瑰浘
extern uint8 imgGray[MT9V03X_H][MT9V03X_W];   //鐏板害鍥�
extern uint8 imgsend_1[MT9V03X_H][MT9V03X_W]; //wifi需要发送的图像数组一号
extern uint8 imgsend_2[MT9V03X_H][MT9V03X_W]; //wifi需要发送的图像数组二号
extern uint8    dealimg_finish_flag;       // 一场图像采集完成标志位
extern uint8    dealimg_send_flag;       // wifi发送的数组号数
extern uint8 basemap[YM][XM];     //鍩虹鍦板浘锛屾爣瀹氬墠鐢ㄤ簬璁＄畻鍋忕Щ
extern uint8 leftmap[YM][XM];     //宸﹁溅閬撶嚎鍦板浘
extern uint8 rightmap[YM][XM];    //鍙宠溅閬撶嚎鍦板浘
extern uint8 insidemap[YM][XM];   //杞﹂亾鍐呴儴鍦板浘
extern uint8 deletemap[YM][XM];   //鍒犻櫎鍦板浘
extern uint8 allmap[YM][XM];      //鎵�鏈夊湴鍥�
extern uint8 midline[YM / 2];     //褰撳墠鍦烘櫙涓墍鏈夎溅閬撶嚎涓績鐐圭殑妯潗鏍�
extern uint8 linemap[YM][XM];     //杞﹂亾绾垮湴鍥�
extern float leftline[YM];          //璁板綍鐨勬槸宸﹁竟鐨勬爣鍑嗙嚎
extern float rightline[YM];         //璁板綍鐨勬槸鍙宠竟鐨勬爣鍑嗙嚎
extern float k1[YM];                //鏍囧噯鐩寸嚎宸﹁竟鏂滅巼
extern float k2[YM];                //鏍囧噯鐩寸嚎鍙宠竟鏂滅巼  閫嗛�忚

extern AnnulusDEV AD;               //鐜舰閬撶殑鏁版嵁
extern AnnulusDEV AD_INIT;          //鐜舰閬撳垵濮嬪寲鏁版嵁
extern ImgInfo imgInfoInit;         //鍒濆鍥惧儚淇℃伅
extern ImgInfo imgInfo;             //褰撳墠鍥惧儚淇℃伅
extern ImgFlag imgFlag;             //褰撳墠鍦烘櫙鏍囧織浣�
extern ImgFlag LAST_IF;             //瀹氫箟涓婁竴涓満鏅爣蹇椾綅
extern uint16_t delaytime_obstacle_threshold_1;//妯柇1闃舵寤舵椂闃堝��
extern uint16_t delaytime_obstacle_threshold_2;//妯柇2闃舵寤舵椂闃堝��
extern uint16_t delaytime_ramp[5];//鍧￠亾寤舵椂闃堝��
extern uint16_t delaytime_Annulus;//鍦嗙幆寤舵椂闃堝��
extern uint16_t delaytime_inloseroad;
extern uint16_t delaytime_outloseroad;
extern uint16_t protect_time;
extern uint8 straight_clear;
extern uint8 speedlineLeft[YM];   //閫熷害鏈�蹇椂宸﹁溅閬撶嚎鍦板浘
extern uint8 speedlineRight[YM];  //閫熷害鏈�蹇椂鍙宠溅閬撶嚎鍦板浘
extern int8_t bottom_line;          //璇嗗埆鐨勬渶搴曢儴杞﹂亾绾�
extern int8_t top_line;             //璇嗗埆鐨勬渶椤堕儴杞﹂亾绾�   鎴浘鑼冨洿
extern PtStack noDown;              //涓�鏃︿粠杞﹂亾涓婁笅鍘诲悗锛岃溅瑕佺户缁墠琛岋紝noDown鏍囧織鍛婅瘔浜嗗皬杞﹀綋鍓嶈溅閬撶嚎鐨勭紪鍙�
extern uint8 leftlineZero;        //宸﹁竟杞﹂亾鏄惁寰�宸�
extern uint8 rightlineZero;       //鍙宠竟杞﹂亾鏄惁寰�鍙�
extern uint8 deleteforleftflag;   //鏄惁鍒犻櫎浜嗗乏杞﹂亾鍦板浘
extern uint8 deleteforrightflag;  //鏄惁鍒犻櫎浜嗗彸杞﹂亾鍦板浘
extern uint8 ob_direction;//障碍方向
extern uint16_t nowAnnulusNum;      //褰撳墠鍦烘櫙涓殑鐜舰鏁扮洰
extern uint16_t nowForkRoadNum;     //褰撳墠鍦烘櫙涓笁宀斿垪琛�
extern uint16_t nowGarageNum;       //褰撳墠鍦烘櫙涓溅搴撴暟鐩�
extern uint16_t nowRampNum;
extern uint16_t _nowRampNum;
extern uint16_t nowobstacleNum;
extern uint16_t nowloseroadNum;
extern int8_t Dir_Out;              //璁板綍鍑鸿杞﹂亾瑕佸乏杩樻槸鍙�
extern uint8 mid_height;
extern float infraredrayDis;
extern void getGrayscaleHistogram(void);// 鑾峰彇鐏板害鍥惧儚鐨勭洿鏂瑰浘
extern uint8 getOSTUThreshold(void);  //浣跨敤闃堝�兼渶閫傚簲鐨勬柟娉曟潵纭畾闃堝��
extern uint8 ostuJudge(uint8 x, uint8 y);//姹傚嚭鏌愪釜鐐圭殑闃堝�肩粨鏋�
extern void OSTU_ROBERT(void);          //闃冲厜绠楁硶
extern void searchimg(uint8 x, uint8 y);    //瀵绘壘鍥惧儚涓婄殑榛戣壊鍖哄煙
extern void searchleftmap(uint8 x, uint8 y);//瀵绘壘鏁翠釜鍥惧儚宸﹀崐閮ㄥ垎鎵�鏈夊彲鑳芥槸杞﹂亾涓績鐨勪綅缃�
extern void searchrightmap(uint8 x, uint8 y);//瀵绘壘鏁翠釜鍥惧儚鍙冲崐閮ㄥ垎鎵�鏈夊彲鑳芥槸杞﹂亾涓績鐨勪綅缃�
extern void Get_insideMap(void);        //鑾峰緱杞﹂亾鍐呴儴鍦板浘
extern void searchdeletemap2(uint8 x, uint8 y);//瀵绘壘鍧忕偣锛屾妸鍧忕偣浠庡綋鍓嶅湴鍥句腑鍘婚櫎
extern void searchdeletemap(uint8 x, uint8 y);//灏嗚垗鍘讳笉鍚堟牸鐨勮溅閬撶嚎
extern void map_init(void);             //鍒濆鍖栬溅閬撶嚎銆侀�佹护鏉°�佽繃婊よ捣鐐广�佺涓�鏉＄嚎浠ュ鐨勭涓�鏉＄嚎銆佺洿绾跨鐐�
extern void standard(void);             // 鏍囧噯鍖栥�佹爣瀹氬垵濮嬪寲

extern void getRegionInfoLeft(uint8 x, uint8 y, uint8 src[][XM],uint8 dst[][XM], RegionInfoLeft *r);//鑾峰彇宸﹁竟鍖哄煙鐨勪俊鎭�
extern void getRegionInfoRight(uint8 x, uint8 y, uint8 src[][XM],uint8 dst[][XM], RegionInfoRight *r);//鑾峰彇鍙宠竟鍖哄煙鐨勪俊鎭�
extern uint16_t cntMap(uint8 x, uint8 y);                                               //瀵绘壘榛戠偣鏁伴噺
uint16_t cntDeleteMap(uint8 x, uint8 y, uint8 map[][XM]);//瀛樺偍鍧忕偣鐨勬暟閲�
extern void searchCountmap(uint8 x, uint8 y, uint8 src[][XM]);//姹傛煇涓偣鍛ㄥ洿鐨勯粦鐐规暟閲�
extern void searchmap(uint8 x, uint8 y, uint8 src[][XM]);//瀵绘壘鏁翠釜鍦板浘涓師鏉ヤ笉灞炰簬杞﹂亾绾跨殑鍧愭爣
extern uint8 isSearch(uint8 x, uint8 y, uint8 map[][XM]);//鍒ゆ柇浠庡綋鍓嶇偣寮�濮嬭兘涓嶈兘鎼滃埌鏁磋杞﹂亾绾�
uint8 getMapYMin_Col2(uint8 x, uint8 y, uint8 map[][XM]);//鑾峰彇杞﹂亾绾跨殑搴曢儴y鍧愭爣
uint8 getMapYMin_Col(uint8 x, uint8 map[][XM], uint8 value);//鑾峰彇鏁版嵁瀛樺偍涓渶灏忕殑Y鍧愭爣锛堝垪淇℃伅锛�
uint8 getMapYMax_Col(uint8 x, uint8 map[][XM], uint8 value);//鑾峰彇鏁版嵁瀛樺偍涓渶澶х殑Y鍧愭爣锛堝垪淇℃伅锛�
uint8 getMapXMax_Row(uint8 y, uint8 map[][XM], uint8 value);//鑾峰彇鏁版嵁瀛樺偍涓渶灏忕殑X鍧愭爣锛堣淇℃伅锛�
uint8 getMapXMin_Row(uint8 y, uint8 map[][XM], uint8 value); // 鑾峰彇瀵瑰簲鍧愭爣鐨勮锛屽父瑙佺殑浣跨敤鍦烘櫙鏄壘鏌愪釜鍧愭爣瀵瑰簲鐨勭殑鏈�宸﹁竟鐨勫儚绱犵偣锛屾垨閫嗗簭鎵炬渶鍙宠竟鐨勫儚绱犵偣銆�
uint8 deleteforleft(uint8 undelete_flag); // 鍒犻櫎宸﹁溅閬撶嚎鍦板浘鐨勪竴琛岀殑鏂规硶锛屽幓鎺夊彲鑳藉共鎵扮殑閮ㄥ垎銆�
uint8 deleteforright(uint8 undelete_flag); // 鍒犻櫎鍙宠溅閬撶嚎鍦板浘鐨勪竴琛岀殑鏂规硶锛屽幓鎺夊彲鑳藉共鎵扮殑閮ㄥ垎銆�
uint8 crossroadforleft(void);                         // 宸﹀叆鍗佸瓧
uint8 crossroadforright(void);                        // 鍙冲叆鍗佸瓧

uint8 X_WBW_Detect(int8_t x1, int8_t x2, uint8 y, uint8 map[][XM],uint8 flag); // 绔栫洿鏂瑰悜涓婄殑榛戠櫧鐩搁棿杩為�氬煙闀垮害娴嬮噺鍑芥暟
uint8 Y_WBW_Detect(uint8 y1, uint8 y2, uint8 x, uint8 map[][XM]); // 姘村钩鏂瑰悜涓婄殑榛戠櫧鐩搁棿杩為�氬煙闀垮害娴嬮噺鍑芥暟
uint8 Y_BWB_Detect(uint8 y1, uint8 y2, uint8 x, uint8 map[][XM]); // 姘村钩鏂瑰悜涓婄殑鐧介粦鐩搁棿杩為�氬煙闀垮害娴嬮噺鍑芥暟
uint8 X_B2W_Num(int y1, int y2, uint8 map[][XM], uint8 dir);//鎵惧埌鐩稿簲y鑼冨洿鍐� 浠庨粦鍒扮櫧鐨勭偣鏁�

void deleteline(void);                  // 鍒犻櫎宸﹀彸杞﹂亾绾挎煇浜涜
void Get_insideMap(void);               // 鑾峰彇杞﹂亾鍐呴儴鍦板浘
void getSpeedTop(void);                 // 璋冪敤鐜矝澶勭悊绋嬪簭
uint8 goAnnulus(void);                // 澶勭悊杩涘叆鐜矝鍦烘櫙鐨勬儏鍐�
uint8 leftAnnulusDetect(void);        // 妫�娴嬪乏杩涚幆宀�
uint8 rightAnnulusDetect(void);       // 妫�娴嬪彸杩涚幆宀�
uint8 isEnter(uint8 dir);       // 鍒ゆ柇鏄惁杩涘叆鍒嗘敮鍙�
uint8 leave(uint8 dir);         // 鍒ゆ柇鏄惁绂诲紑鍒嗘敮鍙�
uint8 AnnulusDeal(uint8 ADir, uint8 status); // 澶勭悊鍒嗘敮鍙ｉ儴鍒嗗嚱鏁�
void drawline(uint8 x1, uint8 y1, uint8 x2, uint8 y2,uint8 map[][XM]); // y1>y2
void drawline2(uint8 x1, uint8 y1, uint8 x2, uint8 y2,uint8 map[][XM], uint8 n); //鍦╩ap鍥惧儚涓婄敾绾�
void drawline3(uint8 x1, uint8 y1, uint8 x2, uint8 y2,uint8 map[][XM]); //鐢荤嚎
uint8 strJudge(uint8 x1, uint8 y1, uint8 x2, uint8 y2,uint8 map[][XM], uint8 sy1, uint8 sy2, int8_t limit,uint8 errorNum); //妫�娴嬩袱涓偣鏄惁鍦ㄦ爣鍑嗙殑璺濈涓�
uint8 strJudge_X(uint8 x1, uint8 y1, uint8 x2, uint8 y2,uint8 map[][XM], uint8 sx1, uint8 sx2, int8_t limit,uint8 errorNum);
uint8 getDown(void); // 鑾峰緱涓嬩竴姝ュ皬杞︽墍鍦ㄧ殑杞﹂亾
float getAveDeviation(ControlLine cl); // 鑾峰彇涓�涓溅閬撶嚎鐨勪腑蹇冩í鍧愭爣鍜屽亸绉婚噺
uint8 getline(float kb[2], ControlLine cl); // 鎷熷悎涓�鏉′腑蹇冭溅閬撶嚎
uint8 ramp(void); // 鍒ゆ柇鏄惁杩涘叆鍧￠亾
uint8 goRamp(void); // 鍧￠亾澶勭悊鍑芥暟
uint8_t straightramp(void);
uint8 goonceRamp(void);
void deleterampline(void); // 鍒犻櫎鍘嬪湪鍧￠亾涓婄殑杞﹂亾绾垮簳绾胯
void ResearchDeletemap(uint8 x, uint8 y, uint8 src[][XM]); // 杩炵画鐨勫垹闄-Y鐐�
void OutPark(void); // 澶勭悊鍑哄仠杞﹀満绋嬪簭
uint8 detectStartline(void); // 绗竴绉嶆ā寮忓鎵惧垵璧涙爣绾�
uint8 detectStartline_roughly(void); // 绮楃硻鐨勫鎵惧垵璧涙爣绾跨殑杩戜技鏂规
uint8 cnt_WBW_num(uint8 x1, uint8 x2, uint8 y, uint8 map[][XM]); // 璁℃暟涓�琛岄粦鐧界浉闂寸殑闀垮害
uint8 detectStartline_v2(void);//妫�娴嬭捣璺戠嚎浜屽�煎寲鏄惁姝ｇ‘
uint8 isEnter_Garage(uint8 dir);//鎺ユ敹涓�涓柟鍚戝弬鏁� dir锛屽垽鏂槸鍚﹁繘鍏ヨ溅搴�
uint8 ParkingDeal(uint8 dir, uint8 status);//澶勭悊鍋滆溅鍦哄満鏅殑鎯呭喌锛屾牴鎹甦ir鍜宻tatus鏉ュ垽鏂�
uint8 findRampLine_L(void);//瀵绘壘鍧￠亾宸﹁竟杞﹂亾绾�
uint8 findRampLine_R(void);//瀵绘壘鍧￠亾鍙宠竟杞﹂亾绾�
uint8 find_out(uint8 status);//澶勭悊鍦烘櫙锛氫粠璺彛鍑哄幓
uint8 sramp(void);//澶勭悊鍧￠亾閮ㄥ垎鐨勫満鏅�
uint8 outGarage(uint8 dir);//鎺ユ敹涓�涓柟鍚戝弬鏁癲ir锛屾墽琛岃溅浣嶅嚭杞﹀簱鍔ㄤ綔
uint8 outGarageDeal(uint8 dir, uint8 status);//澶勭悊鍑鸿溅搴撳満鏅殑鍒嗘敮鎯呭喌
uint8 getParkingline(float kb[2], ControlLine cl);//璁＄畻杞︿綅鎺у埗绾�, kb鏁扮粍涓槸鐩寸嚎鏂圭▼y=kx+b鐨勭郴鏁帮紝 cl鏄溅閬撲俊鎭�
uint8 BackAnnulusDeal(uint8 dir, uint8 status);//澶勭悊浠庣幆宀涘洖鏉ョ殑鎯呭喌
uint8 isEnter_BackAnnulus(uint8 dir);//鍒ゆ柇鏄惁杩涘叆浜�    鍚庨��杩涘叆鐜矝  鐨勬儏鍐�
uint8 go_obstacle(uint8 dir);//妯柇澶勭悊
uint8 go_loseroad(void);
uint8 find_ob_cross(uint8 *x, uint8 *y, uint8 map[][XM], uint8 mapflag);
uint8 loseroad_Detect(void); //妯柇璺殰
uint8 Ob_Img_Character(void); //闅滅銆佹柇璺浘鍍忕壒寰�
uint8 goObstacle();//平移边线的方法很好，但不能在原图上平移，因为原图上的平移不等于变换后的图上的平移，直接给截距加上偏置则可以达到此效果
extern uint8 tips_temp;
void get_position(int control_x, uint8 map_flag, int straight_cut) ;//鑾峰彇璺殰鍒ゆ垚鐬棿鐨勮溅韬�炬枩瑙掑害
int get_down_x(uint8 flag);
float get_correct(uint8 dir, int d_x);
uint8 Obstacle_back(uint8 dir);

#endif // DEAL_IMG_H
