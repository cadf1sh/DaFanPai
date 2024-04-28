#include "widget.h"
#include "ui_widget.h"
#include <math.h>
QT_CHARTS_USE_NAMESPACE
extern uint8_t imgGray[ROW][COL];
extern uint8_t allmap[YM][XM];
extern uint8_t basemap[YM][XM];
extern uint8_t leftmap[YM][XM];
extern uint8_t rightmap[YM][XM];
extern uint8_t insidemap[YM][XM];
extern uint8_t deletemap[YM][XM];
extern uint8_t nowThreshold;
 uint8_t acThreshold;
extern ImgInfo imgInfoInit;
extern ImgInfo imgInfo;
extern ImgFlag imgFlag;
extern ImgFlag LAST_IF;
ImgFlag IIF;
ImgInfo III;
extern float leftline[YM];
extern float rightline[YM];
extern uint8_t speedlineLeft[YM];
extern uint8_t speedlineRight[YM];
extern uint8_t midline[YM/2];
extern int8_t top_line;
extern ControlLine controlLine;
uint8_t drawCurve_flag = 0;  //曲线绘图标志
uint16_t act_aimSpeed;
int x1, x2, x3, x4;
extern EulerAngleTypedef SystemAttitude;
extern EulerAngleTypedef SystemAttitudeRate;
int16_t deviation;
extern float kbleft[2];
extern float kbright[2];
extern float kbmid[2];
extern uint8_t speedtype;
int16_t tarYawRate = 0;
int16_t YawRate = 0;
int16_t tarPitch = 0;
int16_t Pitch = 0;
int16_t tarPitchRate = 0;
int16_t pitchRate = 0;
int16_t curvature = 0;
int16_t speedMin = 0;
EulerAngleTypedef Tar_Ang_Vel;
EulerAngleTypedef Tar_Ang_Vel_Last;
EulerAngleTypedef Target_Attitude;
uint8_t drawLine_flag = 0;
int16_t *drawLine_var1, *drawLine_var2;
RunState runState = RUNNING;
uint8_t true_speedType;
int16_t dealImgtime;
uint8_t true_Threshold;
uint16_t goTime;
int16_t Yaw_Rate_variance;
int16_t Pitch_Rate_variance;
int16_t balance_duty;
int16_t dir_duty;
int32_t centri_acce;
int16_t my_yaw;
int16_t compensate_time;
float path;
uint8_t runstate;
extern uint16_t nowRampNum;
int16 LEncoder=0;
int16 REncoder=0;
int16 Speed_Error=0;
int16 NowAnnulusNum=0;
int16 Current_L_PWM=0;
int16 Current_R_PWM=0;
int16 L_Current_Control=0;
int16 R_Current_Control=0;

int16 Cam_out=0;
int16 Curvature_Final=0;
int16 true_CarDirACC=0;
int16 Inner_Error=0;
int16 Inner_OutPut=0;
int16 Inner_InPut=0;
int16 my_pit=0;
int16_t speed_TrueM = 0;
int16 Speed_Out=0;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    Start_Screen();
    QHostAddress local_ip(1) ;
    QString udp_port = "8080";
    //setWallpaper();
    udp_recv = new udp_thread(local_ip,udp_port);
    connect(udp_recv, SIGNAL(signal_img(QByteArray)), this, SLOT(display_udp_data(QByteArray)));
    connect(this, SIGNAL(send_udp(QByteArray,QHostAddress,quint16)), udp_recv, SLOT(send_message(QByteArray,QHostAddress,quint16)));
    udp_recv->start();
    udp_recv->stop(); //暂停udp线程，等到开启接收画面后再开启工作

    // 创建一个定时器
    fpstimer = new QTimer(this);
    connect(fpstimer, SIGNAL(timeout()), this, SLOT(calculateFPS()));
    fpstimer->start(1000);


    //////////////////////////////串口监听
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    serialPort = new QSerialPort(this);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(display_serial_data()));/*手动连接槽函数*/
    //////////////////////////////

}


void Widget::allinit(){
    ui->graphicsView->hide();
    on_Open_Wifi_checkBox_clicked(0);
    on_Open_Serial_checkBox_clicked(0);
    map_installEventFilter();
    get_the_serialport();
    Widget::setWindowTitle("Car_Link");
    WriteVideo_done_flag  = false;
    WriteVideo_flag  = false;
    map_clicked_mode  = false;
    hightlight_flag = true;
    Wallpaper_Choice = true;
    QIcon windowicon(DefaultFile + "icon.jpg");
    Widget::setWindowIcon(windowicon);  // 替换为你的图标文件路径
    ui->WriteVideo_pushButton->setStyleSheet("color: green;");
    ui->trans_select_tabWidget->setCurrentIndex(0);
    Watch_the_silder(0);
    ui->local_port_lineEdit->setText("8080");
    image_trans_Flag = false;
    ac_pix_index = 0;
    pix_index = 0;
    //auto_night_mode();//自动夜间模式
    get_the_SDDriveLetter();
    picture_count = 0;
    frameCount = 0;
    temp_WifiData = 0;
    temp_SerialData = 0;
    SerialData_rate = 0;
    WifiData_rate = 0;
    ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(0);
    ui->port_lineEdit->setText("6666");
    Hex_SerialShow = false ;
    UTF8_SerialShow = false ;
    Serial_ShowTime = false ;
    serial_open_Flag = false;
    ui->openBt->setStyleSheet("color:green");
    ui->udp_connect_pushButton->setStyleSheet("color:green");
    ui->display_pushButton->setStyleSheet("color:green");
    ui->trans_select_tabWidget->setCurrentIndex(0);

    QIcon icon(DefaultFile + "sun.png") ;
    ui->wallpaper_pushbutton->setIcon(icon);
    QSize iconSize(45, 36);
    ui->wallpaper_pushbutton->setIconSize(iconSize);
}



Widget::~Widget()
{
    delete ui;
}



void Widget::setWallpaper()
{

    changeConeColorSlot();
    this->setAutoFillBackground(true); // 这句要加上，否则可能显示不出背景图
    QPalette palette1 = this->palette();
    palette1.setBrush(this->backgroundRole(), backgroundColor);
    this->setPalette(palette1);
    setAutoFillBackground(true);

 // QPalette palette = this->palette();
//    if (!Wallpaper_Choice)
//    {
//        QPixmap wallpaper(DefaultFile + "Dark_widget.jpg");
//        wallpaper = wallpaper.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        // 设置主窗口背景图
//        palette.setBrush(QPalette::Window, QBrush(wallpaper));
//        this->setPalette(palette);
//        wallpaper = wallpaper.scaled(ui->trans_select_tabWidget->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        // 设置 trans_select_tabWidget 的背景图
//        QPalette tabPalette = ui->trans_select_tabWidget->palette();
//        tabPalette.setBrush(QPalette::Window, QBrush(wallpaper));
//        ui->trans_select_tabWidget->setPalette(tabPalette);

//    }
//    else
//    {

//        QPixmap wallpaper(DefaultFile + "Bright_widget.jpg");
//        wallpaper = wallpaper.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        // 设置主窗口背景图
//        palette.setBrush(QPalette::Window, QBrush(wallpaper));
//        this->setPalette(palette);
//        wallpaper = wallpaper.scaled(ui->trans_select_tabWidget->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        // 设置 trans_select_tabWidget 的背景图
//        QPalette tabPalette = ui->trans_select_tabWidget->palette();
//        tabPalette.setBrush(QPalette::Window, QBrush(wallpaper));
//        ui->trans_select_tabWidget->setPalette(tabPalette);
//    }

}

void Widget::changeConeColorSlot()
{
    QColorDialog dialog(Qt::white, this);
    dialog.setOption(QColorDialog::ShowAlphaChannel);
    connect(&dialog, &QColorDialog::accepted, this, &Widget::colorDialogAccepted);//我们点了ok接受才行

    dialog.exec();
}

void Widget::colorDialogAccepted()
{
    QColorDialog* dialog = qobject_cast<QColorDialog*>(sender());
    if (dialog)
    {
        backgroundColor = dialog->currentColor();
        save_color();
    }
}


void Widget::save_color()
{
    QString fileName = DefaultFile + "/color.txt";  // 构建完整的文件路径
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::information(this, "警告", "无法打开文件！");
        return;
    }
    QTextStream out(&file);
    out << backgroundColor.name(); // 将颜色名称写入文件
    file.close();
}


void Widget::load_color()
{
    QString fileName = DefaultFile + "/color.txt"; // 文件路径
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this, "警告", "无法打开文件！");
        return;
    }
    QTextStream in(&file);
    QString colorName = in.readAll(); // 读取文件中的颜色名称
    file.close();
    backgroundColor = QColor(colorName); // 创建颜色对象
    this->setAutoFillBackground(true); // 这句要加上，否则可能显示不出背景图
    QPalette palette1 = this->palette();
    palette1.setBrush(this->backgroundRole(), backgroundColor);
    this->setPalette(palette1);
    setAutoFillBackground(true);
}



