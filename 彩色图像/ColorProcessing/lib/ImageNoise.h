#ifndef _ImageNoise_H
#define _ImageNoise_H
#include <opencv2\opencv.hpp>
//ÑÎÔëÉù
extern void salt(cv::Mat image, int n);
//½¹ÔêÉù
extern void pepper(cv::Mat image, int n);
extern void myMedianBlur(cv::Mat&src, cv::Mat& dst, int windowSize);
#endif