#include<iostream>
#include<stdlib.h>
#include<opencv2/opencv.hpp>
#include"..\\lib\\ImageNoise.h"
typedef struct
{
	unsigned char x;
	unsigned char y;
	unsigned char z;
}uchar31;
#define uchar3 uchar31
typedef unsigned char uchar;
unsigned char sort_uchar1(unsigned char* filter, int num)
{
	////Sorting in filter
	for (int i = 0; i <num; i++) {
		for (int j = i + 1; j < num; j++) {
			if (filter[i] > filter[j]) {
				//Swap the variables.
				unsigned char tmp = filter[i];
				filter[i] = filter[j];
				filter[j] = tmp;
			}
		}
	}
	unsigned char temp = filter[num / 2];
	return temp;
}
uchar3 sort1(uchar3 *filter, int num)
{
	uchar3 temp;
	unsigned char a[9], b[9], c[9];
	for (int i = 0; i < num; i++)
	{
		a[i] = filter[i].x;
		b[i] = filter[i].y;
		c[i] = filter[i].z;
	}
	temp.x = sort_uchar1(a, 9);
	temp.y = sort_uchar1(b, 9);
	temp.z = sort_uchar1(c, 9);
	return temp;

}
//----------------------------ÖÐÖµÂË²¨Æ÷

void FilterMid(unsigned char *image1,unsigned char * outputimage1, int height, int width)

{

	int i, j, k, l;

	int pos;
	uchar3 *image = (uchar3*)image1;
	uchar3* outputimage = (uchar3*)outputimage1;
	uchar3 temp;

	uchar3 psr[9];

	uchar3 *p = (uchar3*)malloc(height*width*sizeof(uchar3));

	for (i = 1; i<height - 1; i++)

	{

		for (j = 1; j<width - 1; j++)

		{     //---3*3´°¿Ú¾ØÕó

			psr[0] = image[(i - 1)*width + j - 1];

			psr[1] = image[(i - 1)*width + j];

			psr[2] = image[(i - 1)*width + j + 1];

			psr[3] = image[i*width + j - 1];

			psr[4] = image[i*width + j];

			psr[5] = image[i*width + j + 1];

			psr[6] = image[(i + 1)*width + j - 1];

			psr[7] = image[(i + 1)*width + j];

			psr[8] = image[(i + 1)*width + j + 1];
			temp = sort1(psr, 9);
			p[i*width + j] = temp;
		}

	}

	for (i = 1; i<height - 1; i++)

	{

		for (j = 1; j<width - 1; j++)

		{

			outputimage[i*width + j] = p[i*width + j];

		}

	}

	outputimage1 = (unsigned char*)outputimage;
	free(p);

	return;

}
void myMedianBlur(cv::Mat&src, cv::Mat& dst, int windowSize)
{

	dst.create(src.rows,src.cols,src.type());
	FilterMid(src.data, dst.data, src.rows, src.cols);
}