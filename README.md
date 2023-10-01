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

#### Cloudlab Setup

Currently, 2 nodes is sufficient. And since we're fighting against power-saving, let's try for smaller instead of larger.
Since there are just two threads that are used, we want at least 2 cores (hw threads) per node.

There are a lot of options that fit this description in CloudLab. A few of the smallest ones:
* c4130 (Wisconsin) / 2 sockets/ 8 cores / 2 hw threads per core
* c240g1 (Wisconsin) / 2 sockets / 8 cores / 2 hw threads per core
* c220g1 (Wisconsin) / 2 sockets / 8 cores / 2 hw threads per core
* d430 (emulab) / 2 sockets/ 8 cores / 2 hw threads per core
* c6220 (APT) / 2 sockets / 8 cores / 2 hw threads per core
* dss7500 / 2 sockets / 6 cores / 2 hw threads per core

Since there are many d430s, I'll start there.
