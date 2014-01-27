#include "DataSrvManager.h"
#include "DSProtocol.h"

//DServerManager DS_SrvManager[];

void DServerManager::OnRecv(int aIndex,byte HeadCode,byte *aRecv,int iSize)
{
    DSProtocolCore(this,aIndex,HeadCode,aRecv,iSize);
}
