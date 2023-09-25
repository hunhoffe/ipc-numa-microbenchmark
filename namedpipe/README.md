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
```

Same:
```
```

Default:
```
```

### Msg len = 1024

Different:
```
```

Same:
```
```

Default:
```
```

### Msg len = 4096

Different:
```
```

Same:
```
```

Default:
```
```
