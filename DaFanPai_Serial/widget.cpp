#include "widget.h"
#include "ui_widget.h"
#include <math.h>
#include <QThread>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QHostAddress local_ip(1) ;
    QString udp_port = "8080";
    allinit();
    setWallpaper();
    udp_recv = new Udp_recv(local_ip,udp_port);
    connect(udp_recv, SIGNAL(signal_img(QByteArray)), this, SLOT(display_udp_data(QByteArray)));
    connect(this, SIGNAL(send_udp(QByteArray,qint64,QHostAddress,quint16)), udp_recv, SLOT(send_message(QByteArray,qint64,QHostAddress,quint16)));
    udp_recv->start();
    udp_recv->stop(); //暂停udp线程，等到开启接收画面后再开启工作
    ui->WriteVideo_pushButton->hide();

    // 创建一个定时器
    fpstimer = new QTimer(this);
    connect(fpstimer, SIGNAL(timeout()), this, SLOT(calculateFPS()));
    fpstimer->start(1000);


    //////////////////////////////串口监听
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    serialPort = new QSerialPort(this);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(manual_serialPortReadyRead()));/*手动连接槽函数*/
    //////////////////////////////

}


void Widget::allinit(){\

    map_installEventFilter();
    get_the_serialport();
    Widget::setWindowTitle("Sdnu Link");
    WriteVideo_done_flag  = false;
    WriteVideo_flag  = false;
    map_clicked_mode  = false;
    hightlight_flag = true;
    Wallpaper_Choice = true;
    get_the_workshop();//自动获取本地文件地址
    QIcon windowicon(DefaultFile + "icon.jpg");
    Widget::setWindowIcon(windowicon);  // 替换为你的图标文件路径
    ui->WriteVideo_pushButton->setStyleSheet("color: green;");
    ui->trans_select_tabWidget->setCurrentIndex(0);
    ui->local_port_lineEdit->setText("8080");
    image_trans_Flag = false;
    ac_pix_index = 0;
    pix_index = 0;
    auto_night_mode();//自动夜间模式
    get_the_SDDriveLetter();
    picture_count = 0;
    frameCount = 0;

    ui->port_lineEdit->setText("6666");

    Hex_SerialShow = false ;
    UTF8_SerialShow = false ;
    Serial_ShowTime = false ;
    serial_open_Flag = false;

    ui->trans_select_tabWidget->setCurrentIndex(0);

}



Widget::~Widget()
{
    delete ui;
}



