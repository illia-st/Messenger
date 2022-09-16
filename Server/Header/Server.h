#include <thread>
#include <mutex>
#include <semaphore>
#include <thread>
#include <iostream>
#include "IServer.h"

namespace TCP {
#ifdef WIN32

#else

    class Server : public IServer {
    public:
        Server();
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
}

