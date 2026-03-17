import Structure

# PPP : Renvoie le cycle résultant au PVC et la distance totale parcourue selon la stratégie du point le plus proche avec une liste de points et une matrice des distances
# Complexité : O(n^2) avec n le nombre de points de list
def PPP(list, D):
    pointList = list.copy() #Crée une copie de list afin de ne pas la modifier durant exécution, elle contiendra les points qui n'ont pas encore été ajouté au circuit
    index = 0
    Q1 = pointList[index]
    res = [Q1]
    pointList.pop(index)
    while (len(pointList) != 0):
        minValue = 0 #Représente l'indice du point le plus proche du circuit dans pointList
        minInsert = 0 #Représente l'indice du point du circuit le plus proche de minValue
        for i in range(len(pointList)):
            for j in range (len(res)):
                indexCycle = list.index(res[j])
                indexPoint = list.index(pointList[i]) #Récupère les indices selon list (qui reste fixe) afin d'ajouter les bons points au circuit
                indexMin = list.index(pointList[minValue])
                dist = D[indexCycle][indexPoint]
                distMin = D[indexCycle][indexMin]
                if (distMin > dist): #On cherche les indices minimums
                    minValue = i
                    minInsert = j
        res.insert(minInsert, pointList[minValue]) #Ajoute le point le plus proche au circuit
        pointList.remove(pointList[minValue]) #Le point a été ajouté donc il est retiré de pointList
    res.reverse() #On ajoute par insertion donc le circuit est inversé
    res.append(res[0]) #On ajoute le point de départ à la fin afin de créer un vrai circuit
    longueurCircuit = Structure.calculateDistance(D, res, list) #On calcule la distance du circuit
    circuit = res
    return [longueurCircuit, circuit] #On renvoie la longueur du circuit et le circuit en lui-même

# OptPPP : Renvoie le cycle résultant au PVC et la distance totale parcourue après avoir décroisé la solution donné par l'algorithme PPP
# Complexité : O(2 * n^2) avec n le nombre de points de list
def OptPPP(list, D):
    dist, circuit = PPP(list, D) #On récupère le résultat de l'algorithme PPP
    return Structure.décroiser(D, dist, circuit, list) #On décroise avant de renvoyer le résultat