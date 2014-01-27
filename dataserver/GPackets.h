#ifndef _GPACKET_H_
#define _GPACKET_H_

typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char byte;
#define SET_NUMBERH(x) ((byte)((ulong)(x)>>(ulong)8))
#define SET_NUMBERL(x) ((byte)((ulong)(x)&0xFF))

struct PBMSG_HEAD
{
    byte c;
    byte Size;
    byte HeadCode;

    void Set(byte head,byte size)
    {
        this->c = 0xC1;
        this->Size = size;
        this->HeadCode = head;
    }
};

struct PWMSG_HEAD
{
    byte c;
    byte SizeH;
    byte SizeL;
    byte HeadCode;

    void Set(byte head,int size)
    {
        this->c = 0xC2;
        this->SizeH = SET_NUMBERH(size);
        this->SizeL = SET_NUMBERL(size);
        this->HeadCode = head;
    }
};
struct PBMSG_HEAD2
{
    byte c;
    byte Size;
    byte HeadCode;
    byte SubCode;

    void Set(byte headcode,byte subcode,byte size)
    {
        this->c = 0xC1;
        this->Size = size;
        this->HeadCode = headcode;
        this->SubCode = subcode;
    }
};

struct PBMSG_DEFAULT2
{
    PBMSG_HEAD h;
    byte subcode;
};

struct SDHP_SERVERINFO
{
    struct PBMSG_HEAD h;
    ushort Port;
    char ServerName[64];
    ushort ServerCode;
};

struct SDHP_RESULT
{
    struct PBMSG_HEAD h;
    byte Result;
    ulong ItemCount;
};

#endif
