#pragma once

#include <thread>
#include <mutex>
#include <semaphore>
#include <thread>
#include <iostream>
#include <fcntl.h>
#include "IServer.h"

#ifdef WIN32

#else

class TCP::Server : public TCP::IServer {
    public:
        Server();
        ~Server() = default;
        explicit Server(uint16_t port, KeepAlive if_live = KeepAlive::FALSE);
        void SetNewPort(uint16_t port) override;
        int StartServer() override;
        int StopServer() override;
    protected:
        std::mutex client_mutex;
        std::thread acceptor;
        std::thread data_loader;
        [[nodiscard]] const ConnectionsList & GetConnections() const override;
        ConnectionsList & GetConnections() override;
        void AccessingNewConnections() override;
        void LoadData() override;
    };

#endif

