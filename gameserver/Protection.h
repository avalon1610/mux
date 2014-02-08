#ifndef _PROTECTION_H_
#define _PROTECTION_H_

typedef const char * LPCSTR;
typedef unsigned char uchar;

typedef struct
{
    unsigned long   Data1;
    unsigned short  Data2;
    unsigned short  Data3;
    unsigned char   Data4[8];
} GUID;

typedef GUID UUID;

class UnknownC00
{
public:
    UnknownC00(void);
    ~UnknownC00(void);

    void Unknown872();
    bool Unknown193();
    bool Unknown172(LPCSTR Unk183);
    void Unknown382();
    void Unknown726();

    char * Unk449;
private:
    UUID    Unk645;
    /* HWND Unk297;*/
    LPCSTR  Unk614;
    int     Unk349;
    int     qy;
    int     qx;
    char    Unk291[18];
    char    Unk739[200];
    char    txt[128];
    char    t[255];
    char    PRT_MSG[255];
    uchar   Unk129[6];

    // int64
    int     Unk147;
    int     Unk148;
    int     Unk149;
    int     Unk189;
    bool    IsMainFunction;
};

extern UnknownC00 CUnknown;

#endif
