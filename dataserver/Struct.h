#ifndef _STRUCT_H_
#define _STRUCT_H_
#include <string>
#include <sys/epoll.h>
#include "Defines.h"

using namespace std;
typedef unsigned short WORD;
typedef unsigned char BYTE;

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

enum SERVER_STATUS
{
    SRVS_OFFLINE,
    SRVS_ONLINE
};

struct SOCKET_CONTEXT
{
    BYTE buffer[MAX_IO_BUFFER_SIZE];
    BYTE buffer_second[MAX_IO_BUFFER_SIZE];
    epoll_event event;
};

struct OBJECTSTRUCT
{
    short index;
    char server_ip[16];
    SERVER_STATUS status;
    int fd;
    struct SOCKET_CONTEXT *SocketContext;
};

enum QUERY_PROCESS_TYPES
{
    QUERY_NULL = -100,
    QUERY_EOF = -101,
    QUERY_NO_COLUMN = -102,
    QUERY_ERROR = -103,
    MAX_COLUMN = 100
};

#endif
