// S-U400舵机  周期20ms  驱动脉冲范围900us～2100us

#ifndef __SERVO_H
#define __SERVO_H

#include <stdio.h>
#include <iostream>
#include "LQ_GTIM_PWM.hpp"

#define Mid_angle 159300
#define Mid_line 80

void servo_angle(int angle_, GtimPwm &servo);

class servo_pd 
{
public:
    float Kp;
    float Kd;
    float Kp2;

    float target;
    float err;
    float err_last;

    float outmax;
    float outmin;
public:
    servo_pd(float p, float d, float kp2, float target_, float outmax_, float outmin_);
    void set_target(float target_);
    float update(float feedback);
    void set_pd(float p, float d, float kp2);
    void reset();
};


#endif
