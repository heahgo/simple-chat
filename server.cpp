#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>

#define PORT 3000

using std::thread;

int sockArray[5] = {0, };
int idx = 0;

void recv_msg(int clnt_sock) {
     while (1) {
        char msg[256] = {0, };
        read(clnt_sock, msg, sizeof(msg));
        printf("recv msg : %s", msg);
        for (int i = 0; i < 5; i++) {
            if (sockArray[i] != 0) {
                write(sockArray[i], msg, sizeof(msg));
            }
        }
    }
}

int main() {
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    serv_sock = socket( PF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(PORT);

    bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    listen(serv_sock, 5);
    
    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
        sockArray[idx++] = clnt_sock;

        printf("accept\n");
        thread recv(recv_msg, clnt_sock);
        recv.detach();
    }
   
    close(serv_sock);
}