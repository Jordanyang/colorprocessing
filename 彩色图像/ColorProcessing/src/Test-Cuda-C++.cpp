#include<opencv2/opencv.hpp>
#include<iostream>
#include<thread>
#include"..\\lib\\mycuda.h"
#include"..\\lib\\ImageNoise.h"
#include"..\\lib\\MyTime.h"
using namespace std;
void static test0();
void static test3();//测试CPU自带和GPU自己负载均衡
void static test4();//测试自己写的中值滤波程序的正确性
void static test5();//测试CPU自己和GPU自己负载均衡
void static prepare();// CPU和GPU预热程序
void static CPU_GPU(cv::Mat src,double speedup);
double time1;//自己写的CPU端程序运行耗时
double time2;//CPU+GPU运行耗时(CPU自写 GPU自写)
double time3;//自带CPU端程序运行时间
double time4;//CPU+GPU运行耗时(CPU自带 GPU自写)
int main()
{

	/*test1();*/
	prepare();
	/*test4();*/
	for (int i = 0; i < 10; i++)
		test5();

	return 0;
}
// CPU和GPU预热
void static prepare()
{
	using namespace cv;
	typedef Mat mat;
	mat img = imread("1.bmp");
	mat img2 = img{ Rect{0,0,1,1} };
	cv::medianBlur(img2, img2, 3);
	test1(img2);
}
void static test0()
{
	using namespace cv;
	typedef Mat mat;
	mat img = imread("1.bmp");
	mat dst1= img.clone();
	mat dst2 = img.clone();
	mat dst3;
	/*imshow("ԭͼ",img);*/
	salt(dst1, 3000);
	pepper(dst1, 3000);
	salt(dst2, 3000);
	pepper(dst2, 3000);
	/*imshow("ԭͼ1", dst);*/
	MyTimer t1;
	t1.start();
	test1(dst1);
	t1.stop();
	std::cout << "GPU Time  is: " << t1.elapse() << " ms" << std::endl << std::endl;
	MyTimer t2;
	t2.start();
	cv::medianBlur(dst2, dst3, 3);
	t2.stop();
	std::cout << "CPU Time  is: " << t2.elapse() << " ms" << std::endl << std::endl;
	/*imshow("ԭͼ2", dst);*/
	/*waitKey(0);*/
}
void static test3()
{
	using namespace cv;
	typedef Mat mat;
	mat img = imread("1.bmp");
	mat dst1 = img.clone();
	mat dst2 = img.clone();
	mat dst3;
	/*imshow("ԭͼ",img);*/
	salt(dst1, 3000);
	pepper(dst1, 3000);
	salt(dst2, 3000);
	pepper(dst2, 3000);
	/*imshow("ԭͼ1", dst);*/
	MyTimer t1;
	t1.start();
	test1(dst1);
	t1.stop();
	std::cout << "GPU Time  is: " << t1.elapse() << " ms" << std::endl << std::endl;
	MyTimer t2;
	t2.start();
	cv::medianBlur(dst2, dst3, 3);
	t2.stop();
	/*std::cout << "CPU Time  is: " << t2.elapse() << " ms" << std::endl << std::endl;*/
	time3 = t2.elapse();
	double speedup = t2.elapse() / t1.elapse();
	cout << speedup << endl;
	CPU_GPU(img, speedup);
}
void static CPU_GPU(cv::Mat src, double speedup)
{
	using namespace cv;
	typedef Mat mat;
	int rownum = src.rows*(speedup / (1 + speedup))*0.9;
	mat gpu_img;
	gpu_img = src{ Rect{ 0, 0, src.cols, rownum } };
	mat cpu_img;
	cpu_img = src{ Rect{ 0, rownum, src.cols, src.rows - rownum } };
	thread t_cpu, t_gpu;
	t_cpu = thread(test1, ref(cpu_img));
	t_gpu = thread(medianBlur, cpu_img, cpu_img, 3);
	MyTimer t3;
	t3.start();
	t_cpu.join();
	t_gpu.join();
	t3.stop();
	/*std::cout << "CPU+GPU Time  is: " << t3.elapse() << " ms" << std::endl << std::endl;*/
	time2 = t3.elapse();
	time4 = time2;
	cout << "CPU+GPU epeedup=" << time1 / time2 << endl;
}
void static test4()
{
	using namespace cv;
	typedef Mat mat;
	mat img = imread("1.bmp");
	mat dst1 = img.clone();
	mat dst2 = img.clone();
	mat dst3;
	/*imshow("ԭͼ",img);*/
	salt(dst1, 3000);
	pepper(dst1, 3000);
	salt(dst2, 3000);
	pepper(dst2, 3000);
	/*imshow("ԭͼ1", dst);*/
	MyTimer t1;
	t1.start();
	myMedianBlur(img, dst1, 3);
	t1.stop();
	std::cout << "MyCPU Time  is: " << t1.elapse() << " ms" << std::endl << std::endl;
	imshow("my",dst1);
	MyTimer t2;
	t2.start();
	cv::medianBlur(dst2, dst3, 3);
	t2.stop();
	std::cout << "CPU Time  is: " << t2.elapse() << " ms" << std::endl << std::endl;
	imshow("self", dst3);
	waitKey(0);
}
void static test5()
{
	using namespace cv;
	typedef Mat mat;
	mat img = imread("1.bmp");
	mat dst1 = img.clone();
	mat dst2 = img.clone();
	mat dst3;
	/*imshow("ԭͼ",img);*/
	salt(dst1, 3000);
	pepper(dst1, 3000);
	salt(dst2, 3000);
	pepper(dst2, 3000);
	/*imshow("ԭͼ1", dst);*/
	MyTimer t1;
	t1.start();
	test1(dst1);
	t1.stop();
	/*std::cout << "GPU Time  is: " << t1.elapse() << " ms" << std::endl << std::endl;*/
	MyTimer t2;
	t2.start();
	myMedianBlur(dst2, dst3, 3);
	t2.stop();
	/*std::cout << "CPU Time  is: " << t2.elapse() << " ms" << std::endl << std::endl;*/
	double speedup = t2.elapse() / t1.elapse();
	cout << speedup << endl;
	time1 = t2.elapse();
	CPU_GPU(img, speedup);
}