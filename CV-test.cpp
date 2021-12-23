#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std; 
#include"cvtest.h"

int main()
{
	Mat img = imread("D:/PyCharm/pythonProject/OpenCV-py/lslm/13.jpg");
	Mat temp = imread("D:/PyCharm/pythonProject/OpenCV-py/muban/1.jpg");
	if (img.empty() || temp.empty())
	{
		cout << "请确认图像文件名称是否正确" << endl;
		return -1;
	}
	Mat result;
	matchTemplate(img, temp, result, TM_CCOEFF_NORMED);//模板匹配
	double maxVal, minVal;
	Point minLoc, maxLoc;
	//寻找匹配结果中的最大值和最小值以及坐标位置
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
	//保存结果1
	Rect rect(minLoc.x, minLoc.y, temp.cols, temp.rows);//1 
	Mat image_roi = img(rect);
	//绘制最佳匹配区域
	rectangle(img, Rect(maxLoc.x, maxLoc.y, temp.cols, temp.rows), Scalar(0, 0, 255), 2);
	putText(img, "nut", Point(maxLoc.x + temp.cols / 2, maxLoc.y + temp.rows / 2), 2, 1, Scalar(0, 0, 255));
	cout << "相似度" << 0 << ": " << TM_CCOEFF_NORMED << endl;
	for (int i = 0; i < 8; i++)
	{ 
		//计算下一个最大值
		Point new_maxLoc;
		new_maxLoc = getNextMinLoc(result, maxLoc, maxVal, temp.cols, temp.rows);
		rectangle(img, new_maxLoc, Point(new_maxLoc.x + temp.cols, new_maxLoc.y + temp.rows), Scalar(0, 255, 0), 2, 8, 0);
		//保存结果2
		Rect rect2(new_maxLoc.x, new_maxLoc.y, temp.cols, temp.rows);//2
		image_roi = img(rect2);
		putText(img, "nut", Point(new_maxLoc.x + temp.cols / 2, new_maxLoc.y + temp.rows / 2), 2, 1, Scalar(0, 0, 255));
		cout << "相似度" << i << ": " << TM_CCOEFF_NORMED << endl;
		//再计算下一个最大值    
		new_maxLoc = getNextMinLoc(result, new_maxLoc, maxVal, temp.cols, temp.rows);
		rectangle(img, new_maxLoc, Point(new_maxLoc.x + temp.cols, new_maxLoc.y + temp.rows), Scalar(255, 0, 0), 2, 8, 0);
		//保存结果3
		Rect rect3(new_maxLoc.x, new_maxLoc.y, temp.cols, temp.rows);//3
		image_roi = img(rect3);
		putText(img, "nut", Point(new_maxLoc.x + temp.cols / 2, new_maxLoc.y + temp.rows / 2), 2, 1, Scalar(0, 0,255));
		cout << "相似度"<<i+1<<": " << TM_CCOEFF_NORMED << endl;
	}
	imshow("原图", img);
	imshow("模板图像", temp);
	imshow("result", result);
	waitKey(0);
	return 0;
}
