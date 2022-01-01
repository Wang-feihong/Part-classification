#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

void match_min(vector<DMatch> matches, vector<DMatch>& good_matches)
{
	double min_dist = 10000, max_dist = 0;
	for (int i = 0; i < matches.size(); i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	cout << "min_dist=" << min_dist << endl;
	cout << "max_dist=" << max_dist << endl;

	for (int i = 0; i < matches.size(); i++)
		if (matches[i].distance <= max(2 * min_dist, 20.0))
			good_matches.push_back(matches[i]);
}

//RANSAC算法实现
void ransac(vector<DMatch> matches, vector<KeyPoint> queryKeyPoint, vector<KeyPoint> trainKeyPoint, vector<DMatch>& matches_ransac)
{
	//定义保存匹配点对坐标
	vector<Point2f> srcPoints(matches.size()), dstPoints(matches.size());
	//保存从关键点中提取到的匹配点对的坐标
	for (int i = 0; i < matches.size(); i++)
	{
		srcPoints[i] = queryKeyPoint[matches[i].queryIdx].pt;
		dstPoints[i] = trainKeyPoint[matches[i].trainIdx].pt;
	}

	//匹配点对进行RANSAC过滤
	vector<int> inliersMask(srcPoints.size());
	//Mat homography;
	//homography = findHomography(srcPoints, dstPoints, RANSAC, 5, inliersMask);
	findHomography(srcPoints, dstPoints, RANSAC, 5, inliersMask);
	//手动的保留RANSAC过滤后的匹配点对
	for (int i = 0; i < inliersMask.size(); i++)
		if (inliersMask[i])
			matches_ransac.push_back(matches[i]);
}

void orb_features(Mat& gray, vector<KeyPoint>& keypionts, Mat& descriptions)
{
	Ptr<ORB> orb = ORB::create(1000, 1.2f);
	orb->detect(gray, keypionts);
	orb->compute(gray, keypionts, descriptions);
}

int main()
{
	VideoCapture capture(1);//1.读入视频

	while (1)//2.循环显示每一帧
	{
		Mat frame;//定义一个Mat变量，用于存储每一帧的图像
		capture >> frame;//读取当前帧
		//若视频播放完成，退出循环
		if (frame.empty())
		{
			break;
		}

	Mat img2 = frame;
		//Mat img1 = imread("D:/PyCharm/pythonProject/OpenCV-py/ORB/RANSAC/5.jpg");  //读取图像，根据图片所在位置填写路径即可
		Mat img1 = imread("D:/PyCharm/pythonProject/OpenCV-py/ORB/RANSAC/10.jpg");
		if (!(img1.data && img2.data))
		{
			cout << "读取图像错误，请确认图像文件是否正确" << endl;
			return -1;
		}

		//提取ORB特征点
		vector<KeyPoint> Keypoints1, Keypoints2;
		Mat descriptions1, descriptions2;

		//基于区域分割的ORB特征点提取
		orb_features(img1, Keypoints1, descriptions1);
		orb_features(img2, Keypoints2, descriptions2);

		//特征点匹配
		vector<DMatch> matches, good_min, good_ransac;
		BFMatcher matcher(NORM_HAMMING);
		matcher.match(descriptions1, descriptions2, matches);
		cout << "matches=" << matches.size() << endl;

		//最小汉明距离
		match_min(matches, good_min);
		cout << "good_min=" << good_min.size() << endl;

		//用ransac算法筛选匹配结果
		ransac(good_min, Keypoints1, Keypoints2, good_ransac);
		cout << "good_matches.size=" << good_ransac.size() << endl;

		//绘制匹配结果
		Mat outimg, outimg1, outimg2;
		//drawMatches(img1, Keypoints1, img2, Keypoints2, matches, outimg);
		drawMatches(img1, Keypoints1, img2, Keypoints2, good_min, outimg1);
		drawMatches(img1, Keypoints1, img2, Keypoints2, good_ransac, outimg2);
		//imshow("未筛选结果", outimg);
		//imshow("最小汉明距离筛选", outimg1);
		

		std::vector<Point2f> obj;
		std::vector<Point2f> scene;

		for (size_t i = 0; i < good_ransac.size(); i++)
		{
			//-- Get the keypoints from the good matches
			obj.push_back(Keypoints1[good_ransac[i].queryIdx].pt);
			scene.push_back(Keypoints2[good_ransac[i].trainIdx].pt);
		}

		Mat H = findHomography(obj, scene, RANSAC);

		//-- Get the corners from the image_1 ( the object to be "detected" )
		std::vector<Point2f> obj_corners(4);
		obj_corners[0] = Point(0, 0); 
		obj_corners[1] = Point(img1.cols, 0);
		obj_corners[2] = Point(img1.cols, img1.rows);
		obj_corners[3] = Point(0, img1.rows);
		std::vector<Point2f> scene_corners(4);
		perspectiveTransform(obj_corners, scene_corners, H);
		//-- Draw lines between the corners (the mapped object in the scene - image_2 )
		Point2f offset((float)img1.cols, 0);

		line(outimg2, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 0, 255), 4);
		line(outimg2, scene_corners[1] + offset, scene_corners[2] + offset, Scalar(0, 0, 255), 4);
		line(outimg2, scene_corners[2] + offset, scene_corners[3] + offset, Scalar(0, 0, 255), 4);
		line(outimg2, scene_corners[3] + offset, scene_corners[0] + offset, Scalar(0, 0, 255), 4);
		
		//-- Show detected matches
		namedWindow("ransac筛选", WINDOW_FREERATIO);
		//namedWindow("最小汉明距离筛选", WINDOW_FREERATIO);
	    //imshow("最小汉明距离筛选", outimg1);
		imshow("ransac筛选", outimg2);

		waitKey(30);  //等待键盘输入
	}
	waitKey(0);  //等待键盘输入
	return 0;  //程序结束
}
