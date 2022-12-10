#include <iostream>
#include <conio.h>
#include <string>
#include <WinSock2.h>
#include <thread>

bool is_running = true;
SOCKET sock;

void Recv() {
    char buf[1<<10];
    while(is_running) {
        memset(buf, 0, sizeof(buf));
        int ok = recv(sock, buf, sizeof(buf), 0);
        std::cout<<buf<<std::endl;
        if(ok == -1) {
            printf("error\n");
            return;
        }
    }
}

int main() {
    int ok = 0;
    WSADATA wsd;
    WSAStartup(MAKEWORD(2, 2), &wsd);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(8080);
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");

    ok = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
    if(ok == -1) printf("error\n");

    std::thread listen_tread(Recv);

    std::string data;
    while(true) {
        if(_kbhit()) {
            char c = _getche();
            if(c == 27) {
                is_running = false;
                break;
            }
            else if(c == 13) {
                putc('\n', stdin);
                if(data == "quit") {
                    printf("quit\n");
                    is_running = false;
                    break;
                }
                printf("debug >>> %s\n", data.c_str());
                send(sock, data.c_str(), data.size(), 0);
                data = std::string();
            }
            else {
                data.push_back(c);
            }
        }
    }
    

    closesocket(sock);
    listen_tread.join();
    WSACleanup();
}