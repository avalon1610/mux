#ifndef _NETWORK_H
#define _NETWORK_H

#include "../dataserver/IOCP.h"

class Network : public IOCP
{
public:
    Network();
    virtual ~Network();

    bool Initialize();
};

extern Network Net;
#endif
