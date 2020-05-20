#!/bin/bash

# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

banner=$1

num_tests=0
num_passed=0
num_failed=0

declare -a tests
declare -a tests_failed

print_report() {
    echo "Total: ${num_tests} tests"
    echo "Pass: ${num_passed}, fail: ${num_failed}"

    if [ "${num_failed}" -ne "0" ]; then
        echo "Tests failed:"
        for test_name in "${tests_failed[@]}"; do
            echo "    ${test_name}"
        done
    fi
}
trap 'print_report' EXIT


pass_test() {
    num_passed=$((num_passed + 1))
    echo "Test Passed."
}


fail_test() {
    num_failed=$((num_failed + 1))
    tests_failed+=("$1")
    echo "Test Failed."
}


add_test() {
    num_tests=$((num_tests + 1))
    tests+=("$1")
}


run_tests() {
    mapfile -t test_order  < <(shuf -e "${tests[@]}" | sed -e '/^$/d')

    echo "${test_order[@]}"

    echo "Start testing: $(date)"
    echo "-----------------------------"
    echo ""

    for i in "${!test_order[@]}"; do
        current_test="${test_order[$i]}"
        echo "$((i + 1))/${num_tests} Test: ${current_test}"
        echo "\"${current_test}\" start time: $(date)"
        echo "Output:"
        echo "-----------------------------"
        "${current_test}"
        rc=$?
        echo "<end of output>"
        echo "-----------------------------"
        if [ "${rc}" -eq "0" ]; then
            pass_test "${current_test}"
        else
            fail_test "${current_test}"
        fi
        echo "\"${current_test}\" end time: $(date)"
        echo "-----------------------------"
        echo ""
    done
}




banner_usage_test() {
    ! "${banner}"
}
add_test 'banner_usage_test'

banner_permission_test() {
    # Add suid
    chmod +s "${banner}"

    "${banner}" true
    rc=$?

    # Remove suid
    chmod -s "${banner}"

    [ "${rc}" -ne "0" ]
}
add_test 'banner_permission_test'


banner_interaction_test() {
    SECONDS=0
    tmux new -d -s 'interaction-test' "\"${banner}\" sleep 100"
    sleep 1
    tmux send-keys -t 'interaction-test' ' q'

    while tmux has-session -t 'interaction-test' 2> /dev/null ; do
        true
    done
    elapsed_time=${SECONDS}
    [ "${elapsed_time}" -lt "3" ]
}
add_test 'banner_interaction_test'


banner_return_true_test() {
    "${banner}" true
}
add_test 'banner_return_true_test'


banner_return_false_test() {
    ! "${banner}" false
}
add_test 'banner_return_false_test'


banner_execution_failure_test() {
    ! "${banner}" ./a.out
}
add_test 'banner_execution_failure_test'


run_tests


exit ${num_failed}
