#!/bin/bash

# Remplacez les 4 par le SNR que vous voulez tester
# TEST_SNR=10 pour sim 1 
TEST_SNR=8
#echo "🔍 Début de la recherche pour SIM 1 (N=128) au SNR = $TEST_SNR"
# echo "🔍 Début de la recherche pour SIM 2 (N=128) au SNR = $TEST_SNR"
# echo "🔍 Début de la recherche pour SIM 3 (N=96) au SNR = $TEST_SNR"
#echo "🔍 Début de la recherche pour SIM 4 (N=64) au SNR = $TEST_SNR"
echo "🔍 Début de la recherche pour SIM 5 (N=32) au SNR = $TEST_SNR"

echo "=========================================================="

# On boucle sur 's' en descendant de 8 à 1
for s in {8..1..-1}; do
    
    # On boucle sur 'f' de 0 jusqu'à la valeur de 's' (car s >= f)
    for f in $(seq 0 $s); do
        
        echo -n "Test avec s=$s, f=$f  --->  "
        
        # On lance le simulateur pour UN SEUL SNR, on cache les tableaux (2> /dev/null), 
        # et on filtre la sortie avec grep pour ne garder que la ligne de données !
        
        #./simulator -m $TEST_SNR -M $TEST_SNR -s 1 -e 100 -K 32 -N 128 -D "rep-hard8" --qs $s --qf $f 2> /dev/null | grep "^$TEST_SNR" | awk -F',' '{print "FER = " $8 " | BER = " $7}'
        #./simulator -m $TEST_SNR -M $TEST_SNR  -s 1 -e 100 -K 32 -N 128 -D "rep-soft8" --qs $s --qf $f 2> /dev/null | grep "^$TEST_SNR" | awk -F',' '{print "FER = " $8 " | BER = " $7}'
        # ./simulator -m $TEST_SNR -M $TEST_SNR  -s 1 -e 100 -K 32 -N 96 -D "rep-soft8" --qs $s --qf $f 2> /dev/null | grep "^$TEST_SNR" | awk -F',' '{print "FER = " $8 " | BER = " $7}'
        # ./simulator -m $TEST_SNR -M $TEST_SNR  -s 1 -e 100 -K 32 -N 64 -D "rep-soft8" --qs $s --qf $f 2> /dev/null | grep "^$TEST_SNR" | awk -F',' '{print "FER = " $8 " | BER = " $7}'
        ./simulator -m $TEST_SNR -M $TEST_SNR  -s 1 -e 100 -K 32 -N 32 -D "rep-soft8" --qs $s --qf $f 2> /dev/null | grep "^$TEST_SNR" | awk -F',' '{print "FER = " $8 " | BER = " $7}'
    done
    echo "----------------------------------------------------------"
done

echo "✅ Exploration terminée !"








import csv
import glob
import os

# Crée un dossier pour stocker les nouveaux fichiers
output_dir = "donnees_extraites"
os.makedirs(output_dir, exist_ok=True)

# Liste tous les fichiers simX.csv
fichiers = glob.glob("sim*.csv")

for fichier in fichiers:
    nom_base = os.path.basename(fichier)
    nom_sortie = os.path.join(output_dir, f"clean_{nom_base}")
    
    with open(fichier, 'r') as f_in, open(nom_sortie, 'w', newline='') as f_out:
        reader = csv.DictReader(f_in)
        writer = csv.writer(f_out)
        
        # Écriture de l'en-tête
        writer.writerow(["Eb/N0(dB)", "FER"])
        
        # Lecture et extraction
        for row in reader:
            writer.writerow([row["Eb/N0(dB)"],  row["FER"]])
            
    print(f"Fichier créé : {nom_sortie}")
