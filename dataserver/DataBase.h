#ifndef _DATA_BASE_H
#define _DATA_BASE_H
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <string>
#include <list>

using namespace std;
using namespace sql;

class ConnPool
{
public:
    virtual ~ConnPool();
    static ConnPool *GetInstance();
    bool ExecFormat(const char *query)
    {
        return Exec(query);
    }
    bool Exec(const char *query);
    int GetInt(const char *col);
    string GetString(const char *col);

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

    Connection *GetConnection();
    void ReleaseConnection(Connection *conn);

    Statement *stmt;
    ResultSet *res;

    inline void Clear()
    {
        if (stmt)
        {
            delete stmt;
            stmt = NULL;
        }
        if (res)
        {
            delete res;
            res = NULL;
        }
    }

    ConnPool(string url,string user,string password,int maxSize);
};

#endif
