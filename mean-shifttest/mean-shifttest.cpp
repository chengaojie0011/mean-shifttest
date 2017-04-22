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
#define MIN_AREA 1000//������С��Ч�ľ������
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

void   waterLineFound(const Mat  &src, Point  &pmax1, Point  &pmax2)
{
	Mat grayImage, midImage,midImage2,dst;//��ʱ������Ŀ��ͼ�Ķ���  
	Mat out1;
	Mat out2;
	Mat out3;
	Mat out4;

	cvtColor(src, grayImage, CV_BGR2GRAY);//��Ϊ�Ҷ�ͼ

	cvtColor(grayImage, midImage2, CV_GRAY2BGR);
	dst = midImage2;

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
	//morphologyEx(out11, out111, MORPH_CLOSE ,element);

	medianBlur(out11,out2, 5);//��ֵ�˲�

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

void   histogramMy(const Mat  &src, Mat &hisimage,Point  pout1,Point  pout2,int &gmax,int &gmin)
{
	

	Mat colorimage,grayimage;
	cvtColor(src, grayimage, CV_BGR2GRAY);
	cvtColor(grayimage, colorimage, CV_GRAY2BGR);//��Ϊ�Ҷ�ͼ
	cvtColor(colorimage, hisimage, CV_BGR2GRAY);

	//Ϊ����ֱ��ͼ���ñ���  
	//��������Ҫ�����ͼ���ͨ����������Ҫ����ͼ����ĸ�ͨ����bgr�ռ���Ҫȷ������ b��g��r�ռ䣩  
		int channels = 0;
	//Ȼ������������Ľ���洢�� �ռ� ����MatND�������洢���  
	MatND dstHist;
	//��������ֱ��ͼ��ÿһ��ά�ȵ� ��������Ŀ�����ǽ���ֵ���飬���ж����飩  
	int histSize[] = { 256 };       //�������д��int histSize = 256;   ��ô������ü���ֱ��ͼ�ĺ�����ʱ�򣬸ñ�����Ҫд &histSize  
									//�����ȷ��ÿ��ά�ȵ�ȡֵ��Χ�����Ǻ����������  
									//���ȵö���һ�����������洢 ����ά�ȵ� ��ֵ��ȡֵ��Χ  
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

	//���������ˮ���������봹ֱ���ϵĵ����ز����1�������ϸ����صĵ�����ز�С��2�����ж�Ϊ����
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

				//��ȥ���岿�ֵ���ȥ��
				if (grayimage.at<uchar>(y, x) > grayimage.at<uchar>(ycp, x) + topvalue ||grayimage.at<uchar>(y, x) <grayimage.at<uchar>(ycp, x) +downvalue)
				{
					//δ����ȥ�ĵ㴹ֱ�·�����ʮ�����ر��ԭ���ĻҶ�ֵ
					for (int icg = 0; icg < 50; icg++)
					{
						hisimage.at<uchar>(y + icg, x) = grayimage.at<uchar>(y + icg, x);
					}
				}
				//������ص�����8���ض�Ϊ��ȥ�㣬��ĵ�Ҳ�ж�Ϊ��ȥ��
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

	//calcHist  �������ý�����dstHist�����н������� ֱ��ͼ����Ϣ  ��dstHist��ģ�溯�� at<Type>(i)�õ���i��������ֵ  
	//at<Type>(i, j)�õ���i�����ҵ�j��������ֵ  

	//��ʼֱ�۵���ʾֱ��ͼ��������ֱ��ͼ  
	//�����ȴ���һ���ڵ׵�ͼ��Ϊ�˿�����ʾ��ɫ�����Ըû���ͼ����һ��8λ��3ͨ��ͼ��  
	Mat drawImage = Mat::zeros(Size(256, 256), CV_8UC3);
	//��Ϊ�κ�һ��ͼ���ĳ�����ص��ܸ��������п��ܻ��кܶ࣬�ᳬ���������ͼ��ĳߴ磬�������������ȶԸ������з�Χ������  
	//���� minMaxLoc�������õ�����ֱ��ͼ������ص�������  
	double g_dHistMaxValue;
	minMaxLoc(dstHist, 0, &g_dHistMaxValue, 0, 0);
	//�����صĸ������ϵ� ͼ������Χ��  
	//����ֱ��ͼ�õ�������  
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
	//  Ѱ��ֱ��ͼ���Ҷ�  
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
	//imshow("��ֱ��ͼ��", drawImage);


}

void separateShore(const Mat &src, Mat &dst, Point  pout1, Point  pout2)
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
	Mat element = getStructuringElement(MORPH_RECT, Size(7, 7));
	//���б��������  
	morphologyEx(out11, out111, MORPH_CLOSE ,element);

	medianBlur(out111, out2, 21);//��ֵ�˲�

							   //imshow("test",out2);

							   //cvtColor(out2, out3, CV_BGR2Luv);//��Ϊ��ɫͼ
	int spatialRad = 30;  //�ռ䴰�ڴ�С  
	int colorRad = 30;   //ɫ�ʴ��ڴ�С  
	int maxPyrLevel = 2;  //����������  
	pyrMeanShiftFiltering(out2, dst, spatialRad, colorRad, maxPyrLevel); //ɫ�ʾ���ƽ���˲�  
	imshow("test1", dst);

	RNG  rng = theRNG();
	Mat mask(src.rows + 2, src.cols + 2, CV_8UC1, Scalar::all(0));  //��ģ  
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			if (mask.at<uchar>(y + 1, x + 1) == 0)  //��0����Ϊ1����ʾ�Ѿ�������䣬���ٴ���  
			{
				Scalar newVal(rng(256), rng(256), rng(256));
				floodFill(dst, mask, Point(x, y), newVal, 0, Scalar::all(5), Scalar::all(5)); //ִ����ˮ���  

			}
		}
	}

	
	Mat grayImage2;
	cvtColor(dst, grayImage2, CV_BGR2GRAY);//��Ϊ�Ҷ�ͼ
	//imshow("test2", dst);
	imshow("test3", grayImage2);

}

