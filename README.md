# ipc-numa-microbenchmark

Diffferent IPC mechanisms to write a microbenchmark for:
- [x] Socket (loopback device)
- [ ] Named pipes
- [ ] Anonymous pipes
- [ ] Unix Domain socket
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

Since there are many d430s, I'll start there. First up is to disable hyperthreading.

## Environment Setup

* Create small-lan CloudLab experiment with d430 node w/ the Ubuntu 22.04 image.
* Install dependencies 
  ```bash
  sudo apt update
  sudo apt install -y numactl linux-tools-common linux-tools-5.15.0-86-generic
  ```
* Use script from [here](https://www.alexgallego.org/perf/compiler/explorer/flatbuffers/smf/2018/06/30/effects-cpu-turbo.html) to set high/low frequencies
  ```
  # Load the functions
  source set_frequency.sh

  # Set cpu to performance
  # sudo cpupower frequency-set --governor performance
  cpu_enable_performance_cpupower_state

  # Get the high/low frequencies
  cpu_available_frequencies

  # Set min freq to max freq
  cpu_set_min_frequencies <MAX_FREQ>
  cpu_set_max_frequencies <MAX_FREQ>
  ```
* Disable hyperthreading
  ```bash
  echo off | sudo tee /sys/devices/system/cpu/smt/control
  ```
* Ensure AutoNUMA is enabled (should be 1)
  ```bash
  cat /proc/sys/kernel/numa_balancing
  ```