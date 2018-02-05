
#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
#include<map>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
using namespace cv;

//这个是快速均值滤波算法的实现
Mat FastMeanFilter(const Mat&input, int window_size)
{
	int rows = input.rows;
	int cols = input.cols;

	int*** pixel = new int**[rows];
	for (int i = 0; i < rows; i++)
		pixel[i] = new int*[cols];
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			pixel[i][j] = new int[3];
		}
	}

	//对行初始化
	for (int i = 0; i < cols; i++)
	{
		Vec3f intensity=input.at<Vec3b>(0, i);
		pixel[0][i][0] = intensity.val[0];
		pixel[0][i][1] = intensity.val[1];
		pixel[0][i][2] = intensity.val[2];
	}
	//对列进行初始化
	for (int i = 0; i < rows; i++)
	{
		Vec3f intensity = input.at<Vec3b>(i, 0);
		pixel[i][0][0] = intensity.val[0];
		pixel[i][0][1] = intensity.val[1];
		pixel[i][0][2] = intensity.val[2];
	}

	//得到了积分图
	for (int i = 1; i < rows; i++)
	{
		for (int j = 1; j < cols; j++)
		{
			Vec3f intensity = input.at<Vec3b>(i, j);
			pixel[i][j][0] = pixel[i - 1][j][0] + pixel[i][j - 1][0] - pixel[i - 1][j - 1][0] + intensity.val[0];
			pixel[i][j][1] = pixel[i - 1][j][1] + pixel[i][j - 1][1] - pixel[i - 1][j - 1][1] + intensity.val[1];
			pixel[i][j][2] = pixel[i - 1][j][2] + pixel[i][j - 1][2] - pixel[i - 1][j - 1][2] + intensity.val[2];
		}
	}

	int halfwin = window_size / 2;
	Mat output = input.clone();
	for (int i = halfwin; i < rows - halfwin; i++)
	{
		for (int j = halfwin; j < cols - halfwin; j++)
		{
			output.at<Vec3b>(i, j)[0] = (pixel[i + halfwin][j + halfwin][0] + pixel[i - halfwin][j - halfwin][0]
				- pixel[i - halfwin][j + halfwin][0] - pixel[i + halfwin][j - halfwin][0]) / (window_size*window_size);
			output.at<Vec3b>(i, j)[1] = (pixel[i + halfwin][j + halfwin][1] + pixel[i - halfwin][j - halfwin][1]
				- pixel[i - halfwin][j + halfwin][1] - pixel[i + halfwin][j - halfwin][1] )/ (window_size*window_size);
			output.at<Vec3b>(i, j)[2] = (pixel[i + halfwin][j + halfwin][2] + pixel[i - halfwin][j - halfwin][2]
				- pixel[i - halfwin][j + halfwin][2] - pixel[i + halfwin][j - halfwin][2])/(window_size*window_size);
		}
	}
	return output;
}



//这个是高斯滤波和可分离高斯滤波算法的实现
#define pi 3.1415926

