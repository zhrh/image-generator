#include "image_generator.h"
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

ImageGenerator::ImageGenerator()
{

}

ImageGenerator::~ImageGenerator()
{

}

bool ImageGenerator::Init(const std::string &image_name)
{
	image_ = cv::imread(image_name);
	if(image_.rows < 30 || image_.cols < 30)
	{
		printf("Image is too small! Rows = %d, Cols = %d.\n",image_.rows,image_.cols);
		return false;
	}
	return true;
}

// param
//	alpha, contrast control, 取值[-100 100], 代表对比度增加和降低的百分率
//	beta, brightness control, 取值[-100 100], 代表亮度增加和降低的百分率 
void ImageGenerator::ChangeContrastBrightness(int alpha, int beta)
{
	// a new matrix is allocated only if A is not a B matrix. Otherwise, the existing matrix A is filled with zeros.
	new_image_ = cv::Mat::zeros(image_.size(), image_.type());
	alpha = alpha * 255 / 100; // 将百分率映射到灰度值
	beta = beta * 255 / 100;
	const int kContrastThreshold = 128;
	//printf("alpha = %d, beta = %d\n",alpha,beta);
	if(alpha > 0)
	{
	
		if(beta != 0)
		{
			image_.convertTo(new_image_, -1, 1, beta);
			if(alpha == 255)
			{
				for(int y = 0; y < new_image_.rows; ++y)
					for(int x = 0; x < new_image_.cols; ++x)
						for(int c = 0; c < 3; ++c)
							new_image_.at<cv::Vec3b>(y,x)[c] =
							cv::saturate_cast<uchar>(new_image_.at<cv::Vec3b>(y,x)[c] >= kContrastThreshold? 255:0);
				return;
			}
			for(int y = 0; y < new_image_.rows; ++y)
				for(int x = 0; x < new_image_.cols; ++x)
					for(int c = 0; c < 3; ++c)
						new_image_.at<cv::Vec3b>(y,x)[c] =
						cv::saturate_cast<uchar>(new_image_.at<cv::Vec3b>(y,x)[c] +
						(new_image_.at<cv::Vec3b>(y,x)[c] - kContrastThreshold) * (1 / (1 - alpha / 255.0) - 1));
		}
		else
		{
			if(alpha == 255)
			{
				for(int y = 0; y < image_.rows; ++y)
					for(int x = 0; x < image_.cols; ++x)
						for(int c = 0; c < 3; ++c)
							new_image_.at<cv::Vec3b>(y,x)[c] =
							cv::saturate_cast<uchar>(image_.at<cv::Vec3b>(y,x)[c] >= kContrastThreshold? 255:0);
				return;
			}
			for(int y = 0; y < image_.rows; ++y)
				for(int x = 0; x < image_.cols; ++x)
					for(int c = 0; c < 3; ++c)
						new_image_.at<cv::Vec3b>(y,x)[c] =
						cv::saturate_cast<uchar>(image_.at<cv::Vec3b>(y,x)[c] +
						(image_.at<cv::Vec3b>(y,x)[c] - kContrastThreshold) * (1 / (1 - alpha / 255.0) - 1));
		}

	}
	else if(alpha < 0)
	{
		for(int y = 0; y < image_.rows; ++y)
			for(int x = 0; x < image_.cols; ++x)
				for(int c = 0; c < 3; ++c)
					new_image_.at<cv::Vec3b>(y,x)[c] =
					cv::saturate_cast<uchar>(image_.at<cv::Vec3b>(y,x)[c] +
					(image_.at<cv::Vec3b>(y,x)[c] - kContrastThreshold) * alpha / 255.0);
		if(beta != 0)
			new_image_.convertTo(new_image_, -1, 1, beta);
	}
	else
		image_.convertTo(new_image_, -1, 1, beta);
}

void ImageGenerator::GaussianBlur(cv::Size &ksize)
{
	new_image_ = cv::Mat::zeros(image_.size(), image_.type());
	cv::GaussianBlur(image_, new_image_, ksize, 0, 0); // sigmaX=sigmaY=0, means that they are computed from ksize.  
}

// param:
//	mean, std means the gaussian noise's mean and std
void ImageGenerator::AddGaussianNoiseSimple(double mean, double std)
{
	new_image_ = cv::Mat::zeros(image_.size(), image_.type());
	cv::Mat noise_image = cv::Mat::zeros(image_.size(), image_.type());
	cv::randn(noise_image, mean, std);
	cv::add(image_,noise_image,new_image_);
}

