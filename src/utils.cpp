#include "utils.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

bool ReadFilePath(const char *filedir,std::vector<std::string> &filepath)
{
	DIR *dir;
	struct dirent *file; 	//readdir函数的返回值就存放在这个结构体中
	struct stat st;
	static int filenum = 0;
	if(!(dir = opendir(filedir)))
	{
		printf("error opendir %s!!!\n",filedir);
		return false;
	}
	while((file = readdir(dir)) != NULL)
	{
		if((strcmp(file->d_name,".") == 0) || (strcmp(file->d_name,"..") == 0))
      		continue;

		char tmp_path[1024];
		memset(tmp_path,0,1024);
		sprintf(tmp_path,"%s/%s",filedir,file->d_name);
		printf("%s\n",tmp_path);
		if(stat(tmp_path,&st) >= 0 && S_ISDIR(st.st_mode))
		{
			ReadFilePath(tmp_path,filepath);	// 子目录继续递归
		}
		else
		{	++filenum;
			char *sub = strrchr(tmp_path,'.');
			//char surfix[10];
			//memset(surfix,0,10);
			//strncpy(surfix,sub + 1, strlen(sub) - 1);

			//if (!strcmp(sub + 1,"jpg") || !strcmp(sub + 1,"JPG") || !strcmp(sub + 1,"jpeg") || !strcmp(sub + 1,"JPEG") 
            //|| !strcmp(sub + 1,"bmp") || !strcmp(sub + 1,"BMP") || !strcmp(sub + 1,"PNG") || !strcmp(sub + 1,"png") 
            //|| !strcmp(sub + 1,"pgm") || !strcmp(sub + 1,"ppm") || !strcmp(sub + 1,"TIF") || !strcmp(sub + 1,"tif") 
            //|| !strcmp(sub + 1,"tiff") || !strcmp(sub + 1,"TIF")) 
			if (!strcmp(sub + 1,"flv") || !strcmp(sub + 1,"jpg") || !strcmp(sub + 1,"JPG") || !strcmp(sub + 1,"jpeg") || !strcmp(sub + 1,"JPEG"))
			{
				std::string path(tmp_path);
				filepath.push_back(path);
			}
		}
	}
	closedir(dir);
	return true;
}

bool CopyFile(const std::string &infile, const std::string &outpath, unsigned int nameid, const std::string &postfix)
{
	char nameid_str[11];
	sprintf(nameid_str,"%u",nameid);	// 这里一定要使用%u代表无符号, %d代表有符号
	std::string outfile = outpath + "/" + nameid_str + postfix;	// 不一定使jpg时怎么处理

	int read_fd, write_fd;
	struct stat stat_buf;
	off_t offset = 0;
	if((read_fd = open(infile.c_str(), O_RDONLY)) < 0)
	{
		 perror("open");
		 return false;
	}
	fstat(read_fd, &stat_buf);
	if((write_fd = open(outfile.c_str(), O_WRONLY | O_CREAT, stat_buf.st_mode)) < 0)
	{
		 perror("open");
		 return false;
	}
	if(sendfile(write_fd, read_fd, &offset, stat_buf.st_size) < 0)
	{
		 perror("sendfile");
		 return false;
	}
	close(read_fd);
	close(write_fd);
	return true;
}
unsigned int GetFileId(const std::string &filepath)
{
	std::string::size_type slash_pos = filepath.find_last_of('/');
	std::string::size_type dot_pos = filepath.find_last_of('.');
	std::string id_str(filepath.substr(slash_pos + 1, dot_pos - slash_pos - 1));
	return atoi(id_str.c_str());
}

void GetFileName(const std::string &filepath, std::string &filename)
{
	std::string::size_type slash_pos = filepath.find_last_of('/');
	std::string::size_type dot_pos = filepath.find_last_of('.');
	filename.assign(filepath.substr(slash_pos + 1, dot_pos - slash_pos-1));
}

std::string GetRelativePath(const std::string &filepath)
{
	std::string::size_type pos = filepath.find_first_of('/',1); // pos = 1, to ignore the first '/'
	return filepath.substr(pos + 1,filepath.size() - pos -1);
}

void RemovePostfix(const std::string &filepath, std::string &new_path)
{
	std::string::size_type dot_pos = filepath.find_last_of('.');
	new_path.assign(filepath.substr(0, dot_pos));
}

bool ResizeImage(std::string &filepath, cv::Size dst_size, cv::Mat &dst_image)
{
	dst_image = cv::imread(filepath);
	if(dst_image.data == NULL)
	{
		fprintf(stderr,"Loading logo image failed\n");
		return false;
	}
	cv::resize(dst_image, dst_image, dst_size, 0, 0, cv::INTER_LINEAR);
	return true;
}
