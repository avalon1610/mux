#include "FileManager.h"
#include "LogProc.h"

using namespace std;
using namespace boost;

FileManager fm;

bool FileManager::SetInfo()
{
    bf::path filename = "./DataServer.conf";
    if (CheckFile(filename))
    {
        LOG(info) << "File Manager Loaded Successfully.";
        bp::ini_parser::read_ini(filename.string(),pt);
        return true;
    }
    else
    {
        LOG(error) << "Data Server conf file not found!";
        return false;
    }
}

bool FileManager::CheckFile(bf::path path)
{
    return (bf::exists(path)) ? true : false;
}

int FileManager::GetInt(const char *section,const char *key)
{
    return pt.get<int>(str(format("%1%.%2%") % section % key));
}

string FileManager::GetString(const char *section,const char *key)
{
    return pt.get<string>(str(format("%1%.%2%") % section % key));
}

