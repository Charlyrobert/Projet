awk -F';' '{
    trajet[$1] += $5
}
END {
    for (t in trajet) {
        print t, trajet[t]
    }
}' data.csv | sort -n | head -n 10
