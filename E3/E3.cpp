#define N 1000000
int parent[N];
int Label = -2;

//并查集算法
void UFset() {
	for (int i = 0; i < N; i++)
		parent[i] = -1;
}

int Find(int x) {
	int s;

	for (s = x; parent[s] >= 0; s = parent[s]) {
	
	}
	while (s != x) {
		int temp = parent[x];
		parent[x] = s;
		x = temp;
	}
	return s;
}

void Union(int R1, int R2) {
	int r1 = Find(R1);
	int r2 = Find(R2);
	int temp = parent[r1] + parent[r2];
	if (parent[r1] > parent[r2])
	{
		parent[r1] = r2;
		parent[r2] = temp;
	}
	else {
		parent[r2] = r1;
		parent[r1] = temp;
	}
}

//得到连通域算法
Mat getComponent(const Mat& input) {
	int cols = input.cols; //列数
	int rows = input.rows; //行数
	Mat output = Mat::zeros(rows, cols, CV_8UC3);

	int Label = 2;
	UFset();
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (i - 1 >=0)
			{
				if (input.at<Vec3b>(i, j)[0] == input.at<Vec3b>(i - 1, j)[0])
				{
					if(Find(i*cols+j)!=Find((i-1)*cols+j))
						Union(i*cols + j, (i - 1)*cols + j);
				}
			}
			if (j - 1 >=0) {
				if (input.at<Vec3b>(i, j)[0] == input.at<Vec3b>(i, j - 1)[0])
				{
					if(Find(i*cols + j) != Find(i*cols + j-1))
						Union(i*cols + j, i*cols + j - 1);
				}
			}
		}
	}
	map<int, int> dict;
	int num = 0;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (parent[i*cols + j] < 0) {
				dict.insert(pair<int, int>(i*cols + j, num++));
			}
		}
	}

	int k = 5;
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			
			output.at<Vec3b>(i, j)[0] = dict[(parent[i*cols + j])]*10;
			output.at<Vec3b>(i, j)[1] = dict[(parent[i*cols + j])] * 20;
			output.at<Vec3b>(i, j)[2] = dict[(parent[i*cols + j])] * 30;
			
		}
	}

	return output;
}


//这个是实现直方图均衡化的算法
Mat Balance(const Mat& input,int pass) {
	int cols = input.cols; //列数
	int rows = input.rows; //行数
	Mat output = Mat::zeros(rows, cols, CV_8UC3);
	//创建直方图数组
	double** hist = new double*[pass];
	for (int i = 0; i < pass; i++)
	{
		hist[i] = new double[256]();
	}

	//累积分布直方图
	double** hist2 = new double*[pass];
	for (int i = 0; i < pass; i++)
	{
		hist2[i] = new double[256]();
	}
	


	//得到统计直方图数组
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < pass; k++) {
				hist[k][input.at<Vec3b>(i,j)[k]]= hist[k][input.at<Vec3b>(i, j)[k]]+1;
			}
		}
	}
	
	//得到累积统计分布直方图(概率)
	double pixel = rows*cols;
	for (int i = 0; i < pass; i++) {
		hist2[i][0] = hist[i][0];
		for (int j = 1; j < 256; j++) {
			hist2[i][j] = (hist[i][j] + hist2[i][j - 1]);
		}
	}

	

	for (int i = 0; i < pass; i++) {
		for (int j = 0; j < 256; j++) {
			hist2[i][j] = hist2[i][j] / pixel;
		}
	}




	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			for (int k = 0; k < pass; k++) {
				output.at<Vec3b>(i, j)[k] = hist2[k][input.at<Vec3b>(i, j)[k]] * 255;
			}
		}
	}
	
	
	

	return output;

}


int main(){

	Mat src = imread("C:\\Users\\yhr\\Desktop\\2.png");
	
	Mat result=getComponent(src);

	imshow("sss",result);
	//Mat output = Balance(src, 3);
		 
	

	waitKey(0);
	return 0;
}



