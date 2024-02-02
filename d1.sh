cut -d";" -f6 data.csv | LC_ALL=C sort -S50% | LC_ALL=C uniq -c | LC_ALL=C sort -nr | head -n 10
