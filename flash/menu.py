from machine import *
from display import *
from smartcar import *
from seekfree import *
import math
import os
import io
import gc
import time
paper_index = 0
value_index = 0
speed_kp = 0
speed_ki= 0
Angle_kp = 0
Angle_kd = 0
DirInner_KP= 0
DirInner_KD = 0
start_flag = False
end_switch = Pin('C19', Pin.IN, pull=Pin.PULL_UP_47K, value = True)


###################################按键菜单###################################
######################################################################
######################################################################
key2 = KEY_HANDLER(15)
def key_pit(time):
    global paper_index
    global value_index
    key_2data = key2.get()
    # 按键数据为三个状态 0-无动作 1-短按 2-长按
    #lcd.str16(20, 8*16, "p_index = {:>2d}".format(paper_index), 0xFFFF)
    if key_2data[0]:
        print(1)
        paper_index = paper_index - 1
        key2.clear(1)
    if key_2data[1]:
        print(2)
        paper_index = paper_index + 1
        key2.clear(2)
    if key_2data[2]:
        print(3)
        value_index = value_index+1
        key2.clear(3)
    if key_2data[3]:
        print(4)
        value_index = value_index-1
        key2.clear(4)
    
pit3 = ticker(2)
pit3.capture_list(key2)
pit3.callback(key_pit)
pit3.start(10)

###################################按键菜单###################################
######################################################################
######################################################################


# # # # # # # # # # # # # # # 屏幕# # # # # # # # # # # # # # # 
rst = Pin('B9' , Pin.OUT, pull=Pin.PULL_UP_47K, value=1)
dc  = Pin('B8' , Pin.OUT, pull=Pin.PULL_UP_47K, value=1)
blk = Pin('C4' , Pin.OUT, pull=Pin.PULL_UP_47K, value=1)
drv = LCD_Drv(SPI_INDEX=1, BAUDRATE=60000000, DC_PIN=dc, RST_PIN=rst, LCD_TYPE=LCD_Drv.LCD200_TYPE)
lcd = LCD(drv)
lcd.color(0xFFFF, 0x0000)
lcd.mode(2)
lcd.clear(0x0000)
# # # # # # # # # # # # # # # 屏幕# # # # # # # # # # # # # # # 

        


def data_list_read():##从txt中按顺序读出data
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
    
    
   
   
   
def data_list_write():##把data写入txt
    global speed_kp
    global speed_ki
    global Angle_kp
    global Angle_kd
    global DirInner_KP
    global DirInner_KD
    write_data = 512* [0]
    write_data[0] = speed_kp
    write_data[1] = speed_ki
    write_data[2] = Angle_kp
    write_data[3] = Angle_kd
    write_data[4] = DirInner_KP
    write_data[5] = DirInner_KD
    user_file = 'menu.txt'
    # 切换到 /flash 目录
    os.chdir("/flash")
    try:
    # 通过 try 尝试打开文件 因为 r+ 读写模式不会新建文件
        user_file = io.open("menu.txt", "w+")
    except:
    # 如果打开失败证明没有这个文件 所以使用 w+ 读写模式新建文件
        user_file = io.open("menu.txt", "w+")
    
    user_file.seek(0,0)
    # 读取三行数据 到临时变量 分别强制转换回各自类型
    for index in range(512):
         user_file.write("%s\n"%(str(write_data[index])))
    # 将缓冲区数据写入到文件 清空缓冲区 相当于保存指令
    user_file.flush()
    user_file.close()
    
    
####总调参界面########################################################################
####总调参界面########################################################################################
####总调参界面############################################################################
def adjust_menu():
    global speed_kp
    global speed_ki
    global Angle_kp
    global Angle_kd
    global DirInner_KP
    global DirInner_KD
    global paper_index
    global value_index
    global key_ticker_flag
    data_list_read()
    lcd.clear(0xFFFF)
    while True:
        if paper_index == 0:
            if value_index == 1:
                value_index = 0
                speed_kp += 1
            elif value_index == -1:
                value_index = 0
                speed_kp -= 1
        elif paper_index == 1:
            if value_index == 1:
                value_index = 0
                speed_ki += 1
            elif value_index == -1:
                value_index = 0
                speed_ki -= 1
        elif paper_index == 2:
            if value_index == 1:
                value_index = 0
                Angle_kp += 1
            elif value_index == -1:
                value_index = 0
                Angle_kp -= 1
        elif paper_index == 3:
            if value_index == 1:
                value_index = 0
                Angle_kd += 1
            elif value_index == -1:
                value_index = 0
                Angle_kd -= 1
        elif paper_index == 4:
            if value_index == 1:
                value_index = 0
                DirInner_KP += 1
            elif value_index == -1:
                value_index = 0
                DirInner_KP -= 1
        elif paper_index == 5:
            if value_index == 1:
                value_index = 0
                DirInner_KD += 1
            elif value_index == -1:
                value_index = 0
                DirInner_KD -= 1
        elif paper_index == 6: 
            if value_index == 1:
                value_index = 0
                lcd.clear(0x0000)
                time.sleep_ms(50)
                data_list_write()
                time.sleep_ms(50)
                time.sleep_ms(500)
                lcd.clear(0x0000)
                break
        lcd.str16(20, 0*16, "speed_kp = {:>3f}".format(speed_kp), 0x0000)
        lcd.str16(20, 1*16, "speed_ki = {:>3f}".format(speed_ki), 0x0000)
        lcd.str16(20, 2*16, "Angle_kp = {:>3f}".format(Angle_kp), 0x0000)
        lcd.str16(20, 3*16, "Angle_kd = {:>3f}".format(Angle_kd), 0x0000)
        lcd.str16(20, 4*16, "DirInner_KP = {:>3f}".format(DirInner_KP), 0x0000)
        lcd.str16(20, 5*16, "DirInner_KD = {:>3f}".format(DirInner_KD), 0x0000)
        lcd.str16(20, 6*16, "exit", 0xF800)
        lcd.str16(20, 7*16, "paper_index = {:>2d}".format(paper_index), 0x0000)
        
        if paper_index == 7:
            paper_index = 0
        elif paper_index == -1:
            paper_index = 6
            
        lcd.str16(0, 0*16,"  ", 0xFFFF)
        lcd.str16(0, 1*16,"  ", 0xFFFF)
        lcd.str16(0, 2*16,"  ", 0xFFFF)
        lcd.str16(0, 3*16,"  ", 0xFFFF)
        lcd.str16(0, 4*16,"  ", 0xFFFF)
        lcd.str16(0, 5*16,"  ", 0xFFFF)
        lcd.str16(0, 6*16,"  ", 0xFFFF)
        lcd.str16(0, 7*16,"  ", 0xFFFF)
        lcd.str16(0, 8*16,"  ", 0xFFFF)
        lcd.str16(0, paper_index*16,"->", 0xF800)
        if end_switch.value() == 0:
            pit3.stop()
            lcd.clear()
            break

####总调参界面########################################################################
####总调参界面########################################################################################
####总调参界面############################################################################






        
        
            
        
        
    

    

