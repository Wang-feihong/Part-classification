#include<iostream>

#include"cvtest.h"

//Mat 是OpenCV和C++的接口矩阵类，ImlImage是opencv和C语言的接口的结构体
Point getNextMinLoc(Mat result, Point maxLoc, int maxVaule, int templatW, int templatH)
{

    // 先将第一个最大值点附近两倍模板宽度和高度的都设置为最大值防止产生干扰
    int startX = maxLoc.x - 0.55*templatW;
    int startY = maxLoc.y - 0.55*templatH;
    int endX = maxLoc.x + 0.55*templatW;
    int endY = maxLoc.y + 0.55*templatH;
    if (startX < 0 || startY < 0)
    {
        startX = 0;
        startY = 0;
    }
    if (endX > result.cols - 1 || endY > result.rows - 1)
    {
        endX = result.cols - 1;
        endY = result.rows - 1;
    }
    for (int y = startY; y < endY; y++)
    {
        for (int x = startX; x < endX; x++)
        {
            //cvsetReal2D(result, y, x, maxVaule);//读取图像像素值
            result.at<float>(y, x) = maxVaule;//覆盖
        }
    }
    // 然后得到下一个最大值并且返回
    double new_minVaule, new_maxValue;
    Point new_minLoc, new_maxLoc;
    minMaxLoc(result, &new_minVaule, &new_maxValue, &new_minLoc, &new_maxLoc);
    return new_maxLoc;

}
