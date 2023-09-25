#ifndef __ANONPIPEPROG_H
#define __ANONPIPEPROG_H

#define NUM_ARGS 3 
#define ARG_CLIENT_SERVER 1
#define ARG_MSG_LEN 2
#define MAX_MSG_LEN 8192
#define ARG_SERVER_PID 3
#define ARG_SERVER_FD1 4
#define ARG_SERVER_FD2 5
#define USAGE_STR "Usage:\n\t./anonpipeprog server <msg_len>\n\t./anonpipeprog client <msg_len> <server_pid> <server_fd1> <server_fd2>\n"

#define ONE_SEC_NS 1000000000L
#define SEC_PER_TEST 30
#define OPS_PER_CHECK 64

#endif /* __ANONPIPEPROG_H */
