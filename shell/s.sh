#!/bin/bash

# Nom du fichier source C à adapter
fichier_source="progcs.c"

# Nom du fichier exécutable à adapter
fichier_executable="progcs"

# Chemin du fichier data.csv à adapter
chemin_data_csv="chemin/vers/data.csv"

# Compiler le fichier source C avec GCC (sous Linux/macOS)
gcc -o "$fichier_executable" "$fichier_source"

# Vérifier si la compilation a réussi
if [ $? -eq 0 ]; then
    # Exécuter le programme en passant le chemin de data.csv en argument
    echo "Compilation réussie. Exécution du programme avec $chemin_data_csv..."
    ./"$fichier_executable" "$chemin_data_csv"

    # Exécution du script Gnuplot
    echo "Exécution du script Gnuplot..."
    gnuplot plot.gp
else
    # Afficher un message d'erreur si la compilation a échoué
    echo "La compilation a échoué ou gnuplot ne fonctionne pas"
fi
