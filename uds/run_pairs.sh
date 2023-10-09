#!/bin/bash

PRECMD_SERVER=""
PRECMD_SERVER="numactl --cpunodebind=0 --membind=0"

PRECMD_CLIENT=""
PRECMD_CLIENT="numactl --cpunodebind=1 --membind=1"

echo "4 pairs at once"

$PRECMD_CLIENT ./udsprog server 1024 uds0 &
$PRECMD_CLIENT ./udsprog server 1024 uds1 &
$PRECMD_SERVER ./udsprog server 1024 uds2 &
$PRECMD_SERVER ./udsprog server 1024 uds3 &

$PRECMD_CLIENT ./udsprog client 1024 uds0 >> concurrent.log &
$PRECMD_CLIENT ./udsprog client 1024 uds1 >> concurrent.log &
$PRECMD_SERVER ./udsprog client 1024 uds2 >> concurrent.log &
$PRECMD_SERVER ./udsprog client 1024 uds3 >> concurrent.log &
