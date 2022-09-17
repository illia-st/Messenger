#pragma once

#include <cstdint>
#include <unordered_map>
#include <list>
#include <memory>
#include <atomic>
#include <forward_list>
#include "../../Common/Header/CommunicationUnit.h"

namespace TCP {

    using ServerInfo = CommunicationUnit;
    using ConnectionsList = std::list<std::shared_ptr<CommunicationUnit>>;

    enum class KeepAlive {
        TRUE,
        FALSE
    };

    class IServer;

    class Server;
}
// we will inher from TCP, we need to have incapsulaated server info
    class TCP::IServer {
    protected:
        // I don't think that serv info needs synchro because
        // it will be only read during server's work
        ServerInfo &GetInfo();

        [[nodiscard]] const ServerInfo &GetInfo() const;

        virtual ConnectionsList &GetConnections();

        [[nodiscard]] virtual const ConnectionsList &GetConnections() const;

        void SetAliveStatus(KeepAlive if_live);

        [[nodiscard]] KeepAlive GetAliveStatus() const;

        virtual void AccessingNewConnections() = 0;

        virtual void LoadData() = 0;

    public:
        IServer();

        virtual ~IServer() = 0;
        explicit IServer(uint16_t port, KeepAlive if_live = KeepAlive::FALSE);

        virtual void SetNewPort(uint16_t port) = 0;

        [[nodiscard]] uint16_t GetPort() const;

        virtual int StartServer() = 0;

        virtual int StopServer() = 0;
    private:
        // Server info
//    using Client_Info = CommunicationUnit;
        std::atomic<KeepAlive> alive{KeepAlive::FALSE};
        ServerInfo info;
        ConnectionsList connections;
    };