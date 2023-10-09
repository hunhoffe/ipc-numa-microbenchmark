#!/bin/bash

echo "10 iterations: " >> log.txt

./udsprog server 1024 &
./udsprog client 1024 >> log.txt

./udsprog server 1024 &
./udsprog client 1024 >> log.txt

./udsprog server 1024 &
./udsprog client 1024 >> log.txt

./udsprog server 1024 &
./udsprog client 1024 >> log.txt

./udsprog server 1024 &
./udsprog client 1024 >> log.txt

./udsprog server 1024 &
./udsprog client 1024 >> log.txt

./udsprog server 1024 &
./udsprog client 1024 >> log.txt

./udsprog server 1024 &
./udsprog client 1024 >> log.txt

./udsprog server 1024 &
./udsprog client 1024 >> log.txt

./udsprog server 1024 &
./udsprog client 1024 >> log.txt
