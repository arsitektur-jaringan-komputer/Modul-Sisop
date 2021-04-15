#include <stdio.h>
#include <netinet/in.h> 
#include <unistd.h> 
#include <string.h>
#include <errno.h>
#include <poll.h>

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

int main () {
    fd_set read_fd_set;
    struct sockaddr_in new_addr;
    int server_fd, new_fd, ret_val, i, timeout_msecs = 500;
    socklen_t addrlen;
    char buf[DATA_BUFFER];
    struct pollfd all_connections[MAX_CONNECTIONS];

    /* Get the socket server fd */
    server_fd = create_tcp_server_socket(); 
    if (server_fd == -1) {
        fprintf(stderr, "Failed to create a server\n");
        return -1; 
    }   

    /* Initialize all_connections and set the first entry to server fd */
    for (i=0;i < MAX_CONNECTIONS;i++) {
        all_connections[i].fd = -1;
        all_connections[0].events = POLLIN;
    }
    all_connections[0].fd = server_fd;

    while (1) {
        /* Invoke poll() and then wait! */
        ret_val = poll(all_connections, MAX_CONNECTIONS, timeout_msecs);

        /* poll() woke up. Identify the fd that has events */
        if (ret_val > 0 ) {
            /* Check if new events is input */
            if (all_connections[0].revents & POLLIN) { 
                /* accept the new connection */
                printf("Returned fd is %d (server's fd)\n", server_fd);
                new_fd = accept(server_fd, (struct sockaddr*)&new_addr, &addrlen);
                if (new_fd >= 0) {
                    printf("Accepted a new connection with fd: %d\n", new_fd);
                    for (i=0;i < MAX_CONNECTIONS;i++) {
                        if (all_connections[i].fd < 0) {
                            all_connections[i].fd = new_fd; 
                            break;
                        }
                    }
                } else {
                    fprintf(stderr, "accept failed [%s]\n", strerror(errno));
                }
                ret_val--;
                if (!ret_val) continue;
            } 

            /* Check if the fd with event is a non-server fd */
            for (i=1;i < MAX_CONNECTIONS;i++) {
                if (all_connections[i].revents & POLLIN) {
                    /* read incoming data */   
                    printf("Returned fd is %d [index, i: %d]\n", all_connections[i].fd, i);
                    ret_val = recv(all_connections[i].fd, buf, DATA_BUFFER, 0);
                    if (ret_val == 0) {
                        printf("Closing connection for fd:%d\n", all_connections[i].fd);
                        close(all_connections[i].fd);
                        all_connections[i].fd = -1; /* Connection is now closed */
                    } 
                    if (ret_val > 0) { 
                        printf("Received data (len %d bytes, fd: %d): %s\n", 
                            ret_val, all_connections[i].fd, buf);
                        ret_val = send(all_connections[i].fd, SUCCESS_MESSAGE, sizeof(SUCCESS_MESSAGE), 0);
                    } 
                    if (ret_val == -1) {
                        printf("recv() failed for fd: %d [%s]\n", 
                            all_connections[i].fd, strerror(errno));
                        break;
                    }
                }
                ret_val--;
                if (!ret_val) continue;
            } /* for-loop */
        } /* (ret_val >= 0) */
    } /* while(1) */

    /* Last step: Close all the sockets */
    for (i=0;i < MAX_CONNECTIONS;i++) {
        if (all_connections[i].fd > 0) {
            close(all_connections[i].fd);
        }
    }
    return 0;
}