void Widget::setWallpaper()
{
    this->setAutoFillBackground(true); // 这句要加上，否则可能显示不出背景图
    QPalette palette = this->palette();
    if (!Wallpaper_Choice)
    {
        QPixmap wallpaper(DefaultFile + "Dark_widget.jpg");
        wallpaper = wallpaper.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // 设置主窗口背景图
        palette.setBrush(QPalette::Window, QBrush(wallpaper));
        this->setPalette(palette);
        wallpaper = wallpaper.scaled(ui->trans_select_tabWidget->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // 设置 trans_select_tabWidget 的背景图
        QPalette tabPalette = ui->trans_select_tabWidget->palette();
        tabPalette.setBrush(QPalette::Window, QBrush(wallpaper));
        ui->trans_select_tabWidget->setPalette(tabPalette);

    }
    else
    {

        QPixmap wallpaper(DefaultFile + "Bright_widget.jpg");
        wallpaper = wallpaper.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // 设置主窗口背景图
        palette.setBrush(QPalette::Window, QBrush(wallpaper));
        this->setPalette(palette);
        wallpaper = wallpaper.scaled(ui->trans_select_tabWidget->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        // 设置 trans_select_tabWidget 的背景图
        QPalette tabPalette = ui->trans_select_tabWidget->palette();
        tabPalette.setBrush(QPalette::Window, QBrush(wallpaper));
        ui->trans_select_tabWidget->setPalette(tabPalette);
    }
}

void Widget::letGo()//ui变量名显示
{
//    go(imgSize);




    ui->Pic_num_label->setText(QString("Pic_num: %1").arg(dataCnt));
     ui->Pic_num_label->adjustSize();//当前图像的顺序
     ui->Total_num_label->setText(QString("Total_num: %1").arg(dataNum));
     ui->Total_num_label->adjustSize();//当前图像的顺序



    ui->Frame_rate_label->show();
    ui->Frame_rate_label->setText(QString("上位机显示帧率:%1帧").arg(camera_fps));
    ui->Frame_rate_label->setStyleSheet(
                "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:20px;font-family: Microsoft YaHei;color:black");  //255 105 180
    ui->Frame_rate_label->adjustSize();

    QString str;

}



bool Widget::readNextImg()
{
//    QString nextImgName = getImgName(nowImgName, 1);
//    if(readImg(nowImgName))
//    {
//        nowImgName = nextImgName;
//        return true;
//    }
//    return false;
}

bool Widget::readPreviousImg()
{
//    QString nextImgName = getImgName(nowImgName, -1);
//    if(readImg(nowImgName))
//    {
//        nowImgName = nextImgName;
//        return true;
//    }
//    return false;
}

bool Widget::readData()//读取SD卡中的变量信息
{
    if(data[offset] != 'C')
    {
        return false;  //帧头不对
    }
    for(uint16_t i = 0; i < WifiBuffer_AllNum; ++i)
    {
        sdBuff[i] = FKTMDfileData.at(offset + i);  //char sdBuff[512];
    }
    if(sdBuff[0] != 'C' || sdBuff[1] != 'Z' || sdBuff[2] != 'Y' || sdBuff[3] != 'N' || sdBuff[4] != 'B')
    {
        return false;
    }

    int  wifiNum = 5;



    return true;
}

bool Widget::readNextData()
{
    offset += WifiBuffer_AllNum;
    if(++dataCnt > dataNum)
    {
        offset = 0;
        dataCnt = 0;
    }
     ui->Pic_verticalSlider->setValue(dataCnt);
    if(readData())
    {

        letGo();
        showAllMaps(presspoint);
    }
}

bool Widget::readPreviousData()
{
    offset -=WifiBuffer_AllNum;
    if(--dataCnt < 1)
    {
        offset = 0;
        dataCnt = 0;
    }
    ui->Pic_verticalSlider->setValue(dataCnt);
    if(readData())
    {
        letGo();
        showAllMaps(presspoint);
    }
}

bool Widget::readgotoData()
{
    if(data_goto>1&&data_goto<(int)dataNum)
   {
        offset += (data_goto-dataCnt)*WifiBuffer_AllNum;
        dataCnt=data_goto;
    }
    ui->Pic_verticalSlider->setValue(dataCnt);
    if(readData())
    {
        letGo();
        showAllMaps(presspoint);
    }
}
QString Widget::getImgName(QString imgName, int8_t dir)
{
    QString newImgName;
    QString str;
    QString imgPrefix;
    QString imgSuffix = ".bmp";

    imgName = imgName.simplified();
    QStringList sections = imgName.split(QRegExp("[/]"));
    QString realImgName = sections.last();
    for(uint8_t i = 0; i < realImgName.size(); ++i)
    {
        if(realImgName.at(i) >= '0' && realImgName.at(i) <= '9')
        {
            str += realImgName.at(i);
        }
    }
    imgPrefix = imgName.mid(0, imgName.indexOf(str + imgSuffix));
    uint16_t imgNum = str.toUInt();
    if(dir == 1)
    {
        imgNum++;
    }
    else if(dir == -1)
    {
        imgNum--;
    }
    newImgName = imgPrefix + QString::number(imgNum) + imgSuffix;

    if(QFileInfo(newImgName).isFile())
    {

        return newImgName;
    }
    else
    {
        imgNum = 1;
        newImgName = imgPrefix + QString::number(imgNum) + imgSuffix;

        return newImgName;
    }
}

void Widget::showAllMaps(QPoint point)
{


}

//void Widget::Mat2Map(Mat mat)
//{
//    if(imgSize == grayscaleImg)
//    {
//        for(uint8_t j = 0; j < ROW; ++j)
//        {
//            uint8_t *ptr = mat.ptr(j);
//            for(uint8_t i = 0; i < COL; ++i)
//            {
//                imgGray[j][i] = ptr[i];
//            }
//        }
//    }
//    else if(imgSize == binaryImg)
//    {
//        for(uint8_t j = 0; j < YM; ++j)
//        {
//            uint8_t *ptr = mat.ptr(j);
//            for(uint8_t i = 0; i < XM; ++i)
//            {
//                allmap[YY - j][i] = !(ptr[i] >> 7);
//            }
//        }
//    }
//}

//Mat Widget::Map2Mat(uint8_t map[YM][XM], QPoint point)
//{  //MUTIPLE应该是图像放大倍数，CV_8UC3表示图像是3通道的
//    Mat mat(YM * MULTIPLE, XM * MULTIPLE, CV_8UC3);
//    for (int j = 0; j < mat.rows; ++j)
//    {  //MUTIPLE是放大倍数，/MUTIPLE当然是真实的坐标了，不过是CV坐标系的，转成代码的坐标系还需要坐标转换一下
//        Vec3b *ptr = mat.ptr<Vec3b>(j);
//        for (int i = 0; i < mat.cols; ++i)
//        {//BGR
//            uint8_t nowPixel = map[YY - (j / MULTIPLE)][i / MULTIPLE];
//            if (nowPixel == 0)
//            {
//                ptr[i][0] = 255;
//                ptr[i][1] = 255;
//                ptr[i][2] = 255;
//            }
//            else if (nowPixel == 1)
//            {
//                ptr[i][0] = 0;
//                ptr[i][1] = 0;
//                ptr[i][2] = 0;
//            }
//            else if(nowPixel == 2)//red
//            {
//                ptr[i][0] = 0;
//                ptr[i][1] = 0;
//                ptr[i][2] = 255;
//            }
//            else if(nowPixel == 3)//purple
//            {
//                ptr[i][0] = 255;
//                ptr[i][1] = 0;
//                ptr[i][2] = 255;
//            }
//            else if (nowPixel == 4)//blue
//            {
//                ptr[i][0] = 255;
//                ptr[i][1] = 0;
//                ptr[i][2] = 0;
//            }
//            else if (nowPixel == 5)//green
//            {
//                ptr[i][0] = 0;
//                ptr[i][1] = 255;
//                ptr[i][2] = 0;
//            }
//            else if (nowPixel == 6)
//            {
//                ptr[i][0] = 181;
//                ptr[i][1] = 228;
//                ptr[i][2] = 255;
//            }
//            else if (nowPixel == 7)
//            {
//                ptr[i][0] = 181;
//                ptr[i][1] = 228;
//                ptr[i][2] = 255;
//            }
//            else if (nowPixel == 8)
//            {
//                ptr[i][0] = 181;
//                ptr[i][1] = 228;
//                ptr[i][2] = 255;
//            }
//            else if(nowPixel == 250)//yellow
//            {
//                ptr[i][0] = 0;
//                ptr[i][1] = 255;
//                ptr[i][2] = 255;
//            }
//            else if(nowPixel == 251)//green
//            {
//                ptr[i][0] = 0;
//                ptr[i][1] = 255;
//                ptr[i][2] = 0;
//            }
//            else if(nowPixel == 253)
//            {
//                ptr[i][0] = 0;
//                ptr[i][1] = 140;
//                ptr[i][2] = 255;
//            }
//            else if(nowPixel == 254)
//            {
//                ptr[i][0] = 0;
//                ptr[i][1] = 255;
//                ptr[i][2] = 127;
//            }
//        }
//    }


//    return mat;
//}





//void Widget::showImgGray(uint8_t map[ROW][COL], QLabel *label, QPoint point)
//{
//    uint8_t multiple = 4;
//    Mat grayMat(ROW, COL, CV_8UC1, map);
//    Mat rgbMat;
//    cvtColor(grayMat, rgbMat, CV_GRAY2RGB);
//    cv::resize(rgbMat, rgbMat, {0, 0}, multiple, multiple);
//    int _top_line = YY;
//    int _bottom_line = 0;
//    for(int i = 0; i < rgbMat.cols; i++)
//    {

//        for(int j = 0; j < 1 * multiple; j++)
//        {
//            int y1 = (_top_line + (_ROW - top_line) + 1) * multiple * 1 + j;
//            int y2 = (_bottom_line + (_ROW - top_line) + 1) * multiple * 1 + j;
//            if(y1 >= 0 && y1 < rgbMat.rows &&
//                    y2 >= 0 && y2 < rgbMat.rows)
//            {
//                rgbMat.at<Vec3b>(y1 , i)[0] = 178;
//                rgbMat.at<Vec3b>(y1 , i)[1] = 34;
//                rgbMat.at<Vec3b>(y1 , i)[2] = 34;
//                rgbMat.at<Vec3b>(y2 , i)[0] = 178;
//                rgbMat.at<Vec3b>(y2 , i)[1] = 34;
//                rgbMat.at<Vec3b>(y2 , i)[2] = 34;
//            }
//            //显示一下截图的范围
//        }
//    }
//    if (point.x() >= 0 && point.y() >= 0)
//    {
//        int x = point.x();
//        int y = point.y();
//        QPoint highlight;

//        for (int i = 0; i < 4 * multiple; ++i)
//        {
//            for (int j = 0; j < 1 * multiple; ++j) //2
//            {
//                highlight.rx() = x * multiple * 4 + i;
//                highlight.ry() = (y + (_ROW - top_line) + 1) * multiple * 1 + j;  //2  坐标变换，凑一凑啦
//                qout << highlight.rx() << "," << highlight.ry() << endl;
//                if (highlight.y() >= 0 && highlight.y() < rgbMat.rows &&
//                    highlight.x() >= 0 && highlight.x() < rgbMat.cols)
//                {
//                    rgbMat.at<Vec3b>(highlight.y(), highlight.x())[0] = 50;
//                    rgbMat.at<Vec3b>(highlight.y(), highlight.x())[1] = 205;
//                    rgbMat.at<Vec3b>(highlight.y(), highlight.x())[2] = 50;
//                }
//            }
//        }
//    }

//    QImage img(static_cast<const uint8_t *>(rgbMat.data), rgbMat.cols, rgbMat.rows,
//               rgbMat.cols * rgbMat.channels(), QImage::Format_RGB888);
//    label->setPixmap(QPixmap::fromImage(img));
//    label->resize(label->pixmap()->size());  //使图片大小自适应窗口
//    label->show();
//}





//void Widget::showMap(uint8_t map[YM][XM], QLabel *label, QPoint point)
//{
//    Mat mat = Map2Mat(map, point);
//    showMat(mat, label);
//}





void Widget::showMat(Mat bgrMat, QLabel *label)
{

    Mat rgbMat;
    cvtColor(bgrMat, rgbMat, CV_BGR2RGB);
    QImage img(static_cast<const uint8_t *>(rgbMat.data), rgbMat.cols, rgbMat.rows,
               rgbMat.cols * rgbMat.channels(), QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(img));
    label->resize(label->pixmap()->size());
    label->show();

}








//void Widget::imgGrayEvent(uint8_t img[ROW][COL], QLabel *label, uint8_t multiple)
//{
//    QPoint relativePoint =label->mapFromGlobal(QPoint(cursor().pos().x(), cursor().pos().y()));
//    int x = relativePoint.x() / multiple;
//    int y = relativePoint.y() / multiple;      //x, y 是allmap数组的实际坐标系
//    mapPoint.rx() = relativePoint.x() / multiple  ;
//    mapPoint.ry() = relativePoint.y() / multiple;   //mapPoint是CV的坐标系
//    presspoint = mapPoint;

//    map_clicked_mode = true;
//    showAllMaps(mapPoint);  //每点击一次刷新一下图片显示

//    QPoint showPoint = adjustPixelInfo(relativePoint, ROW, COL, multiple);

//    if (ui->pixelInfo_lable->isHidden())
//    {
//        ui->pixelInfo_lable->show();
//    }
//    ui->pixelInfo_lable->raise();
//    ui->pixelInfo_lable->setText(
//        QString("[%1, %2] : %3").arg(x).arg(y).arg(img[y][x]));
//    ui->pixelInfo_lable->setStyleSheet("background-color:rgb(255,218,185)");
//    ui->pixelInfo_lable->move(showPoint.x(), showPoint.y());
//    ui->pixelInfo_lable->adjustSize();
//}





//void Widget::mapEvent(uint8_t map[YM][XM], QLabel *label, uint8_t multiple)
//{
//    QPoint relativePoint =
//        label->mapFromGlobal(QPoint(cursor().pos().x(), cursor().pos().y()));
//    int x = relativePoint.x() / multiple;
//    int y = YY - relativePoint.y() / multiple;  //x, y 是allmap的坐标系
//    mapPoint.rx() = relativePoint.x() / multiple;
//    mapPoint.ry() = relativePoint.y() / multiple;  //mapPoint是CV的坐标系
//    presspoint = mapPoint;


//    map_clicked_mode = false;
//    showAllMaps(mapPoint);

//    QPoint showPoint = adjustPixelInfo(relativePoint, YM, XM, multiple);

//    if (ui->pixelInfo_lable->isHidden())
//    {
//        ui->pixelInfo_lable->show();
//    }
//    ui->pixelInfo_lable->raise();
//    ui->pixelInfo_lable->setText(
//        QString("[%1, %2] : %3").arg(x).arg(y).arg(map[y][x]));
//    ui->pixelInfo_lable->setStyleSheet("background-color:rgb(255,218,185)");
//    ui->pixelInfo_lable->move(showPoint.x(), showPoint.y());
//    ui->pixelInfo_lable->adjustSize();
//    //qout<<y<<" "<<x;
//}





void Widget::map_installEventFilter()
{
//    ui->allmap_label->installEventFilter(this);
//    ui->basemap_lable->installEventFilter(this);
//    ui->deletemap_lable->installEventFilter(this);
//    ui->imgGray_lable->installEventFilter(this);
//    ui->insidemap_lable->installEventFilter(this);
//    ui->leftmap_lable->installEventFilter(this);
//    ui->linemap_lable->installEventFilter(this);
//    ui->rightmap_lable->installEventFilter(this);
}





QPoint Widget::adjustPixelInfo(QPoint relativePoint, uint8_t mapRow, uint8_t mapCol, uint8_t multiple)
{
    QPoint showPoint;
    int titleBarHeight = frameGeometry().height() - geometry().height();
    showPoint.rx() = cursor().pos().x() - this->pos().x();
    showPoint.ry() = cursor().pos().y() - this->pos().y() - titleBarHeight;
    QPoint margin;
    margin.rx() = 10;
    margin.ry() = 10;
    if (relativePoint.x() <
            mapCol * multiple - ui->pixelInfo_lable->width() - margin.x() &&
        relativePoint.y() <
            mapRow * multiple - ui->pixelInfo_lable->height() - margin.y())
    {
        showPoint += margin;
    }
    else if (relativePoint.x() <
                 mapCol * multiple - ui->pixelInfo_lable->width() - margin.x() &&
             relativePoint.y() >
                 mapRow * multiple - ui->pixelInfo_lable->height() - margin.y())
    {
        showPoint.ry() -= ui->pixelInfo_lable->height();
        showPoint.rx() += margin.x();
        showPoint.ry() -= margin.y();
    }
    else if (relativePoint.x() >
                 mapCol * multiple - ui->pixelInfo_lable->width() - margin.x() &&
             relativePoint.y() <
                 mapRow * multiple - ui->pixelInfo_lable->height() - margin.y())
    {
        showPoint.rx() -= ui->pixelInfo_lable->width();
        showPoint.rx() -= margin.x();
        showPoint.ry() += margin.y();
    }
    else if (relativePoint.x() >
                 mapCol * multiple - ui->pixelInfo_lable->width() - margin.x() &&
             relativePoint.y() >
                 mapRow * multiple - ui->pixelInfo_lable->height() - margin.y())
    {
        showPoint.rx() -= ui->pixelInfo_lable->width();
        showPoint.ry() -= ui->pixelInfo_lable->height();
        showPoint -= margin;
    }
    return showPoint;
}





void Widget::timerEvent(QTimerEvent *event)
{    
//    if(readType == dataType)
    {

        readNextData();
    }
    presspoint = mapPoint;
}



void Widget::keyPressEvent(QKeyEvent *event)
{
    if(ui->trans_select_tabWidget->currentIndex() == 1 )//tuchuan
    {
    if(ui->Hostcomputer_Mode_tabWidget->currentIndex() == 0 )
    {
        if(event->key() == Qt::Key_I)
        {
            if(true == runFlag)
            {
                runFlag = false;
                this->killTimer(timerId);
            }
            on_readImg_pushButton_clicked();
        }
        else if(event->key() == Qt::Key_R)
        {
            if(true == runFlag)
            {
                runFlag = false;
                this->killTimer(timerId);
            }
            on_readData_pushButton_clicked();
        }
        else if(event->key() == Qt::Key_G)
        {
            on_run_pushButton_clicked();
        }
        else if(event->key() == Qt::Key_D)
        {
        }    
    }
  }
  else if(ui->trans_select_tabWidget->currentIndex() == 2 )//wifi
  {
    if(event->key() == Qt::Key_Up)//shang
    {
        on_control_up_pushbutton_clicked();
    }
    if(event->key() == Qt::Key_Down)//xia
    {
        on_control_down_pushbutton_clicked();
    }
    if(event->key() == Qt::Key_Left)//zuo
    {
        on_control_left_pushbutton_clicked();
    }
    if(event->key() == Qt::Key_Right)//you
    {
        on_control_right_pushbutton_clicked();
    }
  }

}

bool Widget::eventFilter(QObject *object, QEvent *event)
{
    if(ui->Hostcomputer_Mode_tabWidget->currentIndex() == 0 )
    {
        if (object == ui->imgGray_lable)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                return true;
            }
        }

    }
    return false;
}

