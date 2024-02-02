# Données
datafile = '../temp/s.txt'

# Sortie
outputfile = '../images/s.png'

# Configuration du terminal et du fichier de sortie
set terminal pngcairo enhanced font "arial,10" size 5000,600
set output outputfile

# Configuration du titre
set title "Courbes de Max, Min et Moyenne avec des Barres d'Erreur"

# Configuration de la légende
set key outside right top

# Désactiver les étiquettes de l'axe des abscisses
set xtics format ""

# Tracé des courbes Max, Min et Moyenne avec les numéros ID sous chaque point
plot datafile using 1:4:xtic(2) with lines title "Max", \
     datafile using 1:5:xtic(2) with lines title "Min", \
     datafile using 1:6:xtic(2) with lines title "Moyenne"
