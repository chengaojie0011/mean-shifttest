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

void  rmHighlightRGB(const Mat  &src, Mat &dst)
{

	int a1 = 0;
	dst = src;
	int sl = 210, m = 3;
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
				dst.at<uchar>(y, x) = fr / (c + 1);
			}

		}
	}

	// 把图像边缘像素设置为0
	dst.row(0).setTo(Scalar(0));
	dst.row(dst.rows - 1).setTo(Scalar(0));
	dst.col(0).setTo(Scalar(0));
	dst.col(dst.cols - 1).setTo(Scalar(0));


}

void meanShiftMy()
{
	Mat  img = imread("4.jpg"); //读入图像，RGB三通道    
	imshow("原图像", img);
	Mat res; //分割后图像  
	Mat grayImage;
	Mat out1;
	Mat out2;
	Mat out3;
	Mat out4;
	Mat out11;
	Mat out111;
	int a1 = 0;

	cvtColor(img, grayImage, CV_BGR2GRAY);//变为灰度图

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
	pyrMeanShiftFiltering(out2, res, spatialRad, colorRad, maxPyrLevel); //色彩聚类平滑滤波  
	imshow("res", res);
	RNG rng = theRNG();
	Mat mask(res.rows + 2, res.cols + 2, CV_8UC1, Scalar::all(0));  //掩模  
	for (int y = 0; y < res.rows; y++)
	{
		for (int x = 0; x < res.cols; x++)
		{
			if (mask.at<uchar>(y + 1, x + 1) == 0)  //非0处即为1，表示已经经过填充，不再处理  
			{
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(res, mask, Point(x, y), newVal, 0, Scalar::all(5), Scalar::all(5)); //执行漫水填充  
			}
		}
	}
	imshow("meanShift图像分割", res);
	waitKey();

}




int main(int argc, char** argv)
{
	//test1();
	meanShiftMy();
	return 0;
}
