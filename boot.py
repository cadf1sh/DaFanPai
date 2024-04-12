from machine import *
from display import *
import gc
import time
from smartcar import *
from seekfree import *

# 开发板上的 C19 是拨码开关
end_switch = Pin('C19', Pin.IN, pull=Pin.PULL_UP_47K, value = True)

# # # # # # # # # # # # # # # 六轴 # # # # # # # # # # # # # # #
imu = IMU660RA()
# # # # # # # # # # # # # # # 六轴 # # # # # # # # # # # # # # #

# # # # # # # # # # # # # # # 无线串口 # # # # # # # # # # # # # # #
wireless = WIRELESS_UART(460800)
# # # # # # # # # # # # # # # 无线串口 # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # 按键# # # # # # # # # # # # # # # 
key = KEY_HANDLER(10)
key_a = 0
# # # # # # # # # # # # # # # 按键# # # # # # # # # # # # # # # 

# # # # # # # # # # # # # # # 屏幕# # # # # # # # # # # # # # # 
# 定义控制引脚
rst = Pin('B9' , Pin.OUT, pull=Pin.PULL_UP_47K, value=1)
dc  = Pin('B8' , Pin.OUT, pull=Pin.PULL_UP_47K, value=1)
blk = Pin('C4' , Pin.OUT, pull=Pin.PULL_UP_47K, value=1)
drv = LCD_Drv(SPI_INDEX=1, BAUDRATE=60000000, DC_PIN=dc, RST_PIN=rst, LCD_TYPE=LCD_Drv.LCD200_TYPE)
lcd = LCD(drv)
lcd.color(0xFFFF, 0x0000)
lcd.mode(2)
lcd.clear(0x0000)
# # # # # # # # # # # # # # # 屏幕# # # # # # # # # # # # # # # 


# # # # # # # # # # # # # # # 电机# # # # # # # # # # # # # # # 
motor_l = MOTOR_CONTROLLER(MOTOR_CONTROLLER.PWM_C25_DIR_C27, 13000, duty = 1, invert = True)
motor_r = MOTOR_CONTROLLER(MOTOR_CONTROLLER.PWM_C24_DIR_C26, 13000, duty = 1, invert = True)
motor_dir = 0
motor_duty = 1500
motor_duty_max = 2000
global speed_kp
global speed_ki
speed_kp = 10
speed_ki = 4

OutPut = 0
# # # # # # # # # # # # # # # 电机# # # # # # # # # # # # # # # 


# # # # # # # # # # # # # # # 编码器# # # # # # # # # # # # # # # 
encoder_l = encoder("D0", "D1", True)
encoder_r = encoder("D2", "D3")
encl_data = 0
encr_data = 0
allencl_data = 0#5603  1m/脉冲
allencr_data = 0#5594  wocao ge 1m/脉冲
speed_TrueM = 0
# # # # # # # # # # # # # # # 编码器# # # # # # # # # # # # # # # 


# # # # # # # # # # # # # # # CCD
# 调用 TSL1401 模块获取 CCD 实例
# 参数是采集周期 调用多少次 capture 更新一次数据
# 默认参数为 1 调整这个参数相当于调整曝光时间倍数
ccd = TSL1401(3)
ccd_data1 = ccd.get(0)
ccd_data2 = ccd.get(1)
# # # # # # # # # # # # # # # CCD




# # # # # # # # # # # # # # # 定时中断
# 定义一个回调函数
def time_pit_handler1(time):
    global ticker_flag1
    global ticker_count1
    ticker_flag1 = True
    ticker_count1 = (ticker_count1 + 1) if (ticker_count1 < 100) else (1)
    
def time_pit_handler2(time):
    global ticker_flag2
    global ticker_count2
    ticker_flag2 = True
    ticker_count2 = (ticker_count2 + 1) if (ticker_count2 < 100) else (1)
    
def time_pit_handler3(time):
    global ticker_flag3
    global ticker_count3
    ticker_flag3 = True
    ticker_count3 = (ticker_count3 + 1) if (ticker_count3 < 100) else (1)
    
def time_pit_handler4(time):
    global ticker_flag4
    global ticker_count4
    ticker_flag4 = True
    ticker_count4 = (ticker_count4 + 1) if (ticker_count4 < 100) else (1)
    
    
pit1 = ticker(0)
pit1.capture_list(ccd)
pit1.callback(time_pit_handler1)
pit1.start(3)

pit2 = ticker(1)
pit2.capture_list(encoder_l, encoder_r)
pit2.callback(time_pit_handler2)
pit2.start(2)

pit3 = ticker(2)
pit3.capture_list(key)
pit3.callback(time_pit_handler3)
pit3.start(10)

pit4 = ticker(3)
pit4.capture_list(imu)
pit4.callback(time_pit_handler4)
pit4.start(2)

# # # # # # # # # # # # # # # 定时中断

def direction_outter(error):

    return error

def direction_inner(error):

    return error



