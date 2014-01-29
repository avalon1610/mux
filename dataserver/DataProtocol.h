#ifndef _DATA_PROTOCOL_H
#define _DATA_PROTOCOL_H
#include "GPackets.h"

typedef unsigned short WORD;
typedef unsigned char BYTE;

struct SDHP_GETCHARLIST
{
    PBMSG_HEAD h;   // C1:01
    char Id[10];    // 3
    short Number;   // E
};

struct SDHP_DBCHAR_INFOSAVE
{
    PWMSG_HEAD h;   // C2:07
    char Name[11];  // 4
    short Level;    // 10
    BYTE Class; // 12
    int LevelUpPoint;   // 14
    int Exp;    // 18
    int Money;  // 20
    short Str;  // 24
    short Dex;  // 26
    short Vit;  // 28
    short Energy;   // 2A
    WORD Life;  // 2C
    WORD MaxLife;   // 2E
    WORD Mana;  // 30
    WORD MaxMana;   // 32
    BYTE dbInventory[3776]; // 34
    BYTE dbMagicList[180];  // 6F4
    BYTE MapNumber; // 7A8
    BYTE MapX;  // 7A9
    BYTE MapY;  // 7AA
    BYTE Dir;   // 7AB
    int PkCount;    // 7AC
    int PkLevel;    // 7B0
    int PkTime; // 7B4
    BYTE dbQuest[50];   // 7B8
    BYTE CharInfoSave;  // 7EA
    WORD Leadership;    // 7EC
    WORD ChatLitmitTime;    // 7EE
    int iFruitPoint;    // 7F0
    BYTE GenFamily;
    BYTE GenLevel;
    int GenRanking;
    int GenContribution;
    BYTE GenReward;
    int MasterLevel;
    int MasterPoints;
    int MasterExp;
    int MasterNextExp;
    BYTE NewQuest[600];
    BYTE TutorialReg;
    BYTE MercenaryReg;
    unsigned char ExpandedInventory;
};

#endif