void Widget::letGo()//ui变量名显示
{
    if(readType_flag == 1)
    {
        go();


        ui->ac_pix_index_label->setText(QString("现在实际是%1幅图").arg(ac_pix_index));
        ui->ac_pix_index_label->setStyleSheet(
                "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->ac_pix_index_label->adjustSize();//当前图像的顺序


        ui->Pic_num_label->setText(QString("Pic_num: %1").arg(dataCnt));
        ui->Pic_num_label->setStyleSheet(
                "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->Pic_num_label->adjustSize();//当前图像的顺序

        ui->Total_num_label->setText(QString("Total_num: %1").arg(dataNum));
        ui->Total_num_label->setStyleSheet(
                 "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->Total_num_label->adjustSize();//当前图像的顺序



        ui->Frame_rate_label->show();
        ui->Frame_rate_label->setText(QString("上位机显示帧率:%1帧").arg(camera_fps));
        ui->Frame_rate_label->setStyleSheet(
                    "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->Frame_rate_label->adjustSize();


        /*图像处理时间*/
        ui->dealImgTime_label->setText(QString("dealImgTime:%1").arg(Recvchannel[0]));
        ui->dealImgTime_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->dealImgTime_label->adjustSize();
        /*go时间*/
        ui->gotime_label->setText(QString("gotime:%1").arg(Recvchannel[1]/200.0));
        ui->gotime_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->gotime_label->adjustSize();
        /*电池电压*/
        ui->Battery_label->setText(QString("Battery:%1").arg(Recvchannel[2]/10.0));
        ui->Battery_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Battery_label->adjustSize();


        /*当前图像的顺序*/
        ui->nowThreshold_label->setText(QString("nowThreshold: %1").arg(nowThreshold));
        ui->nowThreshold_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->nowThreshold_label->adjustSize();
        ui->acThreshold_label->setText(QString("acThreshold: %1").arg(acThreshold));
        ui->acThreshold_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->acThreshold_label->adjustSize();
        /*II.top*/
        ui->top_label->setText(QString("top: %1").arg(II.top));
        ui->top_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->top_label->adjustSize();
        /*II.bnum_all*/
        ui->bnum_lable->setText(QString("bnum: %1").arg(II.bnum_all));
        ui->bnum_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->bnum_lable->adjustSize();
        /*II.lnum_all*/
        ui->lnum_lable->setText(QString("lnum: %1").arg(II.lnum_all));
        ui->lnum_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->lnum_lable->adjustSize();
        /*II.rnum_all*/
        ui->rnum_lable->setText(QString("rnum: %1").arg(II.rnum_all));
        ui->rnum_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->rnum_lable->adjustSize();
        /*TR_Y(0)，TR_X(0)*/
        ui->top_right_lable->setText(QString("TR: %1 %2").arg(TR_Y(0)).arg(TR_X(0)));
        ui->top_right_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->top_right_lable->adjustSize();
        /*RT_Y(0)，RT_X(0)*/
        ui->right_top_lable->setText(QString("RT: %1 %2").arg(RT_Y(0)).arg(RT_X(0)));
        ui->right_top_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->right_top_lable->adjustSize();
        /*RB_Y(0)，RB_X(0)*/
        ui->right_bottom_lable->setText(QString("RB: %1 %2").arg(RB_Y(0)).arg(RB_X(0)));
        ui->right_bottom_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->right_bottom_lable->adjustSize();
        /*BR_Y(0)，BR_X(0)*/
        ui->bottom_right_lable->setText(QString("BR: %1 %2").arg(BR_Y(0)).arg(BR_X(0)));
        ui->bottom_right_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->bottom_right_lable->adjustSize();
        /*TL_Y(0)，TL_X(0)*/
        ui->top_left_lable->setText(QString("TL: %1 %2").arg(TL_Y(0)).arg(TL_X(0)));
        ui->top_left_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->top_left_lable->adjustSize();
        /*LT_Y(0)，LT_X(0)*/
        ui->left_top_lable->setText(QString("LT: %1 %2").arg(LT_Y(0)).arg(LT_X(0)));
        ui->left_top_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->left_top_lable->adjustSize();
        /*LB_Y(0)，LB_X(0)*/
        ui->left_bottom_lable->setText(QString("LB: %1 %2").arg(LB_Y(0)).arg(LB_X(0)));
        ui->left_bottom_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->left_bottom_lable->adjustSize();
        /*BL_Y(0)，BL_X(0)*/
        ui->bottom_left_lable->setText(QString("BL: %1 %2").arg(BL_Y(0)).arg(BL_X(0)));
        ui->bottom_left_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->bottom_left_lable->adjustSize();
        /*II.start_lm[0]，II.start_lm[1]*/
        ui->start_lm_lable->setText(QString("start_lm: %1 %2").arg(II.start_lm[0]).arg(II.start_lm[1]));
        ui->start_lm_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->start_lm_lable->adjustSize();
        /*II.start_rm[0]，II.start_rm[1]*/
        ui->start_rm_lable->setText(QString("start_rm: %1 %2").arg(II.start_rm[0]).arg(II.start_rm[1]));
        ui->start_rm_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->start_rm_lable->adjustSize();
        /*II.num_lm*/
        ui->num_lm_lable->setText(QString("num_lm: %1").arg(II.num_lm));
        ui->num_lm_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->num_lm_lable->adjustSize();
        /*II.num_rm*/
        ui->num_rm_lable->setText(QString("num_rm: %1").arg(II.num_rm));
        ui->num_rm_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->num_rm_lable->adjustSize();
        /*II.dnum_all*/
        ui->dnum_all_lable->setText(QString("dnum: %1").arg(II.dnum_all));
        ui->dnum_all_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->dnum_all_lable->adjustSize();
        /*II.speedTop*/
        ui->speedTop_lable->setText(QString("speedTop: %1").arg(II.speedTop));
        ui->speedTop_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->speedTop_lable->adjustSize();
        /*II.repeatNum*/
        ui->repeatnum_label->setText(QString("repeatnum: %1").arg(II.repeatNum));
        ui->repeatnum_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->repeatnum_label->adjustSize();
        /*II.d_bottom.x，II.d_bottom.y*/
        ui->d_bottom_lable->setText(QString("d_bottom: %1 %2").arg(II.d_bottom.x).arg(II.d_bottom.y));
        ui->d_bottom_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->d_bottom_lable->adjustSize();
        /*IF.startline*/
        ui->ac_startline_label->setText(QString("ac_StartLine:%1").arg(IIF.startline));
        ui->ac_startline_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->ac_startline_label->adjustSize();
        /*IF.startline*/
        ui->startline_label->setText(QString("StartLine:%1").arg(IF.startline));
        ui->startline_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->startline_label->adjustSize();
        /*kbleft[0]，kbleft[1]*/
        ui->kbleft_label->setText(QString("kL = %1, bL = %2").arg(kbleft[0]).arg(kbleft[1]));
        ui->kbleft_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->kbleft_label->adjustSize();
        /*kbright[0]，kbright[1]*/
        ui->kbright_lable->setText(QString("kR = %1, bR = %2").arg(kbright[0]).arg(kbright[1]));
        ui->kbright_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->kbright_lable->adjustSize();
        /*LSMI.K，LSMI.B*/
        ui->kk_bb_label->setText(QString("K = %1 B = %2").arg(LSMI.K).arg(LSMI.B));
        ui->kk_bb_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->kk_bb_label->adjustSize();
        /*LSMI.absDeviation[0]*/
        ui->aveDeviation_label->setText(QString("aveDeviation: %1").arg(LSMI.absDeviation[0]));
        ui->aveDeviation_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->aveDeviation_label->adjustSize();
        /*II.leftnum*/
        ui->left_num_lable->setText(QString("leftnum: %1").arg(II.leftnum));
        ui->left_num_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->left_num_lable->adjustSize();
        /*II.midnum*/
        ui->mid_num_lable->setText(QString("midnum: %1").arg(II.midnum));
        ui->mid_num_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->mid_num_lable->adjustSize();
        /*LSMI.nowDeviation*/
        ui->nowDev_label->setText(QString("nowDev: %1").arg(LSMI.nowDeviation));
        ui->nowDev_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->nowDev_label->adjustSize();
        /*IF.ramp*/
        ui->Ramp_label->setText(QString("Ramp: %1").arg(IF.ramp));
        ui->Ramp_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Ramp_label->adjustSize();

       /*deviation*/
        ui->act_deviation_label->setText(QString("true_deviation: %1").arg(deviation));
        ui->act_deviation_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->act_deviation_label->adjustSize();
        /*curvature*/
        ui->curvature_label->setText(QString("Curvature: %1").arg(curvature));
        ui->curvature_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->curvature_label->adjustSize();

        /*六轴*/
        /*Yaw*/
        ui->Yaw_label->setText(QString("Yaw: %1").arg(my_yaw));
        ui->Yaw_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Yaw_label->adjustSize();
        ui->Yawrate_label->setText(QString("YawRate:%1").arg(YawRate));
        ui->Yawrate_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Yawrate_label->adjustSize();
        /*电机相关*/
        ui->motor_label->setText(QString("电机相关数据"));
        ui->motor_label->setStyleSheet(
                     "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->motor_label->adjustSize();
        /*Length*/
        ui->Length_label->setText(QString("Length: %1").arg(SI.Length));
        ui->Length_label->setStyleSheet(
                     "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Length_label->adjustSize();
        /*Speed_Error*/
        ui->SpeedError_label->setText(QString("SpeedError: %1").arg(Speed_Error));
        ui->SpeedError_label->setStyleSheet(
                     "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->SpeedError_label->adjustSize();
        /*SI.aimSpeed*/
        ui->aimSpeed_label->setText(QString("aimSpeed: %1").arg(SI.aimSpeed));
        ui->aimSpeed_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->aimSpeed_label->adjustSize();
        /*SI.speedTrueM*/
        ui->acspeed_label->setText(QString("speedTrueM: %1").arg(speed_TrueM));
        ui->acspeed_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->acspeed_label->adjustSize();
        /*Speed_Out*/
        ui->Speed_Out_label->setText(QString("Speed_Out: %1").arg(Speed_Out));
        ui->Speed_Out_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Speed_Out_label->adjustSize();
        /*Current_L_PWM*/
        ui->Current_Llabel->setText(QString("Current_Llabel: %1").arg(Current_L_PWM));
        ui->Current_Llabel->setStyleSheet(
                     "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Current_Llabel->adjustSize();
        /*Current_R_PWM*/
        ui->Current_Rlabel->setText(QString("Current_Rlabel: %1").arg(Current_R_PWM));
        ui->Current_Rlabel->setStyleSheet(
                     "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Current_Rlabel->adjustSize();
        /*L_Current_Control*/
        ui->L_Current_label->setText(QString("L_Current_Control: %1").arg(L_Current_Control));
        ui->L_Current_label->setStyleSheet(
                     "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->L_Current_label->adjustSize();
        /*R_Current_Control*/
        ui->R_Current_label->setText(QString("R_Current_Control: %1").arg(R_Current_Control));
        ui->R_Current_label->setStyleSheet(
                     "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->R_Current_label->adjustSize();
        /*LEncoder*/
        ui->LEncoder_label->setText(QString("LEncoder: %1").arg(LEncoder));
        ui->LEncoder_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->LEncoder_label->adjustSize();
        /*REncoder*/
        ui->REncoder_label->setText(QString("REncoder: %1").arg(REncoder));
        ui->REncoder_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->REncoder_label->adjustSize();
         /*SI.motorPWMR*/
        ui->motorPWMR_label->setText(QString("motorPWMR: %1").arg(SI.motorPWMR));
        ui->motorPWMR_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->motorPWMR_label->adjustSize();
        /*SI.motorPWML*/
        ui->motorPWML_label->setText(QString("motorPWML: %1").arg(SI.motorPWML));
        ui->motorPWML_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->motorPWML_label->adjustSize();

        /*方向外环*/
        /*true_CarDirACC_label*/
        ui->true_CarDirACC_Final_label->setText(QString("true_CarDirACC: %1").arg(true_CarDirACC));
        ui->true_CarDirACC_Final_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->true_CarDirACC_Final_label->adjustSize();
        /*Cam_out*/
        ui->Cam_out_label->setText(QString("Cam_out: %1").arg(Cam_out));
        ui->Cam_out_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Cam_out_label->adjustSize();
        /*Curvature_Final*/
        ui->Curvature_Final_label->setText(QString("Curvature_Final: %1").arg(Curvature_Final));
        ui->Curvature_Final_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Curvature_Final_label->adjustSize();

        /*方向内环*/
        /*Inner_Error*/
        ui->Inner_Error_label->setText(QString("Inner_Error: %1").arg(Inner_Error));
        ui->Inner_Error_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Inner_Error_label->adjustSize();
        /*Inner_OutPut*/
        ui->Inner_OutPut_label->setText(QString("Inner_OutPut: %1").arg(Inner_OutPut));
        ui->Inner_OutPut_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Inner_OutPut_label->adjustSize();
        /*Inner_InPut*/
        ui->Inner_InPut_label->setText(QString("Inner_InPut: %1").arg(Inner_InPut));
        ui->Inner_InPut_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->Inner_InPut_label->adjustSize();

        ui->ac_Ramp_lable->setText(QString("ac_Ramp: %1").arg(IIF.ramp));
        ui->ac_Ramp_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->ac_Ramp_lable->adjustSize();

        ui->acnowAnnulusNum_label->setText(QString("ac_AnNum: %1").arg(NowAnnulusNum));
        ui->acnowAnnulusNum_label->setStyleSheet(
                     "background-color: rgb(129, 255, 233);font-size:20px;color:black");  //255 105 180
        ui->acnowAnnulusNum_label->adjustSize();

        QString str;
        ui->obstacle_label->setText(QString("obstacle:%1").arg(IF.obstacle));
        ui->obstacle_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
        ui->obstacle_label->adjustSize();

        ui->ac_obstacle_label->setText(QString("ac_obstacle:%1").arg(IIF.obstacle));
        ui->ac_obstacle_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
        ui->ac_obstacle_label->adjustSize();


        switch(IF.annulus)
        {
        case AL1:
            str = "AL1";
            break;
        case AL2:
            str = "AL2";
            break;
        case AL3:
            str = "AL3";
            break;
        case AL4:
            str = "AL4";
            break;
        case AL5:
            str = "AL5";
            break;
        case AR1:
            str = "AR1";
            break;
        case AR2:
            str = "AR2";
            break;
        case AR3:
            str = "AR3";
            break;
        case AR4:
            str = "AR4";
            break;
        case AR5:
            str = "AR5";
            break;
        default:
            str = "0";
            break;
        }
        if(IF.annulusDelay == 1)
        {
            ui->Annulus_label->setText(QString("Annulus: Annulus_dalay"));
        }
        else if(IF.annulusDelay == 0)
        {
            ui->Annulus_label->setText(QString("Annulus: ") + str);
        }
        ui->Annulus_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
        ui->Annulus_label->adjustSize();
        switch(IIF.annulus)
        {
        case AL1:
            str = "AL1";
            break;
        case AL2:
            str = "AL2";
            break;
        case AL3:
            str = "AL3";
            break;
        case AL4:
            str = "AL4";
            break;
        case AL5:
            str = "AL5";
            break;
        case AR1:
            str = "AR1";
            break;
        case AR2:
            str = "AR2";
            break;
        case AR3:
            str = "AR3";
            break;
        case AR4:
            str = "AR4";
            break;
        case AR5:
            str = "AR5";
            break;
        default:
            str = "0";
            break;
        }
        if(IIF.annulusDelay == 1)
        {
            ui->ac_Annulus_label->setText(QString("ac_Annulus: Annulus_dalay"));
        }
        else if(IIF.annulusDelay == 0)
        {
            ui->ac_Annulus_label->setText(QString("ac_Annulus: ") + str);
        }
        ui->ac_Annulus_label->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
        ui->ac_Annulus_label->adjustSize();

                switch(speedtype)
                {
                case StraightL:
                    str = "StraightL";
                    break;
                case StraightS:
                    str = "StraightS";
                    break;
                case StraightBreak:
                    str = "StraightBreak";
                    break;
                case Curve:
                    str = "Curve";
                    break;
                case Ramp:
                    str = "Ramp";
                    break;
                case Annulus:
                    str = "Annulus";
                    break;
                }
            ui->speedType_label->setText(QString("speedType : ") + str);
            ui->speedType_label->setStyleSheet(
                        "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
            ui->speedType_label->adjustSize();
                switch(true_speedType)
                {
                case StraightL:
                    str = "StraightL";
                    break;
                case StraightS:
                    str = "StraightS";
                    break;
                case StraightBreak:
                    str = "StraightBreak";
                    break;
                case Curve:
                    str = "Curve";
                    break;
                case Ramp:
                    str = "Ramp";
                    break;
                case Annulus:
                    str = "Annulus";
                    break;
                }
            ui->true_speedType_label->setText(QString("ac_speedType : ") + str);
            ui->true_speedType_label->setStyleSheet(
                        "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
            ui->true_speedType_label->adjustSize();
            switch(runState)
            {
            case STOP:
                str = "stop";
                break;
            case BALANCE:
                str = "balance";
                break;
            case READY:
                str = "ready";
                break;
            case RUNNING:
                str = "running";
                break;
            case BRAKING:
                str = "braking";
                break;
            }
            ui->runState_lable->setText(QString("runState:" + str));
            ui->runState_lable->setStyleSheet(
                        "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
            ui->runState_lable->adjustSize();
        switch(II.line_forbid)
        {
        case 0:
            str = "BOTHLINE";
            break;
        case 1:
            str = "LEFTLINE";
            break;
        case 2:
            str = "RIGHTLINE";
            break;
        case 3:
            str = "MIDLINE";
            break;
        case 4:
            str = "NONELINE";
            break;
        default:
            break;
        }
        ui->line_forbid_lable->setText(QString("line_forbid: ") + str);
        ui->line_forbid_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
        ui->line_forbid_lable->adjustSize();
        switch(controlLine)
        {
        case 0:
            str = "BOTHLINE";
            break;
        case 1:
            str = "LEFTLINE";
            break;
        case 2:
            str = "RIGHTLINE";
            break;
        case 3:
            str = "MIDLINE";
            break;
        case 4:
            str = "NONELINE";
            break;
        default:
            break;
        }
        ui->controlLine_lable->setText(QString("contrilLine: ") + str);
        ui->controlLine_lable->setStyleSheet(
                    "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
        ui->controlLine_lable->adjustSize();
    }
    else if(readType_flag == 2)
    {

        go();



        ui->Pic_num_label->setText(QString("Pic_num: %1").arg(dataCnt));
        ui->Pic_num_label->setStyleSheet(
                "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->Pic_num_label->adjustSize();//当前图像的顺序

        ui->Total_num_label->setText(QString("Total_num: %1").arg(dataNum));
        ui->Total_num_label->setStyleSheet(
                 "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->Total_num_label->adjustSize();//当前图像的顺序

        ui->ccd111_label->setText(QString("ccd1"));
        ui->ccd111_label->setStyleSheet(
                 "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->ccd111_label->adjustSize();//当前图像的顺序

        ui->ccd222_label->setText(QString("ccd2"));
        ui->ccd222_label->setStyleSheet(
                 "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->ccd222_label->adjustSize();//当前图像的顺序

        ui->ccd333_label->setText(QString("transccd1"));
        ui->ccd333_label->setStyleSheet(
                 "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->ccd333_label->adjustSize();//当前图像的顺序


        ui->ccd444_label->setText(QString("transccd2"));
        ui->ccd444_label->setStyleSheet(
                 "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->ccd444_label->adjustSize();//当前图像的顺序




        ui->s_SpeedLoop_output_label->show();
        ui->s_SpeedLoop_output_label->setText(QString("SpeedLoop_output:%1").arg(1));
        ui->s_SpeedLoop_output_label->setStyleSheet(
                    "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->s_SpeedLoop_output_label->adjustSize();
    }




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
    if(readType_flag == 1)//wifi_Read == 1
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

        for (int j = 0; j < ROW; j++)
        {
            for (int i = 0; i < COL; i++)
            {
                imgGray[j][i] = sdBuff[5 + j*COL + i ];
                wifiNum++;
            }
        }

        ///    /////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        if(sdBuff[13165] != 'V' || sdBuff[13166] != 'A' || sdBuff[13167] != 'L' || sdBuff[13168] != 'U' || sdBuff[13169] != 'E')
        {
            return false;
        }
        //16位数据格式
        ac_pix_index = (sdBuff[13170] << 8) | ((uint8_t)sdBuff[13171]);
        if(ac_pix_index - pix_index == 1 )
        {

            pix_index = ac_pix_index;
        }
        else
        {
            if(pix_index != 0)
            {

                pix_index = ac_pix_index;
            }
        }
        //8位数据格式
        uint8 test = sdBuff[13172];//test ok!!!7
    //    qout<<test;
        Recvchannel[0] = (sdBuff[13173] << 8) | ((uint8_t)sdBuff[13174]);//8,9
        Recvchannel[1] = (sdBuff[13175] << 8) | ((uint8_t)sdBuff[13176]);//10,11
        Recvchannel[2] = (sdBuff[13177] << 8) | ((uint8_t)sdBuff[13178]);//12,13
        Recvchannel[3] = (sdBuff[13179] << 8) | ((uint8_t)sdBuff[13180]);//14,15
        Recvchannel[4] = (sdBuff[13181] << 8) | ((uint8_t)sdBuff[13182]);//16,17
        Recvchannel[5] = (sdBuff[13183] << 8) | ((uint8_t)sdBuff[13184]);//18,29
        Recvchannel[6] = (sdBuff[13185] << 8) | ((uint8_t)sdBuff[13186]);//20,21


        /////////////////////////////////////////////////////////
        /*摄像头参数*/
        /////////////////////////////////////////////////////////
        uint8 test1 = sdBuff[13187];
        acThreshold = (sdBuff[13188] << 8) | ((uint8_t)sdBuff[13189]);
        deviation = (sdBuff[13190] << 8) | ((uint8_t)sdBuff[13191]);
        true_speedType = sdBuff[13192];
        IIF.ramp = sdBuff[13193];
        IIF.annulus = sdBuff[13194];
        IIF.annulusDelay = sdBuff[13195];
        IIF.obstacle = sdBuff[13196];
        IIF.startline = sdBuff[13197];
        NowAnnulusNum = (sdBuff[13198] << 8) | ((uint8_t)sdBuff[13199]);
        my_yaw = (sdBuff[13200] << 8) | ((uint8_t)sdBuff[13201]);
        YawRate = (sdBuff[13202] << 8) | ((uint8_t)sdBuff[13203]);
        /*速度参数*/
        SI.Length = (sdBuff[13204] << 8) | ((uint8_t)sdBuff[13205]);
        Speed_Error = (sdBuff[13206] << 8) | ((uint8_t)sdBuff[13207]);
        SI.aimSpeed = (sdBuff[13208] << 8) | ((uint8_t)sdBuff[13209]);
        speed_TrueM=(sdBuff[13210] << 8) | ((uint8_t)sdBuff[13211]);
        Speed_Out = (sdBuff[13212] << 8) | ((uint8_t)sdBuff[13213]);
        Current_R_PWM=(sdBuff[13214] << 8) | ((uint8_t)sdBuff[13215]);
        Current_L_PWM=(sdBuff[13216] << 8) | ((uint8_t)sdBuff[13217]);
        R_Current_Control=(sdBuff[13218]) | ((uint8_t)sdBuff[13219]);
        L_Current_Control=(sdBuff[13220] << 8) | ((uint8_t)sdBuff[13221]);
        REncoder=(sdBuff[13222] << 8) | ((uint8_t)sdBuff[13223]);
        LEncoder=(sdBuff[13224] << 8) | ((uint8_t)sdBuff[13225]);
        SI.motorPWMR=(sdBuff[13226] << 8) | ((uint8_t)sdBuff[13227]);
        SI.motorPWML=(sdBuff[13228] << 8) | ((uint8_t)sdBuff[13229]);

        /*方向外环*/
        Cam_out =(sdBuff[13230] << 8) | ((uint8_t)sdBuff[13231]);
        Curvature_Final =(sdBuff[13232] << 8) | ((uint8_t)sdBuff[13233]);
        true_CarDirACC =(sdBuff[13234] << 8) | ((uint8_t)sdBuff[13235]);
        /*方向内环*/
        Inner_Error =(sdBuff[13236] << 8) | ((uint8_t)sdBuff[13237]);
        Inner_OutPut =(sdBuff[13238] << 8) | ((uint8_t)sdBuff[13239]);
        Inner_InPut =(sdBuff[13240] << 8) | ((uint8_t)sdBuff[13241]);

        camera_fps = sdBuff[13242];//不要改变//误删//误删这是上位机FPS显示的暂存值
        ////////////////////////


        return true;
    }
    else if(readType_flag == 2)//serial_Read == 2
    {
        if(data[offset] != 'C')
        {
            return false;  //帧头不对
        }

        for (uint16_t i = 0; i < seriallength; i++)
        {
            serialbuff.append(tempserial[offset + i]);
        }

        int recv_serialNum = 5;
        if(serialbuff[0] != 'C' || serialbuff[1] != 'Z' || serialbuff[2] != 'Y' || serialbuff[3] != 'N' || serialbuff[4] != 'B')
        {

            image_trans_Flag = false;
            return false;
        }
        for (uint8 i = 0 ; i < ccd_col; i ++) {
            ccd1[i] = serialbuff[recv_serialNum+i].toUInt();
        }recv_serialNum = ccd_col+recv_serialNum;
        for (uint8 i = 0 ; i < ccd_col; i ++) {
            ccd2[i] = serialbuff[recv_serialNum+i].toUInt();
        }recv_serialNum = ccd_col+recv_serialNum;
        ///    /////////////////////////////////////////////变量区
        /// ////////////////////////////////////////////////变量区
        if(serialbuff[recv_serialNum++] != 'V' || serialbuff[recv_serialNum++] != 'A' || serialbuff[recv_serialNum++] != 'L' || serialbuff[recv_serialNum++] != 'U' || serialbuff[recv_serialNum++] != 'E')
        {
            image_trans_Flag = false;
            return false;
        }



        serialbuff.clear();
        return true;
    }

}

bool Widget::readNextData()
{
    if(readType_flag == 1)//wifi_Read == 1
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
            if(readType_flag == 1)
            {
                my_delay_times(i);
                standard();
                letGo();
                getlinemap();
                showAllMaps(presspoint);
            }
        }
    }
    else if(readType_flag == 2)//serial_Read == 2
    {

        offset += seriallength;
        if(++dataCnt > dataNum)
        {
            offset = 0;
            dataCnt = 0;
        }
         ui->Pic_verticalSlider->setValue(dataCnt);
        if(readData())
        {
                my_delay_times(i);
                letGo();
                showCCD(presspoint);
        }
    }
}

bool Widget::readPreviousData()
{
    if(readType_flag == 1)//wifi_Read == 1
    {
        offset -= WifiBuffer_AllNum;
        if(--dataCnt < 1)
        {
            offset = 0;
            dataCnt = 0;
        }
        ui->Pic_verticalSlider->setValue(dataCnt);
        if(readData())
        {
            my_delay_times(i);
            standard();
            letGo();
            getlinemap();
                showAllMaps(presspoint);

        }
    }
    else if(readType_flag == 2)//serial_Read == 2
    {

        offset -= seriallength;
        if(--dataCnt < 1)
        {
            offset = 0;
            dataCnt = 0;
        }
        ui->Pic_verticalSlider->setValue(dataCnt);
        if(readData())
        {
            my_delay_times(i);
                letGo();
                showCCD(presspoint);

        }
    }


}

bool Widget::readgotoData()
{
    if(readType_flag == 1)//wifi_Read == 1
    {

        if(data_goto>1&&data_goto<(int)dataNum)
       {
            offset += (data_goto-dataCnt)*WifiBuffer_AllNum;
            dataCnt=data_goto;
        }
        ui->Pic_verticalSlider->setValue(dataCnt);
        if(readData())
        {
            standard();
            letGo();
            getlinemap();
            showAllMaps(presspoint);
        }
    }
    else if(readType_flag == 2)//serial_Read == 2
    {


        if(data_goto>1&&data_goto<(int)dataNum)
       {
            offset += (data_goto-dataCnt)*seriallength;
            dataCnt=data_goto;
        }
        ui->Pic_verticalSlider->setValue(dataCnt);
        if(readData())
        {
            my_delay_times(i);
                letGo();
                showCCD(presspoint);
        }
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
    showImgGray(imgGray,ui->imgGray_lable,presspoint);
    showMap(allmap, ui->allmap_label, point);
    showMap(basemap, ui->basemap_lable,point);
    showMap(linemap, ui->linemap_lable, point);
    showMap(leftmap, ui->leftmap_lable, point);
    showMap(rightmap, ui->rightmap_lable, point);
    showMap(insidemap, ui->insidemap_lable, point);
    showMap(deletemap, ui->deletemap_lable, point);
}


void Widget::showCCD(QPoint point)
{

    memset(ccd1_map,0,sizeof(ccd1_map));
    memset(ccd2_map,0,sizeof(ccd1_map));
    memset(ccd3_map,0,sizeof(ccd1_map));
    memset(ccd4_map,0,sizeof(ccd1_map));
    Mat ccd1mat = ccd2Mat(ccd1_map,presspoint,ccd1);
    Mat ccd2mat = ccd2Mat(ccd2_map,presspoint,ccd2);
    showMat(ccd1mat, ui->ccd1_label);
    showMat(ccd2mat, ui->ccd2_label);

    go();
    Mat transccd1 = ccd2Mat(ccd3_map,presspoint,trans_ccd1);
    Mat transccd2 = ccd2Mat(ccd4_map,presspoint,trans_ccd2);
    showMat(transccd1, ui->transccd1_label);
    showMat(transccd2, ui->transccd2_label);

}


Mat Widget::ccd2Mat(uint8_t map[256][128], QPoint point,uint8_t ccd[128])
{  //MUTIPLE应该是图像放大倍数，CV_8UC3表示图像是3通道的
    for (int y = 0; y < 128; ++y) {
        for (int x = 0; x < 256; ++x) {
            if(x == ccd[y])
            {
                map[255 - x][y] = 255;
            }
    }
    }

    Mat mat(ccd_height , ccd_width * 4, CV_8UC3);
    for (int j = 0; j < mat.rows; ++j)
    {  //MUTIPLE是放大倍数，/MUTIPLE当然是真实的坐标了，不过是CV坐标系的，转成代码的坐标系还需要坐标转换一下
        Vec3b *ptr = mat.ptr<Vec3b>(j);
        for (int i = 0; i < mat.cols; ++i)
        {
            //BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR
            //BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR
            //BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR//BGR
            uint8_t nowPixel = map[(j)][i/4 ];
            if(nowPixel == 0)
            {
                ptr[i][0] = 0;
                ptr[i][1] = 0;
                ptr[i][2] = 0;
            }
            else if(nowPixel == 255)
            {
                ptr[i][0] = 255;
                ptr[i][1] = 255;
                ptr[i][2] = 255;
            }

        }
    }

    if (point.x()  >= 0 && point.y() >= 0)
    {
            int x = point.x()  ;
            int y = point.y() ;
        QPoint highlight;

        for(int i = 0; i < MULTIPLE; ++i)
        {  //图片放大其实就是连续几个像素点取和第一个点一样的值
            for(int j = 0; j < MULTIPLE; ++j)
            {
                highlight.rx() = x * MULTIPLE + i;
                highlight.ry() = y * MULTIPLE + j;
                if(highlight.y() >= 0 && highlight.y() < mat.rows &&
                   highlight.x() >= 0 && highlight.x() < mat.cols)
                {
                    mat.at<Vec3b>(highlight.y(), highlight.x())[0] = 50;
                    mat.at<Vec3b>(highlight.y(), highlight.x())[1] = 205;
                    mat.at<Vec3b>(highlight.y(), highlight.x())[2] = 50;
                }
            }
        }
    }

    return mat;
}



Mat Widget::Map2Mat(uint8_t map[YM][XM], QPoint point)
{  //MUTIPLE应该是图像放大倍数，CV_8UC3表示图像是3通道的
    Mat mat(YM * MULTIPLE, XM * MULTIPLE, CV_8UC3);
    for (int j = 0; j < mat.rows; ++j)
    {  //MUTIPLE是放大倍数，/MUTIPLE当然是真实的坐标了，不过是CV坐标系的，转成代码的坐标系还需要坐标转换一下
        Vec3b *ptr = mat.ptr<Vec3b>(j);
        for (int i = 0; i < mat.cols; ++i)
        {//BGR
            uint8_t nowPixel = map[YY - (j / MULTIPLE)][i / MULTIPLE];
            if (nowPixel == 0)
            {
                ptr[i][0] = 255;
                ptr[i][1] = 255;
                ptr[i][2] = 255;
            }
            else if (nowPixel == 1)
            {
                ptr[i][0] = 0;
                ptr[i][1] = 0;
                ptr[i][2] = 0;
            }
            else if(nowPixel == 2)//red
            {
                ptr[i][0] = 0;
                ptr[i][1] = 0;
                ptr[i][2] = 255;
            }
            else if(nowPixel == 3)//purple
            {
                ptr[i][0] = 255;
                ptr[i][1] = 0;
                ptr[i][2] = 255;
            }
            else if (nowPixel == 4)//blue
            {
                ptr[i][0] = 255;
                ptr[i][1] = 0;
                ptr[i][2] = 0;
            }
            else if (nowPixel == 5)//green
            {
                ptr[i][0] = 0;
                ptr[i][1] = 255;
                ptr[i][2] = 0;
            }
            else if (nowPixel == 6)
            {
                ptr[i][0] = 181;
                ptr[i][1] = 228;
                ptr[i][2] = 255;
            }
            else if (nowPixel == 7)
            {
                ptr[i][0] = 181;
                ptr[i][1] = 228;
                ptr[i][2] = 255;
            }
            else if (nowPixel == 8)
            {
                ptr[i][0] = 181;
                ptr[i][1] = 228;
                ptr[i][2] = 255;
            }
            else if(nowPixel == 250)//yellow
            {
                ptr[i][0] = 0;
                ptr[i][1] = 255;
                ptr[i][2] = 255;
            }
            else if(nowPixel == 251)//green
            {
                ptr[i][0] = 0;
                ptr[i][1] = 255;
                ptr[i][2] = 0;
            }
            else if(nowPixel == 253)
            {
                ptr[i][0] = 0;
                ptr[i][1] = 140;
                ptr[i][2] = 255;
            }
            else if(nowPixel == 254)
            {
                ptr[i][0] = 0;
                ptr[i][1] = 255;
                ptr[i][2] = 127;
            }
        }
    }

    if(map_clicked_mode == true )
    {
        if (hightlight_flag && point.x() /4 >= 0 && point.y() /4 >= 0)
        {
            int x = point.x() /4 ;
            int y = point.y() /4 ;
            QPoint highlight;

            for(int i = 0; i < MULTIPLE; ++i)
            {  //图片放大其实就是连续几个像素点取和第一个点一样的值
                for(int j = 0; j < MULTIPLE; ++j)
                {
                    highlight.rx() = x * MULTIPLE + i;
                    highlight.ry() = y * MULTIPLE + j;
                    if(highlight.y() >= 0 && highlight.y() < mat.rows &&
                       highlight.x() >= 0 && highlight.x() < mat.cols)
                    {
                        mat.at<Vec3b>(highlight.y(), highlight.x())[0] = 50;
                        mat.at<Vec3b>(highlight.y(), highlight.x())[1] = 205;
                        mat.at<Vec3b>(highlight.y(), highlight.x())[2] = 50;
                    }
                }
            }
        }
    }
    else{
        if (hightlight_flag && point.x()  >= 0 && point.y() >= 0)
        {
            int x = point.x() ;
            int y = point.y() ;
            QPoint highlight;

            for(int i = 0; i < MULTIPLE; ++i)
            {  //图片放大其实就是连续几个像素点取和第一个点一样的值
                for(int j = 0; j < MULTIPLE; ++j)
                {
                    highlight.rx() = x * MULTIPLE + i;
                    highlight.ry() = y * MULTIPLE + j;
                    if(highlight.y() >= 0 && highlight.y() < mat.rows &&
                       highlight.x() >= 0 && highlight.x() < mat.cols)
                    {
                        mat.at<Vec3b>(highlight.y(), highlight.x())[0] = 50;
                        mat.at<Vec3b>(highlight.y(), highlight.x())[1] = 205;
                        mat.at<Vec3b>(highlight.y(), highlight.x())[2] = 50;
                    }
                }
            }
        }
    }

    return mat;
}





void Widget::showImgGray(uint8_t map[ROW][COL], QLabel *label, QPoint point)
{
    uint8_t multiple = 2;
    Mat grayMat(ROW, COL, CV_8UC1, map);
    Mat rgbMat;
    cvtColor(grayMat, rgbMat, CV_GRAY2RGB);
    cv::resize(rgbMat, rgbMat, {0, 0}, multiple, multiple);
    int _top_line = YY;
    int _bottom_line = 0;
    for(int i = 0; i < rgbMat.cols; i++)
    {

        for(int j = 0; j < 1 * multiple; j++)
        {
            int y1 = (_top_line + (_ROW - top_line) + 1) * multiple * 1 + j;
            int y2 = (_bottom_line + (_ROW - top_line) + 1) * multiple * 1 + j;
            if(y1 >= 0 && y1 < rgbMat.rows &&
                    y2 >= 0 && y2 < rgbMat.rows)
            {
                rgbMat.at<Vec3b>(y1 , i)[0] = 178;
                rgbMat.at<Vec3b>(y1 , i)[1] = 34;
                rgbMat.at<Vec3b>(y1 , i)[2] = 34;
                rgbMat.at<Vec3b>(y2 , i)[0] = 178;
                rgbMat.at<Vec3b>(y2 , i)[1] = 34;
                rgbMat.at<Vec3b>(y2 , i)[2] = 34;
            }
            //显示一下截图的范围
        }
    }
    if (point.x() >= 0 && point.y() >= 0)
    {
        int x = point.x();
        int y = point.y();
        QPoint highlight;

        for (int i = 0; i < 4 * multiple; ++i)
        {
            for (int j = 0; j < 1 * multiple; ++j) //2
            {
                highlight.rx() = x * multiple + i;
                highlight.ry() = (y) * multiple + j;  //2  坐标变换，凑一凑啦
                if (highlight.y() >= 0 && highlight.y() < rgbMat.rows &&
                    highlight.x() >= 0 && highlight.x() < rgbMat.cols)
                {
                    rgbMat.at<Vec3b>(highlight.y(), highlight.x())[0] = 50;
                    rgbMat.at<Vec3b>(highlight.y(), highlight.x())[1] = 205;
                    rgbMat.at<Vec3b>(highlight.y(), highlight.x())[2] = 50;
                }
            }
        }
    }

    QImage img(static_cast<const uint8_t *>(rgbMat.data), rgbMat.cols, rgbMat.rows,
               rgbMat.cols * rgbMat.channels(), QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(img));
    label->resize(label->pixmap()->size());  //使图片大小自适应窗口
    label->show();
}





void Widget::showMap(uint8_t map[YM][XM], QLabel *label, QPoint point)
{
    Mat mat = Map2Mat(map, point);
    showMat(mat, label);
}





void Widget::showMat(Mat bgrMat, QLabel *label)
{

    Mat rgbMat;
    cvtColor(bgrMat, rgbMat, CV_BGR2RGB);
    QImage img(static_cast<const uint8_t *>(rgbMat.data), rgbMat.cols, rgbMat.rows,
               rgbMat.cols * rgbMat.channels(), QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(img));//MULTIPLE*MY_COL  和   MULTIPLE*MY_ROW 大小的图片，也就是扩大四倍的图片
    label->resize(label->pixmap()->size());
    label->show();

}








void Widget::imgGrayEvent(uint8_t map[ROW][COL], QLabel *label, uint8_t multiple)
{
    QPoint relativePoint =label->mapFromGlobal(QPoint(cursor().pos().x(), cursor().pos().y()));
    int x = relativePoint.x() / multiple;
    int y = relativePoint.y() / multiple;      //x, y 是allmap数组的实际坐标系
    mapPoint.rx() = relativePoint.x() / multiple  ;
    mapPoint.ry() = relativePoint.y() / multiple;   //mapPoint是CV的坐标系
    presspoint = mapPoint;

    map_clicked_mode = true;
    showAllMaps(presspoint);  //每点击一次刷新一下图片显示

    QPoint showPoint = adjustPixelInfo(relativePoint, ROW, COL, multiple);

    if (ui->pixelInfo_lable->isHidden())
    {
        ui->pixelInfo_lable->show();
    }
    ui->pixelInfo_lable->raise();
    ui->pixelInfo_lable->setText(
        QString("[%1, %2] : %3").arg(x).arg(y).arg(map[y][x]));
    ui->pixelInfo_lable->setStyleSheet("background-color:rgb(255,218,185)");
    ui->pixelInfo_lable->move(showPoint.x(), showPoint.y());
    ui->pixelInfo_lable->adjustSize();
}




void Widget::mapEvent(uint8_t map[YM][XM], QLabel *label, uint8_t multiple)
{
    QPoint relativePoint =
        label->mapFromGlobal(QPoint(cursor().pos().x(), cursor().pos().y()));
    int x = relativePoint.x() / multiple;
    int y = relativePoint.y() / multiple;  //x, y 是allmap的坐标系
    mapPoint.rx() = relativePoint.x() / multiple;
    mapPoint.ry() = relativePoint.y() / multiple;  //mapPoint是CV的坐标系
    presspoint = mapPoint;

    map_clicked_mode = false;
    showAllMaps(presspoint);

    QPoint showPoint = adjustPixelInfo(relativePoint, YM, XM, multiple);

    if (ui->pixelInfo_lable->isHidden())
    {
        ui->pixelInfo_lable->show();
    }
    ui->pixelInfo_lable->raise();
    ui->pixelInfo_lable->setText(
        QString("[%1, %2] : %3").arg(x).arg(y).arg(map[y][x]));
    ui->pixelInfo_lable->setStyleSheet("background-color:rgb(255,218,185)");
    ui->pixelInfo_lable->move(showPoint.x(), showPoint.y());
    ui->pixelInfo_lable->adjustSize();
    //qout<<y<<" "<<x;
}





void Widget::map_installEventFilter()
{
    ui->allmap_label->installEventFilter(this);
    ui->basemap_lable->installEventFilter(this);
    ui->deletemap_lable->installEventFilter(this);
    ui->imgGray_lable->installEventFilter(this);
    ui->insidemap_lable->installEventFilter(this);
    ui->leftmap_lable->installEventFilter(this);
    ui->linemap_lable->installEventFilter(this);
    ui->rightmap_lable->installEventFilter(this);
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


void Widget::getlinemap()
{
    memset(linemap, 0, sizeof(linemap));
    for (uint8_t j = 0; j < YM; ++j)
    {
        if (ceil(leftline[j]) >= 0)
        {
            linemap[j][(uint8_t)ceil(leftline[j])] = 6;
        }
        if (floor(rightline[j]) <= XX)
        {
            linemap[j][(uint8_t)floor(rightline[j])] = 6;
        }
        linemap[j][speedlineLeft[j]] = 8;
        linemap[j][speedlineRight[j]] = 8;
    }

    for(uint8_t j = 0; j < YM; ++j)
    {
        for(uint8_t i = 0; i < XM; ++i)
        {
            if((IF.ramp) && j < YM / 4 && i == midline[j] / 2)
            {
                linemap[j][i] = 5;
            }
            if(leftmap[j][i] == 2 || rightmap[j][i] == 2)
            {
                linemap[j][i] = 2;
            }
            else if (leftmap[j][i] == 3 || rightmap[j][i] == 3)
            {
                linemap[j][i] = 3;
            }
            else if (leftmap[j][i] == 4 || rightmap[j][i] == 4)
            {
                linemap[j][i] = 4;
            }
        }
    }
}




void Widget::timerEvent(QTimerEvent *event)
{
    readNextData();
    presspoint = mapPoint;
    if(drawCurve_flag){
        drawLine();
    }
    QApplication::processEvents();
}



void Widget::keyPressEvent(QKeyEvent *event)
{
    if(ui->trans_select_tabWidget->currentIndex() == 3 )//WIFI观察
    {
    if(ui->Hostcomputer_Mode_tabWidget->currentIndex() == 0 )
    {
        if(event->key() == Qt::Key_R)
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
        if(drawCurve_flag)
        {
           drawLine();
        }
    }
    else if(event->key() == Qt::Key_D)
    {

    }
    else if(event->key() == Qt::Key_J)
    {

        readNextData();
        if(drawCurve_flag)
        {
           drawLine();
        }
    }
    else if(event->key() == Qt::Key_K)
    {

        readPreviousData();
        if(drawCurve_flag)
        {
           drawLine();
        }
    }
    }
  }
  else if(ui->trans_select_tabWidget->currentIndex() == 4  )//wifi
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
  else if(ui->trans_select_tabWidget->currentIndex() == 1  )//serial观察
  {
   if(ui->Hostcomputer_Mode_tabWidget->currentIndex() == 0 )
    {
        if(event->key() == Qt::Key_R)
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
            if(drawCurve_flag)
            {
               drawLine();
            }

        }
        else if(event->key() == Qt::Key_D)
        {

        }
        else if(event->key() == Qt::Key_J)
        {

            readNextData();
            if(drawCurve_flag)
                drawLine();
        }
        else if(event->key() == Qt::Key_K)
        {

            readPreviousData();
            if(drawCurve_flag)
                drawLine();
        }
    }
    }
    else if(ui->trans_select_tabWidget->currentIndex() == 2  )//serial观察
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
                imgGrayEvent(imgGray, ui->imgGray_lable, 2);
                return true;
            }
        }
        if (object == ui->allmap_label)
        {
            if(event->type() == QEvent::MouseButtonPress)
            {
                mapEvent(allmap, ui->allmap_label, MULTIPLE);
                return true;
            }
        }
        else if (object == ui->basemap_lable)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                mapEvent(basemap, ui->basemap_lable, MULTIPLE);
                return true;
            }
        }
        else if (object == ui->linemap_lable)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                mapEvent(linemap, ui->linemap_lable, MULTIPLE);
                return true;
            }
        }
        else if (object == ui->leftmap_lable)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                mapEvent(leftmap, ui->leftmap_lable, MULTIPLE);
                return true;
            }
        }
        else if (object == ui->rightmap_lable)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                mapEvent(rightmap, ui->rightmap_lable, MULTIPLE);
                return true;
            }
        }
        else if (object == ui->deletemap_lable)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                mapEvent(deletemap, ui->deletemap_lable, MULTIPLE);
                return true;
            }
        }
        else if (object == ui->insidemap_lable)
        {
            if (event->type() == QEvent::MouseButtonPress)
            {
                mapEvent(insidemap, ui->insidemap_lable, MULTIPLE);
                return true;
            }
        }
    }
    return false;
}



void Widget::initChart()
{
    //初始化QChart的实例
    chart = new QChart();

    //初始化两个QSplineSeries的实例
    series1 = new QSplineSeries();
    series2 = new QSplineSeries();

    //设置两条曲线的名称
    switch(drawLine_flag)
    {
    case 0:  //转向内环
        series1->setName("TarYawRate");
        series2->setName("TrueYaweRate");
        drawLine_var1 = &tarYawRate;
        drawLine_var2 = &YawRate;
        break;
    case 1:  //速度环
        series1->setName("TarSpeed");
        series2->setName("TrueSpeed");
        drawLine_var1 = &SI.aimSpeed;
        drawLine_var2 = &speed_TrueM;
        break;
    case 2:  //角度环
        series1->setName("TarPitch");
        series2->setName("TruePitch");
        drawLine_var1 = &tarPitch;
        drawLine_var2 = &Pitch;
        break;
    case 3:
        series1->setName("TarPitchRate");
        series2->setName("TruePitchRate");
        drawLine_var1 = &tarPitchRate;
        drawLine_var2 = &pitchRate;
        break;
    }

    QPen pen1(Qt::green, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen2(Qt::red, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    series1->setPen(pen1);
    series2->setPen(pen2);
    //把曲线添加到QChart的实例chart中
    chart->addSeries(series1);
    chart->addSeries(series2);

    //声明并初始化X轴、两个Y轴
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    //设置坐标轴显示的范围
    axisX->setMin(0);
    axisX->setMax(100);
    if(drawLine_flag != 1 && drawLine_flag != 3)
    {
        if(drawLine_flag == 2)
        {
            axisY->setMin(-2500);
            axisY->setMax(500);
        }
        else
        {
        axisY->setMin(-5000);
        axisY->setMax(5000);
        }
    }
    else if(drawLine_flag == 1)
    {
        axisY->setMin(0);
        axisY->setMax(350);
    }
    else if(drawLine_flag == 3)
    {
        axisY->setMin(-1500);
        axisY->setMax(1500);
    }
    else
    {
        axisY->setMin(-2000);
        axisY->setMax(2000);
    }
    // 设置坐标轴上的格点
    axisX->setTickCount(10);
    axisY->setTickCount(10);
    // 设置坐标轴显示的名称
    axisX->setTitleText("X");
    axisY->setTitleText("Y");
    // 设置坐标轴的颜色、粗细和网格可见
    axisY->setLinePenColor(QColor(Qt::darkBlue));
    axisY->setGridLineColor(QColor(Qt::darkBlue));
    axisY->setGridLineVisible(true);
    QPen penY1(Qt::darkBlue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    axisY->setLinePen(penY1);
    // 添加网格和刻度点的显示
    axisX->setGridLineVisible(true);
    axisY->setGridLineVisible(true);
    axisX->setLabelsVisible(true);
    axisY->setLabelsVisible(true);

    //把坐标轴添加到chart中，
    //addAxis函数的第二个参数是设置坐标轴的位置，
    //只有四个选项，下方：Qt::AlignBottom，左边：Qt::AlignLeft，右边：Qt::AlignRight，上方：Qt::AlignTop
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    //把曲线关联到坐标轴
    series1->attachAxis(axisX);
    series1->attachAxis(axisY);
    series2->attachAxis(axisX);
    series2->attachAxis(axisY);

    //把chart显示到窗口上
    ui->graphicsView->setChart(chart);
    x1 = 0;
    x2 = 0;
}


void Widget::drawLine()
{
    chart->axisX()->setMin(0);
    chart->axisX()->setMax(100);
    tarPitch = Target_Attitude.Pitch;
    Pitch = SystemAttitude.Pitch;
    tarPitchRate = Tar_Ang_Vel.Pitch;
    pitchRate = SystemAttitudeRate.Pitch;
    if(series1->count() > 99)
    {
        x1 = 0;
        series1->clear();
    }
    if(series2->count() > 99)
    {
        x2 = 0;
        series2->clear();
    }
    //这里第二个参数后面改成自己想要显示的
    series1->append(x1++, *drawLine_var1);
    series2->append(x2++, *drawLine_var2);
}


void Widget::DeleteLine()
{
    //按下k键倒退一个单位
    if(x1 > 0)
        series1->removePoints(--x1, 1);
    if(x2 > 0)
        series2->removePoints(--x2, 1);
}

void Widget::timerDeal()
{
    //定时画曲线
    drawLine();
}

void Widget::on_Curve_pushButton_clicked()
{
    //初始化QChart
    initChart();
    if(ui->graphicsView->isHidden())
    {
        ui->graphicsView->show();
        drawCurve_flag = 1;
    }
    else
    {
        ui->graphicsView->hide();
        drawCurve_flag = 0;
    }
}


void Widget::on_Speed_pushButton_clicked()
{
    drawLine_flag = 1;
    initChart();
    if(ui->graphicsView->isHidden())
    {
        ui->graphicsView->show();
    }
}

void Widget::on_Pitch_pushButton_clicked()
{
    drawLine_flag = 2;
    initChart();
    if(ui->graphicsView->isHidden())
    {
        ui->graphicsView->show();
    }
}

void Widget::on_PitchRate_pushButton_clicked()
{
   drawLine_flag = 3;
   initChart();
   if(ui->graphicsView->isHidden())
   {
       ui->graphicsView->show();
   }
}

void Widget::on_yawRate_pushButton_clicked()
{
    drawLine_flag = 0;
    initChart();
    if(ui->graphicsView->isHidden())
    {
        ui->graphicsView->show();
    }
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
    QString dataName = QFileDialog::getOpenFileName(this, "读取数据", DefaultFile); //获取选中文件的名字
    if(dataName.contains("wifi_picture"))
    {
        if(ui->trans_select_tabWidget->tabText(3) == "" || ui->trans_select_tabWidget->tabText(4) == "" )
        {
            ui->Wait_connect_label->setText("请开启wifi模式");
            ui->Wait_connect_label->setStyleSheet(
                        "background-color:rgb(255, 100, 100);font-family: Microsoft YaHei;font-size:35px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Wait_connect_label->adjustSize();
        }
        else
        {

            readType_flag = 1;
            ui->trans_select_tabWidget->setCurrentIndex(3);
        }
    }
    else if(dataName.contains("serial_picture"))
    {
        if(ui->trans_select_tabWidget->tabText(1) == "" || ui->trans_select_tabWidget->tabText(2) == "" )
        {
            ui->Wait_connect_label->setText("请开启Serial模式");
            ui->Wait_connect_label->setStyleSheet(
                        "background-color:rgb(255, 100, 100);font-family: Microsoft YaHei;font-size:35px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Wait_connect_label->adjustSize();
        }
        else
        {

            readType_flag = 2;
            ui->trans_select_tabWidget->setCurrentIndex(1);
        }
    }
    if(dataName != nullptr)
    {
        data.clear();  //String data
        dataNum = 0;   // 重置数据计数器
        dataNum_tips = 0;
        offset = 0;    //uint32
        QFile myFile(dataName);  //创建对应的文件对象
        if(!myFile.open(QIODevice::ReadWrite))  //文件以读写的形式打开
        {
            return;
        }
        else
        {
            FKTMDfileData = myFile.readAll(); // 读取文件中的全部数据
            if(readType_flag == 1)//wifi_Read == 1
            {

                int32 dataSize = FKTMDfileData.size();
                int32 numSegments = (dataSize + WifiBuffer_AllNum - 1) / WifiBuffer_AllNum;  // 计算数据段的数量
                for (int32 i = 0; i < numSegments; i++)
                {
                    int32 startPos = i * WifiBuffer_AllNum;  // 数据段的起始位置
                    int32 segmentSize = qMin(WifiBuffer_AllNum, dataSize - startPos);  // 数据段的实际长度
                    QByteArray segment = FKTMDfileData.mid(startPos, segmentSize);  // 创建数据段
                    data.append(segment);  // 将数据段拼接到 QByteArray 中
                    dataNum_tips += 1;
                }
                dataNum = numSegments - 1;
            }
            else if(readType_flag == 2)//serial_Read == 2
            {
                tempserial = QString(FKTMDfileData).split(","); // 将字符串array按逗号分割成QStringLis

                int32 startIndex = tempserial.indexOf("C");
                int32 endIndex = tempserial.indexOf("V") + QString("VALUE").length(); // 'VALUE'的结束索引
                seriallength = endIndex -  startIndex;
                int32 dataSize = tempserial.size();
                int32 numSegments = (dataSize-1) / seriallength;
                for(int32 i = 0 ; i < numSegments ; i++)
                {
                    data[seriallength * i  ] ='C';
                    data[seriallength * i+1] ='Z';
                    data[seriallength * i+2] ='Y';
                    data[seriallength * i+3] ='N';
                    data[seriallength * i+4] ='B';
                }
                qout<<1;
                dataNum = numSegments - 1;
            }

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
        memset(sdBuff,0,sizeof(sdBuff));
        offset = data.indexOf("CZYNB", offset);
        if(readData())  //读取文件数据
        {
            readFlag = true;
            dataCnt = 1;
            if(readType_flag == 1)
            {
                standard();
                letGo();
                getlinemap();
                showAllMaps(presspoint);
            }
            else if(readType_flag == 2)
            {
                letGo();
                showCCD(presspoint);
            }
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
    fileName += dateTime.toString("yyyy-MM-dd_HH.mm");
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
//                    "background-color: rgb(255, 105, 180);font-size:15px;color:blue");
//                ui->SDInfo_label->adjustSize();
            }
            else
            {
//                ui->SDInfo_label->setText(QString("don't have permission!"));
//                ui->SDInfo_label->setStyleSheet(
//                    "background-color: rgb(255, 105, 180);font-size:15px;color:blue");
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
            timerId = this->startTimer(picture_speed); //这里修改图片播放速度 //100
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
    if(readType_flag == 1)
    {
        standard();
        letGo();
        getlinemap();
        showAllMaps(presspoint);
    }
    else if(readType_flag == 2)
    {
        letGo();
        showCCD(presspoint);
    }
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

        size_t bytesToCopy = COL * ROW * sizeof(uint8);
        memcpy(&imgGray[0], &wifiBuff[recv_wifiNum], bytesToCopy);

        recv_wifiNum = recv_wifiNum +  COL * ROW;

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
        ac_pix_index =(wifiBuff[recv_wifiNum++] << 8) | ((uint8_t)wifiBuff[recv_wifiNum++]);
        pix_num = ac_pix_index;
        if(ac_pix_index - pix_index == 1 )
        {
            pix_index = ac_pix_index;
        }
        else
        {
            if(pix_index != 0)
            {
                //ui->pix_index_comboBox->addItem(QString("掉帧在%1到%2之间，总共丢了%3帧").arg(pix_index).arg(ac_pix_index).arg(pix_index - ac_pix_index));
                pix_index = ac_pix_index;
            }
        }

        //图像处理时间
//        dealImgtime=(wifiBuff[recv_wifiNum++] << 8) | ((uint8_t)wifiBuff[recv_wifiNum++]);
//        ui->dealImgTime_label->setText(QString("dealImgTime:%1").arg(dealImgtime));
//        ui->dealImgTime_label->setStyleSheet(
//                    "background-color: rgb(129, 255, 233);font-size:16px;color:black");  //255 105 180
//        ui->dealImgTime_label->adjustSize();
        wifiBuff[recv_wifiNum++] = fps;//不要改变//误删//误删这是上位机FPS显示的暂存值
        ////////////////////////





        temp_WifiData+=GetValueNum;
        frameCount++;
        image_trans_Flag = true;
        writeDisk();
        if(ui->display_pushButton->text() == "停止")
        {
            showImgGray(imgGray,ui->imgGray_lable,presspoint);
        }
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


cv::Mat Widget::Wifi_Map2Mat_imagGray(uint8_t map[ROW][COL],uint8 beishu)//opencv库里面的矩阵数据类型，一般用来储存矩阵的信息（和二维数组有异曲同工）
{  //beishu应该是图像放大倍数，CV_8UC3表示图像是3通道的
    cv::Mat mat(ROW*beishu , COL*beishu , CV_8UC3);
    for (int j = 0; j < mat.rows; ++j)
    {  //MUTIPLE是放大倍数，/MUTIPLE当然是真实的坐标了，不过是CV坐标系的，转成代码的坐标系还需要坐标转换一下
        cv::Vec3b *ptr = mat.ptr<cv::Vec3b>(j);
        for (int i = 0; i < mat.cols; ++i)
        {//BGR
            uint8_t nowPixel = map[(j / beishu)][i / beishu];
            ptr[i][0] = nowPixel;
            ptr[i][1] = nowPixel;
            ptr[i][2] = nowPixel;
            if(hightlight_flag &&  j / beishu  == presspoint.y() && i / beishu == presspoint.x())
            //鼠标点击的高亮绿点，查看细致坐标
            {
                ptr[i][0] = 50;
                ptr[i][1] = 205;
                ptr[i][2] = 50;
            }

        }
    }

    return mat;

}




void Widget::imageInit(uchar *Pic)
{
//    // 计算需要拷贝的字节数
//    size_t bytesToCopy = ROW * COL * sizeof(uint8);
//    // 使用memcpy进行内存拷贝
//    memcpy(&Image_Used[0][0], &Pic[0], bytesToCopy);

}


void send_udp(QByteArray data,QHostAddress ip,quint16 port)
{

}

void Widget::_SendMessage()
{
    QHostAddress ip(ui->ip_lineEdit->text());
    quint16 port = ui->port_lineEdit->text().toUShort();


    ////////////////////
    QString Send_text = ui->SendMess_textEdit->toPlainText();
    QByteArray datastream = ui->SendMess_textEdit->toPlainText().toStdString().c_str();
    emit send_udp(datastream,ip,port);
    ////////////////////

    datastream.clear();

    QDateTime currentTime = QDateTime::currentDateTime(); // 获取当前时间
    QString messageWithTime = currentTime.toString("(yyyy-MM-dd hh:mm:ss)") + "me:      " + Send_text; // 将时间和文本组合
    ui->Reciver_textEdit->append(messageWithTime);
}



void Widget::writeDisk()
{
    if(readType_flag == 1)
    {
        if(WriteVideo_flag == true)
        {

            picture_count++;
            dataByteArray.append(wifiBuff, sizeof(wifiBuff));// 将数据追加到 QByteArray 中
        }
        else if(WriteVideo_flag == false && WriteVideo_done_flag == true  && !dataByteArray.isEmpty())
        {
            QDateTime dateTime = QDateTime::currentDateTime();
            QString fileName = DefaultFile + "wifi_picture/p";
            fileName += dateTime.toString("yyyy-MM-dd_HH.mm.ss");
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
    else if(readType_flag == 2)
    {
        if(WriteVideo_flag == true)
        {
            picture_count++;
            dataByteArray.append(serialarray);
        }
        else if(WriteVideo_flag == false && WriteVideo_done_flag == true  && !dataByteArray.isEmpty())
        {
            QDateTime dateTime = QDateTime::currentDateTime();
            QString fileName = DefaultFile + "serial_picture/p";
            fileName += dateTime.toString("yyyy-MM-dd_HH.mm.ss");
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
    else
    {
        1;
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
                        "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Frame_rate_label->adjustSize();


            WifiData_all += temp_WifiData;
            WifiData_rate =  temp_WifiData /1024;
            ui->WifiData_send_label->setText(QString("传输速度: %1 KB/s,数据已经累积了 %2 Mbs").arg(int(WifiData_rate)).arg(int(WifiData_all/1024/1024)));
            ui->WifiData_send_label->setStyleSheet(
            "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->WifiData_send_label->adjustSize();

            temp_WifiData = 0;
            frameCount = 0;
        }
        else{
            ui->Frame_rate_label->setText(QString("卡了"));
            ui->Frame_rate_label->setStyleSheet(
                        "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Frame_rate_label->adjustSize();
        }
        // 重置帧计数器
        picture = picture_count;
        picture_count = 0;
    }
    else if(ui->openBt->text() == "关闭串口")
    {
        SerialData_all += temp_SerialData;
        SerialData_rate =  temp_SerialData /1024;
        ui->SerialData_send_label->setText(QString("传输速度: %1 KB/s,数据已经累积了 %2 Mbs").arg(int(SerialData_rate)).arg(int(SerialData_all/1024/1024)));
        ui->SerialData_send_label->setStyleSheet(
        "background-color:rgb(255, 200, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
        ui->SerialData_send_label->adjustSize();
        temp_SerialData = 0;

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
            ui->udp_connect_pushButton->setStyleSheet("color:red");
            ui->Wait_connect_label->setText("连接到Udp客户端");
            ui->Wait_connect_label->setStyleSheet(
                        "background-color:rgb(100, 255, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Wait_connect_label->adjustSize();
        }
        else
        {
            ui->Wait_connect_label->setText("无客户端连接！");
            ui->Wait_connect_label->setStyleSheet(
                        "background-color:rgb(255, 100, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Wait_connect_label->adjustSize();
        }
    }
    else if(ui->udp_connect_pushButton->text() == "中断连接")
    {

        if(ui->WriteVideo_pushButton->text() == "停止录制")
        {
            ui->Wait_connect_label->setText("正在录制！");
            ui->Wait_connect_label->setStyleSheet(
                        "background-color:rgb(100, 255, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Wait_connect_label->adjustSize();
        }
        else
        {
            ui->Wait_connect_label->setText("未连接到Udp客户端");
            ui->Wait_connect_label->setStyleSheet(
                        "background-color:rgb(255, 100, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Wait_connect_label->adjustSize();
            if(ui->display_pushButton->text() == "停止")
            {
                udp_recv->stop();
                ui->display_pushButton->setText("播放");
                ui->display_pushButton->setStyleSheet("color:green");
            }
            ui->udp_connect_pushButton->setText("连接");
            ui->udp_connect_pushButton->setStyleSheet("color:green");
            udp_recv->allow_recv_flag = 0;
        }
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
                ui->display_pushButton->setStyleSheet("color:red");

            }
            else if(ui->display_pushButton->text() == "停止")
            {
                udp_recv->allow_recv_flag = false;
                udp_recv->stop(); //继续
                ui->display_pushButton->setText("播放");
                ui->display_pushButton->setStyleSheet("color:green");
            }
        }
        else
        {
            udp_recv->allow_recv_flag = 0;
            udp_recv->stop(); //继续
            ui->Wait_connect_label->setText("未连接到Udp客户端，请重试");
            ui->Wait_connect_label->setStyleSheet(
                        "background-color:rgb(255, 100, 100);font-family: Microsoft YaHei;font-size:15px;font-family: Microsoft YaHei;color:black");  //255 105 180
            ui->Wait_connect_label->adjustSize();
        }
    }
    else
    {
        //QMessageBox::information(this,"警告","未连接Udp客户端！");
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

    if(ui->Hostcomputer_Mode_tabWidget->currentIndex() == 1 )
    {
        if(ui->display_pushButton->text() == "停止")
        {
            if(ui->WriteVideo_pushButton->text() == "录制")
            {
                WriteVideo_flag = true;
                WriteVideo_done_flag =false;
                ui->WriteVideo_pushButton->setText("停止录制") ;
                ui->WriteVideo_pushButton->setStyleSheet("color: red;");

            }
            else if(ui->WriteVideo_pushButton->text() == "停止录制")
            {
                WriteVideo_flag = false;
                WriteVideo_done_flag = true;
                ui->WriteVideo_pushButton->setText("录制") ;
                ui->WriteVideo_pushButton->setStyleSheet("color: green;");
            }
        }
        else
        {
            ui->Wait_connect_label->setText("未连接到任何网络！");
            ui->Wait_connect_label->setStyleSheet(
            "background-color:rgb(255, 100, 100);font-family: Microsoft YaHei;font-size:30px;font-family: Microsoft YaHei;color:black");  //255 105 180
        }
    }
    else if(ui->Hostcomputer_Mode_tabWidget->currentIndex() == 2 )
    {
        if(serial_open_Flag == true)
        {
            if(ui->WriteVideo_pushButton->text() == "录制")
            {
                WriteVideo_flag = true;
                WriteVideo_done_flag =false;
                ui->WriteVideo_pushButton->setText("停止录制") ;
                ui->WriteVideo_pushButton->setStyleSheet("color: red;");

            }
            else if(ui->WriteVideo_pushButton->text() == "停止录制")
            {
                WriteVideo_flag = false;
                WriteVideo_done_flag = true;
                ui->WriteVideo_pushButton->setText("录制") ;
                ui->WriteVideo_pushButton->setStyleSheet("color: green;");
            }
        }
        else
        {
            ui->Wait_connect_label->setText("未连接到任何串口！");
            ui->Wait_connect_label->setStyleSheet(
            "background-color:rgb(255, 100, 100);font-family: Microsoft YaHei;font-size:30px;font-family: Microsoft YaHei;color:black");  //255 105 180
        }
    }
    else if(ui->Hostcomputer_Mode_tabWidget->currentIndex() == 0 )
    {
        ui->Wait_connect_label->setText("请连接某客户端！");
        ui->Wait_connect_label->setStyleSheet(
        "background-color:rgb(255, 100, 100);font-family: Microsoft YaHei;font-size:30px;font-family: Microsoft YaHei;color:black");  //255 105 180
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
        else if(ui->serail_condtion_label->text() == "连接成功")
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(2);
        }

        if(ui->trans_select_tabWidget->currentIndex() == 1)
        {
            Watch_the_silder(1);
        }
        else if(ui->trans_select_tabWidget->currentIndex() == 3)
        {
            Watch_the_silder(1);
        }
        else
        {
            Watch_the_silder(0);
        }

    }
    else if(index == 1)//Udp
    {
        readType_flag = 1;
        if(runFlag == true)
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(0);
        }
        else if(ui->serail_condtion_label->text() == "连接成功")
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(2);
        }
        else
        {

            hightlight_flag = false;
            showImgGray(imgGray,ui->imgGray_lable,presspoint);
            Watch_the_silder(0);
        }
    }
    else if(index == 2)//serial
    {
        readType_flag = 2;
        if(runFlag == true)
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(0);
        }
        else if(ui->udp_connect_pushButton->text() ==  "中断连接")
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(1);
        }
    }
}



void Widget::get_the_workshop(uint8 i)
{
if(i == 0)
{
    DefaultFile = QDir::currentPath();  // 获取当前工作目录
    DefaultFile.replace("build-", "");
    DefaultFile.replace("-Desktop_Qt_5_9_9_MinGW_32bit-Debug", "");
    DefaultFile.replace("-Desktop_Qt_5_9_9_MinGW_32bit-Profile", "");
    DefaultFile.replace("-Desktop_Qt_5_9_9_MinGW_32bit-Release", "");
    DefaultFile.append("/");
}
else if(i == 1)
{
    DefaultFile = QDir::currentPath();  // 获取当前工作目录
    DefaultFile.append("/");
}

}



//还有opencv支持库的盘符记得在XXXXX.pro文件中修改库的文件目录，////////////////////////////////记得添加对应的环境变量地址
void Widget::on_wallpaper_pushbutton_clicked()
{

    if(Wallpaper_Choice)
    {
        Wallpaper_Choice = 0;
        ui->label_9->setStyleSheet("QLabel#label_9 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_10->setStyleSheet("QLabel#label_10 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_11->setStyleSheet("QLabel#label_11 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_12->setStyleSheet("QLabel#label_12 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_13->setStyleSheet("QLabel#label_13 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_14->setStyleSheet("QLabel#label_14 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_16->setStyleSheet("QLabel#label_16 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_38->setStyleSheet("QLabel#label_38 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_18->setStyleSheet("QLabel#label_18 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->ac_pix_index_label->setStyleSheet("QLabel#label_18 { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->SDDriveLetter_label->setStyleSheet("QLabel#SDDriveLetter_label { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Frame_rate_label->setStyleSheet("QLabel#Frame_rate_label { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->UTF8_WifiShow_checkBox->setStyleSheet("QCheckBox#UTF8_WifiShow_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Hex_WifiShow_checkBox->setStyleSheet("QCheckBox#Hex_WifiShow_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Hex_SerialShow_checkBox->setStyleSheet("QCheckBox#Hex_SerialShow_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->UTF8_SerialShow_checkBox->setStyleSheet("QCheckBox#UTF8_SerialShow_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Serial_ShowTime_checkBox->setStyleSheet("QCheckBox#Serial_ShowTime_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Wifi_ShowTime_checkBox->setStyleSheet("QCheckBox#Wifi_ShowTime_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Open_Wifi_checkBox->setStyleSheet("QCheckBox#Open_Wifi_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Open_Serial_checkBox->setStyleSheet("QCheckBox#Open_Serial_checkBox { color: rgb(255, 200, 100); font-family: Microsoft YaHei; font-size: 15px; }");

        setWallpaper();
    }
    else
    {
        Wallpaper_Choice = 1;
        ui->label_9->setStyleSheet("QLabel#label_9 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_10->setStyleSheet("QLabel#label_10 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_11->setStyleSheet("QLabel#label_11 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_12->setStyleSheet("QLabel#label_12 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_13->setStyleSheet("QLabel#label_13 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_14->setStyleSheet("QLabel#label_14 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_16->setStyleSheet("QLabel#label_16 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_38->setStyleSheet("QLabel#label_38 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->label_18->setStyleSheet("QLabel#label_18 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->ac_pix_index_label->setStyleSheet("QLabel#label_18 { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->SDDriveLetter_label->setStyleSheet("QLabel#SDDriveLetter_label { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Frame_rate_label->setStyleSheet("QLabel#Frame_rate_label { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->UTF8_WifiShow_checkBox->setStyleSheet("QCheckBox#UTF8_WifiShow_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Hex_WifiShow_checkBox->setStyleSheet("QCheckBox#Hex_WifiShow_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Hex_SerialShow_checkBox->setStyleSheet("QCheckBox#Hex_SerialShow_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->UTF8_SerialShow_checkBox->setStyleSheet("QCheckBox#UTF8_SerialShow_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Serial_ShowTime_checkBox->setStyleSheet("QCheckBox#Serial_ShowTime_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Wifi_ShowTime_checkBox->setStyleSheet("QCheckBox#Wifi_ShowTime_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Open_Wifi_checkBox->setStyleSheet("QCheckBox#Open_Wifi_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        ui->Open_Serial_checkBox->setStyleSheet("QCheckBox#Open_Serial_checkBox { color: rgb(0, 0, 0); font-family: Microsoft YaHei; font-size: 15px; }");
        setWallpaper();
    }
}



void Widget::auto_night_mode()
{
//    QIcon icon(DefaultFile + "sun.png") ;
//    ui->wallpaper_pushbutton->setIcon(icon);
//    QSize iconSize(45, 36);
//    ui->wallpaper_pushbutton->setIconSize(iconSize);
//    QTime currentTime = QTime::currentTime();
//    QTime eveningTime(19, 0);//晚上
//    QTime morningTime(6, 0);//白天
//    bool isEvening = (currentTime >= eveningTime || currentTime < morningTime);
//    int Flag = (isEvening) ? 0 : 1;
//    if(!Flag)
//    {
//        on_wallpaper_pushbutton_clicked();
//    }
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
    emit send_udp(datastream,ip,port);
}

void Widget::on_control_down_pushbutton_clicked()
{

    QHostAddress ip(ui->ip_lineEdit->text());
    quint16 port = ui->port_lineEdit->text().toUShort();
    QByteArray datastream = "downdowndown";
    emit send_udp(datastream,ip,port);
}


void Widget::on_control_left_pushbutton_clicked()
{

    QHostAddress ip(ui->ip_lineEdit->text());
    quint16 port = ui->port_lineEdit->text().toUShort();
    QByteArray datastream = "leftleftleft";
    emit send_udp(datastream,ip,port);
}



void Widget::on_control_right_pushbutton_clicked()
{

    QHostAddress ip(ui->ip_lineEdit->text());
    quint16 port = ui->port_lineEdit->text().toUShort();
    QByteArray datastream = "rightrightright";
    emit send_udp(datastream,ip,port);
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



void Widget::display_serial_data()
{
    QByteArray buf;
    buf=QByteArray(serialPort->readAll());
    QString sshowdata;
    QByteArray showdata;
    QString currentDateTimeString = "";
    if(ui->trans_select_tabWidget->currentIndex() == 2)
    {

        buf = buf.replace("\r\n", "");
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
    else if(ui->trans_select_tabWidget->currentIndex() == 1)
    {
        buf = buf.replace("\r\n", ",");
        SerialData.append(buf);
        QList<QByteArray> dataArray; // 用于存储中间数据的数组
        int czynbIndex = SerialData.indexOf("C,Z,Y,N,B,");
        int valueIndex = SerialData.indexOf("V,A,L,U,E,");
        if (czynbIndex != -1 && valueIndex != -1)
        {
            int startIndex = czynbIndex; // 'CZYNB'的起始索引
            int endIndex = valueIndex + QString("V,A,L,U,E,").length(); // 'VALUE'的结束索引
            if (startIndex <= endIndex)
            {
                serialarray = SerialData.mid(startIndex, endIndex - startIndex );
                SerialData.clear();

                serialList = QString(serialarray).split(","); // 将字符串array按逗号分割成QStringLis


                uint32 GetValueNum = serialList.size();
                uint16 serialinex = 0;
                QByteArray tempbuf = buf.replace("\r\n", ",");
                int recv_serialNum = 5;
                if(serialList[0] != 'C' || serialList[1] != 'Z' || serialList[2] != 'Y' || serialList[3] != 'N' || serialList[4] != 'B')
                {
                    image_trans_Flag = false;
                    return;
                }
                for (uint8 i = 0 ; i < ccd_col; i ++) {
                    ccd1[i] = serialList[recv_serialNum+i].toUInt();
                }recv_serialNum = ccd_col+recv_serialNum;

                for (uint8 i = 0 ; i < ccd_col; i ++) {
                    ccd2[i] = serialList[recv_serialNum+i].toUInt();
                }recv_serialNum = ccd_col+recv_serialNum;
                ///    /////////////////////////////////////////////变量区
                /// ////////////////////////////////////////////////变量区
                if(serialList[recv_serialNum++] != 'V' || serialList[recv_serialNum++] != 'A' || serialList[recv_serialNum++] != 'L' || serialList[recv_serialNum++] != 'U' || serialList[recv_serialNum++] != 'E')
                {
                    image_trans_Flag = false;
                    return;
                }

                image_trans_Flag = true;

                temp_SerialData+=GetValueNum;
                showCCD(presspoint);
                writeDisk();


            }
            else
            {
                    SerialData = SerialData.right(valueIndex + 5);
            }
        }


    }
    sshowdata.clear();
    showdata.clear();
    buf.clear();
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
    else if(ui->baundrateCb->currentText()=="230400")
        baudRate=QSerialPort::Baud230400;
    else if(ui->baundrateCb->currentText()=="460800")
        baudRate=QSerialPort::Baud460800;


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

    connect_serialPort = ui->serailCb->currentText();
    uint8 temp_index = connect_serialPort.indexOf("(");
    connect_serialPort = connect_serialPort.left(temp_index-1);
    qout<<connect_serialPort;
    serialPort->setPortName(connect_serialPort);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(checkBits);


    if(ui->openBt->text() == "打开串口")
    {

        if(serialPort->open(QIODevice::ReadWrite)==true)
        {
                //QMessageBox::information(this,"提示","成功");
                serial_open_Flag = true;
                ui->serail_condtion_label->setText(QString("连接成功"));
                ui->serail_condtion_label->setStyleSheet(
                "background-color:rgb(144,238,144);font-size:30px;color:black");  //255 105 180
                ui->openBt->setText("关闭串口");
                ui->openBt->setStyleSheet("color:red");

        }
        else
        {
            serial_open_Flag = false;
            ui->serail_condtion_label->setText(QString("连接失败..."));
            ui->serail_condtion_label->setStyleSheet(
            "background-color:rgb(255,114,114);font-size:30px;color:black");
        }

    }
    else if(ui->openBt->text() == "关闭串口")
    {
        ui->openBt->setText("打开串口");
        ui->openBt->setStyleSheet("color:green");
        close_serail();

    }

}


void Widget::close_serail()
{
    serial_open_Flag = false;
    ui->serail_condtion_label->setText(QString("未连接..."));
    ui->serail_condtion_label->setStyleSheet(
    "background-color:rgb(255,114,114);font-size:30px;color:black");
    serialPort->close();
}




void Widget::on_sendBt_clicked()
{
    if(!serial_open_Flag)
    {
        ui->serail_condtion_label->setText(QString("未连接到串口"));
        ui->serail_condtion_label->setStyleSheet(
        "background-color:rgb(255,114,114);font-size:30px;color:black");
    }
    else{

        ////////////////////
        QString Send_text = ui->sendEdit->toPlainText();
        QByteArray datastream = ui->sendEdit->toPlainText().toStdString().c_str();
        serialPort->write(datastream);
        ////////////////////

        datastream.clear();

        QDateTime currentTime = QDateTime::currentDateTime(); // 获取当前时间
        QString messageWithTime = currentTime.toString("(yyyy-MM-dd hh:mm:ss)") + "me:      " + Send_text; // 将时间和文本组合
        ui->recvEdit->append(messageWithTime);
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
    if(index != 1 && index != 2)
    {
        if(serial_open_Flag == true)
        {
            ui->trans_select_tabWidget->setCurrentIndex(1);
            ui->Wait_connect_label->setText("请先断开串口连接");
            Watch_the_silder(1);
        }
    }
    if(index != 3 && index != 4)
    {
        if(ui->udp_connect_pushButton->text() == "中断连接")
        {
            ui->trans_select_tabWidget->setCurrentIndex(3);
            Watch_the_silder(1);
            ui->Wait_connect_label->setText("请先断开Wifi连接");
        }
    }




    if(index ==3 )
    {
        Watch_the_silder(1);
    }
    else if(index == 1 )
    {
        Watch_the_silder(1);
    }
    else
    {
        Watch_the_silder(0);
    }

}




void Widget::serial_writeDisk()
{
    if(WriteVideo_flag == true)
    {

        picture_count++;
        dataByteArray.append(serialBuff, sizeof(serialBuff));// 将数据追加到 QByteArray 中
    }
    else if(WriteVideo_flag == false && WriteVideo_done_flag == true  && !dataByteArray.isEmpty())
    {
        QDateTime dateTime = QDateTime::currentDateTime();
        QString fileName = DefaultFile +  "serial_picture/p";
        fileName += dateTime.toString("yyyy-MM-dd_HH.mm.ss");
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


void Widget::on_Open_Wifi_checkBox_clicked(bool checked)
{
    if(checked == false)
    {
        uint8 currentindex = ui->Hostcomputer_Mode_tabWidget->currentIndex();
        tabName1 = ui->trans_select_tabWidget->tabText(3);
        tabName2 = ui->trans_select_tabWidget->tabText(4);
        tabName3 = ui->Hostcomputer_Mode_tabWidget->tabText(1);
        ui->trans_select_tabWidget->setTabText(3, "");
        ui->trans_select_tabWidget->setTabText(4, "");
        ui->Hostcomputer_Mode_tabWidget->setTabText(1, "");


        ui->trans_select_tabWidget->setTabEnabled(3, false);
        ui->trans_select_tabWidget->setTabEnabled(4, false);
        ui->Hostcomputer_Mode_tabWidget->setTabEnabled(1, false);

        if(currentindex == 1)
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(0);
        }
        else
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(currentindex);
        }
    }
    else
    {
        ui->trans_select_tabWidget->setTabText(3, tabName1);
        ui->trans_select_tabWidget->setTabText(4, tabName2);
        ui->Hostcomputer_Mode_tabWidget->setTabText(1, tabName3);

        ui->trans_select_tabWidget->setTabEnabled(3, true);
        ui->trans_select_tabWidget->setTabEnabled(4, true);
        ui->Hostcomputer_Mode_tabWidget->setTabEnabled(1, true);
    }
}


void Widget::on_Open_Serial_checkBox_clicked(bool checked)
{
    if(checked == false)
    {
        uint8 currentindex = ui->Hostcomputer_Mode_tabWidget->currentIndex();
        tabName4 = ui->trans_select_tabWidget->tabText(1);
        tabName5 = ui->trans_select_tabWidget->tabText(2);
        tabName6 = ui->Hostcomputer_Mode_tabWidget->tabText(2);
        ui->trans_select_tabWidget->setTabText(1, "");
        ui->trans_select_tabWidget->setTabText(2, "");
        ui->Hostcomputer_Mode_tabWidget->setTabText(2, "");


        ui->trans_select_tabWidget->setTabEnabled(1, false);
        ui->trans_select_tabWidget->setTabEnabled(2, false);
        ui->Hostcomputer_Mode_tabWidget->setTabEnabled(2, false);

        if(currentindex == 2)
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(0);
        }
        else
        {
            ui->Hostcomputer_Mode_tabWidget->setCurrentIndex(currentindex);
        }
    }
    else
    {
        ui->trans_select_tabWidget->setTabText(1, tabName4);
        ui->trans_select_tabWidget->setTabText(2, tabName5);
        ui->Hostcomputer_Mode_tabWidget->setTabText(2, tabName6);

        ui->trans_select_tabWidget->setTabEnabled(1, true);
        ui->trans_select_tabWidget->setTabEnabled(2, true);
        ui->Hostcomputer_Mode_tabWidget->setTabEnabled(2, true);
    }
}


void Widget::Start_Screen()
{

    get_the_workshop(0);//自动获取本地文件地址//0是获取debug文件//1是获取程序文件
    load_color();
    allinit();
    QSplashScreen* splash = new QSplashScreen();
    QMovie movie(DefaultFile + "/load.gif");
    QLabel label;
    label.setWindowFlags(label.windowFlags() | Qt::WindowStaysOnTopHint);
    label.setAttribute(Qt::WA_TranslucentBackground);
    label.setMovie(&movie);
    label.setScaledContents(true); // 自动适应窗口大小
    label.setWindowFlags(Qt::FramelessWindowHint); // 去除边框
    movie.start();
    // 获取屏幕的几何属性
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    // 计算标签的位置，使其居中显示
    int x = (screenGeometry.width() - label.width()) / 2;
    int y = (screenGeometry.height() - label.height()) / 2;
    label.move(x, y);
    label.raise();
    label.show();
    // 延迟5秒
    QTime t;
    t.start();
    while (t.elapsed() < 0)
    {
        QApplication::processEvents();
    }

    label.close();

}


void Widget::on_serailCb_highlighted(const QString &arg1)
{
    serailCb_index  = ui->serailCb->currentIndex() ;
}

void Widget::on_serailCb_currentIndexChanged(const QString &arg1)
{
    if(ui->openBt->text() == "关闭串口")
    {
        ui->serailCb->setCurrentIndex(serailCb_index);
        ui->serail_condtion_label->setText(QString("连接成功\n但请断开原串口连接"));
        ui->serail_condtion_label->setStyleSheet(
        "background-color:rgb(255,114,114);font-size:30px;color:black");
    }
}




void Widget::Watch_the_silder(uint i)
{
    if(i == 0)
    {
        ui->Total_num_label->hide();
        ui->Pic_num_label->hide();
        ui->Pic_verticalSlider->hide();
        ui->Pic_num_lineEdit->hide();
        ui->pixelInfo_lable->hide();
    }
    else if(i == 1)
    {
        ui->Total_num_label->show();
        ui->Pic_num_label->show();
        ui->Pic_verticalSlider->show();
        ui->Pic_num_lineEdit->show();
        ui->pixelInfo_lable->show();
    }
}

void Widget::on_trans_select_tabWidget_tabBarClicked(int index)
{
    if(index == 1 )
    {
        Watch_the_silder(1);
    }
    else if(index == 3)
    {
        Watch_the_silder(1);
    }
    else
    {
        Watch_the_silder(0);
    }
}
