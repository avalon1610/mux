#ifndef _LOG_H_
#define _LOG_H_

#include "boost.h"

class LOG
{
public:
    LOG();
    virtual ~LOG();
private:
    sources::severity_logger<logging::trivial::severity_level> lg;
    void InitLog();
};

#define LOG(x) BOOST_LOG_TRIVIAL(x)
#endif