void ImageGenerator::AddGaussianNoise()
{
	new_image_ = cv::Mat::zeros(image_.size(), image_.type());
	cv::Mat noise_image(image_.size(), CV_8U);
	randn(noise_image, cv::Scalar::all(128), cv::Scalar::all(5));
	// blur the noiseI a bit, kernel size is 3x3 and both sigma's are set to 0.5, 去掉试试看
	cv::GaussianBlur(noise_image, noise_image, cv::Size(3,3), 0.5, 0.5); 

	cv::Mat yuv_image;
	cv::cvtColor(image_, yuv_image, cv::COLOR_BGR2YCrCb);
	std::vector<cv::Mat> planes;
	cv::split(yuv_image, planes);

	const double kBrightnessGain = 0;
	const double kContrastGain = 1.7;
	cv::addWeighted(planes[0], kContrastGain, noise_image, 1, -128 + kBrightnessGain, planes[0]);
	const double kColorScale = 0.5;
	planes[1].convertTo(planes[1], planes[1].type(), kColorScale, 128 * (1 - kColorScale));
	planes[2].convertTo(planes[2], planes[2].type(), kColorScale, 128 * (1 - kColorScale));
	planes[0] = planes[0].mul(planes[0], 1.0 / 255);
	cv::merge(planes, yuv_image);
	cv::cvtColor(yuv_image, new_image_, cv::COLOR_YCrCb2BGR);
}

// param:
//	rate, 椒盐噪声占图像的比率
void ImageGenerator::AddSaltPepperNoise(double rate)
{
	cv::Mat saltpepper_noise = cv::Mat::zeros(image_.size(), CV_8U);
	cv::randu(saltpepper_noise, 0, 255);
	int threshold = rate / 2 * 255;
	cv::Mat black = saltpepper_noise < threshold;
	cv::Mat white = saltpepper_noise > (255 -  threshold);
	image_.copyTo(new_image_);
	new_image_.setTo(255,white);
	new_image_.setTo(0,black);
}

// param
// 	alpha, weight of the first image(beta = 1- alpha, weight of the second image)
//	type, add
bool ImageGenerator::AddLogo(const std::string &logoname,const double alpha, const int logo_location)
{
	cv::Mat logo_image = cv::imread(logoname);
	if(logo_image.data == NULL)
	{
		fprintf(stderr,"Loading logo image failed\n");
		return false;
	}
	cv::resize(logo_image, logo_image, cv::Size(image_.cols / 4, image_.rows / 4), 0, 0, cv::INTER_LINEAR);
	image_.copyTo(new_image_);
	double beta = 1 - alpha; // weight of the second image
	if(logo_location == kLogoUpRight)
	{
		cv::Mat image_roi = new_image_(cv::Rect(new_image_.cols / 8 * 5, new_image_.rows / 8, logo_image.cols, logo_image.rows));
		cv::addWeighted(image_roi, alpha, logo_image, beta, 0.0, image_roi);
	}
	else if(logo_location == kLogoCenter)
	{
		cv::Mat image_roi = new_image_(cv::Rect(new_image_.cols / 8 * 3, new_image_.rows / 8 * 3, logo_image.cols, logo_image.rows));
		cv::addWeighted(image_roi, alpha, logo_image, beta, 0.0, image_roi);
	}
	return true;
}
// param:
//	rate, 裁剪部分面积占全图的比率, 0 < rate < 1
void ImageGenerator::CropImage(const double rate)
{
	double roi_sidelen = sqrt(1.0 - rate);
	double upleft = (1.0 - roi_sidelen) / 2.0;
	cv::Mat image_roi = image_(cv::Rect(image_.cols * upleft, image_.rows * upleft, image_.cols * roi_sidelen, image_.rows * roi_sidelen));	
	image_roi.copyTo(new_image_);
}


void ImageGenerator::SaveNewImage(const std::string &save_name)
{
	cv::imwrite(save_name, new_image_);
}

void ImageGenerator::SaveNewImage(const std::string &save_path, unsigned int nameid)
{
	//sprintf(const_cast<char *>(save_name.c_str()), "%s/%d.jpg", save_path.c_str(), nameid);
	char nameid_str[11];
	sprintf(nameid_str,"%u",nameid);	// 这里一定要使用%u代表无符号, %d代表有符号
	std::string save_name = save_path + "/" + nameid_str + ".jpg";	// 不一定使jpg时怎么处理
	//printf("savename = %s\n",save_name.c_str());
	cv::imwrite(save_name, new_image_);
}
