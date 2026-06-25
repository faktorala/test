#ifndef _IMCU_H__
#define _IMCU_H__

//宏定义区域
//#define PI 3.1415926f
#define squa(Sq) (((float)Sq) * ((float)Sq))
#define absu16(Math_X) ((Math_X) < 0 ? -(Math_X) : (Math_X))
#define absFloat(Math_X) ((Math_X) < 0 ? -(Math_X) : (Math_X))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) > 0 ? (x) : -(x))
#define LIMIT(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#define DT              (0.001)             //Î¢·ÖÊ±¼ä£¬Ò»°ãÈ¡0.005
#define   GYRO_SENS             1/4.5
#define   ACCE_SENS             90.0/4096

void Zero_Point_Detect(void);
void Gyroscope_GetData(void);
void Get_Gyroscope_Angle(void);




#endif