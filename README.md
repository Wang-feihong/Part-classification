//已实现遍历文件夹功能，正在进行单图多匹配操作
#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std; 
#include"cvtest.h"


int main()
{
    Mat img = imread("D:/PyCharm/pythonProject/OpenCV-py/lslm/11.jpg");

    if (img.empty())
    {
        cout << "请确认图像文件名称是否正确" << endl;
        return -1;
    }

    vector<String> filenames;//读取文件夹图片

    glob("D:/PyCharm/pythonProject/OpenCV-py/muban/*.*jpg", filenames);

    for (size_t i = 0; i < filenames.size(); ++i)
    {
        cout << filenames[i] << endl;
        Mat src = imread(filenames[i]);

        if (!src.data)
            cerr << "Problem loading image!!!" << endl;

        imshow("temp", src);
        waitKey(0);
        // do whatever you want with your images here 

        Mat temp = src;

        Mat result;
        matchTemplate(img, temp, result, TM_CCOEFF_NORMED);//模板匹配
        double maxVal, minVal;
        Point minLoc, maxLoc;
        //寻找匹配结果中的最大值和最小值以及坐标位置
        minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
        
        result.rows;
        //绘制最佳匹配区域
        rectangle(img, Rect(maxLoc.x, maxLoc.y, temp.cols, temp.rows), Scalar(0, 0, 255), 2);
        imshow("原图", img);
        imshow("模板图像", temp);
        imshow("result", result);

    }

    waitKey(0);
    return 0;
}



