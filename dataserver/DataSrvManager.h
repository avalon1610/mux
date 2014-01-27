#ifndef DATA_SRV_MANAGER_H
#define DATA_SRV_MANAGER_H
#include "IOCP.h"
typedef unsigned char byte;

class DServerManager : public IOCP
{
public:
    DServerManager(void){}
    virtual ~DServerManager(void){}

    void OnRecv(int aIndex,byte HeadCode,byte *aRecv,int iSize);
};

//extern DServerManager DS_SrvManager[];

#endif
