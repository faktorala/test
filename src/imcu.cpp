#include "main.hpp"
int ZeroDrift_gyro_y=0;
int ZeroDrift_gyro_z=0;
                    //陀螺仪数据
float AngleSpeed = 0;
 float FJ_LastAngleSpeed = 0.0f,FJ_AngleSpeed = 0.0f;
 float FJ_Angle_Max =  720.0f;
 float FJ_Angle_Min = -720.0f;
int16_t gyrox,gyroy,gyroz;
float FJ_Angle = 0;//最后引出的变量
void Zero_Point_Detect(void)
{
    uint8_t i;
    int zero_point_y_accu=0;
    int zero_point_z_accu=0;
    for(i=0;i<=100;i++)//积累100次，求取平均值，获取当前零飘
    {
        MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
        zero_point_y_accu+=gyroy;//
        zero_point_z_accu+=gyroz;
        usleep(10000);
    }
    ZeroDrift_gyro_y=zero_point_y_accu/100.0;
    ZeroDrift_gyro_z=zero_point_z_accu/100.0;
}
void Gyroscope_GetData(void)
{
    
    uint16_t gyro_zzz =0;
    MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
    

    // printf("gyro_zzz   %d\n",gyroz);
    AngleSpeed = ((gyroz-ZeroDrift_gyro_z) * GYRO_SENS)*DT; 
    // printf("AngleSpeed  %f\n",AngleSpeed);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      陀螺仪计算航偏角角度
//  @param
//  @return     void
//-------------------------------------------------------------------------------------------------------------------
void Get_Gyroscope_Angle(void)
{
    float K=0.1;
    FJ_LastAngleSpeed=FJ_AngleSpeed;
    FJ_AngleSpeed += ((gyroz-ZeroDrift_gyro_z) *GYRO_SENS)*DT;
    FJ_Angle = FJ_AngleSpeed*K+FJ_LastAngleSpeed*(1-K);                //向左为正
    FJ_Angle = FJ_Angle > FJ_Angle_Max ? FJ_Angle_Max : FJ_Angle;
    FJ_Angle = FJ_Angle < FJ_Angle_Min ? FJ_Angle_Min : FJ_Angle;
    FJ_Angle = -FJ_Angle;
   
   
    // printf("FJ_LastAngleSpeed   %f\n",FJ_LastAngleSpeed);
    // printf("FJ_AngleSpeed   %f\n",FJ_AngleSpeed);
    // printf("FJ_Angle_Max    %f\n FJ_Angle_Min  %d\n",FJ_Angle_Max,FJ_Angle_Min);
    // printf("FJ_Angle    %f\n",FJ_Angle);
}

