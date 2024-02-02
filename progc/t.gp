# Données
datafile = '../temp/t.txt'

# Sortie
outputfile = '../images/t.png'

# Configuration du terminal et du fichier de sortie
set terminal pngcairo enhanced font "Arial,12" size 1000,700

# Configuration du séparateur de données
set datafile separator ","

# Configuration du fichier de sortie
set output outputfile

# Configuration du titre
set title "Les 10 villes les plus visitées"

# Configuration de la légende
set key outside right top

# Configuration des axes
set xlabel "Nom des villes"
set ylabel "Nombre de passages"

# Ajustement des marges autour de l'histogramme
set lmargin at screen 0.1
set rmargin at screen 0.9
set tmargin at screen 0.9
set bmargin at screen 0.2

# Style de l'histogramme
set style fill solid border -1
set boxwidth 0.8

# Rotation des étiquettes sur l'axe x pour une meilleure lisibilité
set xtics rotate by -45

# Tracé de l'histogramme à partir du fichier "d1.txt"
plot datafile using 2:xtic(3) with boxes lc rgb "#0072BD" notitle
