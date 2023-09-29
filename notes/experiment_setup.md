
# Plan
* Finish writing initial benches for all IPC types.
* Then, for promising ones, attempt to control environment
  * Use smaller node on CloudLab (more pressure on AutoNuma to load balance)
  * Scenario 1: Make client/server processes largeer (spinning threads, large mem allocation w/ touches to that memory) - 1/2 node size maybe (want it theoretically possible to colocate)
  * Scenario 2: Start pinned dummy process node1, start server, start dummy process, start client. All 4 process take ~1/2 available resources.
* When rerunning, monitor core/memory placement by polling:
  * ```numastat``` for memory
  * ```ps``` for cores (https://unix.stackexchange.com/questions/11437/how-to-find-how-many-cores-a-process-is-using)

# Brainstorm: Ideas to control environment:
* Same binary; shared pages?
    * Should only be an issue if not cached, e.g., memory usage is larger than cache, which shouldn't happen right now
    * https://stackoverflow.com/questions/12388918/shared-library-bottleneck-on-numa-machine
* Implement barrier between multiple processes?
* Power save -> run more pairs of processes? More complicated communication pattern?
* Results: latency OR amount of data (maybe latency more important, because data volume in IPC message passing most often small like RPCs; memcached/redis may have large KV pairs)
* AutoNUMA -> use large footprint of memory for load balancing, but need to allocate more memory to trigger this (make sure to touch mem, for phys allocation)
* AutoNUMA -> force migration by creating colocated high priority allocation on the current node
* Fallback: force AutoNUMA to interleave

Different workloads, different things:
* Some will be locality bound
* Some will be bandwidth bound
Centralized 
