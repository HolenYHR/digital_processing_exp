// DP.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "stdafx.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <time.h>
using namespace std;
using namespace cv;



double I[2000][2000];

Mat convolution(const Mat &src, const Mat &temp) {
	Mat asrc;
	Mat atemp;
	Mat result;
	src.convertTo(asrc, CV_64F);
	temp.convertTo(atemp, CV_64F);
	flip(atemp, atemp, -1);
	Size dftSize;
	dftSize.width = cv::getOptimalDFTSize(asrc.cols);
	dftSize.height = cv::getOptimalDFTSize(asrc.rows);
	Mat tempA(dftSize, asrc.type(), cv::Scalar::all(0));
	Mat tempB(dftSize, atemp.type(), cv::Scalar::all(0));
	Mat roiA(tempA, cv::Rect(0, 0, asrc.cols, asrc.rows));
	asrc.copyTo(roiA);
	Mat roiB(tempB, cv::Rect(0, 0, atemp.cols, atemp.rows));
	atemp.copyTo(roiB);
	dft(tempA, tempA, 0, asrc.rows);
	dft(tempB, tempB, 0, atemp.rows);
	mulSpectrums(tempA, tempB, tempA, DFT_COMPLEX_OUTPUT);
	dft(tempA, tempA, DFT_INVERSE+DFT_SCALE, asrc.rows);
	tempA(Rect(0, 0, asrc.cols, asrc.rows)).copyTo(result);
	return result;
}



void templateMatch( Mat &src, const Mat temp) {
	
	//得到积分图的过程
	for (int i = 0; i <src.rows ; i++) 
	{
		
		for (int j = 0; j< src.cols; j++) 
		{
			double sum = 0;
			Vec3b vec = src.at<Vec3b>(i,j);
			sum += pow(vec[0],2) + pow(vec[1],2) + pow(vec[2],2);
			I[i + 1][j + 1] = sum + I[i][j + 1] + I[i + 1][j] - I[i][j];
		}
	}


	//进行卷积操作

	Mat srcGroup[3];
	split(src, srcGroup);
	Mat tempGroup[3];

	split(temp, tempGroup);
	
	Mat conv[3];
	conv[0] = convolution(srcGroup[0], tempGroup[0]);
	conv[1] = convolution(srcGroup[1], tempGroup[1]);
	conv[2] = convolution(srcGroup[2], tempGroup[2]);
	Mat convolved;
	merge(conv, 3, convolved);
	
	//进行计算相似度大小计算
	int x = -1;
	int y = -1;
	double sdd = DBL_MAX;
	for (int i = temp.rows; i < src.rows; i++) {
		for (int j = temp.cols; j< src.cols; j++) {
			Vec3d vec = convolved.at<Vec3d>(i, j);
			double diff = -2 * (vec[0] + vec[1] + vec[2]) +I[i+1][j+1]-I[i - temp.rows][j + 1]
				- I[i + 1][j - temp.cols]
				+ I[i - temp.rows][j - temp.cols];
			if (diff < sdd) {
				sdd = diff;
				x = i;
				y = j;
			}
		}
	}
	
	

	Rect matchRegion=Rect{ y - temp.cols, x - temp.rows,temp.cols,temp.rows };
	rectangle(src,matchRegion, cv::Scalar{ 0, 0, 255 }, 2);
	
	imshow("Result", src);
	waitKey(0);
	
}


int main() {
	Mat image = imread("C:\\Users\\yanghaoran\\Desktop\\图片2.png", IMREAD_COLOR);
	
	Mat stencil = cv::imread("C:\\Users\\yanghaoran\\Desktop\\图片1.png", IMREAD_COLOR);
	clock_t start_time = clock();
	templateMatch(image, stencil);
	clock_t end_time = clock();
	cout << "Running time is: " << static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms" <<endl;
	return 0;
}



