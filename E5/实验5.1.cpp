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

/*
	C:\\Users\\yanghaoran\\Desktop\\picture.png
*/
int DFT(String filename) {
	Mat I = imread(filename, IMREAD_GRAYSCALE);


	if (I.empty())
	{
		cout << "图像加载失败!" << endl;
	
		return -1;
	}
	else
		cout << "图像加载成功!" << endl << endl;

	
	
	Mat padded;
	int m = getOptimalDFTSize(I.rows);
	int n = getOptimalDFTSize(I.cols);


	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
	

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F) };
	Mat plane2 = Mat_<float>(padded);
	Mat complexI;
	merge(planes, 2, complexI);

	clock_t start_time = clock();
	dft(complexI, complexI);
	clock_t end_time = clock();
	cout << "Running time is: " << static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms" << endl;


	split(complexI, planes);
	
	
	magnitude(planes[0], planes[1], planes[0]);
	Mat magI = planes[0];
	
	
	
	

	magI += Scalar::all(1);
	log(magI, magI);




	magI = magI(Rect(0, 0, magI.cols&-2, magI.rows&-2));


	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));
	Mat q1(magI, Rect(cx, 0, cx, cy));
	Mat q2(magI, Rect(0, cy, cx, cy));
	Mat q3(magI, Rect(cx, cy, cx, cy));


	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);


	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	

	
	


	
	normalize(magI, magI, 0, 1, CV_MINMAX);

	
	




	imshow("input", I);
	imshow("o", magI);
	
	waitKey(0);
	return 0;
}