void Widget::on_readImg_pushButton_clicked()
{
//    QString imgName = QFileDialog::getOpenFileName(this, "读取图片", DefaultFile+"GrayImg");

//    if(imgName != nullptr)
//    {
//        readType = imgType;
//        if(readImg(imgName))
//        {
//            readFlag = true;
//            nowImgName = imgName;
//            Mat2Map(imgMat);
//            standard();
//            letGo();
//            getlinemap();
//            showAllMaps(presspoint);
//        }
//    }
}

void Widget::on_readData_pushButton_clicked()
{
    //QString dataName = QFileDialog::getOpenFileName(this, "read Data", "D:/QTtest/PrjTest_QT_DEBUG");  //获取选中文件的名字
    QString dataName = QFileDialog::getOpenFileName(this, "读取数据", DefaultFile+"picture"); //获取选中文件的名字

    if(dataName != nullptr)
    {
        data.clear();  //String data
        dataNum = 0;   // 重置数据计数器
        dataNum_tips = 0;
        offset = 0;    //uint32
//        readType = dataType;  //datatype/imgtype
        QFile myFile(dataName);  //创建对应的文件对象
        if(!myFile.open(QIODevice::ReadWrite))  //文件以读写的形式打开
        {
            qout << "failed" << endl;
            return;
        }
        else
        {
            FKTMDfileData = myFile.readAll(); // 读取文件中的全部数据
            int dataSize = FKTMDfileData.size();
            int numSegments = (dataSize + WifiBuffer_AllNum - 1) / WifiBuffer_AllNum;  // 计算数据段的数量
            for (int i = 0; i < numSegments; i++)
            {
                int startPos = i * WifiBuffer_AllNum;  // 数据段的起始位置
                int segmentSize = qMin(WifiBuffer_AllNum, dataSize - startPos);  // 数据段的实际长度

                QString segment = QString::fromLatin1(FKTMDfileData.constData() + startPos, segmentSize);  // 创建数据段

                data.append(segment);  // 将数据段拼接到 QString 中
                dataNum_tips += segmentSize;
            }
            dataNum = dataNum_tips / WifiBuffer_AllNum;

               /**********对滑动条的设置************/
            ui->Pic_verticalSlider->setMinimum(1);
            //设置滑动条控件的最大值
            ui->Pic_verticalSlider->setMaximum(dataNum);
            ui->Pic_verticalSlider->setTickInterval(dataNum/10);  // 设置刻度间隔
            ui->Pic_verticalSlider->setTickPosition(QSlider::TicksBothSides);  // 显示刻度
        }
//        if(ui->imgGray_lable->isVisible())
//        {
//            ui->imgGray_lable->hide();
//        }

        offset = data.indexOf("CZYNB", offset);
        if(readData())  //读取文件数据
        {
            readFlag = true;
            dataCnt = 1;
            letGo();
            showAllMaps(presspoint);
        }
    }
}

