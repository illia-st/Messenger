#include "../Header/IClient.h"

#ifdef WIN32
#else
#endif

TCP::IClient::IClient(KeepAlive if_live): m_live_status(if_live) {}

TCP::IClient::IClient(uint32_t host, uint16_t port, KeepAlive if_live): m_live_status(if_live), m_host(port, host) {}

TCP::IClient::IClient(const std::string& IP, uint16_t port, KeepAlive if_live): m_live_status(if_live), m_host(port, inet_addr(IP.c_str())) {}

TCP::ServerInfo &TCP::IClient::GetHostInfo() {return m_host;}

TCP::KeepAlive TCP::IClient::GetAliveStatus() const {return m_live_status;}
