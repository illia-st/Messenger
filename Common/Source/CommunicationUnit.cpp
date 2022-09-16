#include "../Header/CommunicationUnit.h"
#include <iostream>

namespace TCP {
#ifdef WIN32
#else

    CommunicationUnit::CommunicationUnit() : m_sock(-1), con(Connection::CLOSED) {
        memset(&addr, 0, sizeof(SockAddr_in));
    }

    CommunicationUnit::CommunicationUnit(uint16_t port, uint32_t host) {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (host == INADDR_ANY) {
            addr.sin_addr.s_addr = INADDR_ANY;
            return;
        }
        addr.sin_addr.s_addr = ntohl(host);
    }

    void CommunicationUnit::CloseSocket() {
        close(m_sock);
        m_sock = -1;
        con = Connection::CLOSED;
    }

    HiddenBuffer<>* CommunicationUnit::LoadData() {
        char * buf = new char[MaxBufferSize]{};
        int ans = recv(this->GetSocket(), buf, MaxBufferSize, MSG_DONTWAIT);
//        std::cout << ans << std::endl;
        if(ans == -1){
            delete [] buf;
            return nullptr;
        }
//        for(size_t i{}; i < 10; ++i){
//            std::cout << buf[i] << " ";
//        }
//        std::cout << std::endl;
        std::cout << "Something was accepted" << std::endl;
        auto BufferPtr {reinterpret_cast<HiddenBuffer<>*>(buf)};
        if(ans == 0){
            std::cout << "Giving a hundred" << std::endl;
            BufferPtr->SetFlag(100);
        }
        BufferPtr->SetSize(ans);
//        std::cout << BufferPtr->GetFlag() << std::endl;
        return BufferPtr;
    }

    int CommunicationUnit::Disconnect() {
        Clear();
    }
#endif

    int CommunicationUnit::SendMsg(const char *buf, size_t N) {
        int error_code = send(this->GetSocket(), reinterpret_cast<const void*>(buf), N, 0);
        if(error_code == -1){
            int error = errno;
            // make log about errno
            return error;
        }
        return 1;
    }
    void CommunicationUnit::SetSocket(Socket sock) {
        if (sock LIN(> 0)WIN(!= INVALID_SOCKET)) {
            m_sock = sock;
            con = Connection::OPENED;
        }
    }

    void CommunicationUnit::Clear() {
        this->CloseSocket();
        memset(&addr, 0, sizeof(SockAddr_in));
    }

    uint16_t CommunicationUnit::GetPort() const {
        return ntohs(addr.sin_port);
    }

    uint32_t CommunicationUnit::GetHost() const {
        return ntohl(addr.sin_addr.s_addr);
    }

    std::string CommunicationUnit::GetIP() const {
        return std::move(std::string((inet_ntoa(addr.sin_addr))));
    }

    void CommunicationUnit::SetConnectionStatus(Connection connection) {
        con = connection;
    }

    Connection CommunicationUnit::GetConnectionStatus() const {
        return con;
    }

    Socket &CommunicationUnit::GetSocket() { return m_sock; }

    SockAddr_in &CommunicationUnit::GetData() {return addr;}
}