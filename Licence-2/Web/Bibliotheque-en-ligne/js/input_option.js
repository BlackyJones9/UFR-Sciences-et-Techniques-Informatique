function validEntier(input) {
    // Convertir le contenu de l'input en chaîne de caractères
    let inputValue = input.value.toString();

    // Vérifier si la lettre "e" est présente dans la chaîne
    if (inputValue.includes('e')) {
        // Remplacer la lettre "e" par une chaîne vide pour supprimer la saisie
        inputValue = inputValue.replace('e', '');
        // Mettre à jour la valeur de l'input
        input.value = inputValue;
    }

    // Vérifier si la saisie est un nombre entier
    let entier = parseInt(input.value);
    if (isNaN(entier) || entier <= 0) {
        // Si ce n'est pas un nombre entier, réinitialiser la valeur à vide
        input.value = '';
    }
}