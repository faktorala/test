#pragma once

#include "main.hpp"

class Motorpid 
{
public:
    float Kp;
    float Ki;

    float target;
    float err;
    float err_last;

    float outmax;
    float outmin;
    float sum;

    Motorpid(float p, float i, float target_, float outmax_, float outmin_);
    void set_target(float target_);
    int update(float tar, float feedback);
    void reset();
};

void motor_left(int duty);
void motor_right(int duty);