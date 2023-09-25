#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>
#include <unistd.h> 

#include <sys/types.h>
#include <sys/syscall.h>

#include <errno.h>
#include <time.h>
#include <inttypes.h>

#include "anonpipeprog.h"

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
            perror("read");
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
            perror("write");
            return EXIT_FAILURE;
        } else {
            bytes_recv += recv_ret;
            buf += recv_ret;
        }
    }
    return EXIT_SUCCESS;
}

int do_work(int *fd, int msg_len, bool is_server) {
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
                    // Read then write
                    if (EXIT_SUCCESS != read_wrapper(fd[0], msg_buf, msg_len)) {
                        goto work_cleanup;
                    }
                } else {
                    // Write and then read
                    if (EXIT_SUCCESS != write_wrapper(fd[1], msg_buf, msg_len)) {
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
    int pipe_fds[2] = {-1, -1};
    
    // Other state
    int ret = EXIT_FAILURE;     
    int server_pid = -1;
    int server_pid_fd = -1;
    
    // Args
    int msg_len = 1;
    bool is_server = false;
 
    // Check number of arguments 
	if (NUM_ARGS > argc) {
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

    // check second argument - the length of the messages to send
    msg_len = atoi(argv[ARG_MSG_LEN]);
    if (msg_len < 1 || msg_len > MAX_MSG_LEN) {
        printf("ERROR: invalid message length. Should be 0 < msg_len <= %d, not %d\n", MAX_MSG_LEN, msg_len);
        goto cleanup;
    } 

    if (is_server) {
        // Make sure no extra args
        if (NUM_ARGS != argc) {
            printf("ERROR: Wrong number of arguments (expected: %d, actual: %d)\n", NUM_ARGS, argc);
            printf(USAGE_STR);
            goto cleanup;
        }

        // Create anonymous pipe
        if (0 != pipe(pipe_fds)) {
            perror("mkfifo");
            goto cleanup;
        }
        printf("Server PID: %d, fd[0]=%d, fd[1]=%d\n", getpid(), pipe_fds[0], pipe_fds[1]);

        if (EXIT_SUCCESS != do_work(pipe_fds, msg_len, is_server)) {
            printf("do_work() failed\n");
            goto cleanup;
        }
    } else {
        // Check for client specific arguments, namely server pid and fd number
        if (NUM_ARGS + 3 != argc) {
            printf("ERROR: Wrong number of arguments\n");
            printf(USAGE_STR);
            goto cleanup;
        }

        // Check third argument - server pid
        server_pid = atoi(argv[ARG_SERVER_PID]);
        if (server_pid <= 1) {
            printf("ERROR: invalid server PID. Should be 1 < server_pid, not %d\n", server_pid);
            goto cleanup;
        } 

        // Check fourth and fifth arguments - fds from server
        pipe_fds[0] = atoi(argv[ARG_SERVER_FD1]);
        pipe_fds[1] = atoi(argv[ARG_SERVER_FD2]);
        if (pipe_fds[0] < 0 || pipe_fds[1] < 0) {
            printf("ERROR: invalid server file descriptor(s): [%d, %d]\n", pipe_fds[0], pipe_fds[1]);
            goto cleanup;
        }

        // Get a fd to the server pid
        if (-1 == (server_pid_fd = syscall(SYS_pidfd_open, server_pid, 0))) {
            perror("pidfd_open");
            goto cleanup;
        }

        // Get a copy of the server fds to the pipe
        if (-1 == (pipe_fds[0] = syscall(SYS_pidfd_getfd, server_pid_fd, pipe_fds[0], 0))) {
            perror("pidfd_getfd 0");
            goto cleanup;
        }
        if (-1 == (pipe_fds[1] = syscall(SYS_pidfd_getfd, server_pid_fd, pipe_fds[1], 0))) {
            perror("pidfd_getfd 1");
            goto cleanup;
        }

        if (EXIT_SUCCESS != do_work(pipe_fds, msg_len, is_server)) {
            printf("do_work() failed\n");
            goto cleanup;
        }
    } 

    ret = EXIT_SUCCESS;
    printf("Exited successfully!\n");

cleanup:
    if (is_server) {
        if (-1 != pipe_fds[0]) {
            close(pipe_fds[0]);
        }
        if (-1 != pipe_fds[1]) {
            close(pipe_fds[1]);
        }
    }
    if (-1 != server_pid_fd) {
        close(server_pid_fd);
    }
    return ret; 
}
