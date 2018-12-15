#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void ping(int s, char *message)
{
    char buf[8192];

    strncpy(buf, message, sizeof(buf));
    send(s, buf, strlen(buf), 0);
    recv(s, buf, 8192, 0);
    strtok(buf, "\n");
    puts(buf);
}

int main()
{
    int s;
    struct sockaddr_in6 addr;

    s = socket(AF_INET6, SOCK_STREAM, 0);
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(7777);
    inet_pton(AF_INET6, "::1", &addr.sin6_addr);
    if(connect(s, (struct sockaddr *)&addr, sizeof(addr))) {
        printf("Connected to server\n");
        while(1) {
            char ch = getchar();
            if(ch == 'q') {
                break;
            }
            send(s, "Hello server!!!", 16, 0);
            char buff[1024];
            size_t recieved = recv(s, buff, 1024, 0);
            printf("Recieved: %d bytes\n", (int)recieved);
        }
    }
    close(s);
    return 0;
}