void Widget::on_readSD_pushButton_clicked()
{
    SDDriveLetter = ui->SDDriveLetter_comboBox->currentText();
    fd = open(("\\\\.\\" + SDDriveLetter.toStdString()).c_str(), O_RDONLY | O_BINARY);//open("\\\\.\\E:", O_RDONLY | O_BINARY);
   // int ret = lseek(fd, 0, SEEK_SET);
   // qout<< ret << endl;
    lseek64(fd, SECTOR_START * SECTOR_SIZE, SEEK_SET);

    QDateTime dateTime = QDateTime::currentDateTime();
    QString fileName = DefaultFile + "picture/p";
    fileName += dateTime.toString("XXCCyyyy-MM-dd_HH.mm");
    fileName += ".FKTMD";
    QFile file(fileName);

    if(file.exists())
    {
    }
    else
    {
        file.open(QIODevice::ReadWrite);
        do
        {
            if(fd != -1)
            {
                QDataStream dataOutput(&file);
                char _sdBuff[512];
                QString _str;
                read(fd, _sdBuff, 512);

                if(!(_sdBuff[0] == 'F' && _sdBuff[1] == 'K' &&
                     _sdBuff[2] == 'T' && _sdBuff[3] == 'M' && _sdBuff[4] == 'D'))
                {
                    qout << "帧头不对！";
                    break;
                }
                for(uint16_t i = 0; i < 512; ++i)
                {
                    _str += _sdBuff[i];
                }
                dataOutput << _str;
//                ui->SDInfo_label->setText(QString("succeed!"));
//                ui->SDInfo_label->setStyleSheet(
//                    "background-color: rgb(255, 105, 180);font-size:20px;color:blue");
//                ui->SDInfo_label->adjustSize();
            }
            else
            {
//                ui->SDInfo_label->setText(QString("don't have permission!"));
//                ui->SDInfo_label->setStyleSheet(
//                    "background-color: rgb(255, 105, 180);font-size:20px;color:blue");
//                ui->SDInfo_label->adjustSize();
                break;
            }
        } while(1);
    }
    file.close();
}

