awk -F';' '{
    conducteur[$6] += $5
}
END {
    for (c in conducteur) {
        print conducteur[c], c
    }
}' data.csv | sort -rn | head -n 10
