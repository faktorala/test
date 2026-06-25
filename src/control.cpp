#include "main.hpp"
#include <thread>
#include "control.hpp"


#define     Encoder        75

float   Exdef_speed = 0,                 //普通差速
        Indef_speed = 0;

float   island_chasu = 0;                   //圆环差速

int   Lupdate = 0, Rupdate = 0;             //左右编码器pid更新值
 
float EnL = 0, EnR = 0;                     //左右编码器值

uint8_t  insland_err = 0,down = 0;                   //圆环直接补的误差

float M = 0.113,K = 1.47;                    //用于差速调整，若差速效果比预想大可调整k,若转弯慢可调m，反之。

GtimPwm servo(88, 2, LS_GTIM_INVERSED, 333334, Mid_angle);// 舵机
GtimPwm nega_press(89, 3, LS_GTIM_INVERSED, 2000000, 100000);
servo_pd pid(0, 0, 0, 0, 179000, 148500);
HWGpio LMotoDir(74, GPIO_Mode_Out);                 // CAN3_RX  left motor dir   /* 电机方向GPIO */
HWGpio RMotoDir(75, GPIO_Mode_Out);                 // CAN3_TX  right motor dir
LS_PwmEncoder EncodersL(0, 73);
LS_PwmEncoder EncodersR(3, 72);
SetPWM LMotorPWM(PWM1, 100000, 0, "inversed");  // TIM2_CH1 GPIO87   /* 电机PWM */
SetPWM RMotorPWM(PWM2, 100000, 0, "inversed");  // TIM2_CH2 GPIO88   
Motorpid Lmotor(4.1, 0.33, Encoder, 150, -100);
Motorpid Rmotor(3.9, 0.33, Encoder, 150, -100);
SetGPIO gpio45(45, "in"); 
SetGPIO gpio44(44, "in");                                      //key_45
HWGpio beep(61, GPIO_Mode_Out);  



/*-------------------------------------------------------------------------------------------------------------------
@brief          陀螺仪初始化
@param          null
@return 
Sample  
@note   
-------------------------------------------------------------------------------------------------------------------*/
void Gyro_Init(void)
{
    IIC_Init();         // IIC初始化
    usleep(100000);
    Gyro_Chose();
    IIC_Gyro_init();    // 陀螺仪初始化
    usleep(100000);
    Zero_Point_Detect();
}


/*-------------------------------------------------------------------------------------------------------------------
@brief          //更新舵机 和 获取gyro
@param  
@return 
Sample  
@note   
-------------------------------------------------------------------------------------------------------------------*/
void get_servo_gyro(void)
{
    servo_angle(pid.update(avg_error),servo);
    Gyroscope_GetData();
    Get_Gyroscope_Angle();
    // 等待 5ms
    this_thread::sleep_for(chrono::milliseconds(5));
}


/*-------------------------------------------------------------------------------------------------------------------
@brief          //速度
@param  
@return 
Sample  
@note   
-------------------------------------------------------------------------------------------------------------------*/
void Speed_control(void)
{
        EnL = EncodersL.Update();
        EnR = (-EncodersR.Update());

        // if(stop_flsh == 1)
        // {
        //     nega_press.Disable();
        //     motor_left(Lmotor.update(0, EnL) * 290);
        //     motor_right(Rmotor.update(0, EnR) * 290);
        //     cout << "overing " <<endl;
        // }
        // else{
            
                if(long_way == 1){
                    // printf("go   way\n");
                    Lupdate = Lmotor.update(Encoder-down-insland_err, EnL);
                    Rupdate = Rmotor.update(Encoder-down-insland_err, EnR);
                    Get_DL(Lupdate);
                    Get_DR(Rupdate);
                }
                else{
                    Lupdate = Lmotor.update(Encoder-down-insland_err, EnL);
                    Rupdate = Rmotor.update(Encoder-down-insland_err, EnR);
                    Get_DL(Lupdate);
                    Get_DR(Rupdate);
                }
          
        // }
        this_thread::sleep_for(chrono::milliseconds(5));

}


