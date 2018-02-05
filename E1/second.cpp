#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;


void alphaBlend(Mat& foreground, Mat& background, Mat& alpha, Mat& outImage)
{
	// Find number of pixels. 
	int numberOfPixels = foreground.rows * foreground.cols * foreground.channels();
	int numberofPixels2 = background.rows*background.cols*background.channels();
	int numberofPixels3 = alpha.rows*alpha.cols*alpha.channels();
	cout << numberOfPixels << " " << numberofPixels2 << " " << numberofPixels3 << endl;
	// Get floating point pointers to the data matrices
	float* fptr = reinterpret_cast<float*>(foreground.data);
	float* bptr = reinterpret_cast<float*>(background.data);
	float* aptr = reinterpret_cast<float*>(alpha.data);
	float* outImagePtr = reinterpret_cast<float*>(outImage.data);


	// Loop over all pixesl ONCE
	for (
		int i = 0;
		i < numberOfPixels;
		i++, outImagePtr++, fptr++, aptr++, bptr++
		)
	{
		*outImagePtr = (*fptr)*(*aptr) + (*bptr)*(1 - *aptr);
	}
}

int main(){
	Mat foreground = imread("C:\\Users\\yhr\\Desktop\\exp1\\opencv1\\a.png",CV_LOAD_IMAGE_UNCHANGED);
	Mat background = imread("C:\\Users\\yhr\\Desktop\\exp1\\opencv1\\back.png");
	
	/* 下面是提取Original各个通道并把他们保存的程序*/
	//std::vector<cv::Mat> imageChannels;
	//cv::split(foreground, imageChannels);
	/* 分别读取RGB 通道。
	cv::imwrite("C:\\Users\\yhr\\Desktop\\exp1\\opencv1\\output\\0.png", imageChannels[0]);
	cv::imwrite("C:\\Users\\yhr\\Desktop\\exp1\\opencv1\\output\\1.png", imageChannels[1]);
	cv::imwrite("C:\\Users\\yhr\\Desktop\\exp1\\opencv1\\output\\2.png", imageChannels[2]);
	*/
	/*读取 alpha 通道*/
	//cv::imwrite("C:\\Users\\yhr\\Desktop\\exp1\\opencv1\\output\\3.png", imageChannels[3]);
	
	
	Mat alpha = imread("C:\\Users\\yhr\\Desktop\\exp1\\opencv1\\output\\3.png");

	// Convert Mat to float data type
	foreground.convertTo(foreground, CV_32FC3);
	background.convertTo(background, CV_32FC3);

	// Normalize the alpha mask to keep intensity between 0 and 1
	alpha.convertTo(alpha, CV_32FC3, 1.0 / 255); // 

												 // Storage for output image
	Mat ouImage = Mat::zeros(foreground.size(), foreground.type());
	
	alphaBlend(foreground, background, alpha, ouImage);
	imshow("alpha blended image", ouImage / 255);
	waitKey(0);
	
	return 0;
}