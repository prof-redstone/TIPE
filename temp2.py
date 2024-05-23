import numpy as np

# Le tableau de valeurs
values = [2, 5, 7, 8, 15, 8, 5, 1, 1, 2, 1, 1, 5, 4, 2, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0]
# Calcul de l'espérance (moyenne)
mean = np.mean(values)

# Calcul de l'écart type
std_deviation = np.std(values)

# Affichage des résultats
print(f"L'espérance (moyenne) est : {mean}")
print(f"L'écart type est : {std_deviation}")