/*-------------------------------------------------------------------------------------------------------------------
@brief          //按键开启速度以及负压
@param  
@return 
Sample  
@note   
-------------------------------------------------------------------------------------------------------------------*/
void Key_open(void)
{
    if(gpio45.GetGpioValue()==0){
        usleep(20000);
        if(gpio45.GetGpioValue()==0){
            beep.SetGpioValue(1);
            gpio45.SetGpioValue(1);        
            sleep(2);
            beep.SetGpioValue(0);
            //////
            nega_press.Enable();
            servo.Enable();
            nega_press.SetDutyCycle(180000);
               
            //////
            sleep(3);
            LMotorPWM.Enable();
            RMotorPWM.Enable();

        }
    }
    if(gpio44.GetGpioValue()==0){
        usleep(20000);
        if(gpio44.GetGpioValue()==0){
            beep.SetGpioValue(1);
            gpio44.SetGpioValue(1);
            sleep(1);
            beep.SetGpioValue(0);
            //////
            down += 5;
            //////

        }
    }
    // cout<< "speed  " << Encoder-down-insland_err<<endl;
        this_thread::sleep_for(chrono::milliseconds(500));

}


/*-------------------------------------------------------------------------------------------------------------------
@brief          //ctrl+c  信号处理
@param  
@return 
Sample  
@note   
-------------------------------------------------------------------------------------------------------------------*/
void handler_(void)
{
    cout <<"signal ..."<<endl;
    nega_press.Disable();
    servo.Disable();
    beep.~HWGpio();
    LMotorPWM.Disable();
    RMotorPWM.Disable();
    exit(0);
}

/*-------------------------------------------------------------------------------------------------------------------
@brief          //处理Segmentation fault
@param  
@return 
Sample  
@note   
-------------------------------------------------------------------------------------------------------------------*/
void seg_handler(void)
{
    cout <<"signal ...Segmentation fault"<<endl;
    nega_press.Disable();
    servo.Disable();
    beep.~HWGpio();
    LMotorPWM.Disable();
    RMotorPWM.Disable();
    exit(0);
}


/*-------------------------------------------------------------------------------------------------------------------
@brief          //浮点/图像异常  信号处理
@param  
@return 
Sample  
@note   
-------------------------------------------------------------------------------------------------------------------*/
void crash_handler_(void)
{
    const char msg[] = "stopping ...\n";
    write(STDERR_FILENO, msg, sizeof(msg) - 1);
    nega_press.Disable();
    servo.Disable();
    beep.~HWGpio();
    LMotorPWM.Disable();
    RMotorPWM.Disable();
    exit(0);
}
    
/*-------------------------------------------------------------------------------------------------------------------
@brief          //速度选择
@param          0, 241, 270, 310, 335, 375, 417
@return 
Sample  
@note   
-------------------------------------------------------------------------------------------------------------------*/
void speed_select(void)
{
    if(Encoder <= 68){              //70 
        pid.Kp = 228;       //有模糊p，这个作用不大
        pid.Kd = 2025;
        pid.Kp2 = 4.9;
        M = -0.121,K = 0.631;
    }
    else if(Encoder < 75){          //75
        pid.Kp = 228;       //有模糊p，这个作用不 大s
        pid.Kd = 2791;
        pid.Kp2 = 4.5;
        M = -0.1,K = 1.25;   
    }else if(Encoder<=78)           //80
    {
        pid.Kp = 228;       //有模糊p，这个作用不大
        pid.Kd = 2578;
        pid.Kp2 = 4.57;
        M = -0.19,K = 1.24;  
        // pid.Kp = 228;       //有模糊p，这个作用不大s
        // pid.Kd = 2500;
        // pid.Kp2 = 5.3;
        // M = -0.107,K = 0.7;
    }else if(Encoder <= 85){        //80
        pid.Kp = 228;       //有模糊p，这个作用不大
        pid.Kd = 2000;
        pid.Kp2 = 5;     
        M = -0.161,K = 0.605;
    }

}


void Get_DR(int v)
{
    float g_control = pid.update(avg_error) - 154000;
    float tn = tan((g_control/350)*3.141592/180);

    if(tn < -0.55) tn=-0.55;

    if(g_control <= 0){
        int vright = v*(1+M+0.15*tn/0.2*K);
        motor_right(vright*290);
        int vleft = v*(1+M-0.15*tn/0.2*K);
        motor_left(vleft*290);
    }
    else motor_right(v*290);
}

void Get_DL(int v)
{
    float g_control = pid.update(avg_error) - 154000;
    float tn = tan((g_control/350)*3.141592/180);

    if(tn > 0.55) tn=0.55;

    if(g_control >= 0){             //左转g_control > 0
        int vleft = v*(1+M-0.15*tn/0.2*K);
        motor_left(vleft*290);
        int vright = v*(1+M+0.15*tn/0.2*K);
        motor_right(vright*290);
        
    }
    else motor_left(v*290);
}

