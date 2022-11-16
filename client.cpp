#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 3000

using std::thread;

void recv_msg(int clnt_sock) {
    while(1) {
        char msg[256] = {0, };
        if (read(clnt_sock, msg, sizeof(msg)) <= 0 ) {
            close(clnt_sock);
            exit(0);
        }

        printf("server msg : %s", msg);
    }
}

void send_msg(int clnt_sock) {
    while(1) {
        char msg[256] = {0, };
        fgets(msg, sizeof(msg), stdin);
        write(clnt_sock, msg, sizeof(msg) - 1);
    }
}

int main() {
    int clnt_sock;
    struct sockaddr_in serv_addr;

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons(SERVER_PORT);
    
    if (connect(clnt_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        return 0;
    
    thread recv(recv_msg, clnt_sock);
    thread send(send_msg, clnt_sock);
    recv.join();
    send.join();
}