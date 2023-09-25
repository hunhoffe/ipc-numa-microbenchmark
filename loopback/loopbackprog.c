#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <errno.h>

#include "loopbackprog.h"

int send_wrapper(int sock_fd, char *buf, int msg_len) {
    int bytes_sent = 0;
    int send_ret = 0;

    while (bytes_sent != msg_len) {
        send_ret = send(sock_fd, buf, msg_len - bytes_sent, 0);
        if (-1 == send_ret) {
            perror("send");
            return EXIT_FAILURE;
        } else {
            bytes_sent += send_ret;
            buf += send_ret;
        }
    }
    return EXIT_SUCCESS;
}

int recv_wrapper(int sock_fd, char *buf, int msg_len) {
    int bytes_recv = 0;
    int recv_ret = 0;

    while (bytes_recv != msg_len) {
        recv_ret = recv(sock_fd, buf, msg_len - bytes_recv, MSG_WAITALL);
        if (-1 == recv_ret) {
            perror("recv");
            return EXIT_FAILURE;
        } else {
            bytes_recv += recv_ret;
            buf += recv_ret;
        }
    }
    return EXIT_SUCCESS;
}

int do_work(int sock_fd, int msg_len, bool is_server) {
    char *msg_buf = NULL;
    int ret = EXIT_FAILURE;
    int bytes_sent = 0;

    // Initialize message buffer
    if (NULL == (msg_buf = malloc(msg_len))) {
        goto work_cleanup;
    }

    // Send once to kick things off
    if (is_server) {
        if (EXIT_SUCCESS != send_wrapper(sock_fd, msg_buf, msg_len)) {
            goto work_cleanup;
        }
        if (EXIT_SUCCESS != recv_wrapper(sock_fd, msg_buf, msg_len)) {
            goto work_cleanup;
        }
    } else {
        if (EXIT_SUCCESS != recv_wrapper(sock_fd, msg_buf, msg_len)) {
            goto work_cleanup;
        }
        if (EXIT_SUCCESS != send_wrapper(sock_fd, msg_buf, msg_len)) {
            goto work_cleanup;
        }
    }

    ret = EXIT_SUCCESS;

work_cleanup:
    // Free message buffer
    if (NULL != msg_buf) {
        free(msg_buf);
    }

    return ret;
}

int main(int argc, char const *argv[]) 
{ 
    // Socket state
    int sock_fd = -1;
    int new_sock_fd = -1;
    struct sockaddr_in server_addr; 
    int addrlen = sizeof(server_addr);
    int opt = 1;
    
    // Other state
    int errnum = -1;
    int ret = EXIT_FAILURE;     
    
    // Args
    int num_threads = 1;
    int msg_len = 1;
    bool is_server = false;
 
    // Check number of arguments 
	if (NUM_ARGS != argc) {
		printf("ERROR: Wrong number of arguments\n");
		printf(USAGE_STR);
		goto cleanup;
	}

    // Check first argument - for whether client or server
    if (0 == strncmp("client", argv[ARG_CLIENT_SERVER], 7)) {
        is_server = false;
    } else if (0 == strncmp("server", argv[ARG_CLIENT_SERVER], 7)) {
        is_server = true;
    } else {
        printf("ERROR: arg %d should be \"server\" or \"client\"\n", ARG_CLIENT_SERVER - 1);
        printf(USAGE_STR);
        goto cleanup;    
    }

    // Check second argument - number of threads to spawn
    num_threads = atoi(argv[ARG_NUM_THREADS]);
    if (num_threads < 1 || num_threads > MAX_NUM_THREADS) {
        printf("ERROR: invalid number of threads. Should be 0 < num_threads <= %d, not %d\n", MAX_NUM_THREADS, num_threads);
        goto cleanup;
    } 

    // Check third argument - the length of the messages to send
    msg_len = atoi(argv[ARG_MSG_LEN]);
    if (msg_len < 1 || msg_len > MAX_MSG_LEN) {
        printf("ERROR: invalid message length. Should be 0 < msg_len <= %d, not %d\n", MAX_MSG_LEN, msg_len);
        goto cleanup;
    } 

    // Create socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        goto cleanup;
    } 

    // Set up local address
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = inet_addr(LOOPBACK_IP);
    if (is_server) {
        server_addr.sin_port = htons(LOOPBACK_PORT); 
    } else {
        server_addr.sin_port = htons(LOOPBACK_PORT + 1);
    }

    // Force to use port
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  &opt, sizeof(opt))) { 
        perror("setsockopt"); 
        goto cleanup;
    }
    
    // Bind the socket to the ip/port
    if (bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) { 
        perror("bind failed"); 
        goto cleanup;
    }

    if (is_server) {
 	    // Listen for a connection
        if (listen(sock_fd, 1) < 0) { 
            perror("listen"); 
            goto cleanup;
        }
        printf("Server listen!\n"); 

	    // Accept the connection
        if ((new_sock_fd = accept(sock_fd, (struct sockaddr *) &server_addr, (socklen_t*) &addrlen)) < 0) { 
            perror("accept"); 
            goto cleanup;
        }
        printf("Server accept!\n");

        if (EXIT_SUCCESS != do_work(new_sock_fd, msg_len, is_server)) {
            printf("do_work() failed\n");
            goto cleanup;
        }
    } else {
        // Set up remote address
        server_addr.sin_family = AF_INET; 
        server_addr.sin_addr.s_addr = inet_addr(LOOPBACK_IP);
        server_addr.sin_port = htons(LOOPBACK_PORT); 

        // Connect to the server
        if (0 > connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) { 
            errnum = errno;
            printf("Connect failed. errno=%d, err=%s\n", errnum, strerror(errnum));
            goto cleanup; 
        }
        printf("Client connect!\n");

        if (EXIT_SUCCESS != do_work(sock_fd, msg_len, is_server)) {
            printf("do_work() failed\n");
            goto cleanup;
        }
    } 

    ret = EXIT_SUCCESS;
    printf("Exited successfully!\n");

cleanup:
    if (sock_fd != -1) {
        close(sock_fd);
    }
    if (new_sock_fd != -1) {
        close(new_sock_fd);
    }
    return ret; 
}
