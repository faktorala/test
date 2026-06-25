#include <stdio.h>
#include <iostream>
#include "GPIO.h"
#include "LQ_GTIM_PWM.hpp"
#include "servo.h"
#include "LQ_PWM.hpp"
#include "LQ_PWM_ENCODER.hpp"
#include "LQ_HW_GPIO.hpp"
#include "LQ_HW_PWM.hpp"
#include "LQ_PWM_ENCODER.hpp"
#include "main.hpp"
#include "Motorpid.hpp"

#include <csignal>
#include <atomic>
#include <signal.h>
#include <sys/time.h>
#include <thread>
#include <chrono>


Mat frame,gray,binary,filtered;
int avg_error = 0;
int Maxleft[2]={0};
int Maxright[2]={0};
int stop_flsh=0; 
vector<int> columns(160);           //初始化columns为所有列数
vector<int> Left(120);
vector<int> Right(120);
vector<int> Midline(120);
vector<int> Left_Lost_Flag(120);    //左丢线数组，丢线置1，不丢线置0
vector<int> Right_Lost_Flag(120);   //右丢线数组，丢线置1，不丢线右舷
int cross_flag=0;

int Island_State=0;        //环岛状态标志

int Lost_start_Right=0;
int Lost_start_Left=0;
int Left_Lost_Time=0;               //边线丢边数
int Right_Lost_Time=0;
int Both_Lost_Time=0;               //两边同时丢线数
int Boundry_Start_Left=0;           //左右边界起始点
int Boundry_Start_Right=0;          //第一个非丢线点,常规边界起始点
int Road_Wide[120]={0};             //每一行的宽
int Left_Island_Flag=0;             //左右环岛标志

int Right_Island_Flag=0;            //左右环岛标志
int Left_Up_Guai[2];                //四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列
int Right_Up_Guai[2];               //四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列
int Search_Stop_Line=0;
int  Left_Up_Find=0,
    Right_Up_Find=0;   
int  Right_Down_Find=0,
    Left_Down_Find=0;
int sart_line=20,end_line=140;
int stop_line=0;
int long_way=0;



// 线程函数
void timer_thread()
{
    Gyro_Init();
    while(true)
    {
        get_servo_gyro();
    }
}

// void uart_thread()
// {
//     while(true)
//     {
//         // 添加自定义逻辑
//         // Uart_Recv();
        
//         Vofa_printf();
//         //printf("\n\n\n\n\n\n\n");
//         // 等待 5ms
//         this_thread::sleep_for(chrono::milliseconds(50));
//     }
// }

void speed_thread()
{   
    
    while(true)
    {
        Speed_control();
    }
    
}

void nega_thread()
{
    while(true)
    {
        Key_open();
    }
}


void handler(int sig)
{
    handler_();
}

void crash_handler(int sig)
{
    crash_handler_();
}

void seg_handler(int sig)
{
    seg_handler();
}

int main()
{
    signal(SIGINT, handler);
    signal(SIGFPE, crash_handler);
    signal(SIGSEGV, seg_handler);
    
    // 创建线程 
    thread t(timer_thread);
    // thread u(uart_thread);
    thread nega(nega_thread);
    thread p(speed_thread);

    speed_select();

    beep.SetGpioValue(0);   // 关闭蜂鸣器

    Camerinit();

  
    sleep(1);
    while(1)
    {
        // auto start = std::chrono::high_resolution_clock::now();
        // if(gpio44.GetGpioValue()==0)
        // {
        //     
        //     usleep(20000);
        //     gpio44.SetGpioValue(1);
        // }
        // if(gpio45.GetGpioValue()==0)
        // {
        //     
        //     usleep(20000);
        //     gpio45.SetGpioValue(1);
        // }

       
        Cameratest();

        

        // cout << "L : " << Lupdate <<endl;
        // cout << "R : " << Rupdate <<endl;
        // // LMotorPWM.SetDutyCycle(Lupdate *290);
        // // RMotorPWM.SetDutyCycle(Rupdate *290);
        // cout << "kp :" << pid.Kp << "   kd: " << pid.Kd <<endl;
        // cout <<" angle :" << pid.update(avg_error) << endl;
        // cout << "Kp  = " << Rmotor.Kp << endl;
        // cout << "left = " << EnL << ", right = " << EnR << endl;
        // cout<< "error"  << avg_error<<endl;

        // auto end = std::chrono::high_resolution_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        // std::cout << "执行时间: " << duration.count() << "微秒"<<endl;
    }
    
}