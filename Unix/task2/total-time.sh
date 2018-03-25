#!/usr/bin/env bash
#set -xe

function show_usage {
  echo "Usage : total-time.sh \n
        For more info please type -h or --help."
}

function show_help {
  echo "Usage : total-time.sh\n
        Computes total time of execution task 'job' from list of task dependencies.\n
        Task dependency is stored as <task_name> <execution_time> [<dependency_task1> <dependency_task2> ...]\n
        \n
        User should supply list of task dependencies to standard input.\n"
}

MAIN_JOB_NAME="job"

function compute_total_time {
  awk -F: '{
    n = split($0, a, " "); s = a[1] ": "; 
    for (i = 3; i <= n; i++) s = s a[i] " "; 
    s = s "\n\t @echo " a[2]; 
    print s
  }' \
    | make "${MAIN_JOB_NAME}" -f - \
    | awk '{ sum += $1 } END { print sum }'
}

export LC_ALL=C

if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
  show_help
  exit 0
fi

if [ "$#" != "0" ]; then
  show_usage >&2
  exit 1
fi

# Wanted to use smth like "if compute_total_time; then", but dunno how.
MAKE_LOG="$( { compute_total_time; } 2>&1 )"
if [[ $MAKE_LOG == *"Circular"* ]]; then
  echo "Cyclic dependency" >&2
  exit 1
else
  echo "${MAKE_LOG}"
  exit 0
fi