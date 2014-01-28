#include "DSProtocol.h"
#include "DataBase.h"

void DSProtocolCore(DServerManager *thisManager,int aIndex,byte HeadCode,byte *aRecv,int iSize)
{
    switch (HeadCode)
    {
        case 0x00:
            DataServerLogin(thisManager,aIndex,(SDHP_SERVERINFO *)aRecv);
            break;
    }

}

void DataServerLogin(DServerManager *thisManager,int aIndex,SDHP_SERVERINFO *Msg)
{
    SDHP_RESULT Result = {0};
    Result.Result = 1;
    ConnPool *cp = ConnPool::GetInstance();
    if (cp != NULL && cp->ExecFormat("SELECT ItemCount FROM GameServerInfo WHERE Number=0"))
    {
        Result.ItemCount = cp->GetInt("ItemCount");
    }
    thisManager->DataSend(aIndex,(byte *)&Result,Result.h.Size);
};
