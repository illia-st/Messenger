#include "../Header/Server.h"
namespace TCP {
    namespace {
        void printMsg(const char * arr, size_t N){
            for(size_t i{}; i < N; ++i){
                std::cout << arr[i];
            }
            std::cout << std::endl;
        }
    }
#ifdef WIN32

#else
    Server::Server(): IServer() {}
    Server::Server(uint16_t port,KeepAlive if_live): IServer(port, if_live) {}
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
//                auto client {dynamic_cast<ClientUnit*>(unit->get())};
                auto client {unit->get()};
                auto result {client->LoadData()};
                if(result == nullptr){
                    client->Disconnect();
                    unit = cons.erase(unit);
                    continue;
                }
                switch(static_cast<Flag>(result->GetFlag())){
                    case Flag::DefaultMsg:{
                        printMsg(result->GetInfo(), result->GetSize());
                        break;
                    }
                    case Flag::MessengerMsg:{
                        break;
                    }
                    default:
                        printMsg(result->GetInfo(), result->GetSize());
                }
                ++unit;
            }
        }
    }
    void Server::AccessingNewConnections(){
        while(this->GetInfo().GetConnectionStatus() == Connection::OPENED) {
            SockAddr_in address;
            memset(&address, 0, sizeof(address));
            // check if it is non-blocking because non block m_flag is set
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
                    std::make_shared<CommunicationUnit>(CommunicationUnit(ntohs(address.sin_port),
                                                                   ntohl(address.sin_addr.s_addr))));
            std::scoped_lock lock(client_mutex);
            GetConnections().push_back(std::move(unit));
        }
    }

#endif

    // Set New port I will implement after Start and Stop
    ConnectionsList &Server::GetConnections() {
        std::scoped_lock lock(client_mutex);
        return IServer::GetConnections();
    }

    const ConnectionsList &Server::GetConnections() const {
        std::scoped_lock lock(const_cast<Server*>(this)->client_mutex);
        return IServer::GetConnections();
    }
    void Server::SetNewPort(uint16_t port) {std::cout << "Later" << std::endl;}
}