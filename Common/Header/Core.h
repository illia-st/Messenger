#pragma once
#include <list>
#include "CommunicationUnit.h"
#include <memory>
#ifdef WIN32
#define WIN(exp) exp
#define LIN(exp)
using SockAddr_in = SOCKADDR_IN;
using Scocket = SOCKET;
#else
#define WIN(exp)
#define LIN(exp) exp

using SockAddr_in = struct sockaddr_in;
using Socket = int;
#endif

namespace TCP{
    class CommunicationUnit;
    class IServer;
    class IClient;
    class Client;
    class Server;

    using ServerInfo = CommunicationUnit;
    using ClientInfo = CommunicationUnit;
    using ConnectionsList = std::list<std::shared_ptr<CommunicationUnit>>;

    enum class KeepAlive {
        TRUE,
        FALSE
    };
}