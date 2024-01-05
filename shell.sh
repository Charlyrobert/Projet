Traitement D1
cut -d";" -f6 data.csv | sort -S1G | uniq -c | sort -nr | head -n 10

Traitement D2
awk -F';' '{
    conducteur[$6] += $5
}
END {
    for (c in conducteur) {
        print conducteur[c], c
    }
}' data.csv | sort -rn | head -n 10


Traitement L
awk -F';' '{
    trajet[$1] += $5
}
END {
    for (t in trajet) {
        print t, trajet[t]
    }
}' data.csv | sort -n | head -n 10

