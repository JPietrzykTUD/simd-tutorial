#!/bin/bash

${TUTORIAL_SCRIPTS}/install_tsl.sh

args_array=("$@")
for i in "${args_array[@]}"
do
  :
  echo "### Got variable $i ###"
done

#if --interactive in args then run bash
if [[ " ${args_array[@]} " =~ " --interactive " ]]; then
  echo "### Running interactive mode ###"
  /bin/bash
else
  echo "### Running non-interactive mode ###"
  # pass args to the /bien/bash
  exec /bin/bash -c "$*"
fi