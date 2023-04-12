#include <stdio.h>
#include <netinet/in.h> 
#include <unistd.h> 
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>

#define DATA_BUFFER 5000
#define MAX_CONNECTIONS 10 
#define SUCCESS_MESSAGE "Your message delivered successfully"

int create_tcp_server_socket() {
    struct sockaddr_in saddr;
    int fd, ret_val;

    /* Step1: create a TCP socket */
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", strerror(errno));
        return -1;
    }
    printf("Created a socket with fd: %d\n", fd);

    /* Initialize the socket address structure */
    saddr.sin_family = AF_INET;         
    saddr.sin_port = htons(7000);     
    saddr.sin_addr.s_addr = INADDR_ANY; 

    /* Step2: bind the socket to port 7000 on the local host */
    ret_val = bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val != 0) {
        fprintf(stderr, "bind failed [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }

    /* Step3: listen for incoming connections */
    ret_val = listen(fd, 5);
    if (ret_val != 0) {
        fprintf(stderr, "listen failed [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }

    return fd;
}

void setup_epoll_connection(int epfd, int fd, struct epoll_event * event) {
    event->events = EPOLLIN;
    event->data.fd = fd;

    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, event);
}

int main () {
    struct sockaddr_in new_addr;
    int server_fd, new_fd, ret_val, temp_fd, i, timeout_msecs = 100;
    socklen_t addrlen;
    char buf[DATA_BUFFER];
    struct epoll_event all_connections[MAX_CONNECTIONS], epoll_temp;
    int epfd = epoll_create(1);

    /* Get the socket server fd */
    server_fd = create_tcp_server_socket(); 

    if (server_fd == -1) {
        fprintf(stderr, "Failed to create a server\n");
        return -1; 
    }   

    setup_epoll_connection(epfd, server_fd, &epoll_temp);

    while (1) {
        ret_val = epoll_wait(epfd, all_connections, MAX_CONNECTIONS, timeout_msecs /*timeout*/ );

        for (int i = 0; i < ret_val; i++)
        {
            if (all_connections[i].data.fd == server_fd) { 
                new_fd = accept(server_fd, (struct sockaddr*)&new_addr, &addrlen);

                if (new_fd >= 0) {
                    setup_epoll_connection(epfd, new_fd, &epoll_temp);
                    printf("Accepted a new connection with fd: %d\n", new_fd);
                } else {
                    fprintf(stderr, "accept failed [%s]\n", strerror(errno));
                }
            } else if (all_connections[i].events & EPOLLIN) {
                if ( (temp_fd = all_connections[i].data.fd) < 0) continue;

                ret_val = recv(all_connections[i].data.fd, buf, DATA_BUFFER, 0);

                if (ret_val > 0) {
                    printf("Returned fd is %d [index, i: %d]\n", all_connections[i].data.fd, i);
                    printf("Received data (len %d bytes, fd: %d): %s\n", ret_val, all_connections[i].data.fd, buf);
                    ret_val = send(all_connections[i].data.fd, SUCCESS_MESSAGE, sizeof(SUCCESS_MESSAGE), 0);
                }
            }
        }
    } /* while(1) */

    /* Last step: Close all the sockets */
    for (i=0;i < MAX_CONNECTIONS;i++) {
        if (all_connections[i].data.fd > 0) {
            close(all_connections[i].data.fd);
        }
    }
    return 0;
}