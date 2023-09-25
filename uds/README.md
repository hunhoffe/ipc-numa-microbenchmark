# Unix Domain Socket for IPC

## Description

Create a server and a client. The server and the client send messages of size n over a Unix domain socket (UDS).
The server sends and then receives a message; the client receives and then sends a message.

The number of messages exchanged (send + recv = 1 operation) is measured per second for 30 seconds.

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
numactl --cpunodebind=1 --membind=1 ./udsprog server 1 <msg_len>
```

In another terminal, run the client:
```
numactl --cpunodebind=1 --membind=1 ./udsprog client 1 <msg_len>
```

#### Run on different nodes

In one terminal, start the server:
```
numactl --cpunodebind=0 --membind=0 ./udsprog server 1 <msg_len>
```

In another terminal, run the client:
```
numactl --cpunodebind=1 --membind=1 ./udsprog client 1 <msg_len>
```

### Run with default policy

In one terminal, start the server:
```
./udsprog server 1 <msg_len>
```

In a second terminal, run the client:
```
./udsprog client 1 <msg_len>
```

## Results

### Msg len = 1

Different:
```
Total operations in the test is: 1001408.000000
Average operations per second is: 33380.265625
```

Same:
```
Total operations in the test is: 1661504.000000
Average operations per second is: 55383.464844
```

Default:
```
Total operations in the test is: 1694976.000000
Average operations per second is: 56499.199219
```

### Msg len = 1024

Different:
```
Total operations in the test is: 988992.000000
Average operations per second is: 32966.39843
```

Same:
```
Total operations in the test is: 1469696.000000
Average operations per second is: 48989.867188
```

Default:
```
Total operations in the test is: 1385344.000000
Average operations per second is: 46178.132812
```

### Msg len = 4096

Different:
```
Total operations in the test is: 753280.000000
Average operations per second is: 25109.333984
```

Same:
```
Total operations in the test is: 1160320.000000
Average operations per second is: 38677.332031
```

Default:
```
Total operations in the test is: 1011968.000000
Average operations per second is: 33732.265625
```
