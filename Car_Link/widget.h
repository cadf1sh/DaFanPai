#ifndef WIDGET_H
#define WIDGET_H
#include "headfile.h"
#define SECTOR_START 31808//1024 ///2048
#define SECTOR_SIZE 270
#define IMG_SIZE (*)
#define SECTOR_NUM_IMGBUFF 30
#define SECTOR_SIZE_IMGBUFF (SECTOR_SIZE * SECTOR_NUM_IMGBUFF)
#define MAP_START 100
#include <opencv2/imgproc/types_c.h>
#include <QtCharts>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QTextEdit>
#include <QMovie>
#include <QSplashScreen>
#include "QSerialPortInfo"
#include <QSerialPort>
#include <QMessageBox>
#include <QColorDialog>
using namespace std;
using namespace cv;

typedef enum ReadType
{
    dataType,
    imgType,
} ReadType;

typedef enum ImgSize
{
    binaryImg,  //二值图
    grayscaleImg,  //灰度图
} ImgSize;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void initChart();
    void drawLine();
    void DeleteLine();
public slots:
    /*slot function**********************************************************************************/
    //声明voltage的槽函数

    void timerDeal();
private slots:
    void on_readImg_pushButton_clicked();
    void on_readData_pushButton_clicked();
    void on_readSD_pushButton_clicked();
    void on_SDimgGray_pushButton_clicked();
    void on_run_pushButton_clicked();
    void on_Pic_num_lineEdit_editingFinished();
    void on_Pic_verticalSlider_sliderMoved(int position);
    void display_udp_data(QByteArray datatemp);
    void on_display_pushButton_clicked();
    void on_get_the_ip_pushButton_clicked();
    void on_WriteVideo_pushButton_clicked();
    void on_Hostcomputer_Mode_tabWidget_currentChanged(int index);
    void calculateFPS();
    void on_wallpaper_pushbutton_clicked();

    void on_SendMess_pushButton_clicked();

    void on_control_up_pushbutton_clicked();

    void on_control_down_pushbutton_clicked();

    void on_control_left_pushbutton_clicked();

    void on_control_right_pushbutton_clicked();

    void on_openBt_clicked();
    void on_reflesh_seiralport_pushbutton_clicked();
    void on_Hex_SerialShow_checkBox_clicked();
    void on_UTF8_SerialShow_checkBox_clicked();
    void on_sendBt_clicked();
    void on_clearBt_clicked();
    void display_serial_data();
    void on_udp_connect_pushButton_clicked();
    void on_Serial_ShowTime_checkBox_clicked();
    void on_trans_select_tabWidget_currentChanged(int index);
    void on_reset_pushButton_clicked();
    void on_Open_Wifi_checkBox_clicked(bool checked);
    void on_Open_Serial_checkBox_clicked(bool checked);
    void on_serailCb_highlighted(const QString &arg1);

    void on_serailCb_currentIndexChanged(const QString &arg1);

    void on_trans_select_tabWidget_tabBarClicked(int index);
    void changeConeColorSlot();
    void colorDialogAccepted();
    void on_Curve_pushButton_clicked();
    void on_Speed_pushButton_clicked();
    void on_Pitch_pushButton_clicked();
    void on_yawRate_pushButton_clicked();
    void on_PitchRate_pushButton_clicked();
