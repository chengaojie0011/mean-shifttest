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
int main(int argc, char** argv)
{
	Mat img = imread("3.jpg"); //����ͼ��RGB��ͨ��    
	imshow("ԭͼ��", img);
	Mat res; //�ָ��ͼ��  
	int spatialRad = 20;  //�ռ䴰�ڴ�С  
	int colorRad = 20;   //ɫ�ʴ��ڴ�С  
	int maxPyrLevel = 2;  //����������  
	pyrMeanShiftFiltering(img, res, spatialRad, colorRad, maxPyrLevel); //ɫ�ʾ���ƽ���˲�  
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
	return 0;
}