#include <iostream>
#include "Server/Header/Server.h"

int main() {
    TCP::Server server(1234);
    int res = server.StartServer();
    if(res != 1){
        std::cout << strerror(res) << std::endl;
        return 1;
    }
    while(true){

    }
    std::cout << "The server is already started" << std::endl;

    return 0;
}
