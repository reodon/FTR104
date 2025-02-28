#! /usr/bin/env nix-shell
#! nix-shell -i bash -p bc

nr_txp=1

while read line_spn
do
    spn="${line_spn%:*}"
    freq_spn="${line_spn#*:}"

    c_txp=0
    prev_txp=
    prev_freq_txp=

    while read line_txp
    do
        c_txp=$((c_txp + 1))

        fields=(${line_txp//:/ })
        txp=${fields[0]}
        freq_txp=${fields[1]}

        if [ `echo "$freq_spn < 30.0" | bc` == 1 ]
        then
            break
        fi

        if [ `echo "$freq_spn > $freq_txp" | bc` == 1 ]
        then
            prev_txp="$txp"
            prev_freq_txp="$freq_txp"
        else
            diff_low=`echo "scale=5; $prev_freq_txp - $freq_spn" | bc`
            diff_high=`echo "scale=5; $freq_txp - $freq_spn" | bc`
            diff_low_abs="${diff_low#-}"

            if [ `echo "$diff_low_abs < $diff_high" | bc` == 1 ]
            then
                echo "$spn": "$freq_spn": "$prev_txp": "$prev_freq_txp": "$diff_low"
            else
                echo "$spn": "$freq_spn": "$txp": "$freq_txp": "$diff_high"
            fi

            # echo "$spn": "$prev_freq_txp": "$freq_spn": "$freq_txp"
            # echo "$spn": "$dl": "$freq_spn": "$dh"

            nr_txp=$((nr_txp + c_txp))
            break
        fi
    done < <(sed -n "$nr_txp"',$p' txp2.dat)
    # done < <(sed -n "$nr_txp"',$p' txp1.w.dat)
done < spn.dat
