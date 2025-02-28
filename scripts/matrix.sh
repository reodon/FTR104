#! /usr/bin/env nix-shell
#! nix-shell -i bash -p bc

for i in {1..65536}
# for i in {1..255}
do
  for j in 1 8 64 256 1024
  # for j in 1 8 32 64 128 256 1024
  do
    echo -n "$i,$j:"
    echo "scale=10; (8*10^6) / ($i*$j)" | bc
  done
done \
| sort -n -t: -k2
