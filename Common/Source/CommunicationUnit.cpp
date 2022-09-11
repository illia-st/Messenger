#include "../Header/CommunicationUnit.h"


#ifdef WIN32
#else
CommunicationUnit::CommunicationUnit(): m_sock(-1), con(Connection::CLOSED){
    memset(&addr, 0, sizeof(SockAddr_in));
}
CommunicationUnit::CommunicationUnit(uint16_t port, uint32_t host) {
    addr.sin_port = htons(port);
    if(host == INADDR_ANY){
        addr.sin_addr.s_addr = INADDR_ANY;
    }
    addr.sin_addr.s_addr = ntohl(host);
}
void CommunicationUnit::CloseSocket() {
    close(m_sock);
    m_sock = -1;
}
#endif
void CommunicationUnit::SetSocket(Socket sock) {
    if(sock LIN( > 0)WIN( != INVALID_SOCKET)){
        m_sock = sock;
        con = Connection::OPENED;
    }
}
void CommunicationUnit::Clear(){
    this->CloseSocket();
    m_sock = LIN(-1)WIN(INVALID_SOCKET);
    memset(&addr, 0, sizeof(SockAddr_in));
}
uint16_t CommunicationUnit::GetPort() const{
    return ntohs(addr.sin_port);
}
uint32_t CommunicationUnit::GetHost() const {
    return ntohl(addr.sin_addr.s_addr);
}
std::string CommunicationUnit::GetIP() const{
    return std::move(std::string((inet_ntoa(addr.sin_addr))));
}
