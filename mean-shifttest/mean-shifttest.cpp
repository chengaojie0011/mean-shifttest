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
#define MIN_AREA 1000//定义最小有效的矩形面积
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

void   waterLineFound(const Mat  &src, Point  &pmax1, Point  &pmax2)
{
	Mat grayImage, midImage,midImage2,dst;//临时变量和目标图的定义  
	Mat out1;
	Mat out2;
	Mat out3;
	Mat out4;

	cvtColor(src, grayImage, CV_BGR2GRAY);//变为灰度图

	cvtColor(grayImage, midImage2, CV_GRAY2BGR);
	dst = midImage2;

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
	//Point pmax1, pmax2,pout1,pout2;
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
		//line(dst, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);

	}


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
	//morphologyEx(out11, out111, MORPH_CLOSE ,element);

	medianBlur(out11,out2, 5);//中值滤波

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

void   histogramMy(const Mat  &src, Mat &hisimage,Point  pout1,Point  pout2,int &gmax,int &gmin)
{
	

	Mat colorimage,grayimage;
	cvtColor(src, grayimage, CV_BGR2GRAY);
	cvtColor(grayimage, colorimage, CV_GRAY2BGR);//变为灰度图
	cvtColor(colorimage, hisimage, CV_BGR2GRAY);

	//为计算直方图配置变量  
	//首先是需要计算的图像的通道，就是需要计算图像的哪个通道（bgr空间需要确定计算 b或g货r空间）  
		int channels = 0;
	//然后是配置输出的结果存储的 空间 ，用MatND类型来存储结果  
	MatND dstHist;
	//接下来是直方图的每一个维度的 柱条的数目（就是将数值分组，共有多少组）  
	int histSize[] = { 256 };       //如果这里写成int histSize = 256;   那么下面调用计算直方图的函数的时候，该变量需要写 &histSize  
									//最后是确定每个维度的取值范围，就是横坐标的总数  
									//首先得定义一个变量用来存储 单个维度的 数值的取值范围  
	float midRanges[] = { 0, 256 };
	const float *ranges[] = { midRanges };
	cout <<"y="<< src.rows <<"x="<< src.cols << endl;
	cout << "y2=" <<hisimage.rows << "x2=" << hisimage.cols << endl;
	cout << "p1=" << pout1 << "p2=" << pout2 << endl;
	cout << "p1.x=" << pout1.x << "p1.y=" << pout1.y << endl;



	Point d201, d202;
	d201.x = pout1.x;
	d201.y = pout1.y + 50;
	d202.x = pout2.x;
	d202.y = pout2.y + 50;

	Point t201, t202;
	t201.x = pout1.x;
	t201.y = pout1.y -20;
	t202.x = pout2.x;
	t202.y = pout2.y - 20;
	//line(grayimage, d201, d202, Scalar(0, 0, 255), 1, CV_AA);
	//line(grayimage, t201, t202, Scalar(0, 0, 255), 1, CV_AA);
	//line(grayimage, pout1, pout2, Scalar(0, 0, 255), 1, CV_AA);

	//如果像素在水天线下且与垂直线上的点像素差大于1，且与上个像素的点的像素差小于2，则判断为岸上
	int topvalue = 20;
	int downvalue = -30;
	for (int y = 0; y <src.rows; y++)
	{
		for (int x = 0; x <src.cols; x++)
		{
			
	
			if ((x ) < 1 || (x ) > (src.cols - 1) || (y ) < 1 || (y ) > (src.rows - 1))
			{
				continue;
			}
		
			if (y > (x*(pout1.y - pout2.y) / (pout1.x - pout2.x) + pout2.y) && y < (x*(pout1.y - pout2.y) / (pout1.x - pout2.x) + pout2.y) + 40)
			{
				int ycp = x*(pout1.y - pout2.y) / (pout1.x - pout2.x) + pout2.y-2; 
				//if (grayimage.at<uchar>(y, x)  <=grayimage.at<uchar>(ycp, x)+1  && grayimage.at<uchar>(y, x) >= grayimage.at<uchar>(ycp, x) -10&& abs(grayimage.at<uchar>(y, x)- grayimage.at<uchar>(y-1, x)<2))
			//	{
				if (grayimage.at<uchar>(y, x) <= grayimage.at<uchar>(ycp, x) + topvalue&& grayimage.at<uchar>(y, x) >= grayimage.at<uchar>(ycp, x) +downvalue)
				{
					hisimage.at<uchar>(y, x) =255;
				}
				

			}
		
		}
	}
	//imshow("test1", hisimage);
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			if ((x) < 10 || (x) > (src.cols - 10) || (y) < 10 || (y) > (src.rows - 10))
			{
				continue;
			}

			if (y >(x*(pout1.y - pout2.y) / (pout1.x - pout2.x) + pout2.y) && y < (x*(pout1.y - pout2.y) / (pout1.x - pout2.x) + pout2.y) + 40)
			{
				int ycp = x*(pout1.y - pout2.y) / (pout1.x - pout2.x) + pout2.y - 2;

				//削去船体部分的误去除
				if (grayimage.at<uchar>(y, x) > grayimage.at<uchar>(ycp, x) + topvalue ||grayimage.at<uchar>(y, x) <grayimage.at<uchar>(ycp, x) +downvalue)
				{
					//未被削去的点垂直下方的五十个像素变回原来的灰度值
					for (int icg = 0; icg < 50; icg++)
					{
						hisimage.at<uchar>(y + icg, x) = grayimage.at<uchar>(y + icg, x);
					}
				}
				//如果像素的左右8像素都为削去点，则改点也判断为削去点
				for (int ilr = 1; ilr < 9; ilr++)
				{
					if (hisimage.at<uchar>(y, x - ilr) == 255 && hisimage.at<uchar>(y, x + ilr) == 255)
					{
						hisimage.at<uchar>(y, x) = 255;
					}
				}
			

			}
		}
	}
	//imshow("test1", hisimage);
	//line(hisimage, d201, d202, Scalar(0, 0, 255), 1, CV_AA);
	//line(hisimage, t201, t202, Scalar(0, 0, 255), 1, CV_AA);
	//line(hisimage, pout1, pout2, Scalar(0, 0, 255), 1, CV_AA);
