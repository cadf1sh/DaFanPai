from machine import *
from display import *
from smartcar import *
from seekfree import *
import math
import os
import io
import gc
import time
from menu import *#从menu库将导入




##从本地txt中按顺序读出data##由于两个文件的变量可能不互通，所以在主循环这里再读一次txt文件的数据
##从本地txt中按顺序读出data##由于两个文件的变量可能不互通，所以在主循环这里再读一次txt文件的数据
def locate_data_read():##从本地txt中按顺序读出data##由于两个文件的变量可能不互通，所以在主循环这里再读一次txt文件的数据
    global speed_kp
    global speed_ki
    global Angle_kp
    global Angle_kd
    global DirInner_KP
    global DirInner_KD
    read_data = 512* [0]
    user_file = 'menu.txt'
    # 切换到 /flash 目录
    os.chdir("/flash")
    try:
    # 通过 try 尝试打开文件 因为 r+ 读写模式不会新建文件
        user_file = io.open("menu.txt", "r+")
    except:
    # 如果打开失败证明没有这个文件 所以使用 w+ 读写模式新建文件
        user_file = io.open("menu.txt", "r+")
    
    user_file.seek(0,0)
    # 读取三行数据 到临时变量 分别强制转换回各自类型
    for index in range(512):
        read_data[index] = int(user_file.readline())
    # 将数据重新输出 这就演示了如何保存数据和读取数据  
    speed_kp= read_data[0]
    speed_ki= read_data[1]
    Angle_kp = read_data[2]
    Angle_kd = read_data[3]
    DirInner_KP= read_data[4]
    DirInner_KD= read_data[5]
    user_file.close()
##从本地txt中按顺序读出data##由于两个文件的变量可能不互通，所以在主循环这里再读一次txt文件的数据
##从本地txt中按顺序读出data##由于两个文件的变量可能不互通，所以在主循环这里再读一次txt文件的数据
    
   
   
##############调参运行##########################################
##############调参运行##########################################
##############调参运行##########################################
paper_index = 0
value_index = 0
pit3 = ticker(2)
pit3.capture_list(key2)
pit3.callback(key_pit)
pit3.start(10)
adjust_menu()
pit3.stop()
time.sleep_ms(500)
locate_data_read()
##############调参运行##########################################
##############调参运行##########################################
##############调参运行##########################################




# # # # # # # # # # # # # # # 六轴初始化 # # # # # # # # # # # # # # #
imu = IMU660RA()
# # # # # # # # # # # # # # # 六轴 # # # # # # # # # # # # # # #



# # # # # # # # # # # # # # # 无线串口 # # # # # # # # # # # # # # #
#wireless = WIRELESS_UART(460800)#不用到先注释
global serialbuffer
serialbuffer = [0]*270
# # # # # # # # # # # # # # # 无线串口 # # # # # # # # # # # # # # #





# # # # # # # # # # # # # # # 屏幕初始化# # # # # # # # # # # # # # # 
# 定义控制引脚
rst = Pin('B9' , Pin.OUT, pull=Pin.PULL_UP_47K, value=1)
dc  = Pin('B8' , Pin.OUT, pull=Pin.PULL_UP_47K, value=1)
blk = Pin('C4' , Pin.OUT, pull=Pin.PULL_UP_47K, value=1)
drv = LCD_Drv(SPI_INDEX=1, BAUDRATE=60000000, DC_PIN=dc, RST_PIN=rst, LCD_TYPE=LCD_Drv.LCD200_TYPE)
lcd = LCD(drv)
lcd.color(0xFFFF, 0x0000)
lcd.mode(2)
lcd.clear(0x0000)
# # # # # # # # # # # # # # # 屏幕初始化# # # # # # # # # # # # # # # 


# # # # # # # # # # # # # # # 电机初始化# # # # # # # # # # # # # # # 
motor_l = MOTOR_CONTROLLER(MOTOR_CONTROLLER.PWM_C25_DIR_C27, 13000, duty = 1, invert = True)
motor_r = MOTOR_CONTROLLER(MOTOR_CONTROLLER.PWM_C24_DIR_C26, 13000, duty = 1, invert = True)
motor_ldir = 0
motor_rdir = 0
motor_duty = 3000
motor_duty_max = 3000
OutSpeed = 0
OutPut = 0

encl_data = 0
encr_data = 0
allenc_data = 0
out = 0
# # # # # # # # # # # # # # # 电机# # # # # # # # # # # # # # # 


