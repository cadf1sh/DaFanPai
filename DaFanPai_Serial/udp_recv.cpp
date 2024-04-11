#include "Udp_recv.h"



Udp_recv::Udp_recv(QHostAddress ip, QString port, QObject *parent) : QThread(parent)
{
    connect_flag = false;
    start_flag = true; //默认开启线程
    save_img_flag = false; //默认不保存图片
    recv_flag = true;
    save_video_start_flag = false; //默认不保存视频
    save_video_end_flag = false;
    connectip = ip;
    connectport = port.toUInt();
    //=======================================================
    Sender = new QUdpSocket();

    //=======================================================


    //=======================================================
    Receiver = new QUdpSocket(); //新建一个udp套接字对象
    Receiver->bind(8080,QUdpSocket::ShareAddress); //绑定ip地址并监听端口
    //=======================================================
}


void Udp_recv::run()
{
    QByteArray img_bytes;
    QString current_time;
    QByteArray data_temp;
    connect(Receiver,SIGNAL(readyRead()),this,SLOT(recv_udp()));
}






//UDP接收数据
void Udp_recv::recv_udp()
{


    QByteArray tempdata;
    QByteArray data;
    if(Receiver->hasPendingDatagrams())
    {
    connect_flag = true;
    int size = Receiver->pendingDatagramSize();//获取数据包大小
    tempdata.resize(size);
    Receiver->readDatagram(tempdata.data(),tempdata.size());
    data.append(tempdata);
    if(data.size() == WifiBuffer_AllNum)
    {
        emit signal_img(data);
    }
    data.clear();
    }

 }


void Udp_recv::process_data(QByteArray data)
{

1;

}


void Udp_recv::send_message(QByteArray data,qint64 size,QHostAddress ip,quint16 port)
{


Sender->writeDatagram(data, data.size(),ip, port);

}


//QImage转Mat，使用opencv库函数进行保存视频
cv::Mat Udp_recv::QImageToMat(QImage image)
{
    cv::Mat mat;
    switch (image.format())
    {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied:
            mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
            break;
        case QImage::Format_RGB888:
            mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
            cv::cvtColor(mat, mat, CV_BGR2RGB);
            break;
        case QImage::Format_Grayscale8:
            mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            break;
        default:
            break;
    }
    return mat;
}








void Udp_recv::stop()
{
    mutex.lock();
}

void Udp_recv::go_on()
{

    mutex.unlock();
}

Udp_recv::~Udp_recv()
{

}
