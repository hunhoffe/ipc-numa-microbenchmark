#!/bin/bash

echo "Set to same node (node 0), 10 iterations: " >> log.txt

numactl --cpunodebind=1 --membind=1 ./udsprog server 1024 &
numactl --cpunodebind=0 --membind=0 ./udsprog client 1024 >> log.txt

numactl --cpunodebind=1 --membind=1 ./udsprog server 1024 &
numactl --cpunodebind=0 --membind=0 ./udsprog client 1024 >> log.txt

numactl --cpunodebind=1 --membind=1 ./udsprog server 1024 &
numactl --cpunodebind=0 --membind=0 ./udsprog client 1024 >> log.txt

numactl --cpunodebind=1 --membind=1 ./udsprog server 1024 &
numactl --cpunodebind=0 --membind=0 ./udsprog client 1024 >> log.txt

numactl --cpunodebind=1 --membind=1 ./udsprog server 1024 &
numactl --cpunodebind=0 --membind=0 ./udsprog client 1024 >> log.txt

numactl --cpunodebind=1 --membind=1 ./udsprog server 1024 &
numactl --cpunodebind=0 --membind=0 ./udsprog client 1024 >> log.txt

numactl --cpunodebind=1 --membind=1 ./udsprog server 1024 &
numactl --cpunodebind=0 --membind=0 ./udsprog client 1024 >> log.txt

numactl --cpunodebind=1 --membind=1 ./udsprog server 1024 &
numactl --cpunodebind=0 --membind=0 ./udsprog client 1024 >> log.txt

numactl --cpunodebind=1 --membind=1 ./udsprog server 1024 &
numactl --cpunodebind=0 --membind=0 ./udsprog client 1024 >> log.txt

numactl --cpunodebind=1 --membind=1 ./udsprog server 1024 &
numactl --cpunodebind=0 --membind=0 ./udsprog client 1024 >> log.txt
