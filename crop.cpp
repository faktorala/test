#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>


using namespace cv;
using namespace std;

/**
 * @brief 从大矩形中裁切指定位置的小矩形
 * @param bigRect 大矩形区域
 * @param smallRect 小矩形在大矩形中的相对位置和尺寸
 * @return 裁切后的小矩形图像
 */
Mat cropSmallRectFromBigRect(const Mat& bigRect, const Rect& smallRect) {
    // 确保小矩形在大矩形范围内
    Rect safeRect = smallRect;
    safeRect.x = max(0, min(safeRect.x, bigRect.cols - 1));
    safeRect.y = max(0, min(safeRect.y, bigRect.rows - 1));
    safeRect.width = min(safeRect.width, bigRect.cols - safeRect.x);
    safeRect.height = min(safeRect.height, bigRect.rows - safeRect.y);
    
    return bigRect(safeRect);
}

int main() {
    // 读取图片
    Mat rsz;
    Mat img = imread("frame.jpg");
    if (img.empty()) {
        cout << "无法读取图片！请确保frame.jpg文件存在" << endl;
        return -1;
    }
    resize(img, rsz, Size(160, 120));
    cout << "图片尺寸: " << rsz.cols << "x" << rsz.rows << endl;
    // rsz.at<cv::Vec3b>(90, 70) = Vec3b(255, 0, 0);
    // rsz.at<cv::Vec3b>(90, 90) = Vec3b(255, 0, 0);
    // rsz.at<cv::Vec3b>(30, 70) = Vec3b(255, 0, 0);
    // rsz.at<cv::Vec3b>(30, 90) = Vec3b(255, 0, 0);
    imshow("frame", rsz);
    Mat cop = rsz;
    imshow("cop", cop);
    int count = 0;
    int red_arr[8][2] = {0};
    int inc = 0;

    for(int i = 70; i <= 90; i++){
        for(int j = 90; j >= 50; j--){
            Vec3b pixel = rsz.at<Vec3b>(j, i);
            // rsz.at<cv::Vec3b>(j, i) = Vec3b(255, 0, 0);
            if(pixel[0]<100 && pixel[1]<100 && pixel[2]<100){
                break;
            }
            if(pixel[0]<100 && pixel[1]<100 && pixel[2]>110){
                count ++;
                printf("count : %d\n", count);
                if(count == 1){
                    red_arr[0][0] = i;
                    red_arr[0][1] = j;
                    // rsz.at<cv::Vec3b>(red_arr[0][1], red_arr[0][0]) = Vec3b(0, 255, 0);
                }
                else if(count == 20){
                    red_arr[1][0] = i;
                    red_arr[1][1] = j;
                    // rsz.at<cv::Vec3b>(red_arr[1][1], red_arr[1][0]) = Vec3b(0, 255, 0);
                }
            }
            
            
            imshow("source", rsz);
        }
    }
    Rect crop(red_arr[1][0]-10, red_arr[1][1]-30, 50, 50);
    Mat cropped = cropSmallRectFromBigRect(cop, crop);
    
    imshow("cropped", cropped);
    imshow("source", rsz);
    imwrite("test.jpg", cropped);
    waitKey(0);
    return 0;
}