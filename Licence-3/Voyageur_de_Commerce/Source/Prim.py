import Structure
import heapq

# Choisir : renvoie le premier sommet d'une liste de points si il existe -1 sinon
# Complexité : Constant, O(1)
def Choisir(pointList):
    if len(pointList) < 1:
        return -1
    return pointList[0]

# Algo_Prim : renvoie un arbre couvrant de poids minimum selon l'algorithme de Prim en fonction d'une liste de sommets, une liste d'aretes et un sommet
# Complexité : O(m * log2(n) + n * log2(n)) avec n le nombre de points
def Algo_Prim(S, LA, Q):
    ACM = []
    T = [Q]
    omega = []
    length = len(S)
    while len(T) < length: #Continue tant que tous les points n'ont pas été ajouté à la liste T
        for arete in LA:
            x, y, poids = arete
            if (x in T and y not in T) or (x not in T and y in T): #On ajoute toutes les arêtes accessibles à partir d'un seul point du tas à omega (file de priorité)
                heapq.heappush(omega, (poids, x, y))
        pmin, x, y = heapq.heappop(omega) #On récupère la première arête de la file (celle de poids minimal)
        arete_min = (pmin, x, y)
        p, t, s = arete_min
        if t not in T: #On ajoute à T le sommet de l'arête minimale qui n'était pas déjà dans T puis on ajoute l'arête à l'arbre couvrant de poids minimal
            T.append(t)
            ACM.append((t, s, p))
        if s not in T:
            T.append(s)
            ACM.append((t, s, p))
    return ACM #On renvoie l'arbre

# ParcoursPrefixe : renvoie le circuit (et son poids total) crée par un parcours préfixe d'un arbre couvrant de poids minimal à partir d'une matrice de distance
# Complexité : O(n) avec n le nombre d'arètes (équivalent au nombre de sommets) du circuit
def ParcoursPrefixe(ACM, D):
    circuit = []
    for arete in ACM: #On parcourt toutes les arêtes de l'arbre
        x, y, poids = arete
        if x not in circuit: #Si on croise un nouveau sommet entrant (l'arête est de la forme x--y), on l'ajoute à la fin du circuit
            circuit.append(x)
        index = circuit.index(x)
        if y not in circuit: #Si on croise un nouveau sommet sortant, on l'ajoute à la fin du circuit si il s'agit du dernier sommet du circuit ou au tout début sinon.
            if index == (len(circuit) - 1):
                circuit.append(y)
            else:
                circuit.insert(0, y)
    
    x = circuit[0]
    y = circuit[len(circuit) - 1]
    circuit.append(circuit[0])
    
    longueurCircuit = 0 #On calcule le poids du circuit
    for i in range (len(circuit) - 1):
        longueurCircuit += D[circuit[i] - 1][circuit[i+1] - 1]
    
    return [longueurCircuit, circuit] #On renvoie la longueur du circuit et le circuit en lui-même

# PvcPrim : renvoie le cycle résultant au PVC selon l'algorithme de Prim et un parcours préfixe selon une liste de points et une matrice des distances
# Compléxité : Somme de Algo_Prim et ParcoursPrefixe soit O(m * log2(n) + n * log2(n) + n)
def PvcPrim(pointList, D):
    S = Structure.pointListEnSommets(pointList) #On convertit la liste de point en liste de sommets 
    l = Structure.listeEnLA(S, D) #On convertit la matrice de distance en liste d'arêtes
    ACM = Algo_Prim(S, l, 1) #On récupère l'arbre résultant de l'algorithme de Prim
    return ParcoursPrefixe(ACM, D) #On renvoie le résultat du parcours préfixe
