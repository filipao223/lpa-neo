#!/usr/bin/env bash

EVENTNUM=0
EVENTDEADL=0

if [ "$#" -ne "1" ]; then
    echo "Number of events as parameter please"
else
    # Create new file called input.in (or overwrite if it exists)
    echo "$1" > input.in
    while [ "$EVENTNUM" -ne "$1" ]; do
        EVENTDEADL=$((1 + RANDOM % 100000))
        echo "$EVENTDEADL $((1 + RANDOM % $EVENTDEADL)) $((1 + RANDOM % 150))" >> input.in
        ((EVENTNUM++))
    done

fi