//头文件
#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

Point getNextMinLoc(Mat result, Point maxLoc, int maxVaule, int templatW, int templatH);
