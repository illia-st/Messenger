#pragma once
#include <unordered_map>
#include <thread>
#include <mutex>
#include "IClient.h"

class TCP::Client : public TCP::IClient{
    using HostConnections = std::unordered_map<std::string, uint16_t>;
public:
    Client() = default;
    Client(uint32_t host, uint16_t port);
    Client(const std::string& IP, uint16_t port );
    int Connect() override;
    int Disconnect() override;
    void LoadData() override;
    int SendData(const std::string& msg, bool send_all = false);
    HostConnections GetHostConnections() const;
    HostConnections& GetHostConnections();
private:
    // this is the data which will be updated
    std::thread data_loader;
    std::mutex data_mutex;
    // here should be a win structure for socket
    HostConnections connections_on_host;
};