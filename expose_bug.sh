#!/bin/bash

inputs=("375 375 4309" "375 374 4309")

make > /dev/null
for ((i = 0; i < ${#inputs[@]}; i++))
do
    echo "${inputs[$i]}"
    ./bug ${inputs[$i]}
    python3 check_matrices.py
    echo ""
done
make clean > /dev/null
