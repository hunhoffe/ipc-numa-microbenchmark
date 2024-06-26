#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "udsprog.h"

int main(int argc, char const *argv[]) 
{ 
    // Socket state
    int sock_fd = -1;
    int new_sock_fd = -1;
    struct sockaddr_un server_addr;
    
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

    int path_len = strlen(UDS_PATH) + strlen(argv[ARG_PAIR_NUM]);
    if (path_len >= UDS_PATH_MAX_LEN - 1) {
        printf("ERROR: UDS path is too long! Should only be %d characters for strlen(%s + %s)", UDS_PATH_MAX_LEN, UDS_PATH, argv[ARG_PAIR_NUM]);
    }

    // Construct uds file path from base path and pair num
    char my_path[UDS_PATH_MAX_LEN] = { '\0' };
    strncpy((char *)my_path, UDS_PATH, UDS_PATH_MAX_LEN);
    strncpy((char *)&(my_path[strnlen(UDS_PATH, UDS_PATH_MAX_LEN - 1)]), argv[ARG_PAIR_NUM], UDS_PATH_MAX_LEN - strlen(UDS_PATH));
    printf("UDS socket path is: %s\n", my_path);

    // Create socket
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        goto cleanup;
    }
    printf("Socket create!\n");

    // Prepare address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, my_path, sizeof(server_addr.sun_path) - 1);

    if (is_server) {    
        // Bind the socket to the ip/port
        if (0 > bind(sock_fd, (const struct sockaddr *) &server_addr, sizeof(server_addr))) {
            perror("bind failed"); 
            goto cleanup;
        }
        printf("Server bind!\n");

 	    // Listen for a connection
        if (listen(sock_fd, 1) < 0) { 
            perror("listen"); 
            goto cleanup;
        }
        printf("Server listen!\n"); 

	    // Accept the connection
        if ((new_sock_fd = accept(sock_fd, NULL, NULL)) < 0) { 
            perror("accept"); 
            goto cleanup;
        }
        printf("Server accept!\n");

        if (EXIT_SUCCESS != do_work(new_sock_fd, msg_len, is_server)) {
            printf("do_work() failed\n");
            goto cleanup;
        }
    } else {
        // Connect to the server
        if (0 > connect(sock_fd, (const struct sockaddr *) &server_addr, sizeof(server_addr))) { 
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
    if (is_server) {
        unlink(my_path);
    }
    return ret; 
}
