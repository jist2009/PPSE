hard pour task6 s=4 f=2 / s=4 f=3











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























import os
import pandas as pd
import glob

# 1. Définir le chemin vers le dossier contenant tes 45 fichiers
dossier_fichiers = "./" 

# Trouver tous les fichiers CSV (modifie "*.txt" si tes fichiers sont des .txt)
fichiers = glob.glob(os.path.join(dossier_fichiers, "*.csv"))

liste_dataframes = []

for fichier in fichiers:
    # Récupérer le nom du fichier (ex: "sim_s5_f3.csv") pour identifier les paramètres
    nom_fichier = os.path.basename(fichier)
    
    # Lire le fichier (ajuste 'sep' si les colonnes sont séparées par des espaces ou point-virgules)
    df = pd.read_csv(fichier, sep=',') 
    
    # Ajouter une colonne avec le nom du fichier
    df['Parametres_Source'] = nom_fichier
    
    liste_dataframes.append(df)

# Combiner tous les fichiers en un seul grand tableau
donnees_globales = pd.concat(liste_dataframes, ignore_index=True)

# Sauvegarder le résultat dans un fichier unique
fichier_sortie = "simulations_compilees.csv"
donnees_globales.to_csv(fichier_sortie, index=False)

print(f"Terminé ! {len(fichiers)} fichiers ont été regroupés dans '{fichier_sortie}'.")

























import pandas as pd
import numpy as np

# 1. Charger le fichier global
df = pd.read_csv('simulations_compilees.csv')
# 1. Affiche les vrais noms pour voir le problème
print("Colonnes détectées :", df.columns.tolist())

# 2. Nettoie les espaces invisibles autour des noms
df.columns = df.columns.str.strip()
# 2. Tes points cibles (ici les 13 points de ton message précédent)
cible = {
    0.0: 1.0, 1.0: 0.8, 2.0: 0.8, 3.0: 0.5, 4.0: 0.3,
    5.0: 0.18, 6.0: 0.07, 7.0: 0.02, 8.0: 0.006,
    9.0: 0.001, 10.0: 0.00012, 11.0: 0.000008, 12.0: 0.00000025
}

resultats = []

# 3. Calcul de l'écart pour chaque fichier de simulation
for fichier, groupe in df.groupby('Parametres_Source'):
    erreur = 0
    n_points = 0
    
    for _, row in groupe.iterrows():
        ebno = row['Eb/N0(dB)']
        if ebno in cible:
            # Différence entre les puissances de 10
            erreur += (np.log10(row['FER'] + 1e-10) - np.log10(cible[ebno] + 1e-10))**2
            n_points += 1
            
    if n_points > 0:
        resultats.append({'Fichier': fichier, 'Score_Erreur': erreur / n_points})

# 4. Trier et afficher le gagnant
df_res = pd.DataFrame(resultats).sort_values(by='Score_Erreur')
print("Top 3 des paramètres les plus proches (score le plus bas = le meilleur) :")
print(df_res.head(3))
