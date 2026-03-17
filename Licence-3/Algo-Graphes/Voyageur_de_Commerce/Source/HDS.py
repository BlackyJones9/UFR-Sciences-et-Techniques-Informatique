import math
import Structure 

# demiSomme : renvoie le 1er et le 2eme poids minimum d'un sommet i, utilisée pour l'heuristique de la demi somme
# Complexité : Linéaire en le nombre de colonne de D, O(n)
def demiSomme(D, i):
    min1er = float('inf')
    min2nd = float('inf')
    for j in range(len(D)): #On cherche les deux arêtes de poids minimal reliés au sommet i
        if i == j:
            continue
        if D[i][j] <= min1er:
            min2nd = min1er
            min1er = D[i][j]
        elif D[i][j] != min1er and D[i][j] <= min2nd:
            min2nd = D[i][j]
    return (min1er, min2nd)

# HDSCalcul : Renvoie le cycle résultant au PVC et la distance totale parcourue selon l'algorithme de brunch and bound HDS
# Complexité : Dans le pire des cas, la complexité est de l'ordre de O(n!) avec n le nombre de ligne de D (Voir Rapport)
def HDSCalcul(D, branche, poids, hauteur, chemin, visited, longueurCircuit, circuit):
    n = len(D)
    if hauteur == n: #Cas d'arrêt de la récursivité
        if D[chemin[hauteur - 1]][chemin[0]] != 0: #On vérifie que le chemin n'est pas une boucle
            res = poids + D[chemin[hauteur - 1]][chemin[0]] #On met à jour le poids afin d'ajouter la dernière arête 
            if res < longueurCircuit: #Si l'ajout de la dernière arête est avantageux alors on contruit le circuit et on le renvoie
                circuit[:n + 1] = chemin[:] #On remplace les n premiers éléments par chemin
                circuit[n] = chemin[0]
                longueurCircuit = res
        return longueurCircuit, circuit #On renvoie la longueur du circuit et le circuit en lui-même
    
    for i in range (n): #On boucle sur tous les sommets
        if (D[chemin[hauteur - 1]][i] != 0. and not(visited[i])): #On vérifie si le sommet i est relié au dernier sommet du chemin et si il n'a pas déjà été visité
            branche_temp = branche #On sauvegarde la branche pour pouvoir la modifier
            poids += D[chemin[hauteur - 1]][i] #On suppose que i est le prochain sommet du circuit et donc on ajoute le poids de l'arête
            
            curr_min1er, curr_min2nd = demiSomme(D, chemin[hauteur - 1]) #On cherche le 1er et le 2eme poids minimum du dernier sommet du chemin
            min1er, _ = demiSomme(D, i) #On cherche le 1er et le 2eme poids minimum du sommet i
            if hauteur == 1: #Si la hauteur de la branche vaut 1 alors on soustrait la moyenne des premiers minimums
                branche -= (curr_min1er + min1er) / 2
            else: #Sinon il y a au moins 2 points donc on soustrait la moyenne du 1er minimum du sommet du chemin et le 2ème minimum de i
                branche -= (curr_min2nd + min1er) / 2
            
            if branche + poids < longueurCircuit: #Appel récursif si ajouter le sommet i au chemin est avantageux par rapport au circuit courant
                chemin[hauteur] = i
                visited[i] = True #Mise à jour du chemin et du tableau visited
                longueurCircuit, circuit = HDSCalcul(D, branche, poids, hauteur + 1, chemin, visited, longueurCircuit, circuit) #Appel récursif = on avance dans la recherche d'une solution
        
            poids -= D[chemin[hauteur - 1]][i] #On prépare la boucle suivante en annulant les mises à jours ayant eu lieu durant la boucle
            branche = branche_temp
            visited = [False] * len(visited) 
            for j in range(hauteur): #On redéfinit le tableau visited afin d'assurer de ne pas passer des circuit possibles
                visited[chemin[j]] = True
    return longueurCircuit, circuit #On renvoie la longueur du circuit et le circuit en lui-même      

# HDS : renvoie les résultats de la fonction HDSCalcul après initialisation des données selon la matrice des distances D
# Complexité : Meme que HDSCalcul avec une boucle supplémentaire soit O(n! + n)
def HDS(pointList, D):
    n = len(D)
    chemin = [-1] * (n + 1)
    chemin[0] = 0 #On initialise les données utilisées par HDSCalcul
    visited = [False] * n
    visited[0] = True
    
    dist = 0
    for i in range(n):
        min1er, min2nd = demiSomme(D, i)
        dist += min1er + min2nd
    racine = math.ceil(dist / 2) #On calcul la valeur de la racine
    
    longueurCircuit = float('inf')
    circuit = [-1] * (n + 1) #On initialise les résultats par défaut
    longueurCircuit, circuit = HDSCalcul(D, racine, 0, 1, chemin, visited, longueurCircuit, circuit) #On stocke le résultat de l'appel à HDSCalcul
    circuit = Structure.sommetsEnPoints(list(map(lambda x:x+1, circuit)), pointList)
    return Structure.décroiser(D, longueurCircuit, circuit, pointList)