#!/bin/sh

# Add suid
chmod +s "$1"
$@

exit_code=$?

# Remove suid
chmod -s "$1"

exit ${exit_code}
