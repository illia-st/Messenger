#include "../Header/Server.h"

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
    TCP::Server::Server(): IServer() {}
    TCP::Server::Server(uint16_t port,KeepAlive if_live): IServer(port, if_live) {}
    int TCP::Server::StartServer() {
        Socket sock = socket(AF_INET, SOCK_STREAM, 0);
        std::cout << this->GetInfo().GetPort() << std::endl;
        if(sock == -1){
            // make loger
            int error = errno;
            return error;
        }
        int opt {1};
        if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt))){
            int error = errno;
            return error;
        }
        if(bind(sock, reinterpret_cast<sockaddr*>(&this->GetInfo().GetData()), sizeof(this->GetInfo())) == -1 ||
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
    int TCP::Server::StopServer() {
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

    void TCP::Server::LoadData() {
        using namespace std::chrono_literals;
        while(this->GetInfo().GetConnectionStatus() == Connection::OPENED){
            std::this_thread::sleep_for(200ms);
            std::cout << "We are running" << std::endl;
            ConnectionsList& cons {this->GetConnections()};
            std::scoped_lock lock(client_mutex);
//            std::cout << "Connections number " << cons.size() << std::endl;
            for(auto unit {cons.begin()}; unit != cons.end(); ){
//                auto client {dynamic_cast<ClientUnit*>(unit->get())};
//                std::cout << "Connection " << (*unit)->GetIP() << ":" << (*unit)->GetPort() << std::endl;
                std::cout << "Looped" << std::endl;
                auto client {unit->get()};
                auto result {client->LoadData()};
                if(result == nullptr){
//                    client->Disconnect();
//                    unit = cons.erase(unit);
                    continue;
                }
//                char * arr = reinterpret_cast<char *>(result);
//                std::cout << arr << std::endl;
                switch(static_cast<Flag>(result->GetFlag())){
                    case Flag::DefaultMsg:{
                        printMsg(result->GetInfo(), result->GetSize());
                        break;
                    }
                    case Flag::MessengerMsg:{
                        break;
                    }
                    default:
                        printMsg(reinterpret_cast<char *>(result), result->GetSize());
                        break;
                }
                ++unit;
            }
        }
    }
    void TCP::Server::AccessingNewConnections(){
        while(this->GetInfo().GetConnectionStatus() == Connection::OPENED) {
            SockAddr_in address;
            int addr_len {sizeof(address)};
            memset(&address, 0, sizeof(address));
            // check if it is non-blocking because non block m_flag is set
            std::cout << "I am before accepting client " << std::endl;
            Socket client_socket{accept(this->GetInfo().GetSocket(),
                                        reinterpret_cast<sockaddr *>(&address),
                                        reinterpret_cast<socklen_t*>(&addr_len))};

            std::cout << "I am after accepting client " << std::endl;
            if (this->GetInfo().GetConnectionStatus() != Connection::OPENED) {
                std::cout << "Return" << std::endl;
                return;
            }
            if (client_socket <= -1) {
                std::cout << "Bad client Socket " << std::endl;
                int error = errno;
                // make log about error
                continue;
            }
            std::cout << "Socket is fine" << std::endl;
            std::shared_ptr<CommunicationUnit> unit(
                    std::make_shared<CommunicationUnit>(CommunicationUnit(ntohs(address.sin_port),
                                                                   ntohl(address.sin_addr.s_addr))));
            unit->SetSocket(client_socket);
            unit->SetConnectionStatus(Connection::OPENED);
            std::cout << "Before lock" << std::endl;
            std::scoped_lock lock(client_mutex);
            std::cout << "Here it is a new connection from " << unit->GetIP() << ":" << unit->GetPort() << std::endl;
            IServer::GetConnections().pop_front(std::move(unit));

        }
    }

#endif

    // Set New port I will implement after Start and Stop
    TCP::ConnectionsList &TCP::Server::GetConnections() {
        std::scoped_lock lock(client_mutex);
        return IServer::GetConnections();
    }

    const TCP::ConnectionsList &TCP::Server::GetConnections() const {
        std::scoped_lock lock(const_cast<Server*>(this)->client_mutex);
        return IServer::GetConnections();
    }
    void TCP::Server::SetNewPort(uint16_t port) {std::cout << "Later" << std::endl;}
