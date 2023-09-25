# Loopback Socket for IPC

## Description

Create a server and a client. The server and the client send messages of size n over a socket (loopback device).
The server sends and then receives a message; the client receives and then sends a message.

The number of messages exchanged (send + recv = 1 operations) is measured per second for 30 seconds.

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
numactl --cpunodebind=1 --membind=1 ./loopbackprog server 1 <msg_len>
```

In another terminal, run the client:
```
numactl --cpunodebind=1 --membind=1 ./loopbackprog client 1 <msg_len>
```

#### Run on different nodes

In one terminal, start the server:
```
numactl --cpunodebind=0 --membind=0 ./loopbackprog server 1 <msg_len>
```

In another terminal, run the client:
```
numactl --cpunodebind=1 --membind=1 ./loopbackprog client 1 <msg_len>
```

### Run with default policy

In one terminal, start the server:
```
./loopbackprog server 1 <msg_len>
```

In a second terminal, run the client:
```
./loopbackprog client 1 <msg_len>
```

## Results

### Msg len = 1

Different:
```
Total operations in the test is: 757632.000000
Average operations per second is: 25254.400391
```

Same:
```
Total operations in the test is: 905344.000000
Average operations per second is: 30178.132812
```

Default:
```
Total operations in the test is: 973888.000000
Average operations per second is: 32462.933594
```

### Msg len = 1024

Different:
```
Total operations in the test is: 861120.000000
Average operations per second is: 28704.000000
```

Same:
```
Total operations in the test is: 946880.000000
Average operations per second is: 31562.666016
```

Default:
```
Total operations in the test is: 1002112.000000
Average operations per second is: 33403.734375
```

### Msg len = 4096

Different:
```
Total operations in the test is: 865472.000000
Average operations per second is: 28849.066406
```

Same:
```
Total operations in the test is: 933312.000000
Average operations per second is: 31110.400391
```

Default:
```
Total operations in the test is: 867008.000000
Average operations per second is: 28900.267578
```
