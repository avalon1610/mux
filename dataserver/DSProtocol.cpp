#include "DSProtocol.h"
#include "DataBase.h"
#include "DataProtocol.h"

void DSProtocolCore(DServerManager *thisManager,int aIndex,byte HeadCode,byte *aRecv,int iSize)
{
    switch (HeadCode)
    {
        case 0x00:
            DataServerLogin(thisManager,aIndex,(SDHP_SERVERINFO *)aRecv);
            break;
        case 0x01:
            //JGPGetCharList(thisManager,aIndex,(SDHP_GETCHARLIST *)aRecv);
            break;
        case 0x04:
            //JGCharacterCreateRequest(thisManager,aIndex,(SDHP_CHEATECHAR *)aRecv);
            break;
        case 0x05:
            //JGCharDelRequest(thisManager,aIndex,(SDHP_CHARDELETE *)aRecv);
            break;
        case 0x06:
            //JGGetCharacterInfo(thisManager,aIndex,(SDHP_DBCHARINFOREQUEST *)aRecv);
            break;
        case 0x07:
            //GJSetCharacterInfo(thisManager,aIndex,(SDHP_DBCHAR_INFOSAVE *)aRecv);
            break;
        case 0x08:
            break;
        case 0x09:
            break;
        case 0x11:
            break;
        case 0x12:
            break;
        case 0x13:
            break;
        case 0x14:
            break;
        case 0x52:
            break;
        case 0x55:
            break;
        case 0x56:
            break;
        case 0x57:
            break;
        case 0x60:
            break;
        case 0x80:
            break;
        case 0x81:
            break;
        case 0x83:
            break;
        case 0x84:
            break;
        case 0x85:
            break;
        case 0x86:
            break;
        case 0x87:
            break;
        case 0x88:
            break;
        case 0x89:
            break;
        case 0xB0:
            break;
        case 0xB1:
            break;
        case 0xCF:
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
