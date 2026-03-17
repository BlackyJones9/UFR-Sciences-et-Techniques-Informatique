import time
import Structure
import PPP
import Prim
import HDS

def CompareMin(listeTemps, listeLen, listeAlgo):
    min = 0
    for i in range (1, 4):
        if listeLen[min] > listeLen[i]:
            min = i
    for i in range(4):
        if i != min:
            percentGain(listeLen[i], listeTemps[i], listeLen[min], listeTemps[min], listeAlgo[i], listeAlgo[min])

def percentGain(len1, temps1, len2, temps2, algo1, algo2):
    percentLen = (len2 / len1) * 100
    print("Le circuit du", algo2, "est", 100 - percentLen,"pourcents plus court que celui du", algo1)
    
    if temps1 < temps2:
        print("Le temps d'exécution du", algo2, "est", temps2 / temps1, "fois plus long que", algo1, "\n")
    else:
        print("Le temps d'exécution du", algo2, "est", temps1 / temps2, "fois plus rapide que", algo1, "\n")

def mainPPP(pointList, D):
    tempsDebutPPP = time.time()
    lenPPP, circuit = PPP.PPP(pointList, D)
    tempsFinPPP = time.time()
    circuit = Structure.circuitEnSommets(pointList, circuit)
    
    print("Résultat du PPP :")
    print("Longueur du circuit obtenu : ", lenPPP)
    print("Circuit Obtenu : ", circuit)
    print("Durée d'execution : ", tempsFinPPP - tempsDebutPPP, "\n")
    
    tempsDebutOptPPP = time.time()
    lenOptPPP, circuit = PPP.OptPPP(pointList, D)
    tempsFinOptPPP = time.time()
    circuit = Structure.circuitEnSommets(pointList, circuit)
    
    print("Résultat du OptPPP :")
    print("Longueur du circuit obtenu : ", lenOptPPP)
    print("Circuit Obtenu : ", circuit)
    print("Durée d'execution : ", tempsFinOptPPP - tempsDebutOptPPP)
    percentGain(lenPPP, tempsFinPPP - tempsDebutPPP, lenOptPPP, tempsFinOptPPP - tempsDebutOptPPP, "PPP", "OptPPP")
    
def mainPvcPrim(pointList, D):
    tempsDebut = time.time()
    longueurCircuit, circuit = Prim.PvcPrim(pointList, D)
    tempsFin = time.time()
    
    print("Résultat de l'algorithme de Prim :")
    print("Longueur du circuit obtenu : ", longueurCircuit)
    print("Circuit Obtenu : ", circuit)
    print("Durée d'execution : ", tempsFin - tempsDebut, "\n")
    
def mainHDS(pointList, D):
    tempsDebut = time.time()
    longueurCircuit, circuit = HDS.HDS(pointList, D)
    tempsFin = time.time()
    
    print("Résultat du HDS :")
    print("Longueur du circuit obtenu : ", longueurCircuit)
    print("Circuit Obtenu : ", Structure.circuitEnSommets(pointList, circuit))
    print("Durée d'execution : ", tempsFin - tempsDebut, "\n")

def mainAvecMoyenne(nbTry, n, isFile, pointList, D):
    tempsMoyenPPP = 0
    distanceMoyennePPP = 0
    tempsMoyenoptPPP = 0
    distanceMoyenneoptPPP = 0
    tempsMoyenPrim = 0
    distanceMoyennePrim = 0
    tempsMoyenHDS = 0
    distanceMoyenneHDS = 0
    for i in range(nbTry):
        if (not(isFile)):
            pointList = Structure.initPointList(n)
            D = Structure.initD(pointList, len(pointList))
        
        tempsDebut = time.time()
        longueurCircuit, _ = PPP.PPP(pointList, D)
        tempsFin = time.time()
        tempsMoyenPPP += tempsFin - tempsDebut
        distanceMoyennePPP += longueurCircuit
        
        tempsDebut = time.time()
        longueurCircuit, _ = PPP.OptPPP(pointList, D)
        tempsFin = time.time()
        tempsMoyenoptPPP += tempsFin - tempsDebut
        distanceMoyenneoptPPP += longueurCircuit
        
        tempsDebut = time.time()
        longueurCircuit, _ = Prim.PvcPrim(pointList, D)
        tempsFin = time.time()   
        tempsMoyenPrim += tempsFin - tempsDebut
        distanceMoyennePrim += longueurCircuit

        tempsDebut = time.time()
        longueurCircuit, _ = HDS.HDS(pointList, D)
        tempsFin = time.time()
        tempsMoyenHDS += tempsFin - tempsDebut
        distanceMoyenneHDS += longueurCircuit
        
    tempsMoyenPPP = tempsMoyenPPP / nbTry
    tempsMoyenoptPPP = tempsMoyenoptPPP / nbTry
    tempsMoyenPrim = tempsMoyenPrim / nbTry
    tempsMoyenHDS = tempsMoyenHDS / nbTry
    distanceMoyennePPP = distanceMoyennePPP / nbTry
    distanceMoyenneoptPPP = distanceMoyenneoptPPP / nbTry
    distanceMoyennePrim = distanceMoyennePrim / nbTry
    distanceMoyenneHDS = distanceMoyenneHDS / nbTry
    
    print("\nRésultats de", nbTry, "essais avec", n, "points\n")
    
    print("Résultat du PPP :")
    print("Temps Moyen :", tempsMoyenPPP)
    print("Distance Moyenne :", distanceMoyennePPP, "\n")
    
    print("Résultat du OPTPPP :")
    print("Temps Moyen :", tempsMoyenoptPPP)
    print("Distance Moyenne :", distanceMoyenneoptPPP, "\n")
    
    print("Résultat de l'algorithme de Prim :")
    print("Temps Moyen :", tempsMoyenPrim)
    print("Distance Moyenne :", distanceMoyennePrim, "\n")
    
    print("Résultat du HDS :")
    print("Temps Moyen :", tempsMoyenHDS)
    print("Distance Moyenne :", distanceMoyenneHDS, "\n")
    
    CompareMin([tempsMoyenPPP, tempsMoyenoptPPP, tempsMoyenPrim, tempsMoyenHDS], [distanceMoyennePPP, distanceMoyenneoptPPP, distanceMoyennePrim, distanceMoyenneHDS],
               ["PPP", "OptPPP", "Prim", "HDS"])
        
def InputFile():
    isValidInput = False
    isFile = False
    while not(isValidInput):
        res = input("Enter the path of a file or none for random points: ")
        if (res == "none"):
            isValidInput = True
        else:
            try:
                file = open(res)
                isValidInput = True
                isFile = True
            except:
                print("Input has to be a valid path or 'none'.")  
    if isFile:
        return file
    else:
        return None

        
def main():
    pointList = []
    D = []
    res = InputFile()
    isFile = not(res is None)    
    if not(isFile):
        isValidInput = False
        while not(isValidInput):
            res = input("Enter a number of Points: ")
            try:
                n = int(res)
                if (n > 2):
                    isValidInput = True
            except:
                print("Input has to be an int superior to 2.")  
        isValidInput = False
        while not(isValidInput):
            res = input("How many tries do you want ? ")
            try:
                nbTry = int(res)
                if (nbTry > 0):
                    isValidInput = True
            except:
                print("Input has to be an int superior to 0, type 1 if you want to see the results or an number n for an average of time and length over n tries.")  
        
    else:
        isValidFile = False
        while not(isValidFile):
            try:
                pointList = Structure.ConvertirEnPointList(res)
                D = Structure.initD(pointList, len(pointList))
                isValidFile = True
            except:
                print("File has to be in the type (x,y) with only one point per line.") 
                res = InputFile()
            
        isFile = True
        n = len(pointList)
        nbTry = 1
            
    if nbTry == 1:
        if not (isFile):
            pointList = Structure.initPointList(n)
            D = Structure.initD(pointList, len(pointList))
        Structure.printPointsEtSommets(pointList)
        mainPPP(pointList, D)
        mainPvcPrim(pointList, D)
        mainHDS(pointList, D)
    else:
        mainAvecMoyenne(nbTry, n, isFile, pointList, D)
        
main()