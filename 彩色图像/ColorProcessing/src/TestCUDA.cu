#include <opencv2\opencv.hpp>
#include <iostream>
#define  WINDOW_SIZE 3
#include <cuda.h> 
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include"..\\lib\\ImageNoise.h"
#include"..\\lib\\MyTime.h"
#include"..\\lib\\mycuda.h"
__device__ void setValue(uchar3* dst, int width, int x, int y, uchar3 value)
{
	int offset = y*width + x;
	dst[offset] = value;
}
__device__ uchar3 getValue(const uchar3* src, int width, int x, int y)
{
	int offset = y*width + x;
	return src[offset];
}
__device__ uchar sort_uchar(uchar* filter, int num)
{
	////Sorting in filter
	for (int i = 0; i <num; i++) {
		for (int j = i + 1; j < num; j++) {
			if (filter[i] > filter[j]) {
				//Swap the variables.
				uchar tmp = filter[i];
				filter[i] = filter[j];
				filter[j] = tmp;
			}
		}
	}
	uchar temp = filter[num / 2];
	return temp;
}
__device__ uchar3 sort(uchar3 *filter, int num)
{
	uchar3 temp;
	uchar a[9], b[9], c[9];
	for (int i = 0; i < num; i++)
	{
		a[i] = filter[i].x;
		b[i] = filter[i].y;
		c[i] = filter[i].z;
	}
	temp.x = sort_uchar(a, 9);
	temp.y = sort_uchar(b, 9);
	temp.z = sort_uchar(c, 9);
	return temp;

}
__global__ void Median_Filter_kernel(const uchar* src1, uchar* dst1, int width, int height)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	const uchar3 *src = (const uchar3 *)src1;
	uchar3 *dst = (uchar3*)dst1;
	//
	//filter mask
	uchar3 filter[9];

	if ((y >= height) || (x >= width))
		return;
	/////checking boundry conditions
	if ((y == 0) || (x == 0) || (y == height - 1) || (x == width - 1))
		dst[y*width + x] = { 0 };
	else {
		for (int x1 = 0; x1 < WINDOW_SIZE; x1++) {
			for (int y1 = 0; y1 < WINDOW_SIZE; y1++){
				filter[x1*WINDOW_SIZE + y1] = src[(y + y1 - 1)*width + x + x1 - 1];   // setup the filterign window.
			}
		}
		uchar3 temp = sort(filter, 9);
		setValue(dst, width, x, y, temp);
	}
}
void Median_Filter_caller(const uchar* src, uchar* dst, int width, int height)
{
	dim3 threads(16, 16, 1);
	dim3 grids((width + threads.x - 1) / threads.x, (height + threads.y - 1) / threads.y, 1);
	Median_Filter_kernel << <grids, threads >> >(src, dst, width, height);
	//cudaDeviceSynchronize();
}

//int main()
//{
//
//	//test1();
//	for (int i = 0; i < 4; i++)
//		test2();
//
//	return 0;
//}

void test1(cv::Mat& img)
{
	using namespace cv;
	typedef Mat mat;
	mat dst;
	size_t memSize = img.step * img.rows;
	uchar* d_src = nullptr;
	uchar* d_dst = nullptr;
	cudaMalloc((void**)&d_src, memSize);
	cudaMalloc((void**)&d_dst, memSize);
	cudaMemcpy(d_src, img.data, memSize, cudaMemcpyHostToDevice);
	Median_Filter_caller(d_src, d_dst, img.cols, img.rows);
	cudaMemcpy(img.data, d_dst, memSize, cudaMemcpyDeviceToHost);
}
void test2()
{

	cv::Mat image = cv::imread("1.bmp");
	cv::Mat dst;
	/*cv::imshow("src", image);*/
	salt(image, 3000);
	pepper(image, 3000);
	/*cv::imshow("noise", image);*/
	MyTimer t1;
	t1.start();
	cv::medianBlur(image, dst, 3);
	t1.stop();
	std::cout << "CPU Time  is: " << t1.elapse() << " ms" << std::endl << std::endl;
	/*cv::imshow("CPU",dst);*/
	size_t memSize = image.step * image.rows;
	uchar* d_src = nullptr;
	uchar* d_dst = nullptr;

	cudaMalloc((void**)&d_src, memSize);
	cudaMalloc((void**)&d_dst, memSize);
	MyTimer t2;

	cudaMemcpy(d_src, image.data, memSize, cudaMemcpyHostToDevice);
	t2.start();
	Median_Filter_caller(d_src, d_dst, image.cols, image.rows);

	cudaMemcpy(image.data, d_dst, memSize, cudaMemcpyDeviceToHost);
	t2.stop();
	std::cout << " GPU Time  is: " << t2.elapse() << " ms" << std::endl << std::endl;
	/*cv::imshow("gpu", image);
	cv::waitKey(0);*/
	cudaFree(d_src);
	cudaFree(d_dst);
}

