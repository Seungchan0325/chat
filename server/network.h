#pragma once

#include "client.h"

#include <string>
#include <queue>
#include <memory>

#include <WinSock2.h>

enum RequestType {
    CONN,
    DISCONN,
    RECV,
};

struct RequestData {
    virtual ~RequestData() {}
};

struct RequestConn : public RequestData {
    RequestConn(SOCKET _sock) : sock(_sock) {}
    SOCKET sock;
};

struct RequestRecv : public RequestData {
    RequestRecv(std::string _msg) : msg(_msg) {}
    std::string msg;
};

struct RequestDisconn : public RequestData {

};

struct Request {
    Request(std::shared_ptr<Client> _clnt, RequestType _type, std::shared_ptr<RequestData> _data) : clnt(_clnt), type(_type), data(_data) {}
    std::shared_ptr<Client> clnt;
    RequestType type;
    std::shared_ptr<RequestData> data;
};

class Network {
public:
    void initialize(short port, std::shared_ptr<ClientSet> _client_set);
    void release();

    void Send(SOCKET sock, const std::string& message);
    void Send(ClientSet& clnts, const std::string& message);

    Request get_request();

private:
    SOCKET GetListenSocket(int type, short port, int backlog = SOMAXCONN);

    SOCKET Accept();
    std::string Recv(SOCKET sock);
    fd_set* Select();

    void FD_INIT(fd_set* fdr);

private:
    SOCKET listen_socket = (SOCKET)NULL;
    std::shared_ptr<ClientSet> client_set;
};