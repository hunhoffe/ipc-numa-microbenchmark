#!/bin/bash

ITERATIONS=10
BASELINE_PREFIX=" "
NUMA_0_PREFIX="numactl --cpunodebind=0 --membind=0"
NUMA_1_PREFIX="numactl --cpunodebind=1 --membind=1"
MSG_SIZE=1024

BINDIR=$(git rev-parse --show-toplevel)
BINDIR=$BINDIR/bin

run_tests() {
    # Parse args
    bin=$1
    binname=$(basename $bin)
    test_type=$2
    iters=$4
    msg_size=$5
    log_file=$3/$binname-$test_type-$msg_size.log
    server_prefix=$6
    client_prefix=$7
    echo "run_test() - log_file=$log_file server_prefix=$server_prefix client_prefix=$client_prefix"

    # Create the log file
    touch $log_file

    # Run the test for $iters iterations and pipe output to the log file
    for (( i = 0; i <= $iters; i++ )) 
    do 
        echo "     iteration $i"
        echo "==== iteration $i" >> $log_file
        $server_prefix $bin server $msg_size &
        $client_prefix $bin client $msg_size >> $log_file
    done

    # kill any zombies
    pkill $binname
}

# First argument is the log output directory
LOGDIR=$1
rm -rf $LOGDIR # TODO(erika): delete this line later. Only for testing.  
mkdir $LOGDIR
echo "==== Create log directory: $LOGDIR"

echo "==== Looking for binaries in: $BINDIR"
for file in $BINDIR/*; do 
    if [ -f "$file" ]; then 
        echo "$file"
        run_tests $file "base" $LOGDIR $ITERATIONS $MSG_SIZE $BASELINE_PREFIX $BASELINE_PREFIX 
        run_tests $file "best" $LOGDIR $ITERATIONS $MSG_SIZE $NUMA_0_PREFIX $NUMA_0_PREFIX 
        run_tests $file "worst" $LOGDIR $ITERATIONS $MSG_SIZE $NUMA_0_PREFIX $NUMA_1_PREFIX
    fi 
done


