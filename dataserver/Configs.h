#ifndef _CONFIG_H
#define _CONFIG_H

class MainConfig
{
public:
    MainConfig(){}
    ~MainConfig(){};
    bool Initialize();
    bool ReadMainInfo();
private:
    int DelayLoad;
};

extern MainConfig mcfg;
#endif
