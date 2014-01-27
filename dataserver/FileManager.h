#ifndef _FILE_MANAGER_H
#define _FILE_MANAGER_H

#include "boost.h"

class FileManager
{
public:
    bool SetInfo();
    bool CheckFile(bf::path);
    int GetInt(const char*,const char*);
    std::string GetString(const char *,const char *);

private:
    bp::ptree pt;
};

extern FileManager fm;
#endif
