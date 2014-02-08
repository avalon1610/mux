#include "log.h"
#include <string>

using namespace std;
using namespace boost;

CLOG::CLOG()
{
    InitLog();
    logging::add_common_attributes();
    using namespace logging::trivial;
    BOOST_LOG_SEV(lg,info) << "thread id: " << this_thread::get_id() << " Initialization succeeded";
}

CLOG::~CLOG()
{
}

void CLOG::InitLog()
{
    string path = bf::canonical(bf::current_path()).string();
    logging::register_simple_formatter_factory<log::trivial::severity_level,char>("Severity");
    logging::add_file_log(
        keywords::file_name = path + "/log/%Y-%m-%d.%N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0),
        keywords::format = "[%TimeStamp%](%Severity%):%Message%",
        keywords::min_free_space = 3 * 1024 * 1024);
    logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::debug);
    logging::add_console_log(cout,keywords::format="[%TimeStamp%](%Severity%):%Message%");
}
