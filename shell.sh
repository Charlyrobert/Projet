Traitement D1
cut -d";" -f6 data.csv | sort -S1G | uniq -c | sort -nr | head -n 10
