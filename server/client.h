#pragma once

#include <cstdio>
#include <set>
#include <memory>

#include <WinSock2.h>

class Client {
public:
    Client(SOCKET s) : client_socket(s) {};
    
    ~Client() {
        if(client_socket != INVALID_SOCKET) {
            closesocket(client_socket);
        }
    }

    SOCKET sock() const { return client_socket; }
    
private:
    SOCKET client_socket;
};

using ClientSet = std::set<std::shared_ptr<Client>>;