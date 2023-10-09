#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <unistd.h> 

#include <sys/socket.h> 
#include <sys/un.h>

#include <errno.h>
#include <time.h>
#include <inttypes.h>

#include "udsprog.h"

// https://stackoverflow.com/questions/64893834/measuring-elapsed-time-usung-clock-gettimeclock-monotonic
int64_t difftimespec_ns(const struct timespec after, const struct timespec before)
{
    return ((int64_t)after.tv_sec - (int64_t)before.tv_sec) * (int64_t)1000000000
         + ((int64_t)after.tv_nsec - (int64_t)before.tv_nsec);
}

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
    struct timespec currentStartTime = { 0 };
    struct timespec currentTime = { 0 };
    struct timespec duration = { 0 };
    long iterations = 0;
    int64_t timediff = 0;
    long results[SEC_PER_TEST] = { 0 };

    // Initialize LARGE buffer (will be used as message buffer)
    if (NULL == (msg_buf = malloc(BUF_SIZE))) {
	perror("malloc");
        goto work_cleanup;
    }
    // Touch every 256 bytes to ensure allocation
    for (size_t i = 0; i < BUF_SIZE; i += 512) {
    	msg_buf[i] = 1;
    }

    for (int sec = 0; sec < SEC_PER_TEST; sec++) {
        clock_gettime(CLOCK_MONOTONIC, &currentStartTime);

        while (true) {
            for (int i = 0; i < OPS_PER_CHECK; i++) {
                if (is_server) {
                    // Send then receive
                    if (EXIT_SUCCESS != send_wrapper(sock_fd, &(msg_buf[((iterations + i) * msg_len) % (BUF_SIZE - msg_len)]), msg_len)) {
                        goto work_cleanup;
                    }
                    if (EXIT_SUCCESS != recv_wrapper(sock_fd, &(msg_buf[((iterations + i) * msg_len) % (BUF_SIZE - msg_len)]), msg_len)) {
                        goto work_cleanup;
                    }
                } else {
                    // Receive then send
                    if (EXIT_SUCCESS != recv_wrapper(sock_fd, &(msg_buf[((iterations + i) * msg_len) % (BUF_SIZE - msg_len)]), msg_len)) {
                        goto work_cleanup;
                    }
                    if (EXIT_SUCCESS != send_wrapper(sock_fd, &(msg_buf[((iterations + i) * msg_len) % (BUF_SIZE - msg_len)]), msg_len)) {
                        goto work_cleanup;
                    }
                }
            }
            iterations += OPS_PER_CHECK;

            clock_gettime(CLOCK_MONOTONIC, &currentTime);
            timediff = difftimespec_ns(currentTime, currentStartTime);
            //printf("Difference is: %09" PRIi64 " \n", timediff);
            if (timediff > ONE_SEC_NS) {
                break;
            }
        }
        results[sec] = iterations;
        iterations = 0;
    }

    float total = 0;
    for (int sec = 0; sec < SEC_PER_TEST; sec++) {
        printf("In second %d, ran %lu iterations.\n", sec, results[sec]);
        total += (float) results[sec];
    }
    printf("Total operations in the test is: %f\n", total);
    total = total / (float) SEC_PER_TEST;
    printf("Average operations per second is: %f\n", total);

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
    struct sockaddr_un server_addr; 
    int addrlen = sizeof(server_addr);
    int opt = 1;
    const char *uds_path;
    
    // Other state
    int errnum = -1;
    int ret = EXIT_FAILURE;     
    
    // Args
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

    // Check second argument - the length of the messages to send
    msg_len = atoi(argv[ARG_MSG_LEN]);
    if (msg_len < 1 || msg_len > MAX_MSG_LEN) {
        printf("ERROR: invalid message length. Should be 0 < msg_len <= %d, not %d\n", MAX_MSG_LEN, msg_len);
        goto cleanup;
    } 

    // Third argument - the uds name.
    uds_path = argv[ARG_UDS_NAME];

    // Create socket
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        goto cleanup;
    } 

    // Set up local address
    server_addr.sun_family = AF_UNIX; 
    strncpy(server_addr.sun_path, uds_path, 32);

    if (is_server) {
        // Bind the socket to the path
        unlink(uds_path);
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
