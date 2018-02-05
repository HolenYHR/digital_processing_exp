#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
using namespace std;
using namespace cv;

//这个是放大缩小函数，input 为输入图片矩阵，output为输出图片矩阵，sx为输入图片长，sy为输入图片宽
void Scale(const Mat &input, Mat &output, double sx, double sy) {
	int i, j;
	double py = output.rows;
	double px = output.cols;
	
	for (i =1; i < output.cols; i++){
		for (j = 1; j < output.rows; j++) {
			 
			//这个是最近邻法
			//int x = i*sx / px;
			//int y = j*sy / py;
			
			
			double x = i*sx / px;
			double y = j*sy / py;
			double u = x - floor(x);
			double v = y - floor(y);
			int cx = (int)x;
			int cy = (int)y;
			if (cx + 1 < sx&&cy + 1 < sy)
				output.at<Vec3b>(j, i) = (1 - v)*(input.at<Vec3b>(cy, cx)*(1 - u) + input.at<Vec3b>(cy, cx + 1)*u) + v*(input.at<Vec3b>(cy + 1, cx)*(1 - u) + input.at<Vec3b>(cy + 1, cx + 1)*u);
			else
				output.at<Vec3b>(j, i) = input.at<Vec3b>(cy, cx);
		}
		
	}
}



//这个是变形函数
void Trans(const Mat &input, Mat &output) {
	int cols = output.cols;
	int rows = output.rows;
	for (int i = 0; i < output.cols; i++) {
		
		double xc = (i - 0.5*cols) / (0.5*cols);
		for (int j = 0; j < output.rows; j++) {
			double yc = (j - 0.5*rows) / (0.5*rows);
			double r = sqrt(xc*xc + yc*yc);
			double theta = (1 - r)*(1 - r);

			if (r > 1)
				output.at<Vec3b>(j, i) = input.at<Vec3b>(j, i);
			else
			{
				double nx = cos(theta)*xc - sin(theta)*yc;
				double ny = sin(theta)*xc + cos(theta)*yc;
				double a = nx*0.5*cols + 0.5*cols;
				double b = ny*0.5*rows + 0.5*rows;

				double u = a - floor(a);
				double v = b - floor(b);
				int cx = (int)a;
				int cy = (int)b;
				//cout << cx + 1 << " " << cy + 1 << " " << endl;
				if (cx + 1 < cols&&cy + 1 < rows)
					output.at<Vec3b>(j, i) = (1 - v)*(input.at<Vec3b>(cy, cx)*(1 - u) + input.at<Vec3b>(cy, cx + 1)*u) + v*(input.at<Vec3b>(cy + 1, cx)*(1 - u) + input.at<Vec3b>(cy + 1, cx + 1)*u);
				else
					output.at<Vec3b>(j, i) = input.at<Vec3b>(cy, cx);
			}

		}
	}

}

int main(){

	Mat src = imread("C:\\Users\\yhr\\Desktop\\p1.jpg");
	Mat img = Mat::zeros(src.rows, src.cols, CV_8UC3);
	Trans(src, img);
	
	/*
	Mat img = Mat::zeros(这里输入新图像的长,新图像的宽, CV_8UC3);
	Scale(src, img, src.cols, src.rows);
	*/
	
	imshow("sss",img);
	waitKey(0);
	return 0;
}
