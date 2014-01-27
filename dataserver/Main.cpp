#include "LogProc.h"
#include "FileManager.h"
#include "Struct.h"
#include "Configs.h"
#include "DataBase.h"
#include "Main.h"

using namespace boost;

MainClass m_app;
MAIN_STRUCT ms;
int main(int argc,char **argv)
{
    if (!fm.SetInfo())
        return -1;
    //thread thd(&MainTask);
    //thd.join();
    if (!mcfg.Initialize())
        return -1;
    ConnPool *cp = ConnPool::GetInstance();
    if (cp != NULL)
    {
        for (int i =1;i < MAX_SRV_TYPES;++i )
        {
            m_app.StartServer(i);
        }
        LOG(info) << "Network Connection Successfully Initialized.";
    }

    return 0;
}
