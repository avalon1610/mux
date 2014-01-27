#include "Configs.h"
#include "ReadFileManager.h"
#include "LogProc.h"
#include "Struct.h"

MainConfig mcfg;
extern MAIN_STRUCT ms;

bool MainConfig::Initialize()
{
    return ReadMainInfo();
}

bool MainConfig::ReadMainInfo()
{
    LOG(info) << "DataServer Emulator Configure loading...";
    try
    {
        ms.SQL_Host = fm.GetString("Database","Server");
        ms.SQL_Db = fm.GetString("Database","DataBase");
        ms.SQL_User = fm.GetString("Database","Username");
        ms.SQL_Pass = fm.GetString("Database","Password");

        ms.JoinSrv_UseMD5 = fm.GetInt("Server","MD5Enabled");
        ms.DataSrv_Count = fm.GetInt("Server","DataServerCount");
        ms.JoinSrv_Port = fm.GetInt("Server","JoinServerPort");
        ms.DataSrv_Port = fm.GetInt("Server","DataServerPort");
    }
    catch(const std::exception &e)
    {
        LOG(error) << e.what();
        return false;
    }
    return true;
}
