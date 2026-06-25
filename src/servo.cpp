// S-U400舵机  周期20ms  驱动脉冲范围900us～2100us
//  333334 ; left_edge: 900us -> 90,000  ; tight_edge 2100us -> 210,000
// LQ_GTIM_PWM      mid -> 154k
// left 130k  ;  right 180k;

#include "servo.h"
#include "pwm_gtim.h"
#include "LQ_GTIM_PWM.hpp"
#include "main.hpp"

void servo_angle(int angle_, GtimPwm &servo)
{
    int angle = angle_;

    if (angle < 130000)
        angle = 130000;
    else if (angle > 180000)
        angle = 180000;

    servo.SetDutyCycle(angle);
}

servo_pd :: servo_pd(float p, float d, float kp2, float target_, float outmax_, float outmin_)
: Kp(p), Kd(d), Kp2(kp2), target(target_), outmin(outmin_), outmax(outmax_)
{};

void servo_pd::set_target(float target_)
{
    this->target = target_;
}


float servo_pd::update(float feedback)
{
    float u;
    this->err = feedback;
    float ek1 = this->err - this->err_last;
    this->Kp = Fuzzy_P((int)this->err, (int)ek1);   //模糊P
    u = Mid_angle + this->err*Kp + fabs(this->err)*this->err*Kp2 + ek1*Kd  ;
    // cout << "E      "<< this->err << endl;
    // cout << "Ek1    "<< ek1 << endl;
    // cout << "Kp     "<< this->Kp <<endl;
    this->err_last = this->err;
    if(u > this->outmax)
        u = this->outmax;
    else if(u < this->outmin)
        u = this->outmin;

    return (int)(u);
}

void servo_pd::reset()
{
    this->err = 0;
    this->err_last = 0;
}

void servo_pd::set_pd(float p, float d, float kp2)
{
    this->Kp = p;
    this->Kd = d;
    this->Kp2 = kp2;
}