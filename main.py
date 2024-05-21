
import subprocess #lancer des applications
import re #trouver sous chaine de caractere
import matplotlib.pyplot as plt #faire les graphiques
from statistics import mode

#param simulation :
nbSimulation = 20
nbTirage = 10
nbFrameSkip = 500
noiseFactor = 0.18
nbBoule = 8
nbBouleTriangle = int(nbBoule*(nbBoule + 1)/2)
bouleSize = 30.0
brasseurSpeed = 0.7
timebtwTirage = 10.0
dt = 0.001
RNDBoulePos = 1 #true
RNDbrasspos = 1 #true
timeBeforStart = 10.0
bounceNoise = 0.1

#analyse :
factorSpeed = 0.5 #le taux de divergence pour considérer que les simulation sont différentes.

#run lance 10 simulation avec differentes seed, moyenne les résultats
def main():
    allres = []
    for i in range(0,nbSimulation):
        seed = i
        res = run(seed)
        allres.append(res)
        print(res)
    #print(allres)

    listeTranspo = [] #chaque liste, contient tous les ieme tirages
    for i in range(0, nbTirage):
        listeTranspo.append([])
        for j in range(0, len(allres)):
            listeTranspo[i].append(allres[j][i])
    #print(listeTranspo)

    nbCorrelation = correlation(allres)
    print(nbCorrelation)


    divtime = nbTirage 
    for i in range(0, nbTirage, 1):
        if(nbCorrelation[i] >= factorSpeed):
            divtime = i+2 #+1 tirage commence en 1, +1 on compte celui ou ca depasse
        else:
            break
    #print(divtime)
    show(nbCorrelation, divtime)


#compte le nombre de correlation a chaque tirage et divise par nbSim
def correlation(listeTranspo): 
    TCorrelation = [0 for i in range(0, nbTirage)]
    for i in range(0, nbTirage):#tirage num i 
        nbcorrelation = 0
        for j in range(0, nbSimulation):#colonne j
            equal = 1 #1 au minimul (lui meme)
            for k in range(j+1, nbSimulation):#colonne k
                sameLine = True
                for l in range(0, i+1): #valeur l
                    if(listeTranspo[k][l] != listeTranspo[j][l]):
                        sameLine = False
                if(sameLine == True):
                    equal += 1
            if(equal > nbcorrelation):
                nbcorrelation = equal
        TCorrelation[i] = nbcorrelation/nbSimulation


    return TCorrelation



def run(seed): #lance une simulation avec les parametres
    res = []
    tipe_exe_path = ".\\x64\\Release\\TIPE.exe"
    tipe_params = [
        "seed", str(seed),
        "nbFrameSkip", str(nbFrameSkip),
        "noise", str(noiseFactor),
        "nbtirage", str(nbTirage),
        "nbboule", str(nbBoule),
        "sizeboule", str(bouleSize),
        "brasseurSpeed", str(brasseurSpeed),
        "timebtwTirage", str(timebtwTirage),
        "timeBeforStart", str(timeBeforStart),
        "dt", str(dt),
        "RNDBoulePos", str(RNDBoulePos),
        "RNDbrassPos", str(RNDbrasspos),
        "bounceNoiseBall", str(bounceNoise),
        "bounceNoiseBrass", str(bounceNoise),
        ]
    process = subprocess.Popen([tipe_exe_path] + tipe_params, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    lines = []
    for line in process.stdout:
        line = line.decode("utf-8").rstrip()  # Convertir les octets en chaîne de caractères et supprimer les caractères de nouvelle ligne
        lines.append(line)  # Ajouter la ligne à la liste
        #print(line)
    for line in lines:
        chaine = re.search(r"\[(.*?)\]", line)
        if chaine: #si il a trouve qqchose
            liste = chaine.group(1) # Extraction de la liste à partir du résultat de la recherche
            res = [int(x) for x in liste.split(",")] # Conversion de la chaîne de caractères en une liste d'entiers

    return res

def show(tab, divtime):
    plt.plot(range(1, nbTirage+1),tab)
    plt.xticks([i for i in range(1, len(tab)+1)])
    plt.ylim(0.0, 1.1)
    plt.ylabel("% de corrélation")
    plt.xlabel('tirage n°')
    plt.axhline(y=factorSpeed, color='red', linestyle='dashed')
    plt.title("Divergence entre les tirages, Tom Demagny")
    plt.suptitle("temps de divergence : " + str(divtime))

    # Positionnement du texte à droite du graphique
    xlim = plt.xlim()
    ylim = plt.ylim()
    x_text = xlim[1] + (xlim[1] - xlim[0]) * 0.05  # Ajustement horizontal
    y_text = ylim[0]  # Position verticale

    plt.text(x_text, y_text, 
        "noise : " + str(noiseFactor) +
        "\nnbSim : " + str(nbSimulation) + 
        "\nnbBoule : " + str(nbBouleTriangle) + 
        "\ndt : " + str(dt) + 
        "\ntime : " + str(timebtwTirage) + 
        "\ndivCap : " + str(factorSpeed)
        , verticalalignment='bottom', horizontalalignment='left')
    plt.subplots_adjust(right=0.75)


    plt.show()

main()