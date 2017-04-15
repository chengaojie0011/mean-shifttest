// mean-shifttest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<opencv2\opencv.hpp>
#include <iostream>
#include<cv.h>  
#include <opencv2\core\core.hpp>
#include<opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <math.h>  
using namespace cv;
using namespace std;

vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合  

//去高光处理
void  rmHighlight(const Mat  &src, Mat &dst)
{

	int a1 = 0;
	dst = src;
	int sl = 200, m = 3;
	int mc = 0, c = 1, fr = 0, a3 = 0, b3 = 0;
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			a1 = src.at<uchar>(y, x);

			if (src.at<uchar>(y, x) > sl)
			{

				mc = 0;
				c = 0;
				fr = 0;
			}
			while (c == 0 && fr == 0)
			{
				if (mc == 0)
				{
					for (a3 = -1; a3 < 2; a3++)
					{
						for (b3 = -1; b3 < 2; b3++)
						{
							int temp = src.at<uchar>(y, x);


							if ((x + b3) < 1 || (x + b3) > (src.cols - 1) || (y + a3) < 1 || (y + a3) > (src.rows - 1))
							{
								continue;
							}

							if (src.at<uchar>(y + a3, x + b3) <= sl)
							{

								c = c + 1;
								fr = fr + src.at<uchar>(y + a3, x + b3);

							}
						}
					}
				}
				else
				{

					for (a3 = -1 - mc; a3 < 2 + mc; a3++)
					{

						for (b3 = -1 - mc; b3 < 2 + mc; b3++)
						{

							if ((x + b3) < 1 || (x + b3) > (src.cols - 1) || (y + a3) < 1 || (y + a3) > (src.rows - 1))
							{
								continue;
							}

							if (src.at<uchar>(y + a3, x + b3) <= sl)
							{

								if ((abs(a3)*abs(a3) + abs(b3)* abs(b3)) >= 4)
								{

									c = c + 1;
									fr = fr + src.at<uchar>(y + a3, x + b3);
								}

							}
						}
					}
				}
				mc = mc + 2;
				int a5 = 0;
				a5= fr / (c + 1);
				//cout <<"a5"<<a5<<endl;
				dst.at<uchar>(y, x) = (fr+190) / (c + 1);
			}

		}
	}

	// 把图像边缘像素设置为0
	dst.row(0).setTo(Scalar(0));
	dst.row(dst.rows - 1).setTo(Scalar(0));
	dst.col(0).setTo(Scalar(0));
	dst.col(dst.cols - 1).setTo(Scalar(0));


}

void   waterLineFound(const Mat  &src,Mat &dst)
{
	Mat grayImage, midImage,midImage2;//临时变量和目标图的定义  
	Mat out1;
	Mat out2;
	Mat out3;
	Mat out4;

	cvtColor(src, grayImage, CV_BGR2GRAY);//变为灰度图

											   //获取自定义核  
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	//进行闭运算操作  
	morphologyEx(grayImage, out1, MORPH_CLOSE, element);

	medianBlur(out1, out2, 3);//中值滤波

							  //【2】进行边缘检测和转化为灰度图  
	Canny(out2, midImage, 30, 200, 3);//进行一此canny边缘检测  

	
											  //【3】进行霍夫线变换  
	//vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合  
	HoughLines(midImage, lines, 1, CV_PI / 180, 100, 0, 0);
	//imshow("test", midImage);
	int x, y, line_length, line_longest;
	Point pmax1, pmax2;
	line_longest = 0;
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));

		x = abs(pt2.x - pt1.x);
		y = abs(pt2.y - pt1.y);
		line_length = x*x + y*y;
		if (line_longest < line_length)
		{
			line_longest = line_length;
			pmax1 = pt1;
			pmax2 = pt2;
		}


	}
	cvtColor(grayImage, midImage2, CV_GRAY2BGR);//转化边缘检测后的图为灰度图  
	dst = midImage2;
	line(dst, pmax1, pmax2, Scalar(0, 0,255), 1, CV_AA);
	

}

void meanShiftMy(const Mat  &src, Mat &dst)
{
	
	Mat res; //分割后图像  
	Mat grayImage;
	Mat out1;
	Mat out2;
	Mat out3;
	Mat out4;
	Mat out11;
	Mat out111;
	int a1 = 0;

	cvtColor(src, grayImage, CV_BGR2GRAY);//变为灰度图

  	rmHighlight(grayImage, out1);							  //高亮处理

	cvtColor(out1, out11, CV_GRAY2BGR);//变为彩色图
	//imshow("caise", out11);
									   //获取自定义核  
	Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
	//进行闭运算操作  
	morphologyEx(out11, out111, MORPH_CLOSE ,element);

	medianBlur(out111,out2, 5);//中值滤波

	//imshow("test",out2);

	//cvtColor(out2, out3, CV_BGR2Luv);//变为彩色图
	int spatialRad = 20;  //空间窗口大小  
	int colorRad = 20;   //色彩窗口大小  
	int maxPyrLevel = 2;  //金字塔层数  
	pyrMeanShiftFiltering(out2, dst, spatialRad, colorRad, maxPyrLevel); //色彩聚类平滑滤波  

}

void floodFillMy(const Mat  &src, Mat &dst)
{
	RNG  rng = theRNG();
	dst = src;
	Mat mask(src.rows + 2, src.cols + 2, CV_8UC1, Scalar::all(0));  //掩模  
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x <src.cols; x++)
		{
			if (mask.at<uchar>(y + 1, x + 1) == 0)  //非0处即为1，表示已经经过填充，不再处理  
			{
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(dst,mask, Point(x, y), newVal, 0, Scalar::all(5), Scalar::all(5)); //执行漫水填充  

			}
		}
	}
	
}

void   histogramMy(const Mat  &src, Mat &dst)
{

}

int main(int argc, char** argv)
{
	//读入图像，RGB三通道    
	Mat  srcImage = imread("4.jpg"); 
	imshow("src", srcImage);

  //进行meanshift处理并进行目标识别
	Mat  outMeanshift;
	meanShiftMy(srcImage,outMeanshift);
	imshow("mean_shift", outMeanshift);

	//识别海天线
	Mat outwater;
	waterLineFound(outMeanshift, outwater);
	imshow("waterline", outwater);

	//漫水填充
	Mat outfloodFill;
	floodFillMy(outMeanshift, outfloodFill);
	imshow("floodFill", outfloodFill);

	waitKey(0);
	
	return 0;
}
