#include <iostream>
#include <string>
#include <WinSock2.h>

int main() {
    int ok = 0;
    WSADATA wsd;
    WSAStartup(MAKEWORD(2, 2), &wsd);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(8080);
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");

    ok = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
    if(ok == -1) printf("error\n");

    char in_buf[1<<10];
    char out_buf[1<<10];
    fd_set fdr;
    while(true) {
        FD_ZERO(&fdr);
        FD_SET(0, &fdr);    
        FD_SET(sock, &fdr);
        select(0, &fdr, 0, 0, 0);
        
        if(FD_ISSET(0, &fdr)) {
            std::cin.getline(out_buf, sizeof(out_buf));
            int len = strlen(out_buf);
            out_buf[len] = 0;
            if(len >= 1)
                send(sock, out_buf, len, 0);
        }
        if(FD_ISSET(sock, &fdr)) {
            recv(sock, in_buf, sizeof(in_buf), 0);
            std::cout<<in_buf<<std::endl;
        }
    }

    closesocket(sock);
    WSACleanup();
}