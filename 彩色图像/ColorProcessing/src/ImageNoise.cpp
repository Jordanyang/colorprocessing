#include"..\\lib\\ImageNoise.h"
#include <opencv2\opencv.hpp>
void salt(cv::Mat image, int n) {

	int i, j;
	for (int k = 0; k<n / 2; k++) {

		// rand() is the random number generator
		i = std::rand() % image.cols; // % ����ȡ���������,rand=1022,cols=1000,rand%cols=22
		j = std::rand() % image.rows;

		if (image.type() == CV_8UC1) { // gray-level image

			image.at<uchar>(j, i) = 255; //at������Ҫָ��Mat��������ֵ����,��uchar��

		}
		else if (image.type() == CV_8UC3) { // color image

			image.at<cv::Vec3b>(j, i)[0] = 255; //cv::Vec3bΪopencv�����һ��3��ֵ����������
			image.at<cv::Vec3b>(j, i)[1] = 255; //[]ָ��ͨ����B:0��G:1��R:2
			image.at<cv::Vec3b>(j, i)[2] = 255;
		}
	}
}

//������
void pepper(cv::Mat image, int n) {

	int i, j;
	for (int k = 0; k<n; k++) {

		// rand() is the random number generator
		i = std::rand() % image.cols; // % ����ȡ���������,rand=1022,cols=1000,rand%cols=22
		j = std::rand() % image.rows;

		if (image.type() == CV_8UC1) { // gray-level image

			image.at<uchar>(j, i) = 0; //at������Ҫָ��Mat��������ֵ����,��uchar��

		}
		else if (image.type() == CV_8UC3) { // color image

			image.at<cv::Vec3b>(j, i)[0] = 0; //cv::Vec3bΪopencv�����һ��3��ֵ����������
			image.at<cv::Vec3b>(j, i)[1] = 0; //[]ָ��ͨ����B:0��G:1��R:2
			image.at<cv::Vec3b>(j, i)[2] = 0;
		}
	}
}