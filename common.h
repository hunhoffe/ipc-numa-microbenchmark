#ifndef __COMMON_H
#define __COMMON_H

#define NUM_ARGS 3
#define ARG_CLIENT_SERVER 1
#define ARG_MSG_LEN 2
#define MAX_MSG_LEN 8192
#define USAGE_STR "<client|server> <msg_len>\n"

#define ONE_SEC_NS 1000000000L
#define SEC_PER_TEST 30
#define OPS_PER_CHECK 64

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
        if (0 <= (send_ret = send(sock_fd, buf, msg_len - bytes_sent, 0))) {
            bytes_sent += send_ret;
            buf += send_ret;
        } else {
            perror("send");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int recv_wrapper(int sock_fd, char *buf, int msg_len) {
    int bytes_recv = 0;
    int recv_ret = 0;

    while (bytes_recv != msg_len) {
        if (0 <= (recv_ret = recv(sock_fd, buf, msg_len - bytes_recv, MSG_WAITALL))) {
            bytes_recv += recv_ret;
            buf += recv_ret;
        } else {
            perror("recv");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int do_work(int sock_fd, int msg_len, bool is_server) {
    char *msg_buf = NULL;
    int ret = EXIT_FAILURE;
    struct timespec currentStartTime = { 0 };
    struct timespec currentTime = { 0 };
    long num_ops = 0;
    int64_t timediff = 0;
    long results[SEC_PER_TEST] = { 0 };

    // Initialize message buffer
    if (NULL == (msg_buf = malloc(msg_len))) {
        perror("malloc failed\n");
        goto work_cleanup;
    }

    for (int sec = 0; sec < SEC_PER_TEST; sec++) {
        clock_gettime(CLOCK_MONOTONIC, &currentStartTime);

        while (true) {
            for (int i = 0; i < OPS_PER_CHECK; i++) {
                if (is_server) {
                    // Send then receive
                    if (EXIT_SUCCESS != send_wrapper(sock_fd, msg_buf, msg_len)) {
                        printf("send_wrapper() failed\n");
                        goto work_cleanup;
                    }
                    if (EXIT_SUCCESS != recv_wrapper(sock_fd, msg_buf, msg_len)) {
                        printf("recv_wrapper() failed\n");
                        goto work_cleanup;
                    }
                } else {
                    // Receive then send
                    if (EXIT_SUCCESS != recv_wrapper(sock_fd, msg_buf, msg_len)) {
                        printf("recv_wrapper() failed\n");
                        goto work_cleanup;
                    }
                    if (EXIT_SUCCESS != send_wrapper(sock_fd, msg_buf, msg_len)) {
                        printf("send_wrapper() failed\n");
                        goto work_cleanup;
                    }
                }
            }
            num_ops += OPS_PER_CHECK;

            clock_gettime(CLOCK_MONOTONIC, &currentTime);
            timediff = difftimespec_ns(currentTime, currentStartTime);
            //printf("Difference is: %09" PRIi64 " \n", timediff);
            if (timediff >= ONE_SEC_NS) {
                break;
            }
        }
        results[sec] = num_ops;
        num_ops = 0;
    }

    float total = 0;
    for (int sec = 0; sec < SEC_PER_TEST; sec++) {
        printf("In second %d, ran %lu operations.\n", sec, results[sec]);
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

#endif /* __LOOPBACKPROG_H */
