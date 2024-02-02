#!/bin/bash

# Fonction d'affichage de l'aide
print_help() {
  echo "Usage: $0 <chemin_fichier_csv> [options]"
  echo "Options:"
  echo "  -h           Afficher l'aide"
  echo "  -d1          Effectuer le traitement D1"
  echo "  -d2          Effectuer le traitement D2"
  echo "  -l           Effectuer le traitement L"
  echo "  -t           Effectuer le traitement T"
  echo "  -s          Effectuer le traitement s"
}

# Mesure du temps avant le traitement
start_time=$(date +%s)

# Mesure du temps après le traitement
end_time=$(date +%s)
duration=$((end_time - start_time))
echo "Durée totale des traitements: $duration secondes"

echo "Terminé!"
