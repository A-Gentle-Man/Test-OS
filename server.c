#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    char str[100];
    char response[100];
    int listen_fd, comm_fd;

    struct sockaddr_in servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);

    bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listen_fd, 10);

    while (1) {
        comm_fd = accept(listen_fd, NULL, NULL);

        while (1) {
            bzero(str, sizeof(str));

            int n = read(comm_fd, str, sizeof(str) - 1);
            if (n == 0) {
                close(comm_fd);
                break;
            } else if (n < 0) {
                perror("read error");
                close(comm_fd);
                break;
            }

            int len = strlen(str);
            if (str[len - 1] == '\n') {
                str[len - 1] = '\0';
                len--;
            }

            snprintf(response, sizeof(response),
                     "Message Length: %d\n", len);

            write(comm_fd, response, strlen(response) + 1);
        }
    }
}
