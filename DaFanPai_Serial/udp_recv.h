#ifndef Udp_recv_H
#define Udp_recv_H
#include "headfile.h"
#include "QtNetwork"
#include "QByteArray"
#include <QNetworkConfigurationManager>
#include <QNetworkInterface>
#include "headfile.h"
#include "QApplication"
#include <QBuffer>
#include <QImageReader>
#include <QDateTime>
#include <QtCore/QVector>
#include <QThread>
#include <QQueue>
#include <QPixmap>
#include <QMutex>
#include <QUdpSocket>
#include <QHostAddress>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/types_c.h>
#include "opencv2/core/core_c.h"
#include <opencv2/imgproc/types_c.h>

class Udp_recv : public QThread
{
    Q_OBJECT
public:
    explicit Udp_recv(QHostAddress ip,QString port,QObject *parent = nullptr);
    ~Udp_recv();
    uint8_t transmap[120][188];
    bool start_flag; //线程工作标志位
    bool allow_recv_flag;  //允许接收udp数据到队列标志位
    bool recv_flag;
    bool connect_flag; //udp连接标志位
    bool allow_connect_flag; //允许连接标志位
    QQueue<QByteArray> bytes_queue; //图像原始数据队列
    QQueue<QPixmap> pixmap_queue; //图像数据队列
    QLabel Temptext;
    QQueue<QString> msg_queue;
    QByteArray data_temp;
    quint16 connect_time;
    QByteArray rec_data;
    QHostAddress connectip;
    quint16 connectport ;

    QByteArray Accumu_data;

    //图片保存相关
    bool save_img_flag;
    QString save_img_type;
    QString save_img_path;
    QString save_img_result;

    //保存视频相关
    bool save_video_start_flag;
    bool save_video_end_flag;
    cv::VideoWriter save_video_writer;
    QString save_video_path;
    QString save_video_result;
    cv::Mat transmat;
    QUdpSocket *Receiver; //udp套接字
    QUdpSocket *Sender; //udp套接字

    void stop(); //暂停线程
    void go_on(); //继续线程
    QMutex mutex;//互斥量，用来暂停和继续线程
//    void new_client_connect();

    cv::Mat QImageToMat(QImage image);
    void process_data(QByteArray data);
public slots:
    void send_message(QByteArray data,qint64 size,QHostAddress ip,quint16 port);



protected:
    void run();

signals:
   void signal_img(QByteArray data);

private:


private slots:
    void recv_udp();


};

#endif // Udp_recv_H