# # # # # # # # # # # # # # # 编码器# # # # # # # # # # # # # # # 
encoder_l = encoder("D0", "D1", True)
encoder_r = encoder("D2", "D3")
encl_data = 0
encr_data = 0
allencl_data = 0#5603  1m/脉冲
allencr_data = 0#5594  wocao ge 1m/脉冲
speed_TrueL = 0
speed_TrueR = 0
# # # # # # # # # # # # # # # 编码器# # # # # # # # # # # # # # # 


# # # # # # # # # # # # # # # CCD
# 调用 TSL1401 模块获取 CCD 实例
# 参数是采集周期 调用多少次 capture 更新一次数据
# 默认参数为 1 调整这个参数相当于调整曝光时间倍数
ccd = TSL1401(1)
ccd_data1 = 0
ccd_data2 = 0
ccd_data1 = ccd.get(0)
ccd_data2 = ccd.get(1)
global trans_ccd1
global trans_ccd2
trans_ccd1 = [0] * 128
trans_ccd2 = [0] * 128
# # # # # # # # # # # # # # # CCD



# # # # # # # # # # # # # # # 定时中断
# 定义一个回调函数
def time_pit_handler1(time):
    global ticker_flag1
    global ticker_count1
    global ccd_data1
    global ccd_data2
    ticker_flag1 = True
    ccd_data1 = ccd.get(0)
    ccd_data2 = ccd.get(1)
    
    
def time_pit_handler2(time):
    global ticker_flag2
    global ticker_count2
    ticker_flag2 = True
    global encl_data
    global encr_data
    global allenc_data
    global allencl_data
    global allencr_data
    global speed_TrueL
    global speed_TrueR
    global DirInner_OutPut
    global speed_kp
    global speed_ki
    global Angle_kp
    global Angle_kd
    global DirInner_KP
    global DirInner_KD
    encl_data = encoder_l.get()
    encr_data = encoder_r.get()
    allencl_data = allencl_data + encl_data
    allencr_data = allencr_data + encr_data
    allenc_data  = ( allencl_data + allencr_data )/2
    speed_TrueL = MotorSpeedGet(encl_data,4500)
    speed_TrueR = MotorSpeedGet(encr_data,4600)
    out_L = -speed_loop(speed_TrueL,0)
    out_R = -speed_loop(speed_TrueR,0)
    motor_control(out_L + DirInner_OutPut,out_L -DirInner_OutPut)
#     motor_control(DirInner_OutPut,-DirInner_OutPut)
    



def time_pit_handler4(time):
    global ticker_flag4
    global ticker_count4  
    ticker_flag4 = True
    global CarAngle_Turn_Angle
    global CarAngle_pitch
    global CarAngle_roll
    CarAngle_Turn_Angle = Get_IMU_TransData()
    #CarAngle_pitch, CarAngle_roll, CarAngle_Turn_Angle = Update_IMU()
    Angle_loop(CarAngle_Turn_Angle,-90);
    Inner_OutPut = direction_inner()
    
    
pit4 = ticker(3)
pit4.capture_list(imu)
pit4.callback(time_pit_handler4)
pit2 = ticker(1)
pit2.capture_list(encoder_l, encoder_r)
pit2.callback(time_pit_handler2)
pit1 = ticker(0)
pit1.capture_list(ccd)
pit1.callback(time_pit_handler1)
pit1.start(10)
pit2.start(2)
pit4.start(2)
# # # # # # # # # # # # # # # 定时中断
def wireless_send_list(temp_list, length):
    for index in range(length):
        #print(str(temp_list[index]))
        wireless.send_str(str(temp_list[index])+",")
        

def LPF_1_db(hz, time, _in, _out):
    _out[0] += (1 / (1 + 1 / (hz * 6.28 * time))) * (_in - _out[0])
    


def Update_IMU():
    return 1

Turn_Angle = 0

def direction_outter(error):
    
    return error


    
    
  
