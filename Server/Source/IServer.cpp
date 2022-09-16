#include "../Header/IServer.h"

namespace TCP {
    IServer::IServer(): info(ServerInfo()), connections(){}
    IServer::IServer(uint16_t port, KeepAlive if_live) {
        alive = if_live;
        info = ServerInfo(port);
    }
    IServer::~IServer(){connections.clear();}
    const ConnectionsList &IServer::GetConnections() const {
        return connections;
    }

    ConnectionsList &IServer::GetConnections() {
        return connections;
    }

    const ServerInfo &IServer::GetInfo() const {return info;}

    ServerInfo &IServer::GetInfo() {return info;}

    void IServer::SetAliveStatus(KeepAlive if_live) {alive = if_live;}

    KeepAlive IServer::GetAliveStatus() const {return alive;}

    uint16_t IServer::GetPort() const {return info.GetPort(); }
}