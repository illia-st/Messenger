#include <iostream>
#include <list>
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
//    std::cout << "The server is already started" << std::endl;
//    std::list<int> l {1,2,3,4,5,6};
//    bool flag {true};
//    for(auto it {l.begin()}; it != l.end(); ++it){
//        if(flag){
//            l.push_back(7);
//            flag = false;
//        }
//        std::cout << *it << " ";
//    }
//    std::cout << std::endl;
    return 0;
}
