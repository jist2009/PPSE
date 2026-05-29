import pandas as pd
import matplotlib.pyplot as plt

# Configuration des fichiers
# Remplacez par vos vrais noms de fichiers
files = {'Sim1': 'sim1.csv', 'Sim2': 'sim2.csv', 'Sim3': 'sim3.csv', 'Sim4': 'sim4.csv', 'Sim5': 'sim5.csv'}
data = {name: pd.read_csv(path) for name, path in files.items()}

def setup_plot(title, xlabel, ylabel):
    plt.figure(figsize=(10, 6))
    plt.yscale('log')
    plt.grid(True, which="both", ls="-", alpha=0.5)
    plt.xlabel(xlabel, fontsize=12)
    plt.ylabel(ylabel, fontsize=12)

# --- Graphique 1 : BER/FER pour Sim1 & Sim2 (vs Eb/N0) ---
setup_plot("Performance par Eb/N0", "Signal-to-Noise Ratio (Eb/N0 (dB))", "Frame/Bit Error Rate")
df1, df2 = data['Sim1'], data['Sim2']
plt.plot(df1['Eb/N0(dB)'], df1['BER'], 'o--', label='Sim1 BER', color='green', mfc='none')
plt.plot(df1['Eb/N0(dB)'], df1['FER'], 'x-', label='Sim1 FER', color='green')
plt.plot(df2['Eb/N0(dB)'], df2['BER'], 'o--', label='Sim2 BER', color='blue', mfc='none')
plt.plot(df2['Eb/N0(dB)'], df2['FER'], 'x-', label='Sim2 FER', color='blue')
plt.legend()
plt.savefig('graphe1_neon.png')
# --- Graphique 2 : FER pour Sim2-5 (vs Eb/N0) ---
setup_plot("FER vs Eb/N0", "Signal-to-Noise Ratio (Eb/N0 (dB))", "Frame Error Rate")
colors = ['tab:blue', 'tab:red', 'tab:orange', 'tab:purple']
for i, name in enumerate(['Sim2', 'Sim3', 'Sim4', 'Sim5']):
    plt.plot(data[name]['Eb/N0(dB)'], data[name]['FER'], 'x-', color=colors[i], label=name)
plt.legend(title="FER")
plt.savefig('graphe2_neon.png')

# --- Graphique 3 : FER pour Sim2-5 (vs Es/N0) ---
setup_plot("FER vs Es/N0", "Signal-to-Noise Ratio (Es/N0 (dB))", "Frame Error Rate")
for i, name in enumerate(['Sim2', 'Sim3', 'Sim4', 'Sim5']):
    plt.plot(data[name]['Es/N0(dB)'], data[name]['FER'], 'x-', color=colors[i], label=name)
plt.legend(title="FER")
plt.savefig('graphe3_neon.png')