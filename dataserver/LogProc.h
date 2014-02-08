#ifndef _LOG_PROC_H
#define _LOG_PROC_H

//#include "boost.h"
#include "../comm/log.h"

class Log : public CLOG
{
public:
    Log(){}
    ~Log(){};

private:
    //sources::severity_logger<logging::trivial::severity_level> lg;
    //void InitLog();
};


#endif
