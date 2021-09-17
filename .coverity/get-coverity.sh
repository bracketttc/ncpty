#!/bin/sh

if [ -z "${COVERITY_TOKEN}" ]; then
    echo "COVERITY_TOKEN environment variable not found, quitting..."
    exit
fi

wget https://scan.coverity.com/download/linux64 --post-data "token=${COVERITY_TOKEN}&project=bracketttc%2Fncpty" -O coverity_tool.tgz