//	imshow("gra", grayimage);
	//imshow("test", hisimage);


	calcHist(&hisimage, 1, &channels, Mat(), dstHist, 1, histSize, ranges, true, false);

	//calcHist  函数调用结束后，dstHist变量中将储存了 直方图的信息  用dstHist的模版函数 at<Type>(i)得到第i个柱条的值  
	//at<Type>(i, j)得到第i个并且第j个柱条的值  

	//开始直观的显示直方图——绘制直方图  
	//首先先创建一个黑底的图像，为了可以显示彩色，所以该绘制图像是一个8位的3通道图像  
	Mat drawImage = Mat::zeros(Size(256, 256), CV_8UC3);
	//因为任何一个图像的某个像素的总个数，都有可能会有很多，会超出所定义的图像的尺寸，针对这种情况，先对个数进行范围的限制  
	//先用 minMaxLoc函数来得到计算直方图后的像素的最大个数  
	double g_dHistMaxValue;
	minMaxLoc(dstHist, 0, &g_dHistMaxValue, 0, 0);
	//将像素的个数整合到 图像的最大范围内  
	//遍历直方图得到的数据  
	for (int i = 0; i < 256; i++)
	{
		int value = cvRound(dstHist.at<float>(i) * 256 * 0.9 / g_dHistMaxValue);
		//cout << dstHist.at<float>(i) << endl;
		line(drawImage, Point(i, drawImage.rows - 1), Point(i, drawImage.rows - 1 - value), Scalar(255, 0, 0));
	}
	int  minValue = 1;
	for (int i=0; i < 256; i++)
	{
		float iminValue = dstHist.at<float>(i);
		if ((iminValue > minValue)&&(i>2))
		{
			gmin = i;
			break;
		}
	}
	//  寻找直方图的右端  
	int imax = 255;
	for (int i = 255; i >= 0; i--)
	{
		float imaxValue = dstHist.at<float>(i);
		if ((imaxValue > minValue) && (i<254))
		{
			gmax = i;
			break;
		}
	}
	//cout << "gmax=" << gmax<< "gmin=" << gmin << endl;
	//imshow("【直方图】", drawImage);


}

void separateShore(const Mat &src, Mat &dst, Point  pout1, Point  pout2)
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
	Mat element = getStructuringElement(MORPH_RECT, Size(7, 7));
	//进行闭运算操作  
	morphologyEx(out11, out111, MORPH_CLOSE ,element);

	medianBlur(out111, out2, 21);//中值滤波

							   //imshow("test",out2);

							   //cvtColor(out2, out3, CV_BGR2Luv);//变为彩色图
	int spatialRad = 30;  //空间窗口大小  
	int colorRad = 30;   //色彩窗口大小  
	int maxPyrLevel = 2;  //金字塔层数  
	pyrMeanShiftFiltering(out2, dst, spatialRad, colorRad, maxPyrLevel); //色彩聚类平滑滤波  
	imshow("test1", dst);

	RNG  rng = theRNG();
	Mat mask(src.rows + 2, src.cols + 2, CV_8UC1, Scalar::all(0));  //掩模  
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			if (mask.at<uchar>(y + 1, x + 1) == 0)  //非0处即为1，表示已经经过填充，不再处理  
			{
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(dst, mask, Point(x, y), newVal, 0, Scalar::all(5), Scalar::all(5)); //执行漫水填充  

			}
		}
	}

	
	Mat grayImage2;
	cvtColor(dst, grayImage2, CV_BGR2GRAY);//变为灰度图
	//imshow("test2", dst);
	imshow("test3", grayImage2);

}

void  boatDetection(const Mat  &src, Mat &dst,Point  pout1, Point  pout2,int gmax,int gmin)
{
	//threshold(src, dst, 120, 255, CV_THRESH_BINARY);
	Mat colorimage, grayimage;
	cvtColor(src, colorimage, CV_GRAY2BGR);
	cvtColor(colorimage,dst, CV_BGR2GRAY);//变为灰度图

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{


			if ((x) < 1 || (x) > (src.cols - 1) || (y) < 1 || (y) > (src.rows - 1))
			{
				continue;
			}

			if (y <=(x*(pout1.y - pout2.y) / (pout1.x - pout2.x) + pout2.y))
			{
				dst.at<uchar>(y, x) = 255;

			}

		}
	}
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			
				if ( dst.at<uchar>(y, x) <130&& dst.at<uchar>(y, x) >53)
				{
					dst.at<uchar>(y, x) = 255;
				}
				else if(dst.at<uchar>(y, x) >=150)
				{
					dst.at<uchar>(y, x) =0;
				}
				else {
					dst.at<uchar>(y, x) =0;
				}
		}
	}
	
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat out;
	//进行腐蚀操作  
	erode(dst, out, element);
	//for (int y = 0; y < src.rows; y++)
	//{
	//	for (int x = 0; x < src.cols; x++)
	//	{
	//		if (y > (x*(pout1.y - pout2.y) / (pout1.x - pout2.x) + pout2.y) && y < (x*(pout1.y - pout2.y) / (pout1.x - pout2.x) + pout2.y) + 50)
	//		{
	//			for (int i = 1; i < 3; i++)
	//			{
	//				if (dst.at<uchar>(y - i, x) == 0 && dst.at<uchar>(y + i, x) == 0)
	//				{
	//					dst.at<uchar>(y, x) = 0;
	//				}
	//			}

	//		}
	//	}
	//}
	
	

	//imshow("test4211", dst);
	//imshow("test4212", out);

}


int main(int argc, char** argv)
{
	//读入图像，RGB三通道    
	Mat  srcImage = imread("4.jpg"); 
	
	//imshow("src", srcImage);

  //进行水面目标的meanshift处理并进行目标识别
	Mat  outMeanshift;
	meanShiftMy(srcImage,outMeanshift);
	//imshow("mean_shift", outMeanshift);

	//识别海天线
	Mat outwater;
	Point pmax1, pmax2, pout1, pout2;
	waterLineFound(srcImage,pmax1, pmax2);

	pout1.x =srcImage.cols;
	pout1.y = pmax2.y - (pmax2.x - srcImage.cols)*(pmax2.y - pmax1.y) / (pmax2.x - pmax1.x);
	pout2.y = pmax2.y - pmax2.x*(pmax2.y - pmax1.y) / (pmax2.x - pmax1.x);
	pout2.x = 0;
	//line(srcImage, pout1, pout2, Scalar(0, 0, 255), 1, CV_AA);

	//imshow("outwater",srcImage);

	//直方图提取
	Mat histogram,test1;
	int hismin, hismax;
	histogramMy(outMeanshift,histogram,pout1,pout2,hismax,hismin);
  //  imshow("his", histogram);
	int x = histogram.at<uchar>(178, 1);
cout << "gmax=" << hismax << "gmin=" << hismin << endl;
cout << "像素值=" <<x<< endl;
	boatDetection(histogram, test1,pout1, pout2, hismax, hismin);
	//imshow("test1", test1);




	//漫水填充
	Mat outfloodFill;

	//floodFillMy(outMeanshift, outfloodFill);
	//imshow("floodFill", outfloodFill);

	//山和水的分割和识别
	Mat	 outseparateshore;
	separateShore(srcImage, outseparateshore,pout1,pout2);
//	imshow("outseparateshore", outseparateshore);


	waitKey(0);
	
	return 0;
}