def direction_inner():
    global DirInner_ErrorLast
    global DirInner_Error
    global DirInner_ErrorFifo
    global DirInner_ErrorDtTemp
    global DirInner_FeedbackTemp
    global DirInner_OutTemp
    global DirInner_OutPut
    global DirInner_KP
    global DirInner_KD
    
    DirInner_Error = CarAngle_Yawrate - Angle_OutPut
    if DirInner_Error >= 1000:
        DirInner_Error = 1000
    if DirInner_Error <= -1000:
        DirInner_Error = -1000

    
    DirInner_ErrorFifo[5] = DirInner_ErrorFifo[4]
    DirInner_ErrorFifo[4] = DirInner_ErrorFifo[3]
    DirInner_ErrorFifo[3] = DirInner_ErrorFifo[2]
    DirInner_ErrorFifo[2] = DirInner_ErrorFifo[1]
    DirInner_ErrorFifo[1] = DirInner_ErrorFifo[0]
    DirInner_ErrorFifo[0] = DirInner_Error
    
    DirInner_ErrorDtTemp[3] = DirInner_ErrorDtTemp[2]
    DirInner_ErrorDtTemp[2] = DirInner_ErrorDtTemp[1]
    DirInner_ErrorDtTemp[1] = DirInner_ErrorDtTemp[0]
    DirInner_ErrorDtTemp[0] = DirInner_ErrorFifo[0] - DirInner_ErrorFifo[3]

    DirInner_FeedbackTemp[3] = DirInner_FeedbackTemp[2]
    DirInner_FeedbackTemp[2] = DirInner_FeedbackTemp[1]
    DirInner_FeedbackTemp[1] = DirInner_FeedbackTemp[0]
    DirInner_FeedbackTemp[0] = CarAngle_Yawrate
    DirInner_Differential = DirInner_FeedbackTemp[0] - DirInner_FeedbackTemp[3]

    DirInner_InOut = (DirInner_KP / 10) * DirInner_ErrorFifo[0] + (DirInner_KD/ 10 ) * (- DirInner_Differential)

    DirInner_OutTemp[3] = DirInner_OutTemp[2]
    DirInner_OutTemp[2] = DirInner_OutTemp[1]
    DirInner_OutTemp[1] = DirInner_OutTemp[0]
    DirInner_OutTemp[0] = DirInner_InOut
    
    #DirInner_OutPut = 0.8 * DirInner_OutTemp[0] + 0.2 * DirInner_OutTemp[1]
    DirInner_OutPut = DirInner_InOut
    return DirInner_OutPut



def serial_write():
    serialbuffer[0] = 'C'
    serialbuffer[1] = 'Z'
    serialbuffer[2] = 'Y'
    serialbuffer[3] = 'N'
    serialbuffer[4] = 'B'
    serialNum = 5
    for num in range(128):
        serialbuffer[serialNum + num] = ccd_data1[num]
    serialNum += 128
    for num in range(128):
        serialbuffer[serialNum + num] = ccd_data2[num]
    serialNum += 128
    serialbuffer[serialNum] = 'V'
    serialNum += 1
    serialbuffer[serialNum] = 'A'
    serialNum += 1
    serialbuffer[serialNum] = 'L'
    serialNum += 1
    serialbuffer[serialNum] = 'U'
    serialNum += 1
    serialbuffer[serialNum] = 'E'
    
    wireless_send_list(serialbuffer, 270)
    

    


    
def Get_the_transCcd(ccd_data1,ccd_data2):
    max_value1 = ccd_data1[0]
    min_value1 = ccd_data1[0]
    for value1 in ccd_data1:
        if value1 >= max_value1:
            max_value1 = value1
        if value1 <= min_value1:
            min_value1 = value1
    threshold1 = (max_value1 + min_value1) / 2
    
    max_value2 = ccd_data2[0]
    min_value2 = ccd_data2[0]
    for value2 in ccd_data2:
        if value2 >= max_value2:
            max_value2 = value2
        if value2 <= min_value2:
            min_value2 = value2
    threshold2 = (max_value2 + min_value2) / 2
    
    for i in range(len(ccd_data1) -1 ):
        if ccd_data1[i] >= threshold1:
            trans_ccd1[i] = 128
        else:
            trans_ccd1[i] = 0
    
    for j in range(len(ccd_data2) - 1):
        if ccd_data2[j] >= threshold2:
            trans_ccd2[j] = 128
        else:
            trans_ccd2[j] = 0

  

def Angle_loop(now_angle,aim_angle):
    global Angle_ErrorDtFifo
    global Angle_ErrorFifo
    global Angle_OutPut_R
    global Angle_OutPut_L
    global Angle_OutPut
    global Angle_kp
    global Angle_kd
    error = aim_angle - now_angle
    if error < 5 or error >-5 :
        Angle_kp = Angle_kp * 11.5
        Angle_kd = Angle_kd * 18
    if error < 10 or error >-10 :
        Angle_kp = Angle_kp * 7
        Angle_kd = Angle_kd * 16
    if error < 15 or error >-15 :
        Angle_kp = Angle_kp * 2
        Angle_kd = Angle_kd * 5
    Angle_ErrorFifo[2] = Angle_ErrorFifo[1]
    Angle_ErrorFifo[1] = Angle_ErrorFifo[0]
    Angle_ErrorFifo[0] = error
    
    Angle_ErrorDtFifo[2] = Angle_ErrorDtFifo[1]
    Angle_ErrorDtFifo[1] = Angle_ErrorDtFifo[0]
    Angle_ErrorDtFifo[0] = Angle_ErrorFifo[0] - Angle_ErrorFifo[1]
    Angle_OutPut =  Angle_kp/10 * error   + Angle_kd/10 * Angle_ErrorDtFifo[0];
    
    Angle_OutPut_L = -Angle_OutPut
    Angle_OutPut_R = Angle_OutPut
    
    
    
