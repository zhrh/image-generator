#include <math.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include <string>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include "utils.h"
#include "image_generator.h"


int main(int argc, char *argv[])
{

	if(argc < 3)
	{
		fprintf(stderr,"Para Error!\n");
		fprintf(stderr,"Use: 1.image path 2.copy path 2.new image path\n"); 
		return -1;
	}	
	srand((unsigned)time(NULL));
	cv::theRNG().state = time(NULL);
	std::vector<std::string> filepath;
	filepath.clear();
	ReadFilePath(argv[1],filepath);
	if(filepath.empty())
	{
		printf("Don't find any features file!\n");
		return false;
	}
	printf("%d featurs file find\n",filepath.size());
	//printf("randomizing image list...\n");
	//random_shuffle(filepath.begin(),filepath.end());
	
	std::string savepath(argv[3]);
	unsigned int init_nameid = 0;
	unsigned int step = 100000000;

	ImageGenerator generator;
	const int kMaxTestImageNum = 1000;
	int i = 0;
	for(std::vector<std::string>::const_iterator iter = filepath.begin(); i != kMaxTestImageNum && iter != filepath.end(); ++iter)
	{
		printf("Process file %s ...\n",(*iter).c_str());
		if(!generator.InitImageName(*iter))
			continue;
		//init_nameid += 1;
		init_nameid = GetFileId(*iter);
		printf("Nameid = %d\n",init_nameid);
		++i;

		int alpha = -50; // contrast control, -70, -50, 50, 70
		int beta = 0; // brightness control, -50, -30, 30, 50
		generator.ChangeContrastBrightness(alpha, beta);
		generator.SaveNewImage(savepath, init_nameid + step);
	//	alpha = -30;
	//	beta = 0;
	//	generator.ChangeContrastBrightness(alpha, beta);
	//	generator.SaveNewImage(savepath, init_nameid + 2 * step);
		alpha = 35;
		beta = 0;
		generator.ChangeContrastBrightness(alpha, beta);
		generator.SaveNewImage(savepath, init_nameid + 2 * step);
	//	alpha = 40;
	//	beta = 0;
	//	generator.ChangeContrastBrightness(alpha, beta);
	//	generator.SaveNewImage(savepath, init_nameid + 4 * step);
		alpha = 0;
		beta = -25;
		generator.ChangeContrastBrightness(alpha, beta);
		generator.SaveNewImage(savepath, init_nameid + 3 * step);
	//	alpha = 0;
	//	beta = -10;
	//	generator.ChangeContrastBrightness(alpha, beta);
	//	generator.SaveNewImage(savepath, init_nameid + 6 * step);
		alpha = 0;
		beta = 25;
		generator.ChangeContrastBrightness(alpha, beta);
		generator.SaveNewImage(savepath, init_nameid + 4 * step);
	//	alpha = 0;
	//	beta = 20;
	//	generator.ChangeContrastBrightness(alpha, beta);
	//	generator.SaveNewImage(savepath, init_nameid + 8 * step);

	//	cv::Size kernel_size(7,7); // must be odd(奇数),11, 21
	//	generator.GaussianBlur(kernel_size);
	//	generator.SaveNewImage(savepath, init_nameid + 9 * step);
	//	kernel_size = cv::Size(9,9);
	//	generator.GaussianBlur(kernel_size);
	//	generator.SaveNewImage(savepath, init_nameid + 10 * step);

	//	cv::Size kernel_size(5,5);
	//	generator.BlockBlur(kernel_size);
	//	generator.SaveNewImage(savepath, init_nameid + 9 * step);
	//	kernel_size = cv::Size(7,7);
	//	generator.BlockBlur(kernel_size);
	//	generator.SaveNewImage(savepath, init_nameid + 10 * step);

		int jpeg_quality = 20;
		generator.SaveJpegQuality(savepath, init_nameid + 5 * step, jpeg_quality);
	//	jpeg_quality = 10;
	//	generator.SaveJpegQuality(savepath, init_nameid + 20 * step, jpeg_quality);

		double mean = 0, std = 30; // 30, 50
		generator.AddGaussianNoiseSimple(mean,std);
		generator.SaveNewImage(savepath, init_nameid + 6 * step);
	//	mean = 0, std = 28;
	//	generator.AddGaussianNoiseSimple(mean,std);
	//	generator.SaveNewImage(savepath, init_nameid + 12 * step);
		double noise_rate = 0.02;	// 0.05, 0.1
		generator.AddSaltPepperNoise(noise_rate);
		generator.SaveNewImage(savepath, init_nameid + 7 * step);
	//	noise_rate = 0.03;
	//	generator.AddSaltPepperNoise(noise_rate);
	//	generator.SaveNewImage(savepath, init_nameid + 14 * step);

		double logo_alpha = 0;
		std::string logoname("../data/lena.jpg");
		generator.AddLogoFileName(logoname, logo_alpha, kLogoUpRight);
		generator.SaveNewImage(savepath, init_nameid + 8 * step);
		generator.AddLogoFileName(logoname, logo_alpha, kLogoCenter);
		generator.SaveNewImage(savepath, init_nameid + 9 * step);

		double crop_rate = 0.15;	// 0.2, 0.5
		generator.CropImage(crop_rate);
		generator.SaveNewImage(savepath, init_nameid + 10 * step);
	//	crop_rate = 0.18;	// 0.2, 0.5
	//	generator.CropImage(crop_rate);
	//	generator.SaveNewImage(savepath, init_nameid + 18 * step);

		//CopyFile((*iter), argv[2], init_nameid, ".jpg");
	}


//	ImageGenerator generator;
//	//std::string imagename("../data/lena.jpg");
//	std::string imagename("../data/ukbench00000.jpg");
//	generator.Init(imagename);
//	int alpha = -50; // contrast control, -70, -50, 50, 70
//	int beta = 0; // brightness control, -50, -30, 30, 50
//	generator.ChangeContrastBrightness(alpha, beta);
  
//	cv::Size kernel_size(9,9); // must be odd(奇数),11, 21
//	//generator.GaussianBlur(kernel_size);
//	generator.BlockBlur(kernel_size);
//	std::string savename("../result/lena00000.jpg");
//	generator.SaveJpegQuality(savename,10);
//  
//	double mean = 0, std = 25; // 30, 50
//	generator.AddGaussianNoiseSimple(mean,std);
//	double noise_rate = 0.01;	// 0.05, 0.1
//	generator.AddSaltPepperNoise(noise_rate);
//
//	double logo_alpha = 0;
//	std::string logoname("../data/lena.jpg");
//	generator.AddLogo(logoname, logo_alpha, kLogoUpRight);
//
//	double crop_rate = 0.15;	// 0.2, 0.5
//	generator.CropImage(crop_rate);
//
//	cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);
//	cv::namedWindow("New Image", cv::WINDOW_AUTOSIZE);
//	cv::imshow("Original Image",generator.image());
//	cv::imshow("New Image", generator.new_image());
//	cv::waitKey(0); // Wait until user press some key
	return 0;
}

