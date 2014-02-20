#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

extern bool ReadFilePath(const char *filedir,std::vector<std::string> &filepath);
extern bool CopyFile(const std::string &infile, const std::string &outpath, unsigned int nameid);
#endif