void  boatDetection(const Mat  &src, Mat &dst,Point  pout1, Point  pout2,int gmax,int gmin)
{
	//threshold(src, dst, 120, 255, CV_THRESH_BINARY);
	Mat colorimage, grayimage;
	cvtColor(src, colorimage, CV_GRAY2BGR);
	cvtColor(colorimage,dst, CV_BGR2GRAY);//��Ϊ�Ҷ�ͼ

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
	//���и�ʴ����  
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
	//����ͼ��RGB��ͨ��    
	Mat  srcImage = imread("4.jpg"); 
	
	//imshow("src", srcImage);

  //����ˮ��Ŀ���meanshift��������Ŀ��ʶ��
	Mat  outMeanshift;
	meanShiftMy(srcImage,outMeanshift);
	//imshow("mean_shift", outMeanshift);

	//ʶ������
	Mat outwater;
	Point pmax1, pmax2, pout1, pout2;
	waterLineFound(srcImage,pmax1, pmax2);

	pout1.x =srcImage.cols;
	pout1.y = pmax2.y - (pmax2.x - srcImage.cols)*(pmax2.y - pmax1.y) / (pmax2.x - pmax1.x);
	pout2.y = pmax2.y - pmax2.x*(pmax2.y - pmax1.y) / (pmax2.x - pmax1.x);
	pout2.x = 0;
	//line(srcImage, pout1, pout2, Scalar(0, 0, 255), 1, CV_AA);

	//imshow("outwater",srcImage);

	//ֱ��ͼ��ȡ
	Mat histogram,test1;
	int hismin, hismax;
	histogramMy(outMeanshift,histogram,pout1,pout2,hismax,hismin);
  //  imshow("his", histogram);
	int x = histogram.at<uchar>(178, 1);
cout << "gmax=" << hismax << "gmin=" << hismin << endl;
cout << "����ֵ=" <<x<< endl;
	boatDetection(histogram, test1,pout1, pout2, hismax, hismin);
	//imshow("test1", test1);




	//��ˮ���
	Mat outfloodFill;

	//floodFillMy(outMeanshift, outfloodFill);
	//imshow("floodFill", outfloodFill);

	//ɽ��ˮ�ķָ��ʶ��
	Mat	 outseparateshore;
	separateShore(srcImage, outseparateshore,pout1,pout2);
//	imshow("outseparateshore", outseparateshore);


	waitKey(0);
	
	return 0;
}
