#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

extern bool ReadFilePath(const char *filedir,std::vector<std::string> &filepath);
extern bool CopyFile(const std::string &infile, const std::string &outpath, unsigned int nameid);
extern unsigned int GetFileId(const std::string &filepath);
extern void GetFileName(const std::string &filepath, std::string &filename);
extern std::string GetRelativePath(const std::string &filepath);
#endif