def MotorSpeedGet(encl_data,encr_data):
    Temp1 = encl_data
    Temp2 = encr_data
    BodyRadius = 15.5
    Temp1_t = Temp1
    Temp2_t = Temp2
    speed_Lencoder = Temp1 * 50000.0 / 4600.000 / 3
    speed_Rencoder = Temp2 * 50000.0 / 4500.000 / 3
    speed_LaverageEncoder = speed_Lencoder
    speed_RaverageEncoder =  speed_Rencoder
    # speed_corr_L = speed_LaverageEncoder  + 0.5 * BodyRadius * Yawrate * 3.14159 / 180
    # speed_corr_R = speed_RaverageEncoder  - 0.5 * BodyRadius * Yawrate * 3.14159 / 180
    speed_corr_L = speed_LaverageEncoder
    speed_corr_R = speed_RaverageEncoder
    speed_True = (speed_corr_L + speed_corr_R) / 2
    return speed_True


def speed_loop(speed_True):
    aim_speed = 0
    speed_kp = 10
    speed_ki = 4
    OutPutMaxLimit = 4000
    OutPutMinLimit = -4000
    error = speed_True - aim_speed
    ErrorFifo = [0, 0, 0]
    ErrorFifo[2] = ErrorFifo[1]
    ErrorFifo[1] = ErrorFifo[0]
    ErrorFifo[0] = error
    
    ErrorDtFifo = [0, 0, 0]
    ErrorDtFifo[2] = ErrorDtFifo[1]
    ErrorDtFifo[1] = ErrorDtFifo[0]
    ErrorDtFifo[0] = ErrorFifo[0] - ErrorFifo[2]
    global OutPut
    OutPut += speed_kp * ErrorDtFifo[0] + speed_ki * error
    
    if OutPut >= OutPutMaxLimit:
        OutPut = OutPutMaxLimit
    if OutPut <= OutPutMinLimit:
        OutPut = OutPutMinLimit
    return OutPut

# 需要注意的是 ticker 是底层驱动的 这导致 Thonny 的 Stop 命令在这个固件版本中无法停止它
# 因此一旦运行了使用了 ticker 模块的程序 要么通过复位核心板重新连接 Thonny
# 或者像本示例一样 使用一个 IO 控制停止 Ticker 后再使用 Stop/Restart backend 按钮
ticker_flag1 = False
ticker_count1 = 0
runtime_count1 = 0
ticker_flag2 = False
ticker_count2 = 0
runtime_count2 = 0
ticker_flag3 = False
ticker_count3 = 0
runtime_count3 = 0
ticker_flag4 = False
ticker_count4 = 0
runtime_count4 = 0
ac_imu_data = [0, 0, 0, 0, 0, 0]

out = 0
while True:
    if (ticker_flag1):
        ccd_data1 = ccd.get(0)
        ccd_data2 = ccd.get(1)
        
        #wireless.send_ccd_image(WIRELESS_UART.CCD2_BUFFER_INDEX)
    # # # # # # # # # # # # # V
    if (ticker_flag2):
        encl_data = encoder_l.get()
        encr_data = encoder_r.get()
        allencl_data = allencl_data + encl_data
        allencr_data = allencr_data + encr_data
        speed_TrueM = MotorSpeedGet(encl_data,encr_data)
        out = speed_loop(speed_TrueM)
        if motor_dir == 1 and out > 0:
            motor_duty = out
        if motor_dir == 0 and out > 0:
            motor_duty = -out
        motor_l.duty(out)
        motor_r.duty(out)
    # # # # # # # # # # # # # V
    if (ticker_flag3):
        key_data = key.get()
        # 按键数据为三个状态 0-无动作 1-短按 2-长按
        if key_data[0]:
            key.clear(1)
        if key_data[1]:
            key.clear(2)
        if key_data[2]:
            key.clear(3)
        if key_data[3]:
            key.clear(4)
        
        ticker_flag = False
    # # # # # # # # # # # # # V
    if (ticker_flag4):
        imu_data = imu.get()
        ac_imu_data[0] = imu_data[0]
        ac_imu_data[1] = imu_data[1]
        ac_imu_data[2] = imu_data[2]
        ac_imu_data[3] = imu_data[3]
        ac_imu_data[4] = imu_data[4]
        ac_imu_data[5] = imu_data[5]
        ticker_flag4 = False
    #wireless.send_ccd_image(WIRELESS_UART.CCD1_BUFFER_INDEX)
    #lcd.str16(0,130,"encoder_l={:>3d}.".format(encl_data),0xFFFF)
    #lcd.str16(0,146,"encoder_r={:>3d}.".format(encr_data),0xFFFF)
    lcd.str16(0,130,"acc = {:>4d}".format(ac_imu_data[0]),0xFFFF)
    lcd.str16(0,146,"gyro = {:>4d}".format(ac_imu_data[3]),0xFFFF)
    #lcd.str16(0,72,"speed_kp={:>3d}.".format(speed_kp),0xFFFF)
    #lcd.str16(0,90,"speed_ki={:>3d}.".format(speed_ki),0xFFFF)
    #lcd.str16(0,16,"speedTrue={:>4f}.".format(speed_TrueM),0xFFFF)
    #lcd.str16(0,32,"allencl={:>3d}.".format(allencl_data),0xFFFF)
    #lcd.str16(0,48,"allencr={:>3d}.".format(allencr_data),0xFFFF)
    lcd.wave(0,  0, 128, 64, ccd_data1)
    lcd.wave(0, 64, 128, 64, ccd_data2)
    gc.collect()
    if end_switch.value() == 0:
        pit1.stop()
        pit2.stop()
        pit3.stop()
        lcd.clear()
        break









 



