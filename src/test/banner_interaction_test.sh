#!/bin/sh

tmux new -d -s 'interaction-test' "$@ sleep 100"
tmux send-keys -t 'interaction-test.0' ' q'

while tmux has-session -t 'interaction-test' 2> /dev/null ; do
    true
done
