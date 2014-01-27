#ifndef _LOG_PROC_H
#define _LOG_PROC_H

#include <iostream>
#include <string>
#include "boost.h"

class Log
{
public:
    Log();
    ~Log();

private:
    sources::severity_logger<logging::trivial::severity_level> lg;
    void InitLog();
};

#define LOG(x) BOOST_LOG_TRIVIAL(x)
#endif
