#pragma once
#include "../Common/Header/Core.h"

class TCP::IClient{
public:
    IClient(KeepAlive if_live = KeepAlive::FALSE);
    IClient(const std::string& IP, uint16_t port, KeepAlive if_live = KeepAlive::FALSE);
    IClient(uint32_t host, uint16_t port, KeepAlive if_live = KeepAlive::FALSE);
    KeepAlive GetAliveStatus() const;
    virtual int Connect() = 0;
    virtual int Disconnect() = 0;
    virtual void LoadData() = 0;
protected:
    ServerInfo& GetHostInfo();
private:
    ServerInfo m_host;
    KeepAlive m_live_status {KeepAlive::FALSE};
};
