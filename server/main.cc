#include "common.h"
#include "network.h"
#include "exception.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <memory>

int port = 8080;

std::shared_ptr<ClientSet> g_client_set;
//std::shared_ptr<RoomSet> g_room_set;
std::shared_ptr<Network> g_network;

void init() {
    g_client_set = std::make_shared<ClientSet>();
    //g_room_set = std::make_shared<RoomSet>();
    g_network = std::make_shared<Network>();

    g_network->initialize(port, g_client_set);
}

void release() {
    g_network->release();
}

int main(int argc, char* argv[]) {
    init();

#ifdef DEBUG
    printf("DEBUGGING!!\n");
#endif

    try {
        while(true) {
            Request req = g_network->get_request();

            if(req.type == RequestType::CONN) {
                std::shared_ptr<RequestConn> req_data = std::dynamic_pointer_cast<RequestConn>(req.data);
                SOCKET sock = req_data->sock;

                g_client_set->insert(std::make_shared<Client>(sock));
                printf("Connected %lld\n", sock);
            }
            else if(req.type == RequestType::RECV) {
                std::shared_ptr<RequestRecv> req_data = std::dynamic_pointer_cast<RequestRecv>(req.data);
                printf("%s\n", req_data->msg.c_str());
                g_network->Send(*g_client_set, req_data->msg);
            }else if(req.type == RequestType::DISCONN) {
                g_client_set->erase(req.clnt);
                printf("Disconnected %lld\n", req.clnt->sock());
            }
        }
    } catch(network_error& e) {
        printf("%s\n", e.what()); 
    } catch(invalid_socket& e) {
        printf("%s\n", e.what());
    }

    release();
}