#ifndef DATA_PROTOCOL_H
#define DATA_PROTOCOL_H
#include "DataSrvManager.h"
#include "GPackets.h"

void DSProtocolCore(DServerManager *,int,byte,byte *,int);
void DataServerLogin(DServerManager *,int,SDHP_SERVERINFO *);

#endif
