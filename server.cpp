#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <list>
#include <mutex>
#include <vector>

#define PORT 3000

using std::thread;

std::mutex m;
std::list<int> sockArray;

void recv_msg(int clnt_sock)
{
    while (1)
    {
        char msg[256] = {
            0,
        };
        int a = read(clnt_sock, msg, sizeof(msg));
        if (a <= 0)
            break;
        printf("recv msg : %s", msg);
        m.lock();
        for (int sock : sockArray)
        {
            write(sock, msg, sizeof(msg));
        }
        m.unlock();
    }
    m.lock();
    sockArray.remove(clnt_sock);
    m.unlock();

}

    int
    main()
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(serv_sock, 5);

    while (1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

        m.lock();
        sockArray.push_back(clnt_sock);
        m.unlock();

        printf("accept\n");
        thread recv(recv_msg, clnt_sock);
        recv.detach();
    }

    close(serv_sock);
}
