#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <algorithm>
#include <string>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include "image_generator.h"
#include "MySQL_API.h"
#include "utils.h"


int main(int argc, char *argv[])
{

/********************generate background images**********************/
	/*
	CMySQL_API mysql_api("root", "root", "web_image_test");
	if(mysql_api.Init_Database_Con("127.0.0.1", 3306) != 0)
		return -1;
	int seqid = 1;
	std::string imagedir("/media/image1/");
	//std::string savedir("/home/zrh/image_generator/result/");
	std::string savedir("/media/gxg_disk/zhourenhao/test");
	std::string filepath, filename, savepath[3], relative_path[3];
	char dirname[1024];
	unsigned int old_id;

	ImageGenerator generator;
	float resize_ratio1 = 0.6;
	float resize_ratio2 = 0.8;
	int jpeg_quality = 15;
	unsigned char md5[MD5_DIGEST_LENGTH];
	memset(md5,0,MD5_DIGEST_LENGTH);
	int split_dir = 100;

	struct timeval start, end;
	float query_time_use = 0;
	float insert_time_use = 0;
	float generator_time_use = 0;
	int entry_num = 3;
	while(seqid < 1000)
	{
		if(seqid % split_dir == 1)
		{
			memset(dirname, 0, 1024);
			sprintf(dirname, "%s/%d/", savedir.c_str(), (seqid + split_dir - 1) * entry_num);
			if(mkdir(dirname, S_IRWXU) == -1)
				if(errno != EEXIST)
				{
					perror("can't mkdir file !");
					break;
				}
		}

		gettimeofday(&start, NULL);
		mysql_api.Image_Map_Table_Read_Path_Oldid(seqid, filepath, old_id);
		gettimeofday(&end, NULL);
		query_time_use += (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000.0;

		printf("path = %s, old_id = %d \n", filepath.c_str(), old_id);
		filepath.assign(imagedir + filepath);
		if(!generator.BgInit(filepath))
		{
			++seqid;
			continue;
		}
		GetFileName(filepath, filename);

		gettimeofday(&start, NULL);
		savepath[0].assign(dirname + filename + "_1.jpg"); 
		generator.ResizeImage(resize_ratio1);
		generator.SaveNewImage(savepath[0]);

		savepath[1].assign(dirname + filename + "_2.jpg"); 
		generator.ResizeImage(resize_ratio2);
		generator.SaveNewImage(savepath[1]);

		savepath[2].assign(dirname + filename + "_3.jpg"); 
		generator.SaveJpegQuality(savepath[2], jpeg_quality);
		gettimeofday(&end, NULL);
		generator_time_use += (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000.0;

		gettimeofday(&start, NULL);
		for(int i = 0; i != entry_num; ++i)
			relative_path[i].assign(GetRelativePath(savepath[i]));
	//	mysql_api.Background_Image_Map_Insert(GetRelativePath(savepath[0]), old_id, md5);
	//	mysql_api.Background_Image_Map_Insert(GetRelativePath(savepath[1]), old_id, md5);
	//	mysql_api.Background_Image_Map_Insert(GetRelativePath(savepath[2]), old_id, md5);
		mysql_api.Background_Image_Map_Group_Insert(relative_path, old_id, md5, entry_num);
		
		gettimeofday(&end, NULL);
		insert_time_use += (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000.0;
		++seqid;
	}
	mysql_api.Release_Database_Con();
	printf("Query image map table using time %.3fs \n", query_time_use/ 1000.0);
	printf("Insert image map table using time %.3fs \n", insert_time_use/ 1000.0);
	printf("General Image using time %.3fs \n", generator_time_use / 1000.0);
	*/
/********************************************************************/
/******************** 100m images ground true************************/
/*
	if(argc < 1)
	{
		fprintf(stderr,"Para Error!\n");
		fprintf(stderr,"Use: 1.copy path \n"); 
	}
	
	struct timeval start, end;
	float time_use;
	gettimeofday(&start, NULL);
	srand(100000);
	//srand((unsigned int)time(NULL));
	std::vector<unsigned int> image_id;
	const int kImageIdStart = 1, kImageIdEnd = 29145268;
	for(int i = kImageIdStart; i != kImageIdEnd; ++i)
	{
		 image_id.push_back(i);
	}
	random_shuffle(image_id.begin(), image_id.end());
	const int kMaxRandomNum = 100000;
	image_id.erase(image_id.begin() + kMaxRandomNum, image_id.end());

	CMySQL_API mysql_api("root", "root", "web_image");
	if(mysql_api.Init_Database_Con("127.0.0.1", 3306) != 0)
		return -1;
	for(std::vector<unsigned int>::iterator iter = image_id.begin(); iter != image_id.end(); ++iter)
	{
		 std::string image_path;
		 mysql_api.Image_Map_Table_Read_Path(*iter, image_path);
		 printf("id = %d, path = %s\n", *iter, image_path.c_str());
		 //CopyFile(image_path.c_str(), argv[1], *iter, ".jpg");
		 if(CopyFile(image_path.c_str(), argv[1], *iter, ".jpg"))
			  mysql_api.Image_Random_Insert(*iter);
	}
	mysql_api.Release_Database_Con();
	gettimeofday(&end, NULL);
	time_use += (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000.0;
	printf("Using time %.3fs \n", time_use/ 1000.0);
*/

/********************************************************************/

/********************generate ground true images**********************/

	if(argc < 3)
	{
		fprintf(stderr,"Para Error!\n");
		fprintf(stderr,"Use: 1.image path 2.copy path 2.new image path\n"); 
		return -1;
	}	

	struct timeval start, end;
	float time_use;
	gettimeofday(&start, NULL);

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
	//
	//const int kHaveGeneratedNum = 29300;
	//filepath.erase(filepath.begin(), filepath.begin() + kHaveGeneratedNum);
	
	std::string savepath(argv[3]);
	unsigned int init_nameid = 0;
	unsigned int step = 100000000;

	ImageGenerator generator;
	const int kMaxTestImageNum = 100000;
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
		generator.SaveNewImage(savepath, 11, init_nameid + 11 * step);
	//	alpha = -30;
	//	beta = 0;
	//	generator.ChangeContrastBrightness(alpha, beta);
	//	generator.SaveNewImage(savepath, init_nameid + 2 * step);
		alpha = 35;
		beta = 0;
		generator.ChangeContrastBrightness(alpha, beta);
		generator.SaveNewImage(savepath, 12, init_nameid + 12 * step);
	//	alpha = 40;
	//	beta = 0;
	//	generator.ChangeContrastBrightness(alpha, beta);
	//	generator.SaveNewImage(savepath, init_nameid + 4 * step);
		alpha = 0;
		beta = -25;
		generator.ChangeContrastBrightness(alpha, beta);
		generator.SaveNewImage(savepath, 13, init_nameid + 13 * step);
	//	alpha = 0;
	//	beta = -10;
	//	generator.ChangeContrastBrightness(alpha, beta);
	//	generator.SaveNewImage(savepath, init_nameid + 6 * step);
		alpha = 0;
		beta = 25;
		generator.ChangeContrastBrightness(alpha, beta);
		generator.SaveNewImage(savepath, 14, init_nameid + 14 * step);
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
		generator.SaveJpegQuality(savepath, 15, init_nameid + 15 * step, jpeg_quality);
	//	jpeg_quality = 10;
	//	generator.SaveJpegQuality(savepath, init_nameid + 20 * step, jpeg_quality);

		double mean = 0, std = 30; // 30, 50
		generator.AddGaussianNoiseSimple(mean,std);
		generator.SaveNewImage(savepath, 16, init_nameid + 16 * step);
	//	mean = 0, std = 28;
	//	generator.AddGaussianNoiseSimple(mean,std);
	//	generator.SaveNewImage(savepath, init_nameid + 12 * step);
		double noise_rate = 0.015;	// 0.05, 0.1
		generator.AddSaltPepperNoise(noise_rate);
		generator.SaveNewImage(savepath, 17, init_nameid + 17 * step);
	//	noise_rate = 0.03;
	//	generator.AddSaltPepperNoise(noise_rate);
	//	generator.SaveNewImage(savepath, init_nameid + 14 * step);

		double logo_alpha = 0;
		std::string logoname("../data/lena.jpg");
		generator.AddLogoFileName(logoname, logo_alpha, kLogoUpRight);
		generator.SaveNewImage(savepath, 18, init_nameid + 18 * step);
		generator.AddLogoFileName(logoname, logo_alpha, kLogoCenter);
		generator.SaveNewImage(savepath, 19, init_nameid + 19 * step);

		double crop_rate = 0.15;	// 0.2, 0.5
		generator.CropImage(crop_rate);
		generator.SaveNewImage(savepath, 20, init_nameid + 20 * step);
	//	crop_rate = 0.18;	// 0.2, 0.5
	//	generator.CropImage(crop_rate);
	//	generator.SaveNewImage(savepath, init_nameid + 18 * step);

		//CopyFile((*iter), argv[2], init_nameid, ".jpg");
	}
	gettimeofday(&end, NULL);
	time_use += (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec) / 1000.0;
	printf("Using time %.3fs \n", time_use/ 1000.0);


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

/**************************************************************/
	return 0;
}

