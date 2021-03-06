// histogram.cpp : Defines the entry point for the console application.
//
//英语单词打错了！
#include "stdafx.h"

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

int main(int argc,char **argv)
{
	//默认的分析图像路径
	String source = "C:\\Users\\11235\\Pictures\\Camera Roll\\IMG_20170915_171544.jpg";
	if (argc > 1) {
		source = argv[0];
	}
	src = imread(source, IMREAD_COLOR);
	if (src.empty()) {
		return -1;
	}
	//vector，Mat类型
	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	int histsize = 256;
	float range[] = { 0,256 };
	const float* histrange = { range };
	bool uniform = true; bool accumulate = false;
	Mat b_hist, g_hist, r_hist;
	//输入参数列表为：输入单通道矩阵(地址)，输入图像的数量，通道数，蒙版，输出图像矩阵，模糊度，直方图的大小（地址），直方图的范围（地址），是否联合，是否加速
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histsize, &histrange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histsize, &histrange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histsize, &histrange, uniform, accumulate);
	//输出图的大小
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histsize);
	//创建一个输出图
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	//将直方图的数据量化在一定范围
	//输入参数为：输入矩阵，输出矩阵，alpha,beta,norm_type,dtype,蒙版
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	//根据直方图数据进行绘图
	for (int i = 1; i < histsize; i++)
	{
		//输出图像，点1，点2，颜色，粗细，线类型，shift默认为0，cvRound()函数是取接近float型点i-1值的个数，即有多少个点是接近这个值的；line是画直线用的
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))), Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);

		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))), Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);

		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))), Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
	}
	//显示直方图窗口
	namedWindow("calcHist Demo", WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);

	waitKey(0);
    return 0;
}

