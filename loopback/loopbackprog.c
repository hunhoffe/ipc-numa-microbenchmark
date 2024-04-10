#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <errno.h>
#include <inttypes.h>

#include "loopbackprog.h"

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
    int msg_len = 1;
    int pair_num = 0;
    bool is_server = false;
 
    // Check number of arguments 
	if (NUM_ARGS != argc) {
		printf("ERROR: Wrong number of arguments\n");
		printf("Usage: %s %s\n", argv[0], USAGE_STR);
		goto cleanup;
	}

    // Check first argument - for whether client or server
    if (0 == strncmp("client", argv[ARG_CLIENT_SERVER], 7)) {
        is_server = false;
    } else if (0 == strncmp("server", argv[ARG_CLIENT_SERVER], 7)) {
        is_server = true;
    } else {
        printf("ERROR: arg %d should be \"server\" or \"client\"\n", ARG_CLIENT_SERVER - 1);
        printf("Usage: %s %s\n", argv[0], USAGE_STR);
        goto cleanup;    
    }

    // Check second argument - the length of the messages to send
    msg_len = atoi(argv[ARG_MSG_LEN]);
    if (msg_len < 1 || msg_len > MAX_MSG_LEN) {
        printf("ERROR: invalid message length. Should be 0 < msg_len <= %d, not %d\n", MAX_MSG_LEN, msg_len);
        printf("Usage: %s %s\n", argv[0], USAGE_STR);
        goto cleanup;
    } 

    // Check third argument - an id unique to this client/server pair
    pair_num = atoi(argv[ARG_PAIR_NUM]);
    if (pair_num < 0 || pair_num > MAX_PAIR_NUM) {
        printf("ERROR: invalid pair number. Should be 0 <= pair_num <= %d, not %d\n", MAX_PAIR_NUM, pair_num);
        printf("Usage: %s %s\n", argv[0], USAGE_STR);
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
        server_addr.sin_port = htons(LOOPBACK_PORT + pair_num * 2); 
        printf("Server binding to port=%d", LOOPBACK_PORT + pair_num * 2);
    } else {
        server_addr.sin_port = htons(LOOPBACK_PORT + pair_num * 2 + 1);
        printf("Local client port=%d", LOOPBACK_PORT + pair_num * 2 + 1);
    }

    if (is_server) {
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
        server_addr.sin_port = htons(LOOPBACK_PORT + pair_num * 2);
        printf("Client connecting on port=%d", LOOPBACK_PORT + pair_num * 2);

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
