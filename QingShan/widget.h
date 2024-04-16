#ifndef WIDGET_H
#define WIDGET_H

#include "headfile.h"
#define SECTOR_START 31808//1024 ///2048
#define SECTOR_SIZE 512
#define IMG_SIZE ( * )
#define SECTOR_NUM_IMGBUFF 30
#define SECTOR_SIZE_IMGBUFF (SECTOR_SIZE * SECTOR_NUM_IMGBUFF)
#define MAP_START 100 //112
#include <opencv2/imgproc/types_c.h>
#include <QtCharts>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QDateTime>
#include <QTextEdit>

#include "QSerialPortInfo"
#include <QSerialPort>
#include <QMessageBox>
using namespace std;
using namespace cv;


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    /*slot function**********************************************************************************/
    //声明voltage的槽函数

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

    void on_closeBt_clicked();


    void manual_serialPortReadyRead();
    void on_udp_connect_pushButton_clicked();
    void on_Serial_ShowTime_checkBox_clicked();
    void on_trans_select_tabWidget_currentChanged(int index);

    void on_reset_pushButton_clicked();
    void on_Open_Wifi_checkBox_clicked(bool checked);

private:
    Ui::Widget *ui;
//    bool isEnd();
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
    Mat Map2Mat(uint8_t map[my_row][my_col], QPoint point);

    Mat GrayMap2Mat(uint8_t map[my_row][_my_col], QPoint point);
    void showCCD(QPoint point);
//    void showImgGray(uint8_t map[][], QLabel *label, QPoint point);
//    void showMap(uint8_t map[][], QLabel *label, QPoint point);
    void showMat(Mat bgrMat, QLabel *label);
//    void showRealMap(QLabel *label, QPoint);
    void imgGrayEvent(uint8_t img[my_row][_my_col], QLabel *label, uint8_t multiple);
    void mapEvent(uint8_t img[my_row][my_col], QLabel *label, uint8_t multiple);
    void map_installEventFilter();
    QPoint adjustPixelInfo(QPoint relativePoint, uint8_t mapRow, uint8_t mapCol,
                           uint8_t multiple);


    //声明QChart的实例，QSplineSeries的实例
//    QChart *chart;
    QSplineSeries *series1;
    QSplineSeries *series2;
    QSplineSeries *series3;
    QSplineSeries *series4;
//    QSplineSeries *series5;
//    QSplineSeries *series6;
//    QSplineSeries *series7;
//    QSplineSeries *series8;
    //声明timer
    QTimer *timer;
    QTimer *fpstimer;
    Mat imgMat;
    QString nowImgName;
    uint32_t offset;
    uint32_t last_offset;
    QString data;
    QString DefaultFile;
    QString SDDriveLetter;                       //sd卡读取的盘符，随时更改
    QString data_temp;
    int data_goto;//需要跳转的页数
    int dataCnt;//当前的图像页数
    uint32_t dataNum;//图像页数总计
    char sdBuff[WifiBuffer_AllNum];
    int fd;
    //AcutalRunInfo ARI;
    int timerId;
    bool readFlag = false;
    uint readType_flag;//读取数据类型
    #define Wifi_read 1;
    #define Seiral_read 2;
    bool runFlag = false;
    String imgName = "imgGray";
    int imgNum = 1;
    QSerialPort *serialPort;//定义串口指针
    Udp_recv *udp_recv;
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
    int picture_count;
    int picture;
    int dataNum_tips;
    int16 camera_fps;
    bool image_trans_Flag;
    bool value_recv_flag ;
    bool img_recv_flag ;
    void imageInit(uchar *Pic);
    bool WriteVideo_flag;
    bool WriteVideo_done_flag;
    void writeDisk();
    uint16 Recvchannel[7];
    QPoint presspoint;
    void Wifi_showMat(Mat bgrMat, QLabel *label);
    bool map_clicked_mode;
    QPoint mapPoint;
    bool hightlight_flag;
    bool Wallpaper_Choice; void get_the_workshop();
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

    QString tabName1;
    QString tabName2;
    QString tabName3;


    char serialBuff[SerialBuffer_AllNum];
    void serial_writeDisk();
signals:
    void send_udp(QByteArray data,qint64 size,QHostAddress ip,quint16 port);

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
