#ifndef __LOOPBACKPROG_H
#define __LOOPBACKPROG_H

#define NUM_ARGS 4 
#define ARG_CLIENT_SERVER 1
#define ARG_NUM_THREADS 2
#define MAX_NUM_THREADS 16
#define ARG_MSG_LEN 3
#define MAX_MSG_LEN 8192
#define USAGE_STR "Usage: <loopbackprog> <client|server> <num_threads> <msg_len>\n"

#define LOOPBACK_IP "127.0.0.1"
#define LOOPBACK_PORT 10110

#define ONE_SEC_NS 1000000000L
#define SEC_PER_TEST 60
#define OPS_PER_CHECK 64

#endif /* __LOOPBACKPROG_H */
