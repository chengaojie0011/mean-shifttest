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

vector<Vec2f> lines;//����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������  

//ȥ�߹⴦��
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

	// ��ͼ���Ե��������Ϊ0
	dst.row(0).setTo(Scalar(0));
	dst.row(dst.rows - 1).setTo(Scalar(0));
	dst.col(0).setTo(Scalar(0));
	dst.col(dst.cols - 1).setTo(Scalar(0));


}

void   waterLineFound(const Mat  &src,Mat &dst)
{
	Mat grayImage, midImage,midImage2;//��ʱ������Ŀ��ͼ�Ķ���  
	Mat out1;
	Mat out2;
	Mat out3;
	Mat out4;

	cvtColor(src, grayImage, CV_BGR2GRAY);//��Ϊ�Ҷ�ͼ

											   //��ȡ�Զ����  
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	//���б��������  
	morphologyEx(grayImage, out1, MORPH_CLOSE, element);

	medianBlur(out1, out2, 3);//��ֵ�˲�

							  //��2�����б�Ե����ת��Ϊ�Ҷ�ͼ  
	Canny(out2, midImage, 30, 200, 3);//����һ��canny��Ե���  

	
											  //��3�����л����߱任  
	//vector<Vec2f> lines;//����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������  
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
	cvtColor(grayImage, midImage2, CV_GRAY2BGR);//ת����Ե�����ͼΪ�Ҷ�ͼ  
	dst = midImage2;
	line(dst, pmax1, pmax2, Scalar(0, 0,255), 1, CV_AA);
	

}

void meanShiftMy(const Mat  &src, Mat &dst)
{
	
	Mat res; //�ָ��ͼ��  
	Mat grayImage;
	Mat out1;
	Mat out2;
	Mat out3;
	Mat out4;
	Mat out11;
	Mat out111;
	int a1 = 0;

	cvtColor(src, grayImage, CV_BGR2GRAY);//��Ϊ�Ҷ�ͼ

  	rmHighlight(grayImage, out1);							  //��������

	cvtColor(out1, out11, CV_GRAY2BGR);//��Ϊ��ɫͼ
	//imshow("caise", out11);
									   //��ȡ�Զ����  
	Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
	//���б��������  
	morphologyEx(out11, out111, MORPH_CLOSE ,element);

	medianBlur(out111,out2, 5);//��ֵ�˲�

	//imshow("test",out2);

	//cvtColor(out2, out3, CV_BGR2Luv);//��Ϊ��ɫͼ
	int spatialRad = 20;  //�ռ䴰�ڴ�С  
	int colorRad = 20;   //ɫ�ʴ��ڴ�С  
	int maxPyrLevel = 2;  //����������  
	pyrMeanShiftFiltering(out2, dst, spatialRad, colorRad, maxPyrLevel); //ɫ�ʾ���ƽ���˲�  

}

void floodFillMy(const Mat  &src, Mat &dst)
{
	RNG  rng = theRNG();
	dst = src;
	Mat mask(src.rows + 2, src.cols + 2, CV_8UC1, Scalar::all(0));  //��ģ  
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x <src.cols; x++)
		{
			if (mask.at<uchar>(y + 1, x + 1) == 0)  //��0����Ϊ1����ʾ�Ѿ�������䣬���ٴ���  
			{
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(dst,mask, Point(x, y), newVal, 0, Scalar::all(5), Scalar::all(5)); //ִ����ˮ���  

			}
		}
	}
	
}

void   histogramMy(const Mat  &src, Mat &dst)
{

}

int main(int argc, char** argv)
{
	//����ͼ��RGB��ͨ��    
	Mat  srcImage = imread("4.jpg"); 
	imshow("src", srcImage);

  //����meanshift��������Ŀ��ʶ��
	Mat  outMeanshift;
	meanShiftMy(srcImage,outMeanshift);
	imshow("mean_shift", outMeanshift);

	//ʶ������
	Mat outwater;
	waterLineFound(outMeanshift, outwater);
	imshow("waterline", outwater);

	//��ˮ���
	Mat outfloodFill;
	floodFillMy(outMeanshift, outfloodFill);
	imshow("floodFill", outfloodFill);

	waitKey(0);
	
	return 0;
}
