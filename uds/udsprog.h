#ifndef __UDSPROG_H
#define __UDSPROG_H

#define NUM_ARGS 3 
#define ARG_CLIENT_SERVER 1
#define ARG_MSG_LEN 2
#define MAX_MSG_LEN 8192
#define USAGE_STR "Usage: ./udsprog <client|server> <msg_len>\n"

#define ONE_SEC_NS 1000000000L
#define SEC_PER_TEST 30
#define OPS_PER_CHECK 64

#endif /* __UDSPROG_H */
