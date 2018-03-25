#!/usr/bin/env bash
# set -xe

function show_usage {
  echo "Usage : fat.sh\n
        For more info please type -h or --help."
}

function show_help {
  echo "Usage : fat.sh\n
        Prints 10 files or dirs from current dir, that allocates most disk space."
}

if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
  show_help
  exit 0
fi

if [ "$#" != "0" ]; then
  show_usage >&2
  exit 1
fi

ls -lahS \
	| awk '{s = $5 "\t"; for (i = 9; i <= NF; i++) s = s $i " "; print s}' \
	| awk NF \
	| head -n 10
exit 0