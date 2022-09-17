#include "../Header/Client.h"


#ifdef WIN32

#else



#endif

TCP::Client::Client(uint32_t host, uint16_t port): IClient(host, port) {}

TCP::Client::Client(const std::string& IP, uint16_t port): IClient(IP, port) {}