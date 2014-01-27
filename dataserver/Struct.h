#ifndef _STRUCT_H_
#define _STRUCT_H_
#include <string.h>

using namespace std;
typedef unsigned short WORD;

enum eSRV_TYPE
{
    NONE,
    ST_JOIN_SERVER,
    ST_DATA_SERVER,
    MAX_SRV_TYPES
};

struct MAIN_STRUCT
{
    string SQL_Host;
    string SQL_Db;
    string SQL_User;
    string SQL_Pass;
    int MAX_COLUMN;
    WORD JoinSrv_Port;
    WORD DataSrv_Port;
    int JoinSrv_UseMD5;
    int DataSrv_Count;
};

#endif