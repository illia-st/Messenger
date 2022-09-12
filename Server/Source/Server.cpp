#include "../Header/Server.h"
namespace TCP {
#ifdef WIN32

#else
    int Server::StartServer() {
        Socket sock = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
        if(sock == -1){
            // make loger
            int error = errno;
            return error;
        }
        if(bind(sock, reinterpret_cast<sockaddr*>(&this->GetInfo()), sizeof(this->GetInfo())) == -1 ||
            listen(sock, SOMAXCONN) == -1){
            // make loger
            int error = errno;
            return error;
        }
        this->GetInfo().SetSocket(sock);// Connection Status is automatically updated
        // run connection acceptor
        acceptor = std::thread(&Server::AccessingNewConnections, this);
        // run data loader. This thread will receive msg from clients
        data_loader = std::thread(&Server::LoadData, this);
        return 1;
    }
    int Server::StopServer() {
        if(this->GetInfo().GetConnectionStatus() == Connection::CLOSED){
            return 1;
        }
        this->GetInfo().SetConnectionStatus(Connection::CLOSED);
        for(auto& clients: GetConnections()){
            if(clients->Disconnect() != 1){
                // make log about error
            }
        }
        GetConnections().clear();
        acceptor.join();
        data_loader.join();
        switch(GetAliveStatus()){
            case KeepAlive::FALSE: exit(EXIT_SUCCESS);
            case KeepAlive::TRUE: return 1;
        }
    }

    void Server::LoadData() {
        using namespace std::chrono_literals;
        while(this->GetInfo().GetConnectionStatus() == Connection::OPENED){
            std::this_thread::sleep_for(100ms);
            std::scoped_lock lock(client_mutex);
            ConnectionsList& cons {this->GetConnections()};
            for(auto unit {cons.begin()}; unit != cons.end(); ){
                auto client {dynamic_cast<ClientUnit*>(unit->get())};
                if(client != nullptr){
                    client->LoadData();
                    if(client->GetConnectionStatus() == Connection::CLOSED){
                        unit = cons.erase(unit);
                        continue;
                    }
                    std::string_view msg {client->GetIP() + ":" + std::to_string(client->GetPort()) + "> " + std::string(client->GetLastMsg())};
                    std::cout << msg;
                    std::this_thread::get_id();
                    SendAllFrom(*unit, msg, std::this_thread::get_id());
                }
                else {
                    (*unit)->LoadData();
                    if((*unit)->GetConnectionStatus() == Connection::CLOSED){
                        unit = cons.erase(unit);
                        continue;
                    }
                }
                ++unit;
            }
        }
    }
    void Server::AccessingNewConnections(){
        while(this->GetInfo().GetConnectionStatus() == Connection::OPENED) {
            SockAddr_in address;
            memset(&address, 0, sizeof(address));
            // check if it is non-blocking because non block flag is set
            Socket client_socket{accept(this->GetInfo().GetSocket(),
                                        reinterpret_cast<sockaddr *>(&address), nullptr)};
            if (this->GetInfo().GetConnectionStatus() != Connection::OPENED) {
                return;
            }
            if (client_socket <= -1) {
                int error = errno;
                // make log about error
                continue;
            }
            std::shared_ptr<CommunicationUnit> unit(
                    std::make_shared<CommunicationUnit>(ClientUnit(ntohs(address.sin_port),
                                                                   ntohl(address.sin_addr.s_addr))));
            std::scoped_lock lock(client_mutex);
            GetConnections().push_back(std::move(unit));
        }
    }

    void Server::SendAllFrom(ClientUnit* ptr, std::string_view msg, const std::thread::id& t_id) {
        if(t_id != data_loader.get_id()){
            client_mutex.lock();
        }

        ConnectionsList& cons {this->GetConnections()};

        for(auto unit {cons.begin()}; unit != cons.end(); ){
            auto client = dynamic_cast<ClientUnit*>(unit->get());
            if(client == nullptr || *client == *ptr) continue;
            unit->get()->SendMsg(msg);
        }
        if(t_id != data_loader.get_id()){
            client_mutex.unlock();
        }
    }

#endif
    Server::Server(): IServer() {}
    Server::Server(uint16_t port): IServer(port) {}
    // Set New port I will implement after Start and Stop
    ConnectionsList &Server::GetConnections() {
        std::scoped_lock lock(client_mutex);
        return IServer::GetConnections();
    }

    const ConnectionsList &Server::GetConnections() const {
        std::scoped_lock lock(const_cast<Server*>(this)->client_mutex);
        return IServer::GetConnections();
    }
}