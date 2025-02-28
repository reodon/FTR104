#!/usr/bin/env bash

cat spn.txt \
| awk -F$'\t' -f spn.awk \
| sort -t: -n -k2 \
| {
    while read line
    do
        if [[ "$line" =~ ♭ ]]; then
            echo "$line" \
            | sed 's/♭/#/' \
            | tr 'ABCDEFG' 'GABCDEF'
        else
            echo "$line" \
            | sed 's/♯/#/'
        fi
    done
} > spn.dat

