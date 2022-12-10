#include "network.h"

#include "exception.h"

#include <string>
#include <memory>

#include <WinSock2.h>

void Network::initialize(short port, std::shared_ptr<ClientSet> _client_set) {
    int error;
    WSADATA wsd;

    error = WSAStartup(MAKEWORD(2, 2), &wsd);
    if(error) {
        throw network_error(std::string("WSAStart failed, code : ") + std::to_string(error));
    }

    listen_socket = GetListenSocket(SOCK_STREAM, port);
    client_set = _client_set;
}

void Network::release() {
    if(listen_socket) {
        closesocket(listen_socket);
    }

    WSACleanup();
}

void Network::Send(SOCKET sock, const std::string& message) {
    long sock_ret = send(sock, message.c_str(), message.size(), 0);
    if(sock_ret == SOCKET_ERROR)  {
        throw network_error(std::string("send failed, code : ") + std::to_string(WSAGetLastError()));
    }
}

void Network::Send(ClientSet& clnts, const std::string& message) {
    for(auto it = clnts.begin(); it != clnts.end(); it++) {
        Send((*it)->sock(), message);
    }
}

Request Network::get_request() {
    static std::queue<Request> req_que;

    if(req_que.empty()) {
        fd_set* fdr = Select();

        if(FD_ISSET(listen_socket, fdr)) {
            req_que.push(Request(
                0,
                RequestType::CONN,
                std::make_shared<RequestConn>(Accept())));
        }

        for(auto it = client_set->begin(); it != client_set->end(); it++) {
            if(FD_ISSET((*it)->sock(), fdr)) {
                std::string msg = Recv((*it)->sock());

                if(msg.empty()) {
                    req_que.push(Request(
                        *it,
                        RequestType::DISCONN,
                        std::make_shared<RequestDisconn>()));
                } else {
                    req_que.push(Request(
                        *it,
                        RequestType::RECV,
                        std::make_shared<RequestRecv>(msg)));
                }
            }
        }
    }

    auto req = req_que.front(); req_que.pop();
    return req;
}

SOCKET Network::GetListenSocket(int type, short port, int backlog) {
    int ok = 0;
    SOCKET listen_sock = socket(AF_INET, type, 0);
    if(listen_sock == INVALID_SOCKET) {
        throw invalid_socket(std::string("socket failed, code : ") + std::to_string(WSAGetLastError()));
    }

    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_family       = AF_INET;
    sa.sin_port         = htons(port);
#ifdef DEBUG
    sa.sin_addr.s_addr  = inet_addr("127.0.0.1");
#else   
    sa.sin_addr.s_addr  = htonl(INADDR_ANY);
#endif
    ok = bind(listen_sock, (PSOCKADDR)&sa, sizeof(SOCKADDR_IN));
    if(ok == SOCKET_ERROR) {
        closesocket(listen_sock);
        throw invalid_socket(std::string("bind failed, code : ") + std::to_string(WSAGetLastError()));
    }

    ok = listen(listen_sock, backlog);
    if(ok == SOCKET_ERROR) {
        closesocket(listen_sock);
        throw invalid_socket(std::string("listen failed, code : ") + std::to_string(WSAGetLastError()));
    }

    return listen_sock;
}

void Network::FD_INIT(fd_set* fdr) {
    FD_ZERO(fdr);

    FD_SET(listen_socket, fdr);

    for(auto it = client_set->begin(); it != client_set->end(); it++) {
        FD_SET((*it)->sock(), fdr);
    }
}

SOCKET Network::Accept() {
    SOCKET sock = accept(listen_socket, 0, 0);
    if(sock == INVALID_SOCKET) {
        throw invalid_socket(std::string("accept failed, code : ") + std::to_string(WSAGetLastError()));
    }
    return sock;
}

std::string Network::Recv(SOCKET sock) {
    static char buffer[1<<10];

    memset(buffer, 0, sizeof(buffer));
    long sock_ret = recv(sock, buffer, sizeof(buffer), 0);
    if(sock_ret == SOCKET_ERROR) {
        throw invalid_socket(std::string("recv  failed, code : " + std::to_string(WSAGetLastError())));
    } else if(sock_ret == 0) {
        return "";
    }

    return buffer;
}

fd_set* Network::Select() {
    static fd_set fdr;
    int avail = 0;

    FD_INIT(&fdr);  

    do {
        avail = select(0, &fdr, NULL, NULL, NULL);
        if(avail == SOCKET_ERROR) {
            throw network_error(std::string("select failed, code : ") + std::to_string(WSAGetLastError()));
        }
    } while(avail == 0);

    return &fdr;
}