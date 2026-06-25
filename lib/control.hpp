#pragma once

#include "main.hpp"
// #include "LQ_GPIO.hpp"

#define BORDWIDTH  0.15     //轮距
#define LDISTANCE  0.2      //轴距


void Gyro_Init(void);
void get_servo_gyro(void);
void Speed_control(void);
void Key_open(void);
void handler_(void);
void crash_handler_(void);
void seg_handler(void);
void speed_select(void);
void Get_DR(int v);
void Get_DL(int v);

extern GtimPwm servo;
extern GtimPwm nega_press;
extern servo_pd pid;
extern HWGpio LMotoDir;
extern HWGpio RMotoDir;
extern LS_PwmEncoder EncodersL;
extern LS_PwmEncoder EncodersR;
extern SetPWM LMotorPWM;
extern SetPWM RMotorPWM;
extern Motorpid Lmotor;
extern Motorpid Rmotor;
extern HWGpio beep;