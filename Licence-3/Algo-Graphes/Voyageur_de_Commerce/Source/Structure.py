import numpy as np
import numpy.random as rd
import ast

# initPointList : initialise une liste de coordonnées (x, y) contenant n points
# Complexité : Linéaire en la valeur de n, O(n)
def initPointList(n):
    res = []
    for i in range (n):
        res.append([rd.random(), rd.random()])
    return res

# initD : initialise une matrice contenant les distances entre chacun des points d'une liste de points contenant n elements
# Complexité : Quadratique en la valeur de n, O(n^2)
def initD(pointList, n):
    M = np.zeros((n, n))
    for i in range (n):
        for j in range (n):
            M[i][j] = np.linalg.norm(np.array((pointList[i][0], pointList[i][1])) - np.array((pointList[j][0], pointList[j][1]))) #Calcule la distance entre le point i et le point j puis l'ajoute au bon endroit de la matrice
    return np.array(M).tolist()

# pointListEnSommets : convertit une liste de sommets en une liste de points
# Complexité : Linéaire en la longueur de pointList, O(n)
def pointListEnSommets(pointList):
    res = []
    for i in range(len(pointList)):
        res.append(i + 1)
    return res

# circuitEnSommets : renvoie une liste de sommets equivalente à un circuit de points selon une liste de points
# Complexité : Linéaire en la taille du circuit, O(n)
def circuitEnSommets(pointList, circuit):
    res = []
    for p in circuit:
        ind = pointList.index(p)
        res.append(ind + 1)
    return res

# sommetsEnPoints : renvoie une liste de point equivalente a une liste de sommets selon une liste de points
# Complexité : Linéaire en la taille du circuit, O(n)
def sommetsEnPoints(circuit, pointList):
    res = []
    for s in circuit:
        res.append(pointList[s - 1])
    return res

# listeEnLA : renvoie une liste d'arète équivalente à une liste de sommets ou de points selon une matrice de distance
# Complexité : Quadratique en la longueur de list, O(n^2)
def listeEnLA(list, D):
    LA = []
    length = len(list)
    for x in range (length):
        for y in range (x + 1, length):
            LA.append((list[x], list[y], D[x][y]))
    return LA

# printPointsEtSommets : associe chaque points de pointList à un sommet et affiche chaque associations
# Complexité : Linéaire en la longueur de pointList, O(n)
def printPointsEtSommets(pointList):
    sommetList = pointListEnSommets(pointList)
    for i in range (len(pointList)):
        print(sommetList[i], " : ", pointList[i])
    print()

# calculateDistance : calcule la distance totale du cycle contenant des points de list avec les distances contenus dans D
# Complexité : Linéaire en la longueur du cycle O(n)
def calculateDistance(D, cycle, list):
    dist = 0
    for i in range(1, len(cycle)):
        dist += D[list.index(cycle[i - 1])] [list.index(cycle[i])] #On ajoute à dist la longueur du lien correspond au point i-1 et i du circuit
    return dist

# verifierCroisement3Points : renvoie Vrai si les 3 points p1, p2, p3 sont listés dans le sens antihoraire
# Complexité : Constant O(1)
def verifierCroisement3Points(p1, p2, p3):
    return (p3[1]-p1[1]) * (p2[0]-p1[0]) > (p2[1]-p1[1]) * (p3[0]-p1[0]) #Afin que les 3 points soient en sens antihoraire, cette inégalité doit être respectée

# verifierCroisement : renvoie Vrai si le segment p1p2 croise le segment p3p4 en utilisant un algorithme d'intersection (Voir Rapport)   
# Complexité : Constant O(1)
def verifierCroisement(p1, p2, p3, p4):
    acd = verifierCroisement3Points(p1, p3, p4) #On fait toutes les combinaisons de points possibles dans un ordre croissant
    bcd = verifierCroisement3Points(p2, p3, p4)
    abc = verifierCroisement3Points(p1, p2, p3)
    abd = verifierCroisement3Points(p1, p2, p4)
    return (acd != bcd) and (abc != abd) #Par propriété (Voir Rapport), si les points 1,3,4 et 2,3,4 ou 1,2,3 et 1,2,4 ont les mêmes orientations, alors il n'y a pas de croisement
        
# echange : echange les valeurs d'indices i1 et i2 dans le circuit c
# Complexité : Constant O(1)
def echange(c, i1, i2):        
    x = c[i1]
    c[i1] = c[i2]
    c[i2] = x
    
# verifierDistance : renvoie vrai si la distance totale d'un circuit c après l'échange des valeurs d'indice i1 et i2 est inférieure à dist
# Complexité : Constant O(1)
def verifierDistance(list, D, c, i1, i2, dist):
    c2 = c.copy() #Crée une copie du circuit afin de réaliser un échange sans modifier le circuit initial
    echange(c2, i1, i2)
    return dist > calculateDistance(D, c2, list) #Compare la distance du circuit initial avec celle du circuit copié après un échange

# ConvertirEnPointList : créer une liste de points selon un fichier
# Complexité : Linéaire en le nombre de points, O(n)
def ConvertirEnPointList(file):
    L = file.readlines()
    file.close()
    res = []
    for i in L:
        i = i.replace('\n','')
        if not(len(i) == 0):
            res.append(ast.literal_eval(i))
    return res

# décroiser : Applique un algorithme de décroisement sur un circuit selon une matrice de distances, une liste de reférence et la distance du circuit
# Complexité : Linéaire en le nombre de points, O(n)
def décroiser(D, dist, circuit, list):
    for i in range (len(circuit) - 4): #Pour détecter les croisements, il faut au moins 4 points donc on définit la valeur maximum de i au nombre de points du circuit - 4 pour s'assurer d'avoir toujours assez de points.
        for j in range(i + 2, len(circuit) - 1): #Afin de travailler sur des arêtes du circuit, pour un i donné, on travaille sur l'arête i--i+1 et on parcourt sur toutes les arêtes du circuit positionnée après i--i+1, c'est-à-dire celle de la forme i+2--i+3--..--n avec n la dernière arête.
            if verifierCroisement(circuit[i], circuit[i + 1], circuit[j], circuit[j + 1]) and verifierDistance(list, D, circuit, i + 1, j, dist): #Si il y a un croisement et il est avantageux
                echange(circuit, i + 1, j) #On échange les deux points à l'origine du croisement
    longueurCircuit = calculateDistance(D, circuit, list) #On calcule la distance du nouveau circuit
    return [longueurCircuit, circuit] #On renvoie la longueur du circuit et le circuit en lui-même