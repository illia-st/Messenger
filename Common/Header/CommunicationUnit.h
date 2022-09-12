#pragma once

#include <cstdint>
#include <cstring>
#include <csignal>
#include <string>
#include "Core.h"
#include "Connection.h"


#ifdef WIN32
//#include <winsock>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

class CommunicationUnit{
public:
    CommunicationUnit();
    explicit CommunicationUnit(uint16_t port, uint32_t host = INADDR_ANY);
    void SetSocket(Socket sock);
    void CloseSocket();
    void SetConnectionStatus(Connection connection);
    Connection GetConnectionStatus() const;
    Socket& GetSocket();
    virtual int Disconnect();
    virtual void LoadData();
    void SendMsg(std::string_view msg);
    void Clear();
    [[nodiscard]] uint16_t GetPort() const;
    [[nodiscard]] uint32_t GetHost() const;
    [[nodiscard]] std::string GetIP() const;
protected:
    Socket m_sock;
    SockAddr_in addr;
private:
    Connection con {} ;
};