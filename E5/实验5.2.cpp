// DP.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cstdint>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;



void RemoveNoise(Mat src) 
{
	Mat padded;
	int m = getOptimalDFTSize(src.rows);
	int n = getOptimalDFTSize(src.cols);

	copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);

	dft(complexI, complexI);
	split(complexI, planes);

	magnitude(planes[0], planes[1], planes[0]);
	Mat magI = planes[0];
	Mat rem = planes[0](Rect(1, 0, complexI.cols - 2, 1));

	double MaxValue, MinValue;
	Point minLocation; Point maxLocation;
	minMaxLoc(rem, &MinValue, &MaxValue, &minLocation, &maxLocation);
	float* a1 = complexI.ptr<float>(0, maxLocation.x + 1);
	float* a2 = complexI.ptr<float>(0, complexI.rows - maxLocation.x - 1);
	float* a3 = complexI.ptr<float>(complexI.cols - 1, maxLocation.x + 1);
	float* a4 = complexI.ptr<float>(complexI.cols - 1, complexI.rows - maxLocation.x - 1);
	a1[0] = 0;
	a2[0] = 0;
	a3[0] = 0;
	a4[0] = 0;
	a1[1] = 0;
	a2[1] = 0;
	a3[1] = 0;
	a4[1] = 0;
	

	Mat inverseTransform;
	dft(complexI, inverseTransform, DFT_INVERSE | DFT_REAL_OUTPUT);
	normalize(inverseTransform, inverseTransform, 0, 1, CV_MINMAX);
	imshow("output", inverseTransform);


	imshow("input", src);
	waitKey(0);


}

int main()
{
	Mat src= imread("C:\\Users\\yanghaoran\\Desktop\\picture2.png", IMREAD_GRAYSCALE);
	RemoveNoise(src);
	return 0;
}