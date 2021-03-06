#!/bin/bash -x

# Copyright 2021 Timothy Brackett
# Licensed under the MIT license

banner=$1

SECONDS=0

tmux new -d -s 'interaction-test' "\"${banner}\"" -f sleep 100
sleep 2
tmux send-keys -t 'interaction-test' ' q'

while tmux has-session -t 'interaction-test' 2> /dev/null ; do
    true
done

elapsed_time=${SECONDS}
[ "${elapsed_time}" -lt "3"  ] && [ "${elapsed_time}" -gt "1" ]
