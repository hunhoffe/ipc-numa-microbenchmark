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

#endif /* __LOOPBACKPROG_H */
