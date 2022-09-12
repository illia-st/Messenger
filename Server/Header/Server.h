#include <thread>
#include <mutex>
#include <semaphore>
#include <thread>
#include <iostream>
#include "IServer.h"
#include "ClientUnit.h"

namespace TCP {
#ifdef WIN32

#else

    class Server : public IServer {
    public:
        Server();
        Server(uint16_t port);
        void SetNewPort(uint16_t) override;
        int StartServer() override;
        int StopServer() override;
    protected:
        [[nodiscard]] const ConnectionsList & GetConnections() const override;
        ConnectionsList & GetConnections() override;
        void AccessingNewConnections() override;
        void LoadData() override;
        void SendAllFrom(ClientUnit* ptr, std::string_view msg, const std::thread::id& t_id);
    private:
        std::mutex client_mutex;
        std::thread acceptor;
        std::thread data_loader;
    };

#endif
}

