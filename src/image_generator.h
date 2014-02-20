#ifndef IMAGE_GENERATOR_H
#define IMAGE_GENERATOR_H

#include <string>
#include <opencv2/core/core.hpp>


enum LogoLocation
{
	kLogoCenter,
	kLogoUpRight
};

class ImageGenerator
{
public:
	ImageGenerator();
	~ImageGenerator();

	bool Init(const std::string &image_name);

	void ChangeContrastBrightness(int alpha, int beta);
	void GaussianBlur(cv::Size &ksize);

	void AddGaussianNoise();
	void AddGaussianNoiseSimple(double mean, double std);
	void AddSaltPepperNoise(double rate);

	bool AddLogo(const std::string &logoname,const double alpha, const int logo_location);
	void CropImage(const double rate);

	void SaveNewImage(const std::string &save_name);
	void SaveNewImage(const std::string &save_path, unsigned int nameid);
	const cv::Mat &new_image() const { return new_image_; }
	const cv::Mat &image() const { return image_; }

private:
	cv::Mat image_;
	cv::Mat new_image_;
};
#endif
