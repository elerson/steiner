#!/bin/bash
FILES=testes/*
n=50
for f in $FILES
do

#echo $n
./projeto world666.stp 5 $n
 n=$((n + 20))
done
