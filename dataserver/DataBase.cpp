#include "DataBase.h"
#include "LogProc.h"
#include "Configs.h"
#include "Struct.h"


using namespace std;
using namespace sql;

ConnPool *ConnPool::connPool = NULL;
extern MAIN_STRUCT ms;

ConnPool::ConnPool(string url,string username,string password,int maxSize)
{
    this->maxSize = maxSize;
    this->curSize = 0;
    this->username = username;
    this->password = password;
    this->url = url;
    this->ready = false;
    try
    {
        this->driver = mysql::get_driver_instance();
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "sql driver error:" << e.what();
    }
    catch(runtime_error &e)
    {
        LOG(error) << "sql runtime error:" << e.what();
    }
    this->InitConnection(maxSize/2);
    this->ready = true;
}

ConnPool *ConnPool::GetInstance()
{
    if (connPool == NULL)
    {
        connPool = new ConnPool(ms.SQL_Host,ms.SQL_User,ms.SQL_Pass,50);
    }

    return (connPool->ready) ? connPool : NULL;
}

void ConnPool::InitConnection(int iInitialSize)
{
    Connection *conn;
    pthread_mutex_lock(&lock);
    for (int i=0; i < iInitialSize; i++)
    {
        conn = this->CreateConnection();
        if (conn)
        {
            connList.push_back(conn);
            ++(this->curSize);
        }
        else
        {
            LOG(error) << "Init Connection Error!";
        }
    }
    pthread_mutex_unlock(&lock);
}

Connection *ConnPool::CreateConnection()
{
    Connection *conn;
    try
    {
        conn = driver->connect(this->url,this->username,this->password);
        return conn;
    }
    catch(SQLException &e)
    {
        LOG(error) << "CreateConnection sql Error:" << e.what();
        return NULL;
    }
    catch(runtime_error &e)
    {
        LOG(error) << "CreateConnection runtime error:" << e.what();
        return NULL;
    }
}

Connection *ConnPool::GetConnection()
{
    Connection *conn;
    pthread_mutex_lock(&lock);
    if (connList.size() > 0)
    {
        conn = connList.front();
        connList.pop_front();

        if (conn->isClosed())
        {
            delete conn;
            conn = this->CreateConnection();
        }

        if (conn == NULL)
        {
            --curSize;
        }

        pthread_mutex_unlock(&lock);
        return conn;
    }
    else
    {
        if (curSize < maxSize)
        {
            conn = this->CreateConnection();
            if (conn)
            {
                ++curSize;
                pthread_mutex_unlock(&lock);
                return conn;
            }
            else
            {
                pthread_mutex_unlock(&lock);
                return NULL;
            }
        }
        else
        {
            pthread_mutex_unlock(&lock);
            return NULL;
        }
    }
}

void ConnPool::ReleaseConnection(Connection *conn)
{
    if (conn)
    {
        pthread_mutex_lock(&lock);
        connList.push_back(conn);
        pthread_mutex_unlock(&lock);
    }
}

ConnPool::~ConnPool()
{
    this->DestoryConnPool();
}

void ConnPool::DestoryConnPool()
{
    list<Connection *>::iterator iter;
    pthread_mutex_lock(&lock);
    for (iter = connList.begin(); iter != connList.end(); ++iter)
    {
        this->DestoryConnection(*iter);
    }
    curSize = 0;
    connList.clear();
    pthread_mutex_unlock(&lock);
}

void ConnPool::DestoryConnection(Connection *conn)
{
    if (conn)
    {
        try
        {
            conn->close();
        }
        catch(SQLException &e)
        {
            LOG(error) << "DestoryConnection sql error:" << e.what();
        }
        catch(std::exception &e)
        {
            LOG(error) << "DestoryConnection error:" << e.what();
        }

        delete conn;
    }
}

bool ConnPool::Exec(const char *query)
{
    Clear();
    Connection *conn;
    try
    {
        conn = GetConnection();
        stmt = conn->createStatement();

        LOG(info) << "[SQL] " << query;
        res = stmt->executeQuery(query);
    }
    catch (SQLException &e)
    {
        LOG(error) << "[SQL] exec error:" << e.getErrorCode()
            << "SQLState: " << e.getSQLState();
        ReleaseConnection(conn);
        return false;
    }
    ReleaseConnection(conn);
    return true;
}
string ConnPool::GetString(const char *col)
{
}

int ConnPool::GetInt(const char *col)
{
    if (res == NULL)
        return QUERY_ERROR;
    if (res->getRow() == 0)
        return QUERY_NO_COLUMN;
    int result;
    try
    {
        result = res->getInt(col);
    }
    catch (SQLException &e)
    {
        LOG(error) << "[SQL] getint error:" << e.getErrorCode()
            << "col:" << col;
        result = QUERY_NULL;
    }
    return result;
}


