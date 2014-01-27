#ifndef _JS_SRV_MANAGER
#define _JS_SRV_MANAGER
#include "IOCP.h"

class JServerManager : public IOCP
{
public:
    JServerManager(){};
    virtual ~JServerManager(){};
private:
    void Initialize();
};

extern JServerManager JSrvManager;

#endif
