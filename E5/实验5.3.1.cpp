// DP.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <time.h>
using namespace std;
using namespace cv;

#define INFINITY 100000000

void MatchTemplate()
{
	Mat src = imread("C:\\Users\\yanghaoran\\Desktop\\图片2.png");
	Mat temp = imread("C:\\Users\\yanghaoran\\Desktop\\图片1.png");

	int tempcols = temp.cols;
	int temprows = temp.rows;
	int srccols = src.cols;
	int srcrows = src.rows;

	int sdd = INFINITY;
	int x = -1;
	int y = -1;
	for (int j = 0; j <= srcrows - temprows; j++)
	{
		for (int i = 0; i <= srccols - tempcols; i++)
		{
			int middle = 0;
			for (int k = 0; k < tempcols; k++)
			{
				for (int l = 0; l < temprows; l++)
				{
					Vec3f intensity1 = src.at<Vec3b>(l + j, k + i);
					Vec3f intensity2 = temp.at<Vec3b>(l, k);

					middle += pow((intensity1.val[0] - intensity2.val[0]), 2);
					middle += pow((intensity1.val[1] - intensity2.val[1]), 2);
					middle += pow((intensity1.val[2] - intensity2.val[2]), 2);
				}
			}
			middle = middle / (tempcols*temprows);
			//cout << middle << "at" << "(" << i << "," << j << ")" << endl;
			if (middle < sdd)
			{
				sdd = middle;
				x = j;
				y = i;
			}
		}
	}
	cout << "x:" << x << "y:" << y << endl;
	Rect matchRegion = Rect{ y, x,temp.cols,temp.rows };
	rectangle(src, matchRegion, cv::Scalar{ 0, 0, 255 }, 2);

	imshow("Result", src);
}



int main()
{
	clock_t start_time = clock();	
	MatchTemplate();
	clock_t end_time = clock();
	cout << "Running time is: " << static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
	return 0;
}