def eliminate_drift(yaw_rate, drift_rate, alpha):
    if yaw_rate > 0 :
        drift_rate = drift_rate
    if yaw_rate < 0:
        drift_rate = -drift_rate
    filtered_yaw_rate = 0.0
    filtered_yaw_rate = (1 - alpha) * (filtered_yaw_rate + yaw_rate - drift_rate)
    return filtered_yaw_rate
#     drift_rate = 0.01  # 零漂速率
#     alpha = 0.2  # 平滑因子


def Get_IMU_TransData():
    global Turn_Angle
    global CarAngle_Yawrate
    imu_data = imu.get()
    ac_imu_data[0] = imu_data[0]
    ac_imu_data[1] = imu_data[1]
    ac_imu_data[2] = imu_data[2]
    ac_imu_data[3] = imu_data[3]
    ac_imu_data[4] = imu_data[4]
    ac_imu_data[5] = imu_data[5]
    gz = eliminate_drift(ac_imu_data[5],2.0,0.00)
    #CarAngle_Yawrate = gz * 0.32577 * 90 / 57.5 * 90 / 190.5
    #CarAngle_Yawrate = gz * 0.240898049
    CarAngle_Yawrate = gz * 0.2409 * 0.9 /120 * 90 
    Turn_Angle += CarAngle_Yawrate * 0.001 
    #Turn_Angle = round(Turn_Angle * 100) / 100.00;
    return Turn_Angle




def MotorSpeedGet(enc_data,enc_trans):
    Temp1 = enc_data
    BodyRadius = 15.5
    Temp1_t = Temp1
    speed_encoder = Temp1 * 50000.0 / enc_trans /3
    speed_averageEncoder = speed_encoder
    # speed_corr_L = speed_LaverageEncoder  + 0.5 * BodyRadius * Yawrate * 3.14159 / 180
    # speed_corr_R = speed_RaverageEncoder  - 0.5 * BodyRadius * Yawrate * 3.14159 / 180
    speed_corr = speed_averageEncoder
    speed_True = speed_corr
    return speed_True


def speed_loop(speed_True,aim_speed):
    
    global speed_kp
    global speed_ki
    global Speed_ErrorFifo
    global Speed_ErrorDtFifo
    global OutPut
    aim_speed = aim_speed + 1 - 1
    OutPutMaxLimit = 3000
    OutPutMinLimit = -3000
    error =   aim_speed - speed_True
    Speed_ErrorFifo[2] = Speed_ErrorFifo[1]
    Speed_ErrorFifo[1] = Speed_ErrorFifo[0]
    Speed_ErrorFifo[0] = error
    Speed_ErrorDtFifo[2] = Speed_ErrorDtFifo[1]
    Speed_ErrorDtFifo[1] = Speed_ErrorDtFifo[0]
    Speed_ErrorDtFifo[0] = Speed_ErrorFifo[0] - Speed_ErrorFifo[2]
    OutPut += speed_kp * Speed_ErrorDtFifo[0] + speed_ki * error
    
    if OutPut >= OutPutMaxLimit:
        OutPut = OutPutMaxLimit
    if OutPut <= OutPutMinLimit:
        OutPut = OutPutMinLimit
    return OutPut



def Len_loop(Len_True,aim_Len):
    aim_Len = aim_Len /3
    Len_kd = 15.0  #减少震荡
    Len_kp = 5   #程度
    OutPutMaxLimit = 300
    OutPutMinLimit = -300
    error = (int)((Len_True- aim_Len) / 10)
    error = error * 10
    global Len_ErrorFifo
    global Len_ErrorDtFifo
    Len_ErrorFifo[2] = Len_ErrorFifo[1]
    Len_ErrorFifo[1] = Len_ErrorFifo[0]
    Len_ErrorFifo[0] = error
    Len_ErrorDtFifo[2] = Len_ErrorDtFifo[1]
    Len_ErrorDtFifo[1] = Len_ErrorDtFifo[0]
    Len_ErrorDtFifo[0] = Len_ErrorFifo[0] - Len_ErrorFifo[2]
    global OutSpeed
    OutSpeed = Len_kd * Len_ErrorDtFifo[0] + Len_kp * error
    
    if OutSpeed >= OutPutMaxLimit:
        OutSpeed = OutPutMaxLimit
    if OutSpeed <= OutPutMinLimit:
        OutSpeed = OutPutMinLimit
    return OutSpeed


