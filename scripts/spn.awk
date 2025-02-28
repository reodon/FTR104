!/^#/ {
    sub(/ +/, "", $1);
    if (length($1) == 1)
    {
        $1 = $1 " ";
    }

    for (i=2; i<=NF; i++)
    {
        sub(/ .*/, "", $i);
        printf "%.2s%2d:%s\n", $1, (i-3), $i;
        #printf "%s%2d:%s\n", $1, (i-3), $i;
        #print length($1);
    }
}
