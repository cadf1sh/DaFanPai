#include "udp_thread.h"

udp_thread::udp_thread(QHostAddress ip, QString port, QObject *parent) : QThread(parent)
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
    Sender = new QUdpSocket();//新建一个udp发送套接字对象

    //=======================================================


    //=======================================================
    Receiver = new QUdpSocket(); //新建一个udp接收套接字对象
    Receiver->bind(connectport,QUdpSocket::ShareAddress); //绑定ip地址并监听端口
    //=======================================================
}


void udp_thread::run()
{
    QByteArray img_bytes;
    QString current_time;
    QByteArray data_temp;
    connect(Receiver,SIGNAL(readyRead()),this,SLOT(recv_udp()));
}






//UDP接收数据
void udp_thread::recv_udp()
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




void udp_thread::send_message(QByteArray data,QHostAddress ip,quint16 port)
{
Sender->writeDatagram(data, data.size(),ip, port);
}





void udp_thread::stop()
{
    mutex.lock();
}

void udp_thread::go_on()
{

    mutex.unlock();
}

udp_thread::~udp_thread()
{

}