def motor_control(aimL,aimR):
    OutPutMaxLimit = 3000
    OutPutMinLimit = -3000
    out_L = aimL
    out_R = aimR
    if out_R >= OutPutMaxLimit:
        out_R = OutPutMaxLimit
    if out_R <= OutPutMinLimit:
        out_R = OutPutMinLimit
    if out_L >= OutPutMaxLimit:
        out_L = OutPutMaxLimit
    if out_L <= OutPutMinLimit:
        out_L = OutPutMinLimit
    if  out_L > 0:
        motor_ldir = 1
        motor_l.duty(out_L)
    if  out_L < 0:
        motor_ldir = 0
        motor_l.duty(out_L)
    if  out_R > 0:
        motor_rdir = 1
        motor_r.duty(out_R)
    if  out_R < 0:
        motor_rdir = 0
        motor_r.duty(out_R)
        
        
        
    
    
# 需要注意的是 ticker 是底层驱动的 这导致 Thonny 的 Stop 命令在这个固件版本中无法停止它
# 因此一旦运行了使用了 ticker 模块的程序 要么通过复位核心板重新连接 Thonny
# 或者像本示例一样 使用一个 IO 控制停止 Ticker 后再使用 Stop/Restart backend 按钮
ticker_flag1 = False
ticker_count1 = 0
runtime_count1 = 0
ticker_flag2 = False
ticker_count2 = 0
runtime_count2 = 0
# ticker_flag3 = False
# ticker_count3 = 0
# runtime_count3 = 0
ticker_flag4 = False
ticker_count4 = 0
runtime_count4 = 0
ac_imu_data = [0, 0, 0, 0, 0, 0]
CarAngle_Turn_Angle = 0
CarAngle_pitch = 0
CarAngle_roll = 0
CarAngle_Yawrate = 0

menu_flag = 1
Len_ErrorDtFifo = [0, 0, 0]
Len_ErrorFifo = [0, 0, 0]
Angle_ErrorDtFifo = [0, 0, 0]
Angle_ErrorFifo = [0, 0, 0]
Speed_ErrorFifo = [0, 0, 0]
Speed_ErrorDtFifo = [0, 0, 0]
Angle_OutPut_L = 0
Angle_OutPut_R = 0
Angle_OutPut = 0
q0 = 1
q1 = 0
q2 = 0
q3 = 0
exInt = 0
eyInt = 0
ezInt = 0
DirInner_ErrorFifo = [0, 0, 0, 0, 0,0]
DirInner_ErrorDtTemp = [0, 0, 0, 0]
DirInner_FeedbackTemp = [0, 0, 0, 0]
DirInner_OutTemp = [0, 0, 0, 0]
DirInner_Error = 0
DirInner_OutPut = 0



##调参界面结束，初始化上面函数变量结束，到真正下位机运行循环界面
##调参界面结束，初始化上面函数变量结束，到真正下位机运行循环界面##调参界面结束，初始化上面函数变量结束，到真正下位机运行循环界面
##调参界面结束，初始化上面函数变量结束，到真正下位机运行循环界面
while True:
# # # # # # # # # # # # # V
    if (ticker_flag1):
        ticker_flag1 = False
    # # # # # # # # # # # # # V
    if (ticker_flag2):
        ticker_flag2 = False
    # # # # # # # # # # # # # V
    if (ticker_flag4):
        ticker_flag4 = False
    lcd.str16(0,130,"Inner_Error = {:>5f}".format(DirInner_Error),0xFFFF)
    lcd.str16(0,146,"Turn_Angle = {:>5f}".format(CarAngle_Turn_Angle),0xFFFF)
    lcd.str16(0,162,"Inner_OutPut = {:>5f}".format(DirInner_OutPut),0xFFFF)
    lcd.str16(0,178,"yawrate = {:>5f}".format(CarAngle_Yawrate),0xFFFF)
    lcd.str16(0,178+16*1,"AngleOutPut = {:>5f}".format(Angle_OutPut),0xFFFF)
    lcd.wave(0,  0, 128, 64, ccd_data1)
    lcd.wave(0, 64, 128, 64, ccd_data2)
    gc.collect()
    if end_switch.value() == 0:
        pit1.stop()
        pit2.stop()
        pit3.stop()
        lcd.clear()
        break










