# ipc-numa-microbenchmark

Diffferent IPC mechanisms to write a microbenchmark for:
- [x] Socket (loopback device)
- [x] Named pipes
- [ ] Anonymous pipes
- [x] Unix Domain socket
- [ ] SysV Message Queues/POSIX Message Queues
- [ ] Signals
- [ ] D-Bus subsystem
- [ ] Netlink Sockets (https://en.wikipedia.org/wiki/Netlink)

Ideas to control environment:
* Same binary; shared pages?
* Implement barrier between multiple processes?
* Power save -> run more pairs of processes? More complicated communication pattern?
* Results: latency OR amount of data (maybe latency more important, because data volume in IPC message passing most often small like RPCs; memcached/redis may have large KV pairs)
* AutoNUMA -> use large footprint of memory for load balancing, but need to allocate more memory to trigger this (make sure to touch mem, for phys allocation)
* AutoNUMA -> force migration by creating colocated high priority allocaiton on the current node
* Fallback: force AutoNUMA to interleave

Different workloads, different things:
* Some will be locality bound
* Some will be bandwidth bound
Centralized 
