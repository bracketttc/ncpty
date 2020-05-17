#!/bin/sh

# Add suid
chmod +s "$1"
$@ true

exit_code=$?

# Remove suid
chmod -s "$1"

exit ${exit_code}
