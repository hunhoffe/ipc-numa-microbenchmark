# Named Pipes for IPC

## Description

Create a server and a client. The client sends messages of size n over a named pipe to the server.

The number of messages exchanged (1 send = 1 operation) is measured per second for 30 seconds.

## Building

To build, run:
```
make
```

To clean, run:
```
make clean
```

## Running

There are three use-cases we want to measure: same node, different node, and autoNuma.
Make sure you are running on a machine with at least 2 sockets and ensure AutoNUMA (NUMA balancing) is enabled
by checking/writing 1 to ```/proc/sys/kernel/numa_balancing```.

#### Run on same node

In one terminal, start the server:
```
numactl --cpunodebind=1 --membind=1 ./namedpipeprog server 1 <msg_len>
```

In another terminal, run the client:
```
numactl --cpunodebind=1 --membind=1 ./namedpipeprog client 1 <msg_len>
```

#### Run on different nodes

In one terminal, start the server:
```
numactl --cpunodebind=0 --membind=0 ./namedpipeprog server 1 <msg_len>
```

In another terminal, run the client:
```
numactl --cpunodebind=1 --membind=1 ./namedpipeprog client 1 <msg_len>
```

### Run with default policy

In one terminal, start the server:
```
./namedpipeprog server 1 <msg_len>
```

In a second terminal, run the client:
```
./namedpipeprog client 1 <msg_len>
```

## Results

### Msg len = 1

Different:
```
Total operations in the test is: 27952256.000000
Average operations per second is: 931741.875000
```

Same:
```
Total operations in the test is: 29626048.000000
Average operations per second is: 987534.937500
```

Default:
```
Total operations in the test is: 33606912.000000
Average operations per second is: 1120230.375000
```

### Msg len = 1024

Different:
```
Total operations in the test is: 18122112.000000
Average operations per second is: 604070.375000
```

Same:
```
Total operations in the test is: 27855680.000000
Average operations per second is: 928522.687500
```

Default:
```
Total operations in the test is: 29271808.000000
Average operations per second is: 975726.937500
```

### Msg len = 4096

Different:
```
Total operations in the test is: 11299264.000000
Average operations per second is: 376642.125000
```

Same:
```
Total operations in the test is: 20520768.000000
Average operations per second is: 684025.625000
```

Default:
```
Total operations in the test is: 20567936.000000
Average operations per second is: 685597.875000

In second 22, ran 989376 iterations.```