private:
    Ui::Widget *ui;
    void setWallpaper();
    void letGo();
    bool readImg(QString imgName);
    bool readNextImg();
    bool readPreviousImg();
    bool readData();
    bool readgotoData();
    bool readNextData();
    bool readPreviousData();
    QString getImgName(QString imgName, int8_t dir);
    void showAllMaps(QPoint point);
    void Mat2Map(Mat mat);
    Mat Map2Mat(uint8_t map[YM][XM], QPoint point);
    Mat ccd2Mat(uint8_t map[256][128], QPoint point,uint8_t ccd[128]);
    void showCCD(QPoint point);
    void showImgGray(uint8_t map[ROW][COL], QLabel *label, QPoint point);
    void showMap(uint8_t map[YM][XM], QLabel *label, QPoint point);
    void showMat(Mat bgrMat, QLabel *label);
    void getlinemap();
    void imgGrayEvent(uint8_t map[ROW][COL], QLabel *label, uint8_t multiple);
    void mapEvent(uint8_t map[YM][XM], QLabel *label, uint8_t multiple);
    void map_installEventFilter();
    QPoint adjustPixelInfo(QPoint relativePoint, uint8_t mapRow, uint8_t mapCol,
                           uint8_t multiple);
    cv::Mat Wifi_Map2Mat_imagGray(uint8_t map[ROW][COL],uint8 beishu);

    //声明QChart的实例，QSplineSeries的实例
    QChart *chart;
    QSplineSeries *series1;
    QSplineSeries *series2;
    QSplineSeries *series3;
    QSplineSeries *series4;
    QSplineSeries *series5;
    QSplineSeries *series6;
    QSplineSeries *series7;
    QSplineSeries *series8;
    //声明timer
    QTimer *timer;
    QTimer *fpstimer;
    Mat imgMat;
    QString nowImgName;
    uint32_t offset;
    uint32_t last_offset;
    QByteArray data;
    QString DefaultFile;
    QString SDDriveLetter;                       //sd卡读取的盘符，随时更改
    QString data_temp;
    int data_goto;//需要跳转的页数
    int dataCnt;//当前的图像页数
    uint32_t dataNum;//图像页数总计
    char sdBuff[WifiBuffer_AllNum];
    int fd;
    int timerId;
    bool readFlag = false;
    uint readType_flag;//读取数据类
    ImgSize imgSize;
    #define Wifi_read 1;
    #define Seiral_read 2;
    bool runFlag = false;
    String imgName = "imgGray";
    int imgNum = 1;
    QSerialPort *serialPort;//定义串口指针
    udp_thread *udp_recv;
    QByteArray byteArray;
    QByteArray valueByte;
    QByteArray value_array;
    QByteArray image_array;
    QByteArray dataByteArray;
    QByteArray FKTMDfileData;
    char wifiBuff[WifiBuffer_AllNum];
    int recv_wifiNum;
    uint16 ac_pix_index;
    uint16 pix_index;
    uint16 pix_num;
    void Camera_FPS();
    QTimer *frameTimer; // 帧率计时器
    int frameCount;     // 帧计数器
    int fps;
    int16 camera_fps;
    int picture_count;
    int picture;
    int dataNum_tips;

    float WifiData_all;
    float SerialData_all;
    float temp_WifiData;
    float temp_SerialData;
    float WifiData_rate;
    float SerialData_rate;

    bool image_trans_Flag;
    bool value_recv_flag ;
    bool img_recv_flag ;
    void imageInit(uchar *Pic);
    bool WriteVideo_flag;
    bool WriteVideo_done_flag;
    void writeDisk();
    uint16 Recvchannel[7];
    QPoint presspoint;
    bool map_clicked_mode;
    uint8 linemap[YM][XM];
    void Wifi_showMat(Mat bgrMat, QLabel *label);
    QPoint mapPoint;
    bool hightlight_flag;
    bool Wallpaper_Choice;
    void get_the_workshop(uint8 i);
    void allinit();
    void auto_night_mode();
    void _SendMessage();
    void get_the_SDDriveLetter();
    void get_the_serialport();
    QString connect_serialPort;
    bool serial_open_Flag = false;
    bool Hex_SerialShow;
    bool UTF8_SerialShow;
    bool Serial_ShowTime;
    QColor backgroundColor;
    QString tabName1;
    QString tabName2;
    QString tabName3;
    QString tabName4;
    QString tabName5;
    QString tabName6;
    uint8 serailCb_index;
    QByteArray SerialData;
    QByteArray serialarray;
    QStringList tempserial;
    QStringList serialbuff;
    int32 seriallength;
    char serialBuff[SerialBuffer_AllNum];
    QStringList serialList;
    void serial_writeDisk();
    void Start_Screen();
    void close_serail();
    void Watch_the_silder(uint i);
    void save_color();
    void load_color();
signals:
    void send_udp(QByteArray data,QHostAddress ip,quint16 port);

protected:
    virtual void timerEvent(QTimerEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual bool eventFilter(QObject *object, QEvent *event);

};



#define seekpoint(seekpoint_J_Flag,point)                   \
point = presspoint;                                         \
while(seekpoint_J_Flag){                                    \
    if(!seekpoint_J_Flag)                                   \
    {                                                       \
        break;                                              \
    }                                                       \
    QThread::usleep(1);                                     \
    QApplication::processEvents();                          \
}                                                           \
seekpoint_J_Flag = 1;                                       \
if(!seekpoint_Flag)seekpoint_J_Flag = 0;                    \


#define my_delay_times(i) long i = 1;\
while (i <15000)\
 {i++;}\

#define my_delay_times_two(i) long i = 1;\
while (i <8000000)\
 {i++;}\



#endif // WIDGET_H
