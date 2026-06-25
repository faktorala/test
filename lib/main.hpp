/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@编   写：龙邱科技
@邮   箱：chiusir@163.com
@编译IDE：Linux 环境、VSCode_1.93 及以上版本、Cmake_3.16 及以上版本
@使用平台：龙芯2K0300久久派和北京龙邱智能科技龙芯久久派拓展板
@相关信息参考下列地址
    网      站：http://www.lqist.cn
    淘 宝 店 铺：http://longqiu.taobao.com
    程序配套视频：https://space.bilibili.com/95313236
@软件版本：V1.0 版权所有，单位使用请先联系授权
@参考项目链接：https://github.com/AirFortressIlikara/ls2k0300_peripheral_library

@修改日期：2025-02-26
@修改内容：
@注意事项：注意查看路径的修改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#pragma once

#include <iostream>
#include <string>
#include <numeric>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <vector>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>    // 共享内存
#include <sys/sem.h>    // 信号量集

#include <opencv2/opencv.hpp>

#include "LQ_TFT18.hpp"
#include "LQ_TFT18_dri.hpp"
#include "LQ_PWM.hpp"
#include "LQ_GTIM_PWM.hpp"
#include "LQ_GPIO.hpp"
#include "LQ_HW_GPIO.hpp"
#include "LQ_PWM_ENCODER.hpp"
#include "LQ_HW_PWM.hpp"
#include "LQ_SOFT_I2C.hpp"
#include "LQ_SOFT_I2C_Gyro.hpp"
#include "LQ_MPU6050.hpp"
#include "LQ_Uart.hpp"
#include "servo.h"
#include <algorithm>
#include "imcu.h"
#include "Motorpid.hpp"
#include "control.hpp"
using namespace cv;
using namespace std;


#pragma once 


extern int avg_error;
extern int Maxleft[2],Maxright[2];
extern int stop_flsh;//停止标志

struct ControlParams {
    float steering_angle;  // 舵机转向角（-100~100）
    float motor_speed;     // 电机速度（0~100）
};
ControlParams process_frame(Mat& frame);
//电机测试程序
       
extern   VideoCapture cap;
extern   VideoCapture cap1;

extern  Mat frame,gray,binary,filtered;
extern double output;
void Camerinit();
void Cameratest();
extern vector<int> columns; //初始化columns为所有列数

extern vector<int> Left;
extern vector<int> Right;
extern vector<int> Midline;
extern int Search_Stop_Line;
extern int  Left_Up_Find,
    Right_Up_Find;   
extern int  Right_Down_Find,
    Left_Down_Find;
cv::Mat get_bottom_roi(const cv::Mat& processed_img, float ratio=0.5);
extern int cross_flag;
extern int Island_State;       //环岛状态标志
extern vector<int> Left_Lost_Flag;//左丢线数组，丢线置1，不丢线置0
extern vector<int> Right_Lost_Flag;//右丢线数组，丢线置1，不丢线右舷
extern int Left_Lost_Time;//边线丢边数
extern int  Right_Lost_Time;
extern int Both_Lost_Time;//两边同时丢线数
extern int Boundry_Start_Left;   //左右边界起始点
extern int Boundry_Start_Right;  //第一个非丢线点,常规边界起始点
extern int  Road_Wide[120];       //每一行的宽


extern float M ,K ;

extern float FJ_Angle; //陀螺仪数据
extern int Left_Island_Flag; //左右环岛标志
extern int Right_Island_Flag;//左右环岛标志
extern int Left_Up_Guai[2];    //四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列
extern int Right_Up_Guai[2];   //四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列

extern int Uart_recv;
extern uint8_t data1[120][160];
float Fuzzy_P(int E, int EC);    //常规模糊 P

extern float EnL, EnR;
extern int Lupdate;
extern int Rupdate;
extern float Exdef_speed;
extern float Indef_speed;

 extern int16_t gyrox, gyroy, gyroz; // 陀螺仪原始数据 extern
 extern int16_t accx,accy,accz;
 extern float AngleSpeed ;
extern float FJ_LastAngleSpeed ,FJ_AngleSpeed ;
extern  float FJ_Angle_Max ;
 extern float FJ_Angle_Min ;
 extern int long_way;
extern int Lost_start_Right;
extern int Lost_start_Left;
 extern int sart_line,end_line;

 extern uint8_t insland_err;
 extern uint8_t down;
 extern float island_chasu;
 extern int stop_line;
// 电机测试程序
void MotorTest();

// 舵机测试程序
void ServoTest();

// 久久派 21 个 GPIO 翻转测试
void GpioTest();

// GPIO 输出测试(设备文件)
void GpioOutputTest1(int gpio);

// GPIO 输出测试(硬件)
void GpioOutputTest2(int gpio);

// GPIO 输入测试(设备文件)
void GpioInputTest1();

// GPIO 输入测试(硬件)
void GpioInputTest2();

// PWM 测试(设备文件)
void PwmDevTest();

// PWM 测试(寄存器)
void PwmHWTest();

// Gtim PWM 测试(硬件)
void GtimPwmTest();

// 编码器测试(硬件)
void EncoderTest();

// 摄像头测试
//void CameraTest();

// ADC 功能测试
void AdcFunTest();

// tft屏幕测试程序
void TFTTest();
uint16_t BGR2RGB565(const Vec3b &color);
// sleep()函数测试 -- 以秒为单位延时
void sleepTest();

// usleep()函数测试 -- 以微秒为单位延时
void usleepTest();

// nanosleep()函数测试 -- 以纳秒为单位延时
void nanosleepTest();

// clock_nanosleep()函数测试 -- 以纳秒为单位延时
void clock_nanosleepTest();

// 计算帧率
void CalculateFrameRate();

// 时间戳打印测试
void GetTimeTest();

// 获取当前毫秒值(日历时间)
void GetCurrentMillisecond();

// 打印当前微秒值
void printMicTimestamp();

void CameraTest();

void Find_In_Three(void);
void Find_Down_Point(int start,int end);
void Find_Up_Point(int start,int end);
void Cross_Detect(void);
void Lengthen_Left_Boundry(int start,int end);
void Lengthen_Right_Boundry(int start,int end);
int Find_Right_Down_Point(int start,int end);//找四个角点，返回值是角点所在的行数
int Find_Right_Up_Point(int start,int end);//找四个角点，返回值是角点所在的行数
int Continuity_Change_Right(int start,int end);
int Continuity_Change_Left(int start,int end);//连续性阈值设置为5

int Monotonicity_Change_Right(int start,int end);//单调性改变，返回值是单调性改变点所在的行数
int Monotonicity_Change_Left(int start,int end);//单调性改变，返回值是单调性改变点所在的行数

int Find_Left_Down_Point(int start,int end);//找四个角点，返回值是角点所在的行数
void K_Draw_Line(float k, int startX, int startY,int endY);
int Find_Left_Up_Point(int start,int end);//找四个角点，返回值是角点所在的行数
void K_Add_Boundary(Mat& image, float k, int startX, int startY, int endY) ;
void Island_Detect(void);
void Longest_White_Column(void);
void get_error(void);
// float get_curvature(void);
void Uart_Recv(void);

template<typename T>
const T& clamps(const T& value, const T& low, const T& high) {
    return (value < low) ? low : (value > high) ? high : value;
}

void Vofa_printf(void);
