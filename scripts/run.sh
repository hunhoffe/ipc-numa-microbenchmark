#!/bin/bash

ITERATIONS=10
BASELINE_PREFIX=""
NUMA_0_PREFIX="numactl --cpunodebind=0 --membind=0"
NUMA_1_PREFIX="numactl --cpunodebind=1 --membind=1"

BINDIR=$(git rev-parse --show-toplevel)
BINDIR=$BINDIR/bin

# First argument is the log output directory
LOGDIR=$1
rm -rf $LOGDIR # TODO(erika): delete this line later. Only for testing.  
mkdir $LOGDIR
echo "==== Create log directory: $LOGDIR"

echo "==== Looking for binaries in: $BINDIR"
for file in $BINDIR/*; do 
    if [ -f "$file" ]; then 
        echo "$file" 
    fi 
done