void Widget::on_SDimgGray_pushButton_clicked()
{
//    SDDriveLetter = ui->SDDriveLetter_comboBox->currentText();
//    std::string imgName = DefaultFile.toStdString() + "GrayImg/p";
//    imgNum = 1;
//    fd = open(("\\\\.\\" + SDDriveLetter.toStdString()).c_str(), O_RDONLY | O_BINARY);//open("\\\\.\\E:", O_RDONLY | O_BINARY);
//    lseek64(fd, SECTOR_START * SECTOR_SIZE, SEEK_SET);//设置读取的起始位置，
//                                                      //lseek函数需要与read配合使用，
//                                                      //每次read后会自动在前一次地址的基础上增加读取的长度
//                                                      //lseek函数传SEEK_SET可设置当前偏移地址

//    do
//    {
//        if(fd != -1)
//        {
//            char imgGray_SDBuff[SECTOR_SIZE * SECTOR_NUM_IMGBUFF];
//            char imgGray_buff[ROW * COL];

//            read(fd, imgGray_SDBuff, sizeof(imgGray_SDBuff));//必须读512的整数倍
//            memcpy(imgGray_buff, imgGray_SDBuff, sizeof(imgGray_buff));
//            Mat mat(ROW, COL, CV_8UC1, imgGray_buff);
//            String imgNameSave = imgName + to_string(imgNum) + ".bmp";
//            imwrite(imgNameSave, mat);

//            ++imgNum;
//            if(imgNum > 2000)
//            {
//                break;
//            }
//        }
//        else
//        {
//            qout << "maybe don't have permission";
//            break;
//        }
//    } while(1);
}



void Widget::on_run_pushButton_clicked()
{
    if(true == readFlag)
    {
        if(false == runFlag)
        {
            timerId = this->startTimer(1); //这里修改图片播放速度 //100
            runFlag = true;
        }
        else if(true == runFlag)
        {
            this->killTimer(timerId);
            runFlag = false;
        }
    }
    else
    {
        return ;
    }
}



int reset_flag = 0;
void Widget::on_reset_pushButton_clicked()
{
    reset_flag = 1;
    letGo();
    showAllMaps(presspoint);
}






void Widget::on_Pic_num_lineEdit_editingFinished()
{
     QString text = ui->Pic_num_lineEdit->text();
     data_goto = text.toInt();
     ui->Pic_verticalSlider->setValue(data_goto);
     readgotoData();
}



void Widget::on_Pic_verticalSlider_sliderMoved(int position)
{
    data_goto = position;
    readgotoData();
}







//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//                                       wifi
//显示图片、视频
void Widget::display_udp_data(QByteArray datatemp)
{

    if(ui->udp_connect_pushButton->text() == "中断连接" && udp_recv->connect_flag == true)
    {
        byteArray.append(datatemp);
        int16 GetValueNum = byteArray.size();
        for(uint16_t i = 0; i < GetValueNum; ++i)
        {
            wifiBuff[i] = byteArray.at(i);  //char sdBuff[512];
        }
        byteArray.clear();

        recv_wifiNum = 5;
        if(wifiBuff[0] != 'C' || wifiBuff[1] != 'Z' || wifiBuff[2] != 'Y' || wifiBuff[3] != 'N' || wifiBuff[4] != 'B')
        {
            image_trans_Flag = false;
            return;
        }




        if(ui->display_pushButton->text() == "停止")
        {

        }


        ///    /////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        if(wifiBuff[recv_wifiNum++] != 'V' || wifiBuff[recv_wifiNum++] != 'A' || wifiBuff[recv_wifiNum++] != 'L' || wifiBuff[recv_wifiNum++] != 'U' || wifiBuff[recv_wifiNum++] != 'E')
        {
            image_trans_Flag = false;
            return;
        }

        //图像处理时间
//        dealImgtime=(wifiBuff[recv_wifiNum++] << 8) | ((uint8_t)wifiBuff[recv_wifiNum++]);
//        ui->dealImgTime_label->setText(QString("dealImgTime:%1").arg(dealImgtime));
//        ui->dealImgTime_label->setStyleSheet(
//                    "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
//        ui->dealImgTime_label->adjustSize();
        wifiBuff[recv_wifiNum++] = fps;//不要改变//误删//误删这是上位机FPS显示的暂存值
        ////////////////////////






        frameCount++;
        image_trans_Flag = true;
        wifi_writeDisk();

    }
}




//将opencv的矩阵转换为QImage再转换为QPixmap
void Widget::Wifi_showMat(Mat bgrMat, QLabel *label)//一张图一张图
{
    Mat rgbMat;
    QString time1;
    QByteArray byteArray;
    cvtColor(bgrMat, rgbMat, CV_BGR2RGB);
    QImage img(static_cast<const uint8_t *>(rgbMat.data), rgbMat.cols, rgbMat.rows,
    rgbMat.cols * rgbMat.channels(), QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(img));
//    ui->pix_index_label->setText(QString("现在已经传了%1张图片了").arg(ac_pix_index));
//    ui->pix_index_label->adjustSize();
//    ui->camera_fps_label->setText(QString("摄像头显示帧率为:%1帧").arg(camera_fps));
//    ui->camera_fps_label->adjustSize();

}



//cv::Mat Widget::Wifi_Map2Mat(uint8_t map[][],uint8 beishu)//opencv库里面的矩阵数据类型，一般用来储存矩阵的信息（和二维数组有异曲同工）
//{  //beishu应该是图像放大倍数，CV_8UC3表示图像是3通道的
//    cv::Mat mat(128*beishu , 128*beishu , CV_8UC3);
//    for (int j = 0; j < mat.rows; ++j)
//    {  //MUTIPLE是放大倍数，/MUTIPLE当然是真实的坐标了，不过是CV坐标系的，转成代码的坐标系还需要坐标转换一下
//        cv::Vec3b *ptr = mat.ptr<cv::Vec3b>(j);
//        for (int i = 0; i < mat.cols; ++i)
//        {//BGR
//            uint8_t nowPixel = map[(j / beishu)][i / beishu];
//            ptr[i][0] = nowPixel;
//            ptr[i][1] = nowPixel;
//            ptr[i][2] = nowPixel;

//            if( hightlight_flag && j / beishu  == presspoint.y() + 15 && i / beishu == 4*presspoint.x())
//            //鼠标点击的高亮绿点，查看细致坐标
//            {
//                ptr[i][0] = 50;
//                ptr[i][1] = 50;
//                ptr[i][2] = 205;
//            }
//            if( hightlight_flag && j / beishu  == presspoint.y() + 15 && i / beishu == 4*presspoint.x() +1)
//            //鼠标点击的高亮绿点，查看细致坐标
//            {
//                ptr[i][0] = 50;
//                ptr[i][1] = 205;
//                ptr[i][2] = 50;
//            }
//            if( hightlight_flag && j / beishu  == presspoint.y() + 15 && i / beishu == 4*presspoint.x()+2)
//            //鼠标点击的高亮绿点，查看细致坐标
//            {
//                ptr[i][0] = 50;
//                ptr[i][1] = 205;
//                ptr[i][2] = 50;
//            }
//            if( hightlight_flag && j / beishu  == presspoint.y() + 15 && i / beishu == 4*presspoint.x()+3)
//            //鼠标点击的高亮绿点，查看细致坐标
//            {
//                ptr[i][0] = 50;
//                ptr[i][1] = 205;
//                ptr[i][2] = 50;
//            }
//        }
//    }
//    return mat;

//}



