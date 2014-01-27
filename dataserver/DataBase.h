#ifndef _DATA_BASE_H
#define _DATA_BASE_H
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <string>
#include <list>

using namespace std;
using namespace sql;

class ConnPool
{
public:
    virtual ~ConnPool();

    Connection *GetConnection();
    void ReleaseConnection(Connection *conn);
    static ConnPool *GetInstance();

private:
    int curSize;
    int maxSize;
    string username;
    string password;
    string url;
    list<Connection *> connList;
    pthread_mutex_t lock;
    static ConnPool *connPool;
    Driver *driver;
    Connection *CreateConnection();
    void InitConnection(int iInitialSize);
    void DestoryConnection(Connection *conn);
    void DestoryConnPool();
    bool ready;

    ConnPool(string url,string user,string password,int maxSize);
};

#endif
