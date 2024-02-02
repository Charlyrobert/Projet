# Données
datafile = '../temp/d2.txt'

# Sortie
outputfile = '../images/d2.png'


# Configuration du terminal et du fichier de sortie
set terminal pngcairo enhanced font "Arial,12" size 700,1000

# Configuration du séparateur de données
set datafile separator ","

# Configuration du fichier de sortie
set output outputfile



# Configuration de la légende
set key outside right top

# Configuration des axes
set ylabel 'Nombre de kilomètres par conducteur' 
set xlabel 'Nom' rotate by 90 offset -34,-4
set y2label 'Nombre de kilomètres ' offset -1


# Ajustement des marges autour de l'histogramme
set lmargin at screen 0.1
set rmargin at screen 0.9
set tmargin at screen 0.9
set bmargin at screen 0.2

# Activation des graduations sur l'axe Y2
set y2tics

# Style de l'histogramme
set style fill solid border -1
set boxwidth 0.8

# Rotation des étiquettes sur l'axe x pour une meilleure lisibilité
set xtic rotate by 90 scale 0 
set xtics offset 0,-10
set y2tics rotate by 90 offset 0,-2


# Tracé de l'histogramme à partir du fichier "d1.txt"
plot datafile using 2:xtic(3) with boxes lc rgb "#0072BD" notitle axes x1y2
