
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
        fcntl(sock, F_SETFL, fcntl(sock, F_GETFL)|O_NONBLOCK);
        this->GetInfo().SetSocket(sock);// Connection Status is automatically updated
        // run connection acceptor
        acceptor = std::thread(&Server::AccessingNewConnections, this);
        // run data loader. This thread will receive msg from clients
        data_loader = std::thread(&Server::LoadData, this);
        return 1;
    }
    int TCP::Server::StopServer() {
        std::cout << "I am stopping the server" << std::endl;
        if(this->GetInfo().GetConnectionStatus() == Connection::CLOSED){
            return 1;
        }
        this->GetInfo().SetConnectionStatus(Connection::CLOSED);
        std::cout << "The status is set" << std::endl;
        for(auto& clients: GetConnections()){
            if(clients->Disconnect() != 1){
                // make log about error
            }
        }
        GetConnections().clear();
        std::cout << "All the clients are disconnected now. Proof " << GetConnections().size() << std::endl;
        this->GetInfo().CloseSocket();
        acceptor.join();
        data_loader.join();
        std::cout << "Threads are being joined now" << std::endl;
        switch(GetAliveStatus()){
            case KeepAlive::FALSE: exit(EXIT_SUCCESS);
            case KeepAlive::TRUE: return 1;
        }
    }

    void TCP::Server::LoadData() {
#define accepted_buf result.first
#define accepted_bytes result.second
        using namespace std::chrono_literals;
        while(this->GetInfo().GetConnectionStatus() == Connection::OPENED){
            std::this_thread::sleep_for(200ms);
            ConnectionsList& cons {this->GetConnections()};
            std::scoped_lock lock(client_mutex);
            for(auto unit {cons.begin()}; unit != cons.end(); ){
                auto client {unit->get()};
                auto result {client->LoadData()};
                if(accepted_buf == nullptr){
                    goto INCREMENTOR;
                }
                std::cout << "A message from " << (*unit)->GetIP() << ":" << (*unit)->GetPort() << std::endl;

                switch(static_cast<Flag>(accepted_buf->GetFlag())){
                    case Flag::DefaultMsg:{
                        //// A message which is suitable for the server and can be parsed correctly by buffer interface
                        printMsg(accepted_buf->GetInfo(), accepted_buf->GetSize());
                        break;
                    }
                    case Flag::MessengerMsg:{
                        //// A message which is suitable for the server and can be parsed correctly by buffer interface
                        break;
                    }
                    case Flag::Disconnect:{
                        std::cout << "A disconnect from " << (*unit)->GetIP() << ":" << (*unit)->GetPort() << std::endl;
                        client->Disconnect();
                        unit = cons.erase(unit);
                        break;
                    }
                    default:
                        printMsg(reinterpret_cast<char *>(accepted_buf), accepted_bytes);
                        break;
                }
                delete [] reinterpret_cast<char *>(accepted_buf);
                INCREMENTOR:
                ++unit;
            }
        }
    }
    void TCP::Server::AccessingNewConnections(){
    using namespace std::chrono_literals;
        while(this->GetInfo().GetConnectionStatus() == Connection::OPENED) {
            SockAddr_in address;
            int addr_len {sizeof(address)};
            memset(&address, 0, sizeof(address));
            std::this_thread::sleep_for(100ms);
            Socket client_socket{accept(this->GetInfo().GetSocket(),
                                        reinterpret_cast<sockaddr *>(&address),
                                        reinterpret_cast<socklen_t*>(&addr_len))};

            if (this->GetInfo().GetConnectionStatus() != Connection::OPENED) {
                return;
            }
            if (client_socket <= -1) {
                int error = errno;
                continue;
            }
            std::cout << "Socket is fine" << std::endl;
            std::shared_ptr<CommunicationUnit> unit(
                    std::make_shared<CommunicationUnit>(CommunicationUnit(ntohs(address.sin_port),
                                                                   ntohl(address.sin_addr.s_addr))));
            unit->SetSocket(client_socket);
            unit->SetConnectionStatus(Connection::OPENED);
            std::scoped_lock lock(client_mutex);
            std::cout << "Here is a new connection from " << unit->GetIP() << ":" << unit->GetPort() << std::endl;
            IServer::GetConnections().push_back(std::move(unit));
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
