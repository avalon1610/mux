#ifndef _IOCP_H_
#define _IOCP_H_
#include <string>
#include <sys/epoll.h>

using namespace std;

typedef unsigned int UINT;

class IOCP
{
public:
    IOCP()
    {
        nPort = 0;
    }
    void StartServer(UINT);
    bool DataSend(int aIndex,unsigned char *Msg,unsigned int size);
protected:
    bool StartNetwork(UINT NumberOfThreads);
    inline void SetPort(UINT Port)
    {
        this->nPort = Port;
    }

    inline void SetName(string Name)
    {
        this->Name = Name;
    }
private:
    int efd; // epoll fd
    int sfd; // socket fd
    int i; // events no
    epoll_event *events;
    UINT nPort;
    string Name;
    int create_and_bind();
    int make_socket_non_blocking(int sfd);
    void Worker();
    void ListenWorker();
    void RecvWorker();
};

#endif
