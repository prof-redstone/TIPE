import matplotlib.pyplot as plt

# Le tableau de valeurs
values =[5, 6, 2, 2, 2, 3, 1, 7, 4, 5, 3, 3, 1, 2, 3, 1, 3, 0, 4, 1, 1, 2, 3, 3, 0, 3, 0, 0]

# Les indices des valeurs
indices = list(range(len(values)))

# Création du graphique avec une taille ajustée
plt.figure(figsize=(12, 6))
bars = plt.bar(indices, values, color='blue')

# Ajouter des labels et un titre
plt.xlabel('indice des boules')
plt.ylabel("Nombre d'apparition")
plt.title('70 simulations, premier tirage, 30 sec de mélange')

# Ajouter les valeurs au-dessus des barres
for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2.0, yval + 0.2, str(int(yval)), ha='center', va='bottom', fontsize=9)

# Afficher le graphique
plt.xticks(indices)  # Assure que tous les indices sont affichés sur l'axe des x
plt.show()
