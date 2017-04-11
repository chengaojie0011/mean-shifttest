// mean-shifttest.cpp : �������̨Ӧ�ó������ڵ㡣
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


int min_my(int num1, int num2)
{
	return num1 < num2 ? num1 : num2;
}

void meanShiftMy()
{
	Mat  img = imread("C:\\Workspace\\mean-shifttest\\mean-shifttest\\5.jpg"); //����ͼ��RGB��ͨ��    
	imshow("ԭͼ��", img);
	Mat res; //�ָ��ͼ��  
	Mat grayImage;
	Mat out1;
	Mat out2;
	Mat out11;
	Mat out111;
	double  a1 = 0;
	cvtColor(img, grayImage, CV_BGR2GRAY);//��Ϊ�Ҷ�ͼ
										  //��������
	out1 = grayImage;
	int sl =210, m =3;
	int mc = 0, c = 1, fr = 1, a3 = 0, b3 = 0;
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{

			//cout << "y=" << y << endl;
		//	cout << "x=" << x << endl;
			a1 = grayImage.at<uchar>(y, x);
			//cout << "a1=" << a1 << endl;
			if (grayImage.at<uchar>(y, x) > sl)
			{
				mc = 0;
				c = 0;
				fr = 0;
			}
			while (c == 0 && fr == 0)
			{
				if (mc == 0)
				{
					bool isvalid = true;
						for (a3 = -1; a3 < 2; a3++)
						{
							for (b3 = -1; b3 < 2; b3++)
							{
								int temp = grayImage.at<uchar>(y, x);
							//	cout << "temp=" << temp << endl;
								if ((min_my(x + b3, img.cols - x - b3 - 1) < mc + m) || (min_my(y + a3, img.cols - y - a3 - 1) < mc + m))
								{
									continue;
								}
								if ((grayImage.at<uchar>(y + a3, x + b3) > sl))
								{
									isvalid = false;
								}
								if (grayImage.at<uchar>(y + a3, x + b3) <= sl)
								{

									c = c + 1;
									fr = fr + grayImage.at<uchar>(y + a3, x + b3);
								//	cout << "c=" << c << endl;
									//cout << "fr=" << fr << endl;
								}
								if (c<3)
								{
									c = 0;
									fr = 0;
								}
							}
						}
				}
				else
				{
				//	cout << "a3=" << a3 << endl;
				//	cout << "mc=" << mc << endl;
					for (a3 = -1 - mc; a3 < 2 + mc; a3++)
					{

						for (b3 = -1 - mc; b3 < 2 + mc; b3++)
						{
								if ((min_my(x + b3, img.cols - x - b3 - 1) < mc + m) || (min_my(y + a3, img.cols - y - a3 - 1) < mc + m))
								{
									continue;
								}
							if (grayImage.at<uchar>(y + a3, x + b3) <= sl)
							{

								if ((abs(a3)*abs(a3) + abs(b3)* abs(b3)) >= 4)
								{

									c = c + 1;
									fr = fr + grayImage.at<uchar>(y + a3, x + b3);
								}

							}
						}
					}
				}
				mc = mc + 1;
				cout << "mc=" << mc << endl;
			}
			//mc = mc + 2;
			out1.at<uchar>(y, x) = fr / (c);
		}
	}

	// ��ͼ���Ե��������Ϊ0
	out1.row(0).setTo(Scalar(0));
	out1.row(out1.rows - 1).setTo(Scalar(0));
	out1.col(0).setTo(Scalar(0));
	out1.col(out1.cols - 1).setTo(Scalar(0));
	cvtColor(out1, out11, CV_GRAY2BGR);//��Ϊ�Ҷ�ͼ
									   //��ȡ�Զ����  
	Mat element = getStructuringElement(MORPH_RECT, Size(17, 17));
	//���б��������  
	morphologyEx(out11, out111, MORPH_CLOSE, element);

	medianBlur(out111, out2, 11);//��ֵ�˲�

	int spatialRad = 20;  //�ռ䴰�ڴ�С  
	int colorRad = 20;   //ɫ�ʴ��ڴ�С  
	int maxPyrLevel = 2;  //����������  
	pyrMeanShiftFiltering(out2, res, spatialRad, colorRad, maxPyrLevel); //ɫ�ʾ���ƽ���˲�  
	imshow("res", res);
	RNG rng = theRNG();
	Mat mask(res.rows + 2, res.cols + 2, CV_8UC1, Scalar::all(0));  //��ģ  
	for (int y = 0; y < res.rows; y++)
	{
		for (int x = 0; x < res.cols; x++)
		{
			if (mask.at<uchar>(y + 1, x + 1) == 0)  //��0����Ϊ1����ʾ�Ѿ�������䣬���ٴ���  
			{
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(res, mask, Point(x, y), newVal, 0, Scalar::all(5), Scalar::all(5)); //ִ����ˮ���  
			}
		}
	}
	imshow("meanShiftͼ��ָ�", res);
	waitKey();

}


void test1()
{
	Mat  img = imread("C:\\Workspace\\mean-shifttest\\mean-shifttest\\4.jpg"); //����ͼ��RGB��ͨ��    
	imshow("ԭͼ��", img);
	Mat res; //�ָ��ͼ��  
	Mat grayImage;
	Mat out1;
	Mat out2;
	Mat out11;
	Mat out111;
	int a1 = 0;
	cvtColor(img, grayImage, CV_BGR2GRAY);//��Ϊ�Ҷ�ͼ
										  //��������
	out1 = grayImage;
	int sl = 100, m = 3;
	int mc = 0, c = 1, fr = 1, a3 = 0, b3 = 0;
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{

			if ((min_my(x, img.cols - x - 1) < mc + m) || (min_my(y, img.cols - y - 1) < mc + m))
			{
				continue;
			}
			cout << "x=" << x << endl;
			cout << "y=" << y << endl;

		}
	}
}


int main(int argc, char** argv)
{
	//test1();
	meanShiftMy();
	return 0;
}