//可分离高斯滤波
Mat EasyGaussian(const Mat& input, double sigma)
{
	int kersize = int(floor(6 * sigma - 1) / 2) * 2 + 1;
	
	int rows = input.rows;
	int cols = input.cols;
	int halfker = kersize / 2;
	double* kernel = new double[kersize];
	int* kernel2 = new int[kersize];
	
	for(int i = 0; i < kersize; i++)
	{
		double g = exp(-(i - halfker) * (i - halfker) / (2 * sigma * sigma));
		kernel[i] = g;
	}
	double k = 1 / kernel[0];
	int sum = 0;

	for (int i = 0; i < kersize; i++)
	{
		kernel[i] *= k;
		kernel2[i] = int(kernel[i]);
		//cout << kernel2[i] << " ";
		sum += kernel2[i];
	}
	Mat output = Mat::zeros(rows, cols, CV_8UC3);

	//水平方向进行
	for (int i = 0; i < rows; i++)
	{
		for (int j = halfker; j < cols - halfker; j++)
		{
			for (int channel = 0; channel < 3; channel++)
			{
				double sum2 = 0;
				for (int k = 0; k < kersize; k++)
				{
					sum2 += kernel2[k] * input.at<Vec3b>(i,j-halfker)[channel];
				}
				output.at<Vec3b>(i,j)[channel]=int(sum2 / sum);
			}
		}
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < halfker; j++)
		{
			output.at<Vec3b>(i, j) = input.at<Vec3b>(i, j);
			output.at<Vec3b>(i, cols-1-j) = input.at<Vec3b>(i, cols-1-j);
		}
	}

	//竖直方向进行
	for (int i = 0; i < cols; i++)
	{
		for (int j = halfker; j < rows - halfker; j++)
		{
			for (int channel = 0; channel < 3; channel++)
			{
				double sum2 = 0;
				for (int k = 0; k < kersize; k++)
				{
					sum2 += kernel2[k] * output.at<Vec3b>(j-halfker+k,i)[channel];
				}
				output.at<Vec3b>(j, i)[channel] = int(sum2 / sum);
			}
		}
	}


	return output;
}
//得到滤波核的函数
int** GetKernel(int kersize, double sigma) {
	double** ker = new double*[kersize];

	int center = kersize / 2;
	for (int i = 0; i < kersize; i++)
		ker[i] = new double[kersize];
	int x, y;
	for (int i = 0; i < kersize; i++)
	{
		x = pow(i - center, 2);
		for (int j = 0; j < kersize; j++)
		{
			y = pow(j - center, 2);
			double g = exp(-(x + y) / (2 * sigma * sigma));
			g /= 2 * pi * sigma;
			ker[i][j] = g;
		}
	}
	int** inker = new int*[kersize];

	//int center = kersize / 2;
	for (int i = 0; i < kersize; i++)
		inker[i] = new int[kersize];
	
	double k = 1 / ker[0][0]; // 将左上角的系数归一化为1
	for (int i = 0; i < kersize; i++)
	{
		for (int j = 0; j < kersize; j++)
		{
			ker[i][j] *= k;
			inker[i][j] = int(ker[i][j]);
			//cout << inker[i][j] << " ";
		}
		//cout << endl;
	}
	return inker;
}

//普通高斯滤波的实现
Mat Gaussian(const Mat& input, double sigma) 
{
	int kersize = int(floor(6 * sigma - 1) / 2) * 2 + 1;
	int rows = input.rows;
	int cols = input.cols;

	int halfker = kersize / 2;
	//cout << kersize<<endl;
	int** kernel = GetKernel(kersize, sigma);
	int sum = 0;
	for (int i = 0; i < kersize; i++)
	{
		for (int j = 0; j < kersize; j++)
		{
			sum += kernel[i][j];
			//cout << kernel[i][j] << " ";
		}
		//cout << endl;
	}
	//cout << sum << endl;
	Mat output =Mat::zeros(rows, cols, CV_8UC3);
	
	for (int i = halfker; i < rows - halfker; i++)
	{
		for (int j = halfker; j < cols - halfker; j++)
		{
			
			for (int chaneel = 0; chaneel < 3; chaneel++)
			{
				int sum2 = 0;
				for (int k = 0; k < kersize; k++)
				{
					for (int l = 0; l < kersize; l++)
					{
						sum2 += kernel[k][l] * input.at<Vec3b>(i-halfker+k, j-halfker+l)[chaneel];
						//cout << sum2 << endl;
					}
				}
				output.at<Vec3b>(i, j)[chaneel] = int(sum2 / sum);
				//cout << int(sum2/sum)<<" ";
			}
			//cout << endl;
		}
	}
	
	

	for (int i = 0; i < halfker; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			output.at<Vec3b>(i, j) = input.at<Vec3b>(i, j);
			output.at<Vec3b>(rows - i - 1, j) = input.at<Vec3b>(rows - i - 1, j);
		}
		for (int k = 0; k < rows; k++)
		{
			output.at<Vec3b>(k, i) = input.at<Vec3b>(k, i);
			output.at<Vec3b>(k, cols-i-1) = input.at<Vec3b>(i, cols-i-1);
		}
	}
	

	/*
	//为了处理边界，所以进行边界填充，方法为镜像映射
	int newImagerows = rows + halfker * 2;
	int newImagecols = cols + halfker * 2;
	Mat changedImage = Mat::zeros(newImagerows, newImagecols, CV_8UC3);

	//先修改最上面
	for(int i=0;i<halfker;i++)
	{
		for (int j = 0; j < newImagecols; j++)
		{

		}
	}
	*/
	return output;
}


int main(){
	
	Mat src = imread("C:\\Users\\yhr\\Desktop\\Test2.jpg");

	Mat result=FastMeanFilter(src, 5);
	imshow("first", src);
	imshow("second", result);
	
	
	/*
	高斯滤波实现
	
	Mat src = imread("C:\\Users\\yhr\\Desktop\\Test2.jpg");
	
	Mat result=EasyGaussian(src,4); //或者改为 Mat result=Gaussian(src,4);
	
	imshow("first", src);
	imshow("second", result);
	*/

	waitKey(0);
	return 0;
}
