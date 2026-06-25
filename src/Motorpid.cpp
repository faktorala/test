#include "main.hpp"
#include "Motorpid.hpp"


Motorpid::Motorpid(float p, float i, float target_,float outmax_, float outmin_)
:Kp(p), Ki(i), target(target_), outmax(outmax_), outmin(outmin_), err(0), err_last(0), sum(0)
{};

void Motorpid::set_target(float tar)
{
    this->target = tar;   
}

int Motorpid::update(float tar, float feedback)
{
    this->target = tar;
    this->err = this->target - feedback;
    this->sum += this->Kp * (this->err - this->err_last) + this->Ki * (this->err);
    if(this->sum > this->outmax)  this->sum = this->outmax;
    if(this->sum < this->outmin)  this->sum = this->outmin;
    this->err_last = this->err;
    return (int)sum;
}

void Motorpid::reset()
{
    this->err = 0;
    this->err_last = 0;
}

void motor_left(int duty)
{
    if(duty > 45000)    duty = 45000;
    if(duty < -45000)   duty = -45000;

    if(duty >= 0){
        LMotoDir.SetGpioValue(GPIO_Mode_Out);
        LMotorPWM.SetDutyCycle(duty);
    }
    if(duty < 0){
        LMotoDir.SetGpioValue(GPIO_Mode_In);
        LMotorPWM.SetDutyCycle(-duty);
    }
}

void motor_right(int duty)
{
    if(duty > 45000)    duty = 45000;
    if(duty < -45000)   duty = -45000;

    if(duty >= 0){
        RMotoDir.SetGpioValue(GPIO_Mode_Out);
        RMotorPWM.SetDutyCycle(duty);
    }
    if(duty < 0){
        RMotoDir.SetGpioValue(GPIO_Mode_In);
        RMotorPWM.SetDutyCycle(-duty);
    }
}