#include "main.hpp"

const uint8_t Weight[120] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

        1, 1, 1, 1, 1, 9,11,15,23,25,
        20,15,11, 8, 5, 5, 2, 2, 1, 1,

        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       
};

VideoCapture cap(0);

void Camerinit()
{

    if (!cap.isOpened())
    {
        cerr << "Error open video stream" << endl;
        LMotorPWM.Disable();
        RMotorPWM.Disable();
        return;
    }
    // 设置视频流编码器
    cap.set(cv::CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
    // 设置摄像头图像宽高和帧率
    cap.set(CAP_PROP_FRAME_WIDTH, 160);
    cap.set(CAP_PROP_FRAME_HEIGHT, 120);
    cap.set(CAP_PROP_FPS, 120);
    // 获取摄像头图像宽高和帧率
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double frame_fps = cap.get(cv::CAP_PROP_FPS);
    printf("frame:%d*%d, fps:%3f", frame_width, frame_height, frame_fps);

    // 获取视频流
}

/*-------------------------------------------------------------------------------------------------------------------
@brief     找下面的两个拐点，供十字使用
@param     搜索的范围起点，终点
@return    修改两个全局变量
         Right_Down_Find=0;
         Left_Down_Find=0;
Sample     Find_Down_Point(int start,int end)
@note      运行完之后查看对应的变量，注意，没找到时对应变量将是0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Down_Point(int start, int end)
{
    int i, t;
    Right_Down_Find = 0;
    Left_Down_Find = 0;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (start >= 120 - 1 - 5) // 下面5行数据不稳定，不能作为边界点来判断，舍弃
        start = 120 - 1 - 5;
    if (end <= 120 - Search_Stop_Line)
        end = 120 - Search_Stop_Line;
    if (end <= 5)
        end = 5;
    for (i = start; i >= end; i--)
    {
        if (Left_Down_Find == 0 &&             // 只找第一个符合条件的点a
            abs(Left[i] - Left[i + 1]) <= 5 && // 角点的阈值可以更改
            abs(Left[i + 1] - Left[i + 2]) <= 5 &&
            abs(Left[i + 2] - Left[i + 3]) <= 5 &&
            (Left[i] - Left[i - 3]) >= 5 &&
            (Left[i] - Left[i - 4]) >= 8 &&
            (Left[i] - Left[i - 5]) >= 8)
        {
            Left_Down_Find = i; // 获取行数即可
        }
        if (Right_Down_Find == 0 &&              // 只找第一个符合条件的点
            abs(Right[i] - Right[i + 1]) <= 3 && // 角点的阈值可以更改
            abs(Right[i + 1] - Right[i + 2]) <= 3 &&
            abs(Right[i + 2] - Right[i + 3]) <= 3 &&
            (Right[i] - Right[i - 3]) <= -5 &&
            (Right[i] - Right[i - 4]) <= -5 &&
            (Right[i] - Right[i - 5]) <= -5)
        {
            Right_Down_Find = i;
        }
        if (Left_Down_Find != 0 && Right_Down_Find != 0) // 两个找到就退出
        {
            break;
        }
    }
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     找上面的两个拐点，供十字使用
  @param     搜索的范围起点，终点
  @return    修改两个全局变量
             Left_Up_Find=0;
             Right_Up_Find=0;
  Sample     Find_Up_Point(int start,int end)
  @note      运行完之后查看对应的变量，注意，没找到时对应变量将是0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Up_Point(int start, int end)
{
    int i, t;
    Left_Up_Find = 0;
    Right_Up_Find = 0;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (end <= 120 - Search_Stop_Line)
        end = 120 - Search_Stop_Line;
    if (end <= 5) // 及时最长白列非常长，也要舍弃部分点，防止数组越界
        end = 5;
    if (start >= 120 - 1 - 5) // 下面5行数据不稳定，不能作为边界点来判断，舍弃
        start = 120 - 1 - 5;
    for (i = end; i <= start; i++)
    {
        if (Left_Up_Find == 0 &&               // 只找第一个符合条件的点
            abs(Left[i] - Left[i - 1]) <= 5 && // 角点的阈值可以更改
            abs(Left[i - 1] - Left[i - 2]) <= 5 &&
            abs(Left[i - 2] - Left[i - 3]) <= 5 &&
            (Left[i] - Left[i + 2]) >= 8 &&
            (Left[i] - Left[i + 3]) >= 10 &&
            (Left[i] - Left[i + 4]) >= 10)
        {
            Left_Up_Find = i; // 获取行数即可
        }
        if (Right_Up_Find == 0 &&                // 只找第一个符合条件的点
            abs(Right[i] - Right[i - 1]) <= 5 && // 角点的阈值可以更改
            abs(Right[i - 1] - Right[i - 2]) <= 5 &&
            abs(Right[i - 2] - Right[i - 3]) <= 5 &&
            (Right[i] - Right[i + 2]) <= -8 &&
            (Right[i] - Right[i + 3]) <= -10 &&
            (Right[i] - Right[i + 4]) <= -10)
        {
            Right_Up_Find = i; // 获取行数即可
        }
        if (Left_Up_Find != 0 && Right_Up_Find != 0) // 下面两个找到就出去
        {
            break;
        }
    }
    // if(abs(Right_Up_Find-Left_Up_Find)>=30)//纵向撕裂过大，视为误判
    // {
    //     Right_Up_Find=0;
    //     Left_Up_Find=0;
    // }
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     十字检测
  @param     null
  @return    null
  Sample     Cross_Detect(void);
  @note      利用四个拐点判别函数，查找四个角点，根据找到拐点的个数决定是否补线
-------------------------------------------------------------------------------------------------------------------*/
int cross_low=0;
void Cross_Detect()
{
    int down_search_start = 0; // 下点搜索开始行
    cross_flag = 0;
    // printf("left    %d   right   %d\n",Left_Lost_Time,Right_Lost_Time);
    // printf("    (Both_Lost_Time>=10)    %d\n",Both_Lost_Time);
    if (Island_State == 0) // 与环岛互斥开
    {

        Left_Up_Find = 0;
        Right_Up_Find = 0;
        if (Right_Lost_Time>=15&&Left_Lost_Time>=15&&Both_Lost_Time>=10) // 十字必定有双边丢线，在有双边丢线的情况下再开始找角点
        {
           
             
            Find_Up_Point(106, 120 - Search_Stop_Line + 1);
            //   printf("\nleftuo:%d\n",Left_Up_Find);
            //      printf("\nrightuo:%d\n",Right_Up_Find);
            if (Left_Up_Find == 0 && Right_Up_Find == 0) // 只要没有同时找到两个上点，直接结束
            {
                cross_flag = 0;
                return;
            }
        }
        // printf("\ncross:%d\n",cross_flag);

        if (Left_Up_Find != 0 && Right_Up_Find != 0) // 找到两个上点，就找到十字了
        {

            cross_flag=1;
           
            down_search_start = Left_Up_Find > Right_Up_Find ? Left_Up_Find : Right_Up_Find; // 用两个上拐点坐标靠下者作为下点的搜索上限
            Find_Down_Point(120 - 5, down_search_start + 2);                                 // 在上拐点下2行作为下点的截止行
            if (Left_Down_Find <= Left_Up_Find)
            {
                Left_Down_Find = 0; // 下点不可能比上点还靠上
            }
            if (Right_Down_Find <= Right_Up_Find)
            {
                Right_Down_Find = 0; // 下点不可能比上点还靠上
            }
            if (Left_Down_Find != 0 && Right_Down_Find != 0)
            { // 四个点都在，无脑连线，这种情况显然很少
                Point pt1(Left[Left_Up_Find], Left_Up_Find + 1),
                    pt2(Left[Left_Down_Find], Left_Down_Find);
                cv::line(binary, pt1, pt2, Scalar(0, 0, 0), 2);
                Point pt3(Right[Right_Up_Find], Right_Up_Find),
                    pt4(Right[Right_Down_Find], Right_Down_Find);
                cv::line(binary, pt3, pt4, Scalar(0, 0, 0), 2); 
                printf("in    cross\n");
                  Longest_White_Column(); // 改正坐标
            }
            else if (Left_Down_Find == 0 && Right_Down_Find != 0) // 11//这里使用的都是斜率补线
            {                                                     // 三个点                                     //01
                Lengthen_Left_Boundry(Left_Up_Find - 1, 120 - 1);
                Point pt3(Right[Right_Up_Find], Right_Up_Find),
                    pt4(Right[Right_Down_Find], Right_Down_Find);
                cv::line(binary, pt3, pt4, Scalar(0, 0, 0), 2);
                  Longest_White_Column(); // 改正坐标
            }
            else if (Left_Down_Find != 0 && Right_Down_Find == 0) // 11
            {                                                     // 三个点                                     //10
                Point pt1(Left[Left_Up_Find], Left_Up_Find),
                    pt2(Left[Left_Down_Find], Left_Down_Find);
                cv::line(binary, pt1, pt2, Scalar(0, 0, 0), 2);
                Lengthen_Right_Boundry(Right_Up_Find - 1, 120 - 1);
                  Longest_White_Column(); // 改正坐标
            }
            else if (Left_Down_Find == 0 && Right_Down_Find == 0) // 11
            {                            
                            cross_low=1;          // 就俩上点                                   //00
                Lengthen_Left_Boundry(Left_Up_Find - 1, 120 - 1);
                Lengthen_Right_Boundry(Right_Up_Find - 1, 120 - 1);
                  Longest_White_Column(); // 改正坐标
            }
          
        }
        // else if(Left_Up_Find==0&&Right_Up_Find!=0)
        // {   cross_flag=1;
        //     Point ritli(Right[Right_Up_Find],Right_Up_Find),
        //     ritli2(Right[119],119);
        //                  cv::  line (binary,ritli,ritli2,Scalar(0,0,0),2);

        // }
        // else if(Left_Up_Find!=0&&Right_Up_Find==0)
        //  {  cross_flag=1;
        //       Point pta1(Left[Left_Up_Find],Left_Up_Find+1),
        //           pta2(Left[119],119);
        //   cv:: line (binary,pta1,pta2,Scalar(0,0,0),2);
        // }
    }
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     左边界延长
  @param     延长起始行数，延长到某行
  @return    null
  Sample     Stop_Detect(void)
  @note      从起始点向上找5个点，算出斜率，向下延长，直至结束点
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Left_Boundry(int start, int end)
{
    int i, t;
    float k = 0;
    if (start >= 120 - 1) // 起始点位置校正，排除数组越界的可能
        start = 120 - 1;
    else if (start <= 0)
        start = 0;
    if (end >= 120 - 1)
        end = 120 - 1;
    else if (end <= 0)
        end = 0;
    if (end < start) //++访问，坐标互换
    {
        t = end;
        end = start;
        start = t;
    }

    if (start <= 5) // 因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Point line1(Left[start], start);
        Point line2(Left[end], end);
        cv::line(binary, line1, line2, Scalar(0, 0, 0), 2);
    }

    else
    {
        k = (float)(Left[start - 1] - Left[start - 5]) / 5.0; // 这里的k是1/斜率
        for (i = start; i <= end; i++)
        {
            Left[i] = (int)(i - start) / k + Left[start]; //(x=(y-y1)*k+x1),点斜式变形
            if (Left[i] >= 160 - 1)
            {
                Left[i] = 159;
            }
            else if (Left[i] <= 0)
            {
                Left[i] = 0;
            }
            line(binary, Point(Left[i], i), Point(Left[i], i), Scalar(0, 0, 0), 2);
            // line(binary,Point(Left[i],i),Point(Left[i],i),Scalar(0,0,0),2);
        }
        // line(binary,Point(),Point(Left[i],i),Scalar(0,0,0),2);
    }
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     右左边界延长
  @param     延长起始行数，延长到某行
  @return    null
  Sample     Stop_Detect(void)
  @note      从起始点向上找3个点，算出斜率，向下延长，直至结束点
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Right_Boundry(int start, int end)
{
    int i, t;
    float k = 0;
    if (start >= 120 - 1) // 起始点位置校正，排除数组越界的可能
        start = 120 - 1;
    else if (start <= 0)
        start = 0;
    if (end >= 120 - 1)
        end = 120 - 1;
    else if (end <= 0)
        end = 0;
    if (end < start) //++访问，坐标互换
    {
        t = end;
        end = start;
        start = t;
    }

    if (start <= 5) // 因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Point line3(Right[start], start);
        Point line4(Right[end], end);
        cv::line(binary, line3, line4, Scalar(0, 0, 0), 2);
    }
    else
    {
        k = (float)(Right[start-1] - Right[start - 5]) / 5.0; // 这里的k是1/斜率
        for (i = start; i <= end; i++)
        {
            Right[i] = (int)((i - start) / (k+0.2) + Right[start]); //(x=(y-y1)*k+x1),点斜式变形
            if (Right[i] >= 160 - 1)
            {
                Right[i] = 160 - 1;
            }
            else if (Right[i] <= 0)
            {
                Right[i] = 0;
            }
            line(binary, Point(Right[i], i), Point(Right[i], i), Scalar(0, 0, 0), 2);
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右上角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Up_Point(int start, int end) // 找四个角点，返回值是角点所在的行数
{
    int i, t;
    int right_up_line = 0;
    if (Right_Lost_Time >= 0.9 * 120) // 大部分都丢线，没有拐点判断的意义
        return right_up_line;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (end <= 120 - Search_Stop_Line) // 搜索截止行往上的全都不判
        end = 120 - Search_Stop_Line;
    if (end <= 5) // 及时最长白列非常长，也要舍弃部分点，防止数组越界
        end = 5;
    if (start >= 120 - 1 - 5)
        start = 120 - 1 - 5;
    for (i = start; i >= end; i--)
    {
        if (right_up_line == 0 &&                // 只找第一个符合条件的点
            abs(Right[i] - Right[i - 1]) <= 8 && // 下面两行位置差不多
            abs(Right[i - 1] - Right[i - 2]) <= 8 &&
            (Right[i] - Right[i + 3]) <= -15 &&
            (Right[i] - Right[i + 4]) <= -15)
        {
            right_up_line = i; // 获取行数即可
            break;
        }
    }
    return right_up_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     左赛道连续性检测
  @param     起始点，终止点
  @return    连续返回0，不连续返回断线出行数
  Sample     Continuity_Change_Left(int start,int end);
  @note      连续性的阈值设置为5，可更改
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Left(int start, int end) // 连续性阈值设置为5
{
    int i;
    int t;
    int continuity_change_flag = 0;
    if (Left_Lost_Time >= 0.9 * 120) // 大部分都丢线，没必要判断了
        return 1;
    if (Search_Stop_Line <= 5) // 搜所截止行很矮
        return 1;
    if (start >= 120 - 1 - 5) // 数组越界保护
        start = 120 - 1 - 5;
    if (end <= 5)
        end = 5;
    if (start < end) // 都是从下往上计算的，反了就互换一下
    {
        t = start;
        start = end;
        end = t;
    }

    for (i = start; i >= end; i--)
    {
        if (abs(Left[i] - Left[i - 1]) >= 5) // 连续判断阈值是5,可更改
        {
            continuity_change_flag = i;
            break;
        }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右赛道连续性检测
  @param     起始点，终止点
  @return    连续返回0，不连续返回断线出行数
  Sample     continuity_change_flag=Continuity_Change_Right(int start,int end)
  @note      连续性的阈值设置为5，可更改
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Right(int start, int end)
{
    int i;
    int t;
    int continuity_change_flag = 0;
    if (Right_Lost_Time >= 0.9 * 120) // 大部分都丢线，没必要判断了
        return 1;
    if (start >= 120 - 5) // 数组越界保护
        start = 120 - 5;
    if (end <= 5)
        end = 5;
    if (start < end) // 都是从下往上计算的，反了就互换一下
    {
        t = start;
        start = end;
        end = t;
    }

    for (i = start; i >= end; i--)
    {
        if (abs(Right[i] - Right[i - 1]) >= 5) // 连续性阈值是5，可更改
        {
            continuity_change_flag = i;
            break;
        }
    }
    return continuity_change_flag;
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     单调性突变检测
  @param     起始点，终止行
  @return    点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      前5后5它最大（最小），那他就是角点
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Right(int start, int end) // 单调性改变，返回值是单调性改变点所在的行数
{
    int i;
    int monotonicity_change_line = 0;

    if (Right_Lost_Time >= 0.9 * 120) // 大部分都丢线，没有单调性判断的意义
        return monotonicity_change_line;
    if (start >= 120 - 1 - 5) // 数组越界保护
        start = 120 - 1 - 5;
    if (end <= 5)
        end = 5;
    if (start <= end)
        return monotonicity_change_line;
    for (i = start; i >= end; i--) // 会读取前5后5数据，所以前面对输入范围有要求
    {
        if (Right[i] == Right[i + 5] && Right[i] == Right[i - 5] &&
            Right[i] == Right[i + 4] && Right[i] == Right[i - 4] &&
            Right[i] == Right[i + 3] && Right[i] == Right[i - 3] &&
            Right[i] == Right[i + 2] && Right[i] == Right[i - 2] &&
            Right[i] == Right[i + 1] && Right[i] == Right[i - 1])
        { // 一堆数据一样，显然不能作为单调转折点
            continue;
        }
        else if (Right[i] <  Right[i + 5] &&Right[i]  <  Right[i - 5] &&
                 Right[i] < Right[i + 4] && Right[i]  < Right[i - 3] &&
                 Right[i] <= Right[i + 3] && Right[i] <= Right[i - 3] &&
                 Right[i] <= Right[i + 2] && Right[i] <= Right[i - 2] &&
                 Right[i] <= Right[i + 1] && Right[i] <= Right[i - 1])
        { // 就很暴力，这个数据是在前5，后5中最大的，那就是单调突变点
            monotonicity_change_line = i;
            break;
        }
    }
    return monotonicity_change_line;
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     单调性突变检测
  @param     起始点，终止行
  @return    点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      前5后5它最大（最小），那他就是角点
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Left(int start, int end) // 单调性改变，返回值是单调性改变点所在的行数
{
    int i;
    int monotonicity_change_line = 0;
    if (Left_Lost_Time >= 0.9 * 120) // 大部分都丢线，没有单调性判断的意义
        return monotonicity_change_line;
    if (start >= 120 - 1 - 5) // 数组越界保护，在判断第i个点时
        start = 120 - 1 - 5;  // 要访问它前后5个点，数组两头的点要不能作为起点终点
    if (end <= 5)
        end = 5;
    if (start <= end) // 递减计算，入口反了，直接返回0
        return monotonicity_change_line;
    for (i = start; i >= end; i--) // 会读取前5后5数据，所以前面对输入范围有要求
    {
        if (Left[i] == Left[i + 5] && Left[i] == Left[i - 5] &&
            Left[i] == Left[i + 4] && Left[i] == Left[i - 4] &&
            Left[i] == Left[i + 3] && Left[i] == Left[i - 3] &&
            Left[i] == Left[i + 2] && Left[i] == Left[i - 2] &&
            Left[i] == Left[i + 1] && Left[i] == Left[i - 1])
        { // 一堆数据一样，显然不能作为单调转折点
            continue;
        }
        else if (
                 Left[i] > Left[i + 4] && Left[i] > Left[i - 4] &&
                 Left[i] > Left[i + 3] && Left[i] > Left[i - 3] &&
                 Left[i] >= Left[i + 2] && Left[i] >= Left[i - 2] &&
                 Left[i] >= Left[i + 1] && Left[i + 1] >= Left[i - 1] && 
                Left[i]>=30)
        { // 就很暴力，这个数据是在前5，后5中最大的（可以取等），那就是单调突变点
            monotonicity_change_line = i;
            break;
        }
    }
    return monotonicity_change_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右下角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Right_Down_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Down_Point(int start, int end) // 找四个角点，返回值是角点所在的行数
{
    int i, t;
    int right_down_line = 0;
    if (Right_Lost_Time >= 0.9 * 120) // 大部分都丢线，没有拐点判断的意义
        return right_down_line;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (start >= 120 - 1 - 5) // 下面5行数据不稳定，不能作为边界点来判断，舍弃
        start = 120 - 1 - 5;
    if (end <= 120 - Search_Stop_Line)
        end = 120 - Search_Stop_Line;
    if (end <= 5)
        end = 5;
    for (i = start; i >= end; i--)
    {
        if (right_down_line == 0 &&              // 只找第一个符合条件的点
            abs(Right[i] - Right[i + 1]) <= 5 && // 角点的阈值可以更改
            abs(Right[i + 1] - Right[i + 2]) <= 5 &&
            abs(Right[i + 2] - Right[i + 3]) <= 5 &&
            (Right[i] - Right[i - 2]) <= -5 &&
            (Right[i] - Right[i - 3]) <= -5&&
            (Right[i] - Right[i - 4]) <= -5)
        {
            right_down_line = i; // 获取行数即可
            break;
        }
    }
    return right_down_line;
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     左下角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Left_Down_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Down_Point(int start, int end) // 找四个角点，返回值是角点所在的行数
{
    int i, t;
    int left_down_line = 0;
    if (Left_Lost_Time >= 0.9 * 120) // 大部分都丢线，没有拐点判断的意义
        return left_down_line;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (start >= 120 - 1 - 5) // 下面5行数据不稳定，不能作为边界点来判断，舍弃
        start = 120 - 1 - 5;
    if (end <= 120 - Search_Stop_Line)
        end = 120 - Search_Stop_Line;
    if (end <= 5)
        end = 5;
    for (i = start; i >= end; i--)
    {
        if (left_down_line == 0 &&             // 只找第一个符合条件的点
            abs(Left[i] - Left[i + 1]) <= 5 && // 角点的阈值可以更改
            abs(Left[i + 1] - Left[i + 2]) <= 5 &&
            abs(Left[i + 2] - Left[i + 3]) <= 5 &&
            (Left[i] - Left[i - 2]) >= 5 &&
            (Left[i] - Left[i - 3]) >= 5 &&
            (Left[i] - Left[i - 4]) >= 5)
        {
            left_down_line = i; // 获取行数即可
            break;
        }
    }
    return left_down_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     根据斜率划线
  @param     输入斜率，定点，画一条黑线
  @return    null
  Sample     K_Draw_Line(k, 20,MT9V03X_H-1 ,0)
  @note      补的就是一条线，需要重新扫线
-------------------------------------------------------------------------------------------------------------------*/
void K_Draw_Line(float k, int startX, int startY, int endY)
{
    int endX = 0;

    if (startX >= 160 - 1) // 限幅处理
        startX = 160 - 1;
    else if (startX <= 0)
        startX = 0;
    if (startY >= 120 - 1)
        startY = 120 - 1;
    else if (startY <= 0)
        startY = 0;
    if (endY >= 120 - 1)
        endY = 120 - 1;
    else if (endY <= 0)
        endY = 0;
    endX = (int)((endY - startY) * k + startX); //(y-y1)=k(x-x1)变形，x=(y-y1)/k+x1
    Point lll(startX, startY),
        lll1(endX, endY);
    cv::line(binary, lll, lll1, Scalar(0, 0, 0), 2);
}
void Find_In_Three(void)
{
    Lost_start_Left=0;
    Lost_start_Right=0;
     for(int i=60;i>=10;i--)
    {
        // printf(" %d    %d\n",i,binary.at<uchar>(i, 159));
        if(binary.at<uchar>(i, 159)!=0 && binary.at<uchar>(i+1,159)==0)
        {
            Lost_start_Right=i;
            break;
        } 
        // if(binary.at<uchar>(i, 20)!=0&&binary.at<uchar>(i+1, 20)!=0)
        // {
        //     Lost_start_Left=i;
        //     break;
        // }
    }
}
void Left_find_three(void)
{
    Lost_start_Left=0;
    Lost_start_Right=0;
    for(int i=60;i>=10;i++)
    {
  if(binary.at<uchar>(i, 20)!=0&&binary.at<uchar>(i+1, 20)!=0)
        {
            Lost_start_Left=i;
            break;
        }
    }
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     左上角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Left_Up_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Left_Up_Point(int start, int end) // 找四个角点，返回值是角点所在的行数
{
    int i, t;
    int left_up_line = 0;
    if (Left_Lost_Time >= 0.9 * 120) // 大部分都丢线，没有拐点判断的意义
        return left_up_line;
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    if (end <= 120 - Search_Stop_Line) // 搜索截止行往上的全都不判
        end = 120 - Search_Stop_Line;
    if (end <= 5) // 及时最长白列非常长，也要舍弃部分点，防止数组越界
        end = 5;
    if (start >= 120 - 1 - 5)
        start = 120 - 1 - 5;
    for (i = start; i >= end; i--)
    {
        if (left_up_line == 0 && // 只找第一个符合条件的点
            abs(Left[i] - Left[i - 1]) <= 5 &&
            abs(Left[i - 1] - Left[i - 2]) <= 5 &&
            abs(Left[i - 2] - Left[i - 3]) <= 5 &&
            abs(Left[i - 3] - Left[i - 4]) <= 5 &&

            (Left[i] - Left[i + 2]) >= 25 &&
            (Left[i] - Left[i + 3]) >= 25 &&
            (Left[i] - Left[i + 4]) >= 25)
        {
            left_up_line = i; // 获取行数即可
            break;
        }
    }
    return left_up_line; // 如果是，说明没有这么个拐点
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     通过斜率，定点补线
  @param     k       输入斜率
             startY  输入起始点纵坐标
             endY    结束点纵坐标
  @return    null    直接补边线
  Sample    
  @note      补得线直接贴在边线上
-------------------------------------------------------------------------------------------------------------------*/

void Left_K_Add_Boundary(float k)
{

    
    for (int i = 100; i >= 0; i--)
    {
        Left[i] = (int)((i - 100) / k + Left[100]); //(x=(y-y1)*k+x1),点斜式变形
        if (Left[i] >= 160 - 1)
        {
            Left[i] = 159;
        }
        else if (Left[i] <= 0)
        {
            Left[i] = 0;
        }
        line(binary, Point(Left[i], i), Point(Left[i], i), Scalar(0, 0, 0), 3);
    }
    Longest_White_Column(); // 刷新边界数据
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     通过斜率，定点补线
  @param     k       输入斜率
             startY  输入起始点纵坐标
             endY    结束点纵坐标
  @return    null    直接补边线
  Sample    
  @note      补得线直接贴在边线上
-------------------------------------------------------------------------------------------------------------------*/

void Right_K_Add_Boundary(float k)
{

    
    for (int i = 115; i >= 0; i--)
    {
        Right[i] = (int)((115-i) / k + Right[115]); //(x=(y-y1)*k+x1),点斜式变形
        if (Right[i] >= 160 - 1)
        {
            Right[i] = 159;
        }
        else if (Right[i] <= 0)
        {
            Right[i] = 0;
        }
        line(binary, Point(Right[i], i), Point(Right[i], i), Scalar(0, 0, 0), 3);
    }
    Longest_White_Column(); // 刷新边界数据
}

/* 使用示例：
Mat img = Mat::zeros(480, 640, CV_8UC3); // 创建640x480黑色画布
K_Add_Boundary_Right(img, 0.5f, 300, 100, 400);
imshow("Result", img);
waitKey(0);
*/
/*-------------------------------------------------------------------------------------------------------------------
  @brief     环岛检测
  @param     null
  @return    null
  Sample     Island_Detect(void);
  @note      利用四个拐点判别函数，单调性改变函，连续性数撕裂点，分为8步
-------------------------------------------------------------------------------------------------------------------*/

float k = 0;                      // 3和5状态的k
int island_state_5_down[2] = {0}; // 状态5时即将离开环岛，左右边界边最低点，[0]存y，第某行，{1}存x，第某列
int island_state_3_up[2] = {0};   // 状态3时即将进入环岛用，左右上面角点[0]存y，第某行，{1}存x，第某列
int left_down_guai[2] = {0};      // 四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列
int right_down_guai[2] = {0};     // 四个拐点的坐标存储，[0]存y，第某行，{1}存x，第某列

void Island_Detect()
{
    int monotonicity_change_line[2];        // 单调性改变点坐标，[0]寸某行，[1]寸某列
    int monotonicity_change_left_flag = 0;  // 不转折是0
    int monotonicity_change_right_flag = 0; // 不转折是0
    int continuity_change_right_flag = 0;   // 连续是0
    int continuity_change_left_flag = 0;    // 连续是0
    // printf("Boundry_Start_Left   %d\n", Boundry_Start_Left);
    // printf("Boundry_Start_Right   %d\n", Boundry_Start_Right);
    // printf("both lost   %d\n",Both_Lost_Time);
    // 以下是常规判断法
    // continuity_change_left_flag=Continuity_Change_Left(120-1-5,20);//连续性判断
    // continuity_change_right_flag=Continuity_Change_Right(120-1-5,20);
    // monotonicity_change_right_flag=Monotonicity_Change_Right(120-1-10,20);
    // monotonicity_change_left_flag=Monotonicity_Change_Left(120-1-10,20);
    if (cross_flag == 0 && Island_State == 0) //&&Ramp_Flag==0
    {
        continuity_change_left_flag = Continuity_Change_Left(120 - 10 - 10, 120 - Search_Stop_Line + 5); // 连续性判断
        monotonicity_change_left_flag = Monotonicity_Change_Left(120 - 20, 120 - Search_Stop_Line + 5);
        continuity_change_right_flag = Continuity_Change_Right(120 - 10 - 10, 120 - Search_Stop_Line + 5);
        monotonicity_change_right_flag = Monotonicity_Change_Right(120 - 10 - 10, 120 - Search_Stop_Line + 5);
        // printf("Right_Lost_Time  %d\n", Right_Lost_Time);
        // printf("L  lost %d\n",Left_Lost_Time);
        // printf("monotonicity_change_right_flag  %d\n", monotonicity_change_right_flag);
        // printf("continuity_change_left_flag   %d\n", continuity_change_left_flag);
        // printf("continuity_change_right_flag   %d\n", continuity_change_right_flag);
        // printf("monotonicity_change_left_flag   %d\n", monotonicity_change_left_flag);

        // printf("Left_Lost_Time   %d\n", Left_Lost_Time);
        // printf("both   %d\n",Both_Lost_Time);
        if (Left_Island_Flag == 0) // 左环
        {
            if (monotonicity_change_right_flag == 0 && // 右边是单调的
                continuity_change_left_flag <=62  &&    // 左边是不连续的
                continuity_change_left_flag !=0  &&    // 左边是不连续的
                continuity_change_right_flag == 0 &&   // 左环岛右边是连续的
                Left_Lost_Time >= 10 &&                // 左边丢线很多
                // Left_Lost_Time <= 50 &&                // 也不能全丢了
                Right_Lost_Time <= 10 &&               // 右边丢线较少
                Search_Stop_Line >= 102 &&
                Boundry_Start_Left >= 75 && Boundry_Start_Right >= 80 && // 边界起始点靠下
                Both_Lost_Time <= 2)                                                // 双边丢线少
            {

                left_down_guai[0] = Find_Left_Down_Point(115 - 1, 120 - Search_Stop_Line + 5); // 找左下角点
                // printf("leftguai  %d\n", left_down_guai[0]);
                if (left_down_guai[0] >= 35&&left_down_guai[0]>=60) // 条件1很松，在这里判断拐点，位置不对，则是误判，跳出
                {
                    Island_State = 1;
                    Left_Island_Flag = 1;
                                printf("goto11111111\n");

                }
                else // 误判，归零
                {
                    Island_State = 0;
                    Left_Island_Flag = 0;
                }
            }
        }
        if (Right_Island_Flag == 0) // 右环
        {
            if (monotonicity_change_left_flag == 0 &&
                continuity_change_left_flag == 0 &&  // 右环岛左边是连续的
                continuity_change_right_flag <=60 &&  // 右边是不连续的
                 continuity_change_right_flag !=0&&
                Right_Lost_Time >= 5 &&              // 右丢线多
                // Right_Lost_Time <= 70 &&             // 右丢线不能太多
                Left_Lost_Time <= 10 &&              // 左丢线少
                Search_Stop_Line >= 102 &&
                Both_Lost_Time <= 2)
            {

                right_down_guai[0] = Find_Right_Down_Point(100 - 1, 120 - Search_Stop_Line + 5); // 右下点
                printf("right_down_guai[0]  %d\n",right_down_guai[0]);
                if (right_down_guai[0] >= 35&&right_down_guai[0]<=58) // 条件1很松，在这里加判拐点，位置不对，则是误判，跳出
                {
     
                    Island_State = 1;
                    Right_Island_Flag = 1;
                    printf("gotoringt----------------------------------------------------\n");
                }
                else
                {
                    Island_State = 0;
                    Right_Island_Flag = 0;
                }
            }
        }
    }
    if (Left_Island_Flag == 1) // 1状态下拐点还在，没丢线
    {
        if (Island_State == 1)
        {
            // insland_err = 15;
            // printf("goto11111111");
            //    printf("leftguai  %d\n", left_down_guai[0]);
            left_down_guai[0] = Find_Left_Down_Point(115, 20);

            monotonicity_change_line[0] = Monotonicity_Change_Left(left_down_guai[0]-5 , 120 - Search_Stop_Line ); // 寻找单调性改变点
            monotonicity_change_line[1] = Left[monotonicity_change_line[0]];
            // printf("monotonicity_change_line[0]  %d\n",monotonicity_change_line[0]);
            // printf(" left_down_guai[0] %d \n", left_down_guai[0]);
            Point left1(Left[left_down_guai[0]], left_down_guai[0]),
                left2(monotonicity_change_line[1], monotonicity_change_line[0]);
            cv::line(binary, left1, left2, Scalar(0, 0, 0), 2);
           
            //  Point leftn((int)((Left[(left_down_guai[0])]) * (1.7)), 20),
            //     leftb(Left[left_down_guai[0]],left_down_guai[0]);
            //                 cv::line(binary, leftn, leftb, Scalar(0, 0, 0), 2);
            //    printf("Boundry_Start_Left   %d\n",Boundry_Start_Left);
            if ((Island_State == 1) && (Boundry_Start_Left < 51||left_down_guai[0]>=80)) // 下方当丢线时候进2
            {
                Island_State = 2;
            } 
            Longest_White_Column(); // 刷新边界
        }

        else if (Island_State == 2) // 下方角点消失，2状态时下方应该是丢线，上面是弧线
        {
            // printf("go to 222222");
            AngleSpeed = 0;
            FJ_LastAngleSpeed = 0.0f;
            FJ_AngleSpeed = 0.0f;
            FJ_Angle = 0;
            // Left_find_three();
            monotonicity_change_line[0] = Monotonicity_Change_Left(110, 120-Search_Stop_Line+5); // 寻找单调性改变点
            // printf("120-Search_Stop_Line  %d\n",120-Search_Stop_Line);
            monotonicity_change_line[1] = Left[monotonicity_change_line[0]];
// printf("monotonicity_change_line[0]   %d  \n",monotonicity_change_line[0]);
            Point left1((int)(monotonicity_change_line[1] * 0.05), 120),
                left2(monotonicity_change_line[1], monotonicity_change_line[0]);
            cv::line(binary, left1, left2, Scalar(0, 0, 0), 2);
            // printf("bound   %d\n",Boundry_Start_Left);
            if (Island_State == 2 && Boundry_Start_Left>=50) // 当圆弧靠下时候，进3
            {           
                // k=0.7;
                Island_State = 3; // 最长白列寻找范围也要改，见camera.c
                            printf("goto 3333333333333333333333333333333333333333333333333\n");


            }
            Longest_White_Column(); // 刷新边界
        }
        else if (Island_State == 3) // 3状态准备进环，寻找上拐点，连线
        {
            // printf("goto 3333333333333333333333333333333333333333333333333\n");
            Left_Up_Guai[0] = Find_Left_Up_Point(100, 120 - Search_Stop_Line + 1); // 找左上拐点
            Left_Up_Guai[1] = Left[Left_Up_Guai[0]];
            // printf("Left_Up_Guai[0]   %d\n", Left_Up_Guai[0]);
            // printf("Right[Left_Up_Guai[0]]   %d\n",Right[Left_Up_Guai[0]]);
            // if (Left_Up_Guai[0] <= 5 && FJ_Angle >= -30) // 此处为了防止误判，如果经常从3状态归零，建议修改此处判断条件
            // {
            //     Island_State = 3;
            //     Left_Island_Flag = 1;
            //     return;
            // }
            // Right_K_Add_Boundary(k);
            // island_state_3_up[0] = Left_Up_Guai[0];
            // island_state_3_up[1] = Left_Up_Guai[1];
            Point left1(Right[119], 119),
                left2(Left[Left_Up_Guai[0]] -5, Left_Up_Guai[0]);
            cv::line(binary, left1, left2, Scalar(0, 0, 0), 2);
                               // 刷新边界数据
            if ((Island_State == 3) && ((FJ_Angle) <= -35)) // 纯靠陀螺仪积分入环
            {
                Island_State = 4;
            } 
            Longest_White_Column();     
        }
        else if (Island_State == 4) // 状态4已经在里面
        {
            insland_err = 0;
            if ((FJ_Angle) <-100) // 积分200度以后在打开出环判断
            {
                // printf("goto44444444444444444444444444444444444444444\n");
                monotonicity_change_line[0] = Monotonicity_Change_Right(105 , 120-Search_Stop_Line+1); // 单调性改变
                monotonicity_change_line[1] = Right[monotonicity_change_line[0]];
                // printf("monotonicity_change_line0  %d\n",monotonicity_change_line[0]);
                // printf("monotonicity_change_line[1   %d",monotonicity_change_line[1]);
                if ((Island_State == 4) && (30 <= monotonicity_change_line[0] )) // 单调点靠下，进去5
                {
                    // k=((0-monotonicity_change_line[0])/(100-monotonicity_change_line[0]))-0.3;
                    Island_State = 5;
                    k=-1.3;
                    //  printf("k   %f\n",k);
                    Right_K_Add_Boundary(k);
                }
            }
        }
        else if (Island_State == 5) // 出环
        {
            // printf("k   %f\n",k);
             Right_K_Add_Boundary(k);
            printf("goto55555555555\n");
            // printf("Boundry_Start_Right   %d\n",Boundry_Start_Right);
            if ((Island_State == 5) &&  (FJ_Angle <= -125))// 右边先丢线
            {
                Island_State = 6;
                k=-1.3;
            }
        }
        else if (Island_State == 6) // 还在出

        {
            // printf("k   %f\n",k);
            // printf("goto66666666666\n");
            // Point left1(40, 120 - Search_Stop_Line),
            //     left2(Right[119], 119);
            // cv::line(binary, left1, left2, Scalar(0, 0, 0), 2);
            // Longest_White_Column(); // 刷新边界数据
            //  printf("Boundry_Start_Right   %d\n",Boundry_Start_Right);
             Right_K_Add_Boundary(k);
            if ((Island_State == 6) && ((FJ_Angle) <= -198)) // 右边不丢线
            {
                k = 0;
                Island_State = 7;
            }
        }
        else if (Island_State == 7) // 基本出去了，在寻找拐点，准备离开环岛状态
        {
         
            // printf("goto7777777777\n");
            Left_Up_Guai[0] = Find_Left_Up_Point(100 - 10, 10); // 获取左上点坐标，坐标点合理去8
            Left_Up_Guai[1] = Left[Left_Up_Guai[0]];
            // printf("Left_Up_Guai[0]  %d\n",Left_Up_Guai[0]);
            if ((Island_State == 7) && (Left_Up_Guai[0] != 0))
            //<=100)&&(5<=Left_Up_Guai[0]&&Left_Up_Guai[0]<=120-20))//注意这里，对横纵坐标都有要求
            {
                Island_State = 8; // 基本上找到拐点就去8
            }
        }
        else if (Island_State == 8) // 连线，出环最后一步
        {
            printf("goto88888888\n");
            // printf("Left_Up_Guai[0]  %d\n",Left_Up_Guai[0]);

            Left_Up_Guai[0] = Find_Left_Up_Point(100 - 1, 10); // 获取左上点坐标
            Left_Up_Guai[1] = Left[Left_Up_Guai[0]];
            if(Left_Up_Guai[0]==0)
            {
                return ;
            }
            // printf("Left_Up_Guai[0]   %d",Left_Up_Guai[0]);
            Point leftup1(Left_Up_Guai[1] , Left_Up_Guai[0]),
                leftup2(Left[105], 105);
            // Lengthen_Left_Boundry(Left_Up_Guai[0]+15,120);
            cv::line(binary, leftup1, leftup2, Scalar(0, 0, 0), 2);
            Longest_White_Column(); // 刷新边界数据
            // printf("Boundry_Start_Left     %d\n",Boundry_Start_Left);
            if ((Island_State == 8) && (Left_Up_Guai[0] >= 44||Left_Up_Guai==0) )// 当拐点靠下时候，认为出环了，环岛结束
            {                                                                              // 要么拐点靠下，要么拐点丢了，切下方不丢线，认为环岛结束了
                insland_err = 0;
                AngleSpeed = 0;
                FJ_LastAngleSpeed = 0.0f;
                FJ_AngleSpeed = 0.0f;
                FJ_Angle = 0;
                Island_State = 0; // 8时候环岛基本结束了，为了防止连续判环，8后会进9，大概几十毫秒后归零，
                Left_Island_Flag = 0;
                printf("yuanhuanover\n");
            }
        }
    }
    else if (Right_Island_Flag == 1)
    {
        if (Island_State == 1) // 1状态下拐点还在，没丢线
        {
            // island_pd_in();
            // printf("right-------------------------------------------------1\n");
            insland_err = 15;
            right_down_guai[0] = Find_Right_Down_Point(105,  30);
            // monotonicity_change_line[0] = Monotonicity_Change_Right(right_down_guai[0], 120 - Search_Stop_Line + 5); // 单调性改变
            // printf(" monotonicity_change_line[0]   %d\n", monotonicity_change_line[0]);
            // monotonicity_change_line[1] = Right[monotonicity_change_line[0]];
            
            //    printf("right_down_guai []  %d\n",right_down_guai[0]);
            // Point right1(Right[right_down_guai[0]], right_down_guai[0]),
            //     right2(monotonicity_change_line[1], monotonicity_change_line[0]);
            // cv::line(binary, right1, right2, Scalar(0, 0, 0), 2);


             Point rightn((int)((Right[(right_down_guai[0])]) * (0.75)), 10),
                rightb(Right[right_down_guai[0]],right_down_guai[0]);
                            cv::line(binary, rightn, rightb, Scalar(0, 0, 0), 2);
// printf("bound   %d\n",Boundry_Start_Right);
            if (Boundry_Start_Right <= 51) // 右下角先丢线
            {
                Island_State = 2;
                printf("go-----------------------------------------------2\n");
            }
            Longest_White_Column(); // 刷新边界
        }
        else if (Island_State == 2) // 2状态下方丢线，上方即将出现大弧线
        {
            //  HWGpio beep(61, GPIO_Mode_Out);

            //  beep.SetGpioValue(0);
             AngleSpeed = 0;
            FJ_LastAngleSpeed = 0.0f;
            FJ_AngleSpeed = 0.0f;
            FJ_Angle = 0;
           
          
            Find_In_Three();
            monotonicity_change_line[0] = Monotonicity_Change_Right(110, 120-Search_Stop_Line+5); // 单调性改变
            monotonicity_change_line[1] = Right[monotonicity_change_line[0]] ;
            // printf("monotonicity_change_line   %d\n",monotonicity_change_line[0]);
            // printf("Boundry_Start_Right     %d \n",Boundry_Start_Right);
            Point right1((int)((monotonicity_change_line[1] * (1.6))), 115),
                right2(monotonicity_change_line[1], monotonicity_change_line[0]);
            cv::line(binary, right2, right1, Scalar(0, 0, 0), 2);
            // printf("rightlost  %d\n",Lost_start_Right);
            //         printf("Boundry_Start_Right   %d\n", Boundry_Start_Right);

            if (Island_State == 2 && Lost_start_Right>50)//右下角再不丢线进3三
            {
            Island_State = 3; // 下方丢线，说明大弧线已经下来了
             k=-0.7;
            printf("go----------------3\n");
            }
            Longest_White_Column();
        }
        else if (Island_State == 3) // 下面已经出现大弧线，且上方出现角点
        {
           
            // 
            // island_chasu = 0.1;

            // Right_Up_Guai[0] = Find_Right_Up_Point(100, 15); // 找右上拐点
            // Right_Up_Guai[1] = Right[Right_Up_Guai[0]];
            // printf("Right_Up_Guai[0]    %d\n", Right_Up_Guai[0]);
            
            // if (FJ_Angle <40) // 这里改过，此处为了防止环岛误判，如果经常出现环岛3归零，请修改此处判断条件
            // {
            //     Island_State = 3;
            //     Right_Island_Flag = 1;
            //     return;
            // }
                Left_K_Add_Boundary(k);
            // island_state_3_up[0] = Right_Up_Guai[0];
            // island_state_3_up[1] = Right_Up_Guai[1];
            // Point right3(Left[115], 115),
            //     right4(Right[Right_Up_Guai[0]] +15, Right_Up_Guai[0]);
            //     cv::line(binary, right3, right4, Scalar(0, 0, 0), 2);

            if ((Island_State == 3) && ((FJ_Angle) >=22)) // 只依靠陀螺仪积分
            {
                Island_State = 4;
                // k=0;
                printf("go----------------------------------4\n");
            } // 记得去最长白列那边改一下，区分下左右环岛
            // Longest_White_Column(); // 刷新边界数据
        }
        else if (Island_State == 4) // 4状态完全进去环岛了
        {
            // if (FJ_Angle > 90) // 环岛积分200度后再打开单调转折判断
            {
               
                // printf("goringht444444444444444\n");

                monotonicity_change_line[0] = Monotonicity_Change_Left(105, 120-Search_Stop_Line+1); // 单调性改变
                monotonicity_change_line[1] = Left[monotonicity_change_line[0]];
                // printf("monotonicity_change_line   %d      monotonicity_change_line 0000      %d   \n",monotonicity_change_line[0],monotonicity_change_line[1]);
                
                if ((Island_State == 4) && (30 <= monotonicity_change_line[0]) )// 单调点靠下，进去5
                {
                    //  printf("monotonicity_change_line   %d     \n",monotonicity_change_line[0]);
                    // k=((0-monotonicity_change_line[0])/(100-monotonicity_change_line[0]))+3;
                    // if(k>=-0.8)    k=-1.4;
                    k=-0.63;          //65    0.7     75    0.68
                    Island_State = 5;
                     
                    Left_K_Add_Boundary(k);
                    printf("go----------------------------------5\n");
                    // printf("fj    =%f\n", FJ_Angle);
                }
            }
        }
        else if (Island_State == 5) // 准备出环岛
        {
            // printf("avg-long    %d      ",avg_error);
            // if(avg_error<=-20)
            // {
            //     avg_error+=5;
            // }

 

                Left_K_Add_Boundary(k);
            // printf("avg   %d\n",avg_error);
            if (Island_State == 5 && (FJ_Angle > 127)) // 左边先丢线
            {
                k=-0.65;
                Island_State = 6;
                printf("go----------------------------------6\n");
                // printf("fj    =%f\n", FJ_Angle);
            }
        }
        else if (Island_State == 6) // 继续出
        {
            // Point left1(116, 120 - Search_Stop_Line),
            //     left2(Left[119], 119);
            // cv::line(binary, left1, left2, Scalar(0, 0, 0), 2);

            Left_K_Add_Boundary(k);
            if ((Island_State == 6) && ((FJ_Angle) >= 198)) // 左边先丢线
            {
                k=0;
                Island_State = 7;
                 
                // printf("fj    =%f\n", FJ_Angle);
            }
        }
        else if (Island_State == 7) // 基本出环岛，找角点
        {
            // speed_select();
            //  printf("lostavg   %d       ",avg_error);
            // avg_error+=20;
            // printf("  7   avg   %d\n",avg_error);
            //    printf("go----------------------------------7\n");
            island_chasu = 0;
            Right_Up_Guai[0] = Find_Right_Up_Point(100 , 120-Search_Stop_Line+1); // 获取左上点坐标，找到了去8
            Right_Up_Guai[1] = Right[Right_Up_Guai[0]];
            // printf(" Right_Up_Guai   %d\n  ", Right_Up_Guai[0]);
            if ((Island_State == 7) && (Right_Up_Guai[0] != 0)) // 注意这里，对横纵坐标都有要求，因为赛道不一样，会意外出现拐点
            {                                                   // 当角点位置合理时，进8
                Island_State = 8;
                printf("go----------------------------------8\n");
            }
        }
        else if (Island_State == 8) // 环岛8
        {
            Right_Up_Guai[0] = Find_Right_Up_Point(100 - 1, 10); // 获取右上点坐标
            Right_Up_Guai[1] = Right[Right_Up_Guai[0]];
            Lengthen_Right_Boundry(Right_Up_Guai[0], 119);
            // printf("Right_Up_Guai[0]    %d   \n",Right_Up_Guai[0]);
      
            Longest_White_Column();                                                        // 刷新边界数据
            if ((Island_State == 8) && (Right_Up_Guai[0] >= 47||Right_Up_Guai==0)) // 当拐点靠下时候，认为出环了，环岛结束
            {                
                insland_err = 0;                                                 // 角点靠下，或者下端不丢线，认为出环了
                AngleSpeed = 0;
                FJ_LastAngleSpeed = 0.0f;
                FJ_AngleSpeed = 0.0f;
                FJ_Angle = 0;
                Island_State = 0;
                Right_Island_Flag = 0;
                k=0;
                printf("game over\n");
            }
        }
        //   cout <<"Island_State   "  << Island_State<< endl;
    }

    //    if(Err>=0)
    //    {
    //       Flag_Show_101(MT9V03X_W,image_two_value,(uint8)(uint8)Err/10);//上面那个数字
    //       Flag_Show_102(MT9V03X_W,image_two_value,(uint8)((uint8)Err%10));//下面的数字
    //    }
    //    else
    //    {
    //        Flag_Show_101(MT9V03X_W,image_two_value,(uint8)(uint8)-Err/10);//上面那个数字
    //        Flag_Show_102(MT9V03X_W,image_two_value,(uint8)((uint8)-Err%10));//下面的数字
    //    }

    //    ips200_showfloat(0,12,FJ_Angle,3,3);
    //    ips200_showuint16(0,11,Island_State);
}
/*-------------------------------------------------------------------------------------------------------------------
  @brief     双最长白列巡线
  @param     null
  @return    null
  Sample     Longest_White_Column_Left();
  @note      最长白列巡线，寻找初始边界，丢线，最长白列等基础元素，后续读取这些变量来进行赛道识别
-------------------------------------------------------------------------------------------------------------------*/
int stop_tim = 0;
void Longest_White_Column()
{
    int right_border = 0,
        left_boarder = 0;
    vector<int> columns(160); // 初始化columns为所有列数
    Maxleft[0] = 0;
    Maxleft[1] = 0;
    Maxright[0] = 0;
    Maxright[1] = 0;
    Right_Lost_Time = 0; // 边界丢线数
    Left_Lost_Time = 0;
    Boundry_Start_Left = 0; // 第一个非丢线点,常规边界起始点
    Boundry_Start_Right = 0;
    Both_Lost_Time = 0; // 两边同时丢线数
   
    for (int i = 15; i <= 119; i++) // 数据清零
    {
        Right_Lost_Flag[i] = 0;
        Left_Lost_Flag[i] = 0;
        Left[i] = 0;
        Right[i] = 0;
    }
    for (int x = 3; x < binary.cols - 2; x++) // 寻找每一列的白线数
    {
        for (int y = binary.rows - 1; y >= 0; y--)
        {
            if (binary.at<uchar>(y, x) == 0)
            {
                break;
            }
            else
                columns[x]++;
        }
    }
    // if (Search_Stop_Line >= 70)
    // {
    //     for (int i = 30; i <= 130; i++)
    //     {
    //         if (columns[i] - columns[i + 1] >= 5)
    //         {
    //             stop_line++;
    //         }
    //     }
        // printf("stop_line     %d\n",stop_line);

        // if (stop_line >= 5)
    //     {
    //         stop_tim++;
    //     }
    //     if (stop_tim == 50)
    //     {
    //         stop_flsh = 1;
    //     }
    //     else
    //     {
    //         stop_line = 0, stop_flsh = 0;
    //     };
    // }

    if (Left_Island_Flag == 1)
    {
        // if (Island_State == 3)
        // {
        //     sart_line = 31;
        //     end_line = 160 - 60;
        // }
    }
    else if(cross_low==1)
    {
        sart_line=79;
        end_line=81;
    }
    else if (Right_Island_Flag == 1)
    {
       
        if (Island_State == 3&&FJ_Angle<=20)
        {
            sart_line = 75;
            end_line = 160 - 20;
        }
    }
    if (cross_flag == 1)
    {
        sart_line = 75;
        end_line = 120;
    }
    else
        (sart_line = 20, end_line = 140);
    // Max[0]是最多白条所在的位置，Max[1]是白条的长度；
    for (int a = sart_line; a < end_line; a++)
    {
        if (columns[a] > Maxleft[1])
        {
            Maxleft[0] = a;
            Maxleft[1] = columns[a];
        }
    }
    // for(int a=end_line;a>=sart_line;a--)
    // {
    //     if(columns[a]>Maxright[1])
    //     {
    //         Maxright[0]=a;
    //         Maxright[1]=columns[a];
    //     }
    // }
    Search_Stop_Line = Maxleft[1];
    

    //    printf("maxleft %d\n",Search_Stop_Line);
    //    printf("maxeright %d\n",Maxright[1]);
    // 开始寻线
    for (int y = binary.rows - 1; y >=  binary.rows - Search_Stop_Line; y--)
    {
        for (int x = Maxleft[0]; x >= 2; x--) // 左线
        {
            if (binary.at<uchar>(y, x) != 0 && binary.at<uchar>(y, x - 1) == 0 && binary.at<uchar>(y, x - 2) == 0)
            {
                left_boarder = x;
                Left_Lost_Flag[y] = 0;
                break;
            }

            else if (x <= 2)
            {
                left_boarder = x;
                Left_Lost_Flag[y] = 1;
                break;
            }
        }
        for (int x = Maxleft[0]; x <= binary.cols - 2; x++)
        {
            if (binary.at<uchar>(y, x) != 0 && binary.at<uchar>(y, x + 1) == 0 && binary.at<uchar>(y, x + 2) == 0)
            {
                right_border = x;
                Right_Lost_Flag[y] = 0;
                break;
            }
            else if (x >= 160 - 2)
            {
                right_border = x;
                Right_Lost_Flag[y] = 1;
                break;
            }
        }

        Left[y] = left_boarder;
        Right[y] = right_border;
        // Midline[y]=(Left[y] + Right[y])/2;
    }

    for (int i = 80; i >= 120 - Search_Stop_Line + 1; i--)
    {
        if (Left_Lost_Flag[i] == 1) // 单边丢线数
            Left_Lost_Time++;
        if (Right_Lost_Flag[i] == 1)
            Right_Lost_Time++;
        if (Left_Lost_Flag[i] == 1 && Right_Lost_Flag[i] == 1) // 双边丢线数
            Both_Lost_Time++;
       
        // Road_Wide[i]=Right[i]-Left[i];
    }
    for(int i=100 ; i>=120-Search_Stop_Line+1;i--)
    {
         if (Boundry_Start_Left == 0 && Left_Lost_Flag[i] != 1) // 记录第一个非丢线点，边界起始点
            Boundry_Start_Left = i;
        if (Boundry_Start_Right == 0 && Right_Lost_Flag[i] != 1)
            Boundry_Start_Right = i;
    }
   
    // if(Search_Stop_Line>=102&&Boundry_Start_Left>=83&&Boundry_Start_Right>=83)
    // {
    //       if(Island_State==0&&cross_flag==0)
    //     {
    //         long_way=1;

    //     }
    //     else 
    //     {
    //         long_way=0;
    //     }
    // }
    // printf(" err:%d\n",  avg_error);
    // printf(" errcccc:%d\n",  err_count);
    // cout <<"Left_Lost_Time"  << Left_Lost_Time<< endl;
    // cout <<"Right_Lost_Time"  << Right_Lost_Time<< endl;
    // cout <<"Both_Lost_Time"<<Both_Lost_Time<<endl;
    cross_low=0;
}

// float get_curvature(void)
// {
//     int Row_A=100;
//     int Row_B=120-(Search_Stop_Line/2);
//     int Row_C=120-Search_Stop_Line+3;
//     int Col_A= 0;
//     int Col_B= 0;
//     int Col_C= 0;
// // printf("Row_A   %d\n,Rowb   %d\n    rowc   %d\n",Row_A,Row_B,Row_C );

// // printf("max    %d\n",Maxleft[0]);
//     if(Maxleft[0]>=80)
//     {
//          Col_A= Right[Row_A];
//          Col_B= Right[Row_B];
//          Col_C= Right[Row_C];

//     }
//     else
//     {
//          Col_A= Left[Row_A];
//          Col_B= Left[Row_B];
//          Col_C= Left[Row_C];
//     }

//     float dy_AB =Row_B - Row_A;
//     float dx_AB =Col_B - Col_A;
//     float dy_BC =Row_C - Row_B;
//     float dx_BC =Col_C - Col_B;

//     float theat =atan2(dx_BC,dy_BC)-atan2(dx_AB,dy_AB);     //方向角变化量
//     float avg_dx=(fabs(dx_AB)+fabs(dx_BC))/2;              //平衡横向偏移
//     if(fabs(theat)<=0.001f||avg_dx<=0.1f)
//     {return 0.0f;}
//     return  (1.0f/(2*avg_dx/fabs(sinf(theat))))*360;
// }
void get_error(void)
{
    avg_error = 0;
    int error = 0, err_count = 0;
    for (int i = 110 - 1; i > 120 - Search_Stop_Line + 1; i--)
    {
        error += (80 - (Left[i] + Right[i]) / 2) * Weight[i];
        err_count += Weight[i];
    }

    avg_error = error / err_count;
    //    if(Island_State==4||Island_State==5)
    //    {
    //     avg_error-=10;
    //    }
    // if(Right_Island_Flag==1)
    // {  if(Island_State==1)
    //     {
    //         avg_error-=10;
    //     }
    //     else if(Island_State==2)
    //     {
    //         avg_error-=5;
    //     }
    // }
    // if(avg_error >= 50)     avg_error = 50;
    // if(avg_error <= -50)    avg_error = -50;
}
void Cameratest()
{

    // 读取摄像头一帧图像
    cap.read(frame); // cap >> frame;

    if (frame.empty())
    {
        cerr << "Error read frame" << endl;
        LMotorPWM.Disable();
        RMotorPWM.Disable();
    }
    // 预处理 -----------------
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, filtered, Size(5, 5), 0);
    threshold(filtered, binary, 90, 255, THRESH_BINARY);

    Longest_White_Column();
    Cross_Detect();
    Island_Detect();
    // printf("both   %d\n",Both_Lost_Time);
    //  printf( " FJ_Angle=   %f\n",FJ_Angle);
    get_error();
        // printf("Boundry_Start_Right   %d\n", Boundry_Start_Right);

    // printf("search    %d\n",Search_Stop_Line);
    // printf("bounder   l  %d    boundeer   r   %d\n",Boundry_Start_Left,Boundry_Start_Right);
    // printf("error    %d\n",avg_error);
    // for (int i=0;i<=118;i++)
    // {
    //     printf("%d:%d        ",i,Left[i]);
    //     printf("%d:%d\n",i,Right[i]);
    // }
    // TFTSPI_BinRoad(1, 1, 119, 159,binary.data);

    // printf("Maxleft[0]   %d\n",Maxleft[0]);
    //  printf("get  %f\n",get_curvature() );      //获取曲率
    // //获取图像数据
    // for (int i = 0; i < 120; i++)
    // {
    //     for (int j = 0; j < 160; j++)
    //     {
    //         data1[i][j] = binary.at<unsigned char>(i, j);
    //     }
    //     //cout<< "left "<< i << "  "<< Left[i] << "   right " << i << "  " << Right[i] << endl;
    // }
    // TFTSPI_dir_binRoad(0, 0, 120, 160, (uint8_t*)data1);
    // TFTSPI_dri_draw_line(Midline[20], 20, Midline[119], 119, u16RED);
    // for (int i = 0; i < 120; i+=2)
    // {
    //     for (int j = 0; j < 160; j+=2)
    //     {
    //         printf("%4d", binary.at<unsigned char>(i, j));
    //     }
    //     printf("\n");
    // }
    // printf("\n\n");
}