//cv::Mat Widget::Wifi_Map2Mat_imagGray(uint8_t map[ROW][COL],uint8 beishu)//opencv库里面的矩阵数据类型，一般用来储存矩阵的信息（和二维数组有异曲同工）
//{  //beishu应该是图像放大倍数，CV_8UC3表示图像是3通道的
//    cv::Mat mat(ROW*beishu , COL*beishu , CV_8UC3);
//    for (int j = 0; j < mat.rows; ++j)
//    {  //MUTIPLE是放大倍数，/MUTIPLE当然是真实的坐标了，不过是CV坐标系的，转成代码的坐标系还需要坐标转换一下
//        cv::Vec3b *ptr = mat.ptr<cv::Vec3b>(j);
//        for (int i = 0; i < mat.cols; ++i)
//        {//BGR
//            uint8_t nowPixel = map[(j / beishu)][i / beishu];
//            ptr[i][0] = nowPixel;
//            ptr[i][1] = nowPixel;
//            ptr[i][2] = nowPixel;
//            if(hightlight_flag &&  j / beishu  == presspoint.y() && i / beishu == presspoint.x())
//            //鼠标点击的高亮绿点，查看细致坐标
//            {
//                ptr[i][0] = 50;
//                ptr[i][1] = 205;
//                ptr[i][2] = 50;
//            }

//        }
//    }

//    return mat;

//}




void Widget::imageInit(uchar *Pic)
{
//    // 计算需要拷贝的字节数
//    size_t bytesToCopy = ROW * COL * sizeof(uint8);
//    // 使用memcpy进行内存拷贝
//    memcpy(&Image_Used[0][0], &Pic[0], bytesToCopy);

}


void send_udp(QByteArray data,qint64 size,QHostAddress ip,quint16 port)
{

}

void Widget::_SendMessage()
{
    QHostAddress ip(ui->ip_lineEdit->text());
    quint16 port = ui->port_lineEdit->text().toUShort();


    ////////////////////
    QString Send_text = ui->SendMess_textEdit->toPlainText();
    QByteArray datastream = ui->SendMess_textEdit->toPlainText().toStdString().c_str();
//    qout<<ip;
//    qout<<port;
//    qout<<datastream;
    emit send_udp(datastream,datastream.size(),ip,port);
    ////////////////////


    QDateTime currentTime = QDateTime::currentDateTime(); // 获取当前时间
    QString messageWithTime = currentTime.toString("(yyyy-MM-dd hh:mm:ss)") + "me:      " + Send_text; // 将时间和文本组合
    ui->Reciver_textEdit->append(messageWithTime);
}


void Widget::wifi_writeDisk()
{
    if(WriteVideo_flag == true)
    {

        picture_count++;
//        for(int i = 0; i < recv_wifiNum; i++)
//        {
//            local_wifiBuff[i] = wifiBuff[i];
//        }

        dataByteArray.append(wifiBuff, sizeof(wifiBuff));// 将数据追加到 QByteArray 中
    }
    else if(WriteVideo_flag == false && WriteVideo_done_flag == true  && !dataByteArray.isEmpty())
    {
        QDateTime dateTime = QDateTime::currentDateTime();
        QString fileName = "D:/HostComputerFor_C_Wifi/picture/p";
        fileName += dateTime.toString("XXCCyyyy-MM-dd_HH.mm.ss");
        fileName += ".FKTMD";
        QFile file(fileName);
        if(file.exists()) // 文件已存在
        {



            return; // 可以选择直接返回，或者采取其他操作
        }
        if(file.open(QIODevice::ReadWrite)) // 打开文件
        {
            file.write(dataByteArray); // 将 QByteArray 数据写入文件
            file.close(); // 关闭文件
            dataByteArray.clear();



            WriteVideo_done_flag = false;
            WriteVideo_flag = false;
        }
        else // 打开文件失败
        {




        }
    }
}


void Widget::calculateFPS()
{
    // 计算帧率*
    if(frameCount > 300)
    {
        frameCount = 0;
    }
    if(ui->display_pushButton->text() == "停止")
    {
        ui->Frame_rate_label->show();
        if(image_trans_Flag){
            fps = frameCount;
            ui->Frame_rate_label->setText(QString("上位机显示帧率:%1帧").arg(fps));
            ui->Frame_rate_label->setStyleSheet(
                        "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:20px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Frame_rate_label->adjustSize();
            qout<<fps;
            frameCount = 0;
        }
        else{
            ui->Frame_rate_label->setText(QString("卡了"));
            ui->Frame_rate_label->setStyleSheet(
                        "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:20px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Frame_rate_label->adjustSize();
        }
        // 重置帧计数器

        picture = picture_count;
        picture_count = 0;
    }
    else
    {
        ui->Frame_rate_label->hide();
    }
}



void Widget::on_udp_connect_pushButton_clicked()
{
    if(ui->udp_connect_pushButton->text() == "连接")
    {
        pix_index = 0;
        QHostAddress tempip(ui->local_ip_comboBox->currentText());
        udp_recv->connectip = tempip;
        udp_recv->connectport = ui->local_port_lineEdit->text().toUInt();
        if( udp_recv->connect_flag)
        {
            ui->udp_connect_pushButton->setText("中断连接");
//            ui->Wait_connect_label->setText("连接到Udp客户端");
        }
        else
        {
//            ui->Wait_connect_label->setText("无客户端连接！");
            ui->WriteVideo_pushButton->hide();
        }
    }
    else if(ui->udp_connect_pushButton->text() == "中断连接")
    {

//        ui->Wait_connect_label->setText("未连接到Udp客户端");
        if(ui->display_pushButton->text() == "停止")
        {
            udp_recv->stop();
            ui->display_pushButton->setText("播放");
        }
        ui->WriteVideo_pushButton->hide();
        ui->udp_connect_pushButton->setText("连接");
        udp_recv->allow_recv_flag = 0;
    }
}




void Widget::on_display_pushButton_clicked()
{
    if(ui->udp_connect_pushButton->text() == "中断连接")
    {
        if(udp_recv->connect_flag == true)
        {
            if(ui->display_pushButton->text() == "播放")
            {
                pix_index = 0;
                udp_recv->allow_recv_flag = true;
                udp_recv->go_on(); //继续
                ui->display_pushButton->setText("停止");
                ui->WriteVideo_pushButton->show();

            }
            else if(ui->display_pushButton->text() == "停止")
            {
                udp_recv->allow_recv_flag = false;
                udp_recv->stop(); //继续
                ui->display_pushButton->setText("播放");
                ui->WriteVideo_pushButton->hide();
            }
        }
        else
        {
            udp_recv->allow_recv_flag = 0;
            udp_recv->stop(); //继续
//            ui->Wait_connect_label->setText("未连接到Udp客户端，请重试");
        }
    }
    else
    {
        QMessageBox::information(this,"警告","未连接Udp客户端！");
    }
}



void Widget::on_get_the_ip_pushButton_clicked()
{
    ui->local_ip_comboBox->clear();
    // 获取所有网络接口
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    // 遍历接口列表
    foreach (QNetworkInterface netInterface, interfaces) {
        // 过滤回环和非活动接口
        if (netInterface.flags().testFlag(QNetworkInterface::IsUp) && !netInterface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            // 获取接口地址条目列表
            QList<QNetworkAddressEntry> addressEntries = netInterface.addressEntries();

            foreach (QNetworkAddressEntry entry, addressEntries) {
                // 获取IP地址
                QHostAddress ip = entry.ip();
//                ui->ip_lineEdit->setText(ip.toString());
                QString ipAddress = ip.toString();
                // 判断IP地址类型，并添加到下拉框中
                if(ipAddress.contains("192.168"))
                {
                    ui->local_ip_comboBox->addItem(ipAddress);
                }

            }
        }
    }
}




void Widget::on_WriteVideo_pushButton_clicked()
{

    if(ui->display_pushButton->text() == "停止")
    {
        if(ui->WriteVideo_pushButton->text() == "录制△")
        {
            WriteVideo_flag = true;
            WriteVideo_done_flag =false;
            ui->WriteVideo_pushButton->setText("停止||") ;
            ui->WriteVideo_pushButton->setStyleSheet("color: red;");

        }
        else if(ui->WriteVideo_pushButton->text() == "停止||")
        {
            WriteVideo_flag = false;
            WriteVideo_done_flag = true;
            ui->WriteVideo_pushButton->setText("录制△") ;
            ui->WriteVideo_pushButton->setStyleSheet("color: green;");
        }

    }
    else
    {
            QMessageBox::information(this,"警告",QString("未连接到UDP客户端！\n请播放灰度图！"));
    }

}



void Widget::on_Hostcomputer_Mode_tabWidget_currentChanged(int index)
{

    if(index == 0)//本地
    {
        hightlight_flag = true;
        if(ui->udp_connect_pushButton->text() ==  "中断连接")
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(1);

        }
        else
        {
        ui->Pic_verticalSlider->show();
        ui->Pic_num_lineEdit->show();
        }

    }
    else if(index == 1)//Udp
    {
        if(runFlag == true)
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(0);


        }
        else
        {

            hightlight_flag = false;


//            Mat mat = Wifi_Map2Mat_imagGray(imgGray,MULTIPLE);
//            Wifi_showMat(mat, ui->imgGray_lable);
            ui->Pic_verticalSlider->hide();
            ui->Pic_num_lineEdit->hide();
            ui->pixelInfo_lable->hide();
        }
    }
}



void Widget::get_the_workshop()
{
DefaultFile = QDir::currentPath();  // 获取当前工作目录
DefaultFile.replace("build-", "");
DefaultFile.replace("-Desktop_Qt_5_9_9_MinGW_32bit-Debug", "");
DefaultFile.replace("-Desktop_Qt_5_9_9_MinGW_32bit-Profile", "");
DefaultFile.replace("-Desktop_Qt_5_9_9_MinGW_32bit-Release", "");
DefaultFile.append("/");
}



//还有opencv支持库的盘符记得在XXXXX.pro文件中修改库的文件目录，////////////////////////////////记得添加对应的环境变量地址
void Widget::on_wallpaper_pushbutton_clicked()
{

    if(Wallpaper_Choice)
    {
        Wallpaper_Choice = 0;
        ui->label_9->setStyleSheet("QLabel#label_9 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_10->setStyleSheet("QLabel#label_10 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_11->setStyleSheet("QLabel#label_11 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_12->setStyleSheet("QLabel#label_12 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_13->setStyleSheet("QLabel#label_13 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_14->setStyleSheet("QLabel#label_14 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_16->setStyleSheet("QLabel#label_16 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_38->setStyleSheet("QLabel#label_38 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->SDDriveLetter_label->setStyleSheet("QLabel#SDDriveLetter_label { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->Frame_rate_label->setStyleSheet("QLabel#Frame_rate_label { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->UTF8_WifiShow_checkBox->setStyleSheet("QCheckBox#UTF8_WifiShow_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->Hex_WifiShow_checkBox->setStyleSheet("QCheckBox#Hex_WifiShow_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->Hex_SerialShow_checkBox->setStyleSheet("QCheckBox#Hex_SerialShow_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->UTF8_SerialShow_checkBox->setStyleSheet("QCheckBox#UTF8_SerialShow_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->Serial_ShowTime_checkBox->setStyleSheet("QCheckBox#Serial_ShowTime_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->Wifi_ShowTime_checkBox->setStyleSheet("QCheckBox#Wifi_ShowTime_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 25px; }");



        setWallpaper();
    }
    else
    {
        Wallpaper_Choice = 1;
        ui->label_9->setStyleSheet("QLabel#label_9 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_10->setStyleSheet("QLabel#label_10 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_11->setStyleSheet("QLabel#label_11 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_12->setStyleSheet("QLabel#label_12 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_13->setStyleSheet("QLabel#label_13 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_14->setStyleSheet("QLabel#label_14 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_16->setStyleSheet("QLabel#label_16 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->label_38->setStyleSheet("QLabel#label_38 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->SDDriveLetter_label->setStyleSheet("QLabel#SDDriveLetter_label { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->Frame_rate_label->setStyleSheet("QLabel#Frame_rate_label { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->UTF8_WifiShow_checkBox->setStyleSheet("QCheckBox#UTF8_WifiShow_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->Hex_WifiShow_checkBox->setStyleSheet("QCheckBox#Hex_WifiShow_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->Hex_SerialShow_checkBox->setStyleSheet("QCheckBox#Hex_SerialShow_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->UTF8_SerialShow_checkBox->setStyleSheet("QCheckBox#UTF8_SerialShow_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->Serial_ShowTime_checkBox->setStyleSheet("QCheckBox#Serial_ShowTime_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");
        ui->Wifi_ShowTime_checkBox->setStyleSheet("QCheckBox#Wifi_ShowTime_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 25px; }");


        setWallpaper();
    }
}



void Widget::auto_night_mode()
{
    QIcon icon(DefaultFile + "sun.png") ;
    ui->wallpaper_pushbutton->setIcon(icon);
    QSize iconSize(60, 45);
    ui->wallpaper_pushbutton->setIconSize(iconSize);
    QTime currentTime = QTime::currentTime();
    QTime eveningTime(19, 0);//晚上
    QTime morningTime(6, 0);//白天
    bool isEvening = (currentTime >= eveningTime || currentTime < morningTime);
    int Flag = (isEvening) ? 0 : 1;
    if(!Flag)
    {
        on_wallpaper_pushbutton_clicked();
    }
}


void Widget::get_the_SDDriveLetter()
{
    // 清空ComboBox的内容
    ui->SDDriveLetter_comboBox->clear();

    // 使用Qt的QDir类获取系统的根目录
    QDir dir;
    QFileInfoList drives = dir.drives();

    // 将盘符添加到ComboBox
    foreach (QFileInfo drive, drives) {
        // 添加盘符到ComboBox
        QString SDletter = drive.absoluteFilePath();
        SDletter.replace("/", "");
        if(SDletter != "C:")
        {

            ui->SDDriveLetter_comboBox->addItem(SDletter);
        }
    }
}

void Widget::on_SendMess_pushButton_clicked()
{
    _SendMessage();
}

void Widget::on_control_up_pushbutton_clicked()
{

    QHostAddress ip(ui->ip_lineEdit->text());
    quint16 port = ui->port_lineEdit->text().toUShort();
    QByteArray datastream = "upupup";
    emit send_udp(datastream,datastream.size(),ip,port);
}

void Widget::on_control_down_pushbutton_clicked()
{

    QHostAddress ip(ui->ip_lineEdit->text());
    quint16 port = ui->port_lineEdit->text().toUShort();
    QByteArray datastream = "downdowndown";
    emit send_udp(datastream,datastream.size(),ip,port);
}


void Widget::on_control_left_pushbutton_clicked()
{

    QHostAddress ip(ui->ip_lineEdit->text());
    quint16 port = ui->port_lineEdit->text().toUShort();
    QByteArray datastream = "leftleftleft";
    emit send_udp(datastream,datastream.size(),ip,port);
}



void Widget::on_control_right_pushbutton_clicked()
{

    QHostAddress ip(ui->ip_lineEdit->text());
    quint16 port = ui->port_lineEdit->text().toUShort();
    QByteArray datastream = "rightrightright";
    emit send_udp(datastream,datastream.size(),ip,port);
}



/******************************************************************************
*
*
*
*
*
*
*
*
*
*串口有关函数
*
*
*
*
*
*
*
*
*
******************************************************************************/

void Widget::get_the_serialport()
{
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &portInfo, portList) {
        QString portName = portInfo.portName();
        connect_serialPort = portName;
        QString description = portInfo.description();
        QString manufacturer = portInfo.manufacturer();

        QString itemText = QString("%1 (%2, %3)").arg(portName, description, manufacturer);

        ui->serailCb->addItem(itemText, portName);
    }
}



void Widget::manual_serialPortReadyRead()
{
    QByteArray buf;
    buf=QByteArray(serialPort->readAll());
    qout<<buf;
    QString sshowdata;
    QByteArray showdata;
    QByteArray SerialData = buf;
    QString currentDateTimeString = "";
    if(Hex_SerialShow && UTF8_SerialShow)
    {
        sshowdata = buf;
        showdata  =  sshowdata.toUtf8().toHex();
    }
    else if(Hex_SerialShow&&!UTF8_SerialShow)
    {
        showdata = buf.toHex();
    }
    else if(!Hex_SerialShow&&UTF8_SerialShow)
    {
        sshowdata = buf;
        showdata =sshowdata.toUtf8();
    }
    else if(!Hex_SerialShow&&!UTF8_SerialShow)
    {
        showdata = buf;
    }
    qout<<showdata;
    if(Serial_ShowTime)
    {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        currentDateTimeString = currentDateTime.toString("(yyyy-MM-dd hh:mm:ss):     ");
        ui->recvEdit->append(currentDateTimeString + showdata);
    }
    else
    {
        ui->recvEdit->append(showdata);
    }


}


void Widget::on_Hex_SerialShow_checkBox_clicked()
{
    Hex_SerialShow = !Hex_SerialShow;
}

void Widget::on_UTF8_SerialShow_checkBox_clicked()
{
    UTF8_SerialShow = !UTF8_SerialShow;
}



void Widget::on_openBt_clicked()
{
    /*串口初始化*/
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity checkBits;


    if(ui->baundrateCb->currentText()=="4800")
        baudRate=QSerialPort::Baud4800;
    else if(ui->baundrateCb->currentText()=="9600")
        baudRate=QSerialPort::Baud9600;
    else if(ui->baundrateCb->currentText()=="115200")
        baudRate=QSerialPort::Baud115200;


    if(ui->databitCb->currentText()=="5")
        dataBits=QSerialPort::Data5;
    else if(ui->databitCb->currentText()=="6")
        dataBits=QSerialPort::Data6;
    else if(ui->databitCb->currentText()=="7")
        dataBits=QSerialPort::Data7;
    else if(ui->databitCb->currentText()=="8")
        dataBits=QSerialPort::Data8;



    if(ui->stopbitCb->currentText()=="1")
        stopBits=QSerialPort::OneStop;
    else if(ui->stopbitCb->currentText()=="1.5")
        stopBits=QSerialPort::OneAndHalfStop;
    else if(ui->stopbitCb->currentText()=="2")
        stopBits=QSerialPort::TwoStop;


    if(ui->checkbitCb->currentText()=="none")
        checkBits=QSerialPort::NoParity;

    serialPort->setPortName(connect_serialPort);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(checkBits);

    if(serialPort->open(QIODevice::ReadWrite)==true)
    {
            QMessageBox::information(this,"提示","成功");
            serial_open_Flag = true;
            ui->serail_condtion_label->setText(QString("连接成功"));
            ui->serail_condtion_label->setStyleSheet(
            "background-color:rgb(144,238,144);font-size:30px;color:black");  //255 105 180

    }
    else
    {

        if(serial_open_Flag)
        {
            QMessageBox::information(this,"警告","可能已连接其他串口，请关闭串口\n若需切换串口，请点击关闭串口");
            serial_open_Flag = true;
            ui->serail_condtion_label->setText(QString("连接成功"));
            ui->serail_condtion_label->setStyleSheet(
            "background-color:rgb(144,238,144);font-size:30px;color:black");  //255 105 180
        }
        else{
            serial_open_Flag = false;
            ui->serail_condtion_label->setText(QString("连接失败..."));
            ui->serail_condtion_label->setStyleSheet(
            "background-color:rgb(255,209,224);font-size:30px;color:black");
            QMessageBox::critical(this,"提示","失败");
        }
    }
}




void Widget::on_closeBt_clicked()
{
    serial_open_Flag = false;
    ui->serail_condtion_label->setText(QString("未连接..."));
    ui->serail_condtion_label->setStyleSheet(
    "background-color:rgb(255,209,224);font-size:30px;color:black");
    serialPort->close();
}



void Widget::on_sendBt_clicked()
{
    if(!serial_open_Flag)
    {
        QMessageBox::information(this,"提示","未连接串口！");
    }
    else{
        serialPort->write(ui->sendEdit->toPlainText().toLocal8Bit().data());
    }

}


/*清空*/
void Widget::on_clearBt_clicked()
{

    ui->recvEdit->clear();
}



void Widget::on_Serial_ShowTime_checkBox_clicked()
{
    Serial_ShowTime = !Serial_ShowTime;

}




void Widget::on_reflesh_seiralport_pushbutton_clicked()
{
    ui->serailCb->clear();
    get_the_serialport();

}






void Widget::on_trans_select_tabWidget_currentChanged(int index)
{
    if(index != 3)
    {
        if(serial_open_Flag == true)
        {
            ui->trans_select_tabWidget->setCurrentIndex(3);
            QMessageBox::information(this,"警告","请先断开串口连接");
        }
    }

}




