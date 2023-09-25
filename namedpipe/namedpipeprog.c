#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <unistd.h> 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <time.h>
#include <inttypes.h>

#include "namedpipeprog.h"

// https://stackoverflow.com/questions/64893834/measuring-elapsed-time-usung-clock-gettimeclock-monotonic
int64_t difftimespec_ns(const struct timespec after, const struct timespec before)
{
    return ((int64_t)after.tv_sec - (int64_t)before.tv_sec) * (int64_t)1000000000
         + ((int64_t)after.tv_nsec - (int64_t)before.tv_nsec);
}

int read_wrapper(int fd, char *buf, int msg_len) {
    int bytes_sent = 0;
    int send_ret = 0;

    while (bytes_sent != msg_len) {
        send_ret = read(fd, buf, msg_len - bytes_sent);
        if (-1 == send_ret) {
            perror("write");
            return EXIT_FAILURE;
        } else {
            bytes_sent += send_ret;
            buf += send_ret;
        }
    }
    return EXIT_SUCCESS;
}

int write_wrapper(int fd, char *buf, int msg_len) {
    int bytes_recv = 0;
    int recv_ret = 0;

    while (bytes_recv != msg_len) {
        recv_ret = write(fd, buf, msg_len - bytes_recv);
        if (-1 == recv_ret) {
            perror("read");
            return EXIT_FAILURE;
        } else {
            bytes_recv += recv_ret;
            buf += recv_ret;
        }
    }
    return EXIT_SUCCESS;
}

int do_work(int fd, int msg_len, bool is_server) {
    char *msg_buf = NULL;
    int ret = EXIT_FAILURE;
    struct timespec currentStartTime = { 0 };
    struct timespec currentTime = { 0 };
    long iterations = 0;
    int64_t timediff = 0;
    long results[SEC_PER_TEST] = { 0 };

    // Initialize message buffer
    if (NULL == (msg_buf = malloc(msg_len))) {
        goto work_cleanup;
    }

    for (int sec = 0; sec < SEC_PER_TEST; sec++) {
        clock_gettime(CLOCK_MONOTONIC, &currentStartTime);

        while (true) {
            for (int i = 0; i < OPS_PER_CHECK; i++) {
                if (is_server) {
                    // Read only
                    if (EXIT_SUCCESS != read_wrapper(fd, msg_buf, msg_len)) {
                        goto work_cleanup;
                    }
                } else {
                    // Write only
                    if (EXIT_SUCCESS != write_wrapper(fd, msg_buf, msg_len)) {
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
    int pipe_fd = -1;
    
    // Other state
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

    if (is_server) {
        // Create named pipe
        if (0 != mkfifo(PIPE_PATH, 0777)) {
            perror("mkfifo");
            goto cleanup;
        }

        // Open pipe for reading
        if ((pipe_fd = open(PIPE_PATH, O_RDONLY)) == -1) {
            perror("openRead");
            goto cleanup;
        }

        if (EXIT_SUCCESS != do_work(pipe_fd, msg_len, is_server)) {
            printf("do_work() failed\n");
            goto cleanup;
        }
    } else {
        // Open pipe for writing
        if ((pipe_fd = open(PIPE_PATH, O_WRONLY)) == -1) {
            perror("openWrite");
            goto cleanup;
        }

        if (EXIT_SUCCESS != do_work(pipe_fd, msg_len, is_server)) {
            printf("do_work() failed\n");
            goto cleanup;
        }
    } 

    ret = EXIT_SUCCESS;
    printf("Exited successfully!\n");

cleanup:
    if (-1 != pipe_fd) {
        close(pipe_fd);
    }
    unlink(PIPE_PATH);
    return ret; 
}
