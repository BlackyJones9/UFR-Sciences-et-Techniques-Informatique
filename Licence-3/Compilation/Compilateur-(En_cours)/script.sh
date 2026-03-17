#!/bin/bash

# Arrêter le script dès qu'une commande échoue, variable non définie, ou pipe échoue
set -euo pipefail

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <fichier_source.txt> <fichier_destination sans_extension>"
    exit 1
fi

SOURCE_FILE="$1"
DESTINATION_FILE="$2"
DIR="bin"

# Vérifier que le fichier source existe
if [ ! -f "$SOURCE_FILE" ]; then
    echo "Erreur : Le fichier '$SOURCE_FILE' n'existe pas."
    exit 1
fi

# Créer le répertoire de sortie si nécessaire
mkdir -p "$DIR"

ASM="$DIR/$DESTINATION_FILE.asm"
SIP="$DIR/$DESTINATION_FILE.sip"

# Génération du fichier assembleur
echo "Génération du fichier assembleur : $ASM"
if ! bison_flex/./compil "$SOURCE_FILE" > "$ASM"; then
    echo "Erreur : La génération de $ASM a échoué."
    exit 1
fi

# Génération du fichier SIP
echo "Génération du fichier SIP : $SIP"
if ! asipro "$ASM" "$SIP"; then
    echo "Erreur : La génération de $SIP a échoué."
    exit 1
fi

# Exécution du fichier SIP
echo "Exécution du fichier SIP :"
if ! sipro "$SIP"; then
    echo "Erreur : L'exécution de $SIP a échoué."
    exit 1
fi

echo ""

# Déplacer gmon.out si présent
if [ -f gmon.out ]; then
    mv gmon.out "$DIR/gmon.out"
fi