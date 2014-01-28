#include "IOCP.h"
#include "LogProc.h"
#include "JoinSrvManager.h"
#include "DataSrvManager.h"
#include "Struct.h"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include "boost.h"

extern MAIN_STRUCT ms;
#define MAXEVENTS 64

int IOCP::create_and_bind()
{
    struct addrinfo hints;
    memset(&hints,0,sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int sfd;
    std::string port = boost::str(boost::format("%1%") % nPort);

    struct addrinfo *result,*rp;
    int s = getaddrinfo(NULL,port.c_str(),&hints,&result);
    if (s)
    {
        LOG(error) << "[IOCP] getaddrinfo error:" << gai_strerror(s);
        return -1;
    }
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family,rp->ai_socktype,rp->ai_protocol);
        if (sfd == -1)
            continue;
        s = bind(sfd,rp->ai_addr,rp->ai_addrlen);
        if (s == 0)
        {
            // managed to bind successfully
            break;
        }
        close(sfd);
    }

    if (rp == NULL)
    {
        LOG(error) << "[IOCP] could not bind";
        return -1;
    }

    freeaddrinfo(result);
    return sfd;
}

int IOCP::make_socket_non_blocking(int sfd)
{
    int flags = fcntl(sfd,F_GETFL,0);
    if (flags == -1)
    {
        LOG(error) << "[IOCP] fcntl get  error";
        return -1;
    }
    flags |= O_NONBLOCK;
    int s = fcntl(sfd,F_SETFL,flags);
    if (s == -1)
    {
        LOG(error) << "[IOCP] fcntl set error";
        return -1;
    }

    return 0;
}

void IOCP::Worker()
{
    int n;
    while(1)
    {
        n = epoll_wait(efd,events,MAXEVENTS,-1);
        for (i = 0; i < n; ++i)
        {
            if ((events[i].events & EPOLLERR) ||
                (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN)))
            {
                LOG(error) << "[IOCP Worker] epoll error";
                close(events[i].data.fd);
                continue;
            }
            else if (sfd == events[i].data.fd)
            {
                // we have a notification on the listening socket, which
                // means one or more incoming connections.
                boost::thread conn_t(boost::bind(&IOCP::ListenWorker,this));
                conn_t.join();
            }
            else
            {
                // We have data on the fd waiting to be read. Read and
                // display it. We must read whatever data is available
                // completely, as we are running in edge-triggered mode
                // and won't get a notification again for the same data.
                boost::thread recv_t(boost::bind(&IOCP::RecvWorker,this));
                recv_t.join();
            }
        }
    }
    free(events);
    close(sfd);
}

void IOCP::ListenWorker()
{
    while(1)
    {
        struct sockaddr in_addr;
        char hbuf[NI_MAXHOST],sbuf[NI_MAXSERV];

        socklen_t in_len = sizeof in_addr;
        int infd = accept(sfd,&in_addr,&in_len);
        if (infd == -1)
        {
            if ((errno == EAGAIN) ||
                (errno == EWOULDBLOCK))
            {
                // we have processed all incoming connections
                break;
            }
            else
            {
                LOG(error) << "[IOCP ListenWorker] accept error:" << errno;
                break;
            }
        }

        int s = getnameinfo(&in_addr,in_len,hbuf,sizeof hbuf,
                sbuf,sizeof sbuf,NI_NUMERICHOST|NI_NUMERICSERV);
        if (s == 0)
            LOG(info) << "[IOCP ListenWorker] Accepted conn on descriptor "
                << infd << "(host=" << hbuf << ",port=" << sbuf << ")";

        // Make the incoming socket non-blocking and add it to the
        // list of fds to monitor
        s = make_socket_non_blocking(infd);
        if (s == -1)
        {
            LOG(debug) << "[IOCP ListenWorker] make socket non blocking error";
            abort();
        }

        struct epoll_event event;
        event.data.fd = infd;
        event.events = EPOLLIN|EPOLLET;
        s = epoll_ctl(efd,EPOLL_CTL_ADD,infd,&event);
        if (s == -1)
        {
            LOG(error) << "[IOCP ListenWorker] epoll_ctl error";
            abort();
        }
    }
}

void IOCP::RecvWorker()
{
    int done = 0;
    while(1)
    {
        ssize_t count;
        char buf[1024] = {0};

        count = read(events[i].data.fd,buf,sizeof buf);
        if (count == -1)
        {
            // If errno == EAGAIN, that means we have read all
            // data. break to the main loop
            if (errno != EAGAIN)
            {
                LOG(error) << "[IOCP RecvWorker] read error:" << errno;
                done = 1;
            }
            break;
        }
        else if (count == 0)
        {
            // End of file. The remote has closed the connection.
            done = 1;
            break;
        }

        // Writer the buffer to standard output
        int s = write(1,buf,count);
        if (s == -1)
        {
            LOG(error) << "[IOCP RecvWorker] write error:" << errno;
            abort();
        }
    }

    if (done)
    {
        LOG(info) << "Closed connection on descriptor " << events[i].data.fd;
        // Closing the descriptor will make epoll remove it
        // from the set of descriptor which are monitored.
        close(events[i].data.fd);
    }
}

bool IOCP::StartNetwork(UINT NumberOfThreads)
{
   int sfd = create_and_bind();
   if (sfd == -1)
        return false;

   int s = make_socket_non_blocking(sfd);
   if (s == -1)
       return false;

    s = listen(sfd,SOMAXCONN);
    if (s == -1)
    {
        LOG(error) << "[IOCP] listen error";
        return false;
    }

    efd = epoll_create1(0);
    if (efd == -1)
    {
        LOG(error) << "[IOCP] epoll_create error";
        return false;
    }

    epoll_event event;

    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET;
    s = epoll_ctl(efd,EPOLL_CTL_ADD,sfd,&event);
    if (s == -1)
    {
        LOG(error) << "[IOCP] epoll_ctl error";
        return false;
    }

    events = (epoll_event *)calloc(MAXEVENTS,sizeof event);

    boost::thread t(boost::bind(&IOCP::Worker,this));
}

void IOCP::StartServer(UINT SrvType)
{
    switch(SrvType)
    {
        case ST_JOIN_SERVER:
            JSrvManager.SetPort(ms.JoinSrv_Port);
            JSrvManager.SetName("JoinServer");
            if (!JSrvManager.StartNetwork(2))
                abort();
            break;
        case ST_DATA_SERVER:
            string temp;
            DServerManager *DS_SrvManager = new DServerManager[ms.DataSrv_Count];
            for (int i = 0; i < ms.DataSrv_Count;++i)
            {
                temp = boost::str(boost::format("DataServer[%1%]") % (i+1));
                DS_SrvManager[i].SetPort((i*2)+ms.DataSrv_Port);
                DS_SrvManager[i].SetName(temp);
                DS_SrvManager[i].StartNetwork(16);
            }
            break;
    }

}

bool IOCP::DataSend(int aIndex,unsigned char *Msg,unsigned int size)
{
}
