#!/bin/sh

# Copyright (c) 2020 Timothy Brackett
# Licensed under the MIT license

tmpscript=$(mktemp)
tmpcode=$(mktemp)
tmpoutput=$(mktemp)

trap 'rm "${tmpscript}" "${tmpcode}"' EXIT

# create a temporary script to pass the exit code back through a temp file
cat <<EOF > "${tmpscript}"
#!/bin/sh

$@ > "${tmpoutput}" 2>&1
echo \$? > "${tmpcode}"
EOF

tmux new -d -s 'bg-session' "sh \"${tmpscript}\""

# wait for tmux session to end
while tmux has-session -t 'bg-session' 2> /dev/null; do
    true
done

cat "${tmpoutput}"

# read exit code and return that ourselves
exit "$(cat "${tmpcode}")"
