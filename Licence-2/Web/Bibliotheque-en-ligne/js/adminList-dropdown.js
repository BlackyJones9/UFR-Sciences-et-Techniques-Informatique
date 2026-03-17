//------------------------
//  CATEGORIES DROPDOWN
//------------------------
let ajouterDropDownList = document.querySelector("#liste-ajouter");
let gererDropDownList = document.querySelector("#liste-gerer");

let btnAjouter = document.querySelector("#btn-ajouter");
let btnGerer = document.querySelector("#btn-gerer");

const toggleGerer = ()=> {
    gererDropDownList.classList.toggle("active");
    btnGerer.classList.toggle("active");
}
const toggleAjouter = ()=> {
    ajouterDropDownList.classList.toggle("active");
    btnAjouter.classList.toggle("active");
}



//--------------------
//  ELEMENTS ONCLICK
//--------------------

//button
let gererLivre = document.querySelector("#gererLivre");
let gererUtilisateur = document.querySelector("#gererUtilisateur");

let ajouterLivre = document.querySelector("#ajouterLivre");
let ajouterUtilisateur = document.querySelector("#ajouterUtilisateur");
let ajouterAuteur = document.querySelector("#ajouterAuteur");
let ajouterAutre = document.querySelector("#ajouterAutre");

let lastBtnActive = document.querySelector("#ajouterLivre");

//menu
let menuGererLivre = document.querySelector(".gerer_livre");
let menuGererUtilisateur = document.querySelector(".gerer_utilisateur");

let menuAjoutLivre = document.querySelector(".ajout-livre");
let menuAjoutUtilisateur = document.querySelector(".ajout-utilisateur");
let menuAjoutAuteur = document.querySelector(".ajout-auteur");
let menuAjoutAutre = document.querySelector(".ajout-autre");

let lastMenuActive = document.querySelector(".ajout-livre");

const toggleGererLivre = ()=> {
    if (!gererLivre.classList.contains("active")) {
        gererLivre.classList.toggle("active");
        lastBtnActive.classList.toggle("active");
        lastBtnActive = gererLivre;

        menuGererLivre.classList.toggle("active");
        lastMenuActive.classList.toggle("active");
        lastMenuActive = menuGererLivre;
    }
}

const toggleGererUtilisateur = ()=> {
    if (!gererUtilisateur.classList.contains("active")) {
        gererUtilisateur.classList.toggle("active");
        lastBtnActive.classList.toggle("active");
        lastBtnActive = gererUtilisateur;

        menuGererUtilisateur.classList.toggle("active");
        lastMenuActive.classList.toggle("active");
        lastMenuActive = menuGererUtilisateur;
    }
}

//---
const toggleAjouterLivre = ()=> {
    if (!ajouterLivre.classList.contains("active")) {
        ajouterLivre.classList.toggle("active");
        lastBtnActive.classList.toggle("active");
        lastBtnActive = ajouterLivre;

        menuAjoutLivre.classList.toggle("active");
        lastMenuActive.classList.toggle("active");
        lastMenuActive = menuAjoutLivre;
    }
}

const toggleAjouterUtilisateur = ()=> {
    if (!ajouterUtilisateur.classList.contains("active")) {
        ajouterUtilisateur.classList.toggle("active");
        lastBtnActive.classList.toggle("active");
        lastBtnActive = ajouterUtilisateur;

        menuAjoutUtilisateur.classList.toggle("active");
        lastMenuActive.classList.toggle("active");
        lastMenuActive = menuAjoutUtilisateur;
    }
}

const toggleAjouterAuteur = ()=> {
    if (!ajouterAuteur.classList.contains("active")) {
        ajouterAuteur.classList.toggle("active");
        lastBtnActive.classList.toggle("active");
        lastBtnActive = ajouterAuteur;

        menuAjoutAuteur.classList.toggle("active");
        lastMenuActive.classList.toggle("active");
        lastMenuActive = menuAjoutAuteur;
    }
}

const toggleAjouterAutre = ()=> {
    if (!ajouterAutre.classList.contains("active")) {
        ajouterAutre.classList.toggle("active");
        lastBtnActive.classList.toggle("active");
        lastBtnActive = ajouterAutre;

        menuAjoutAutre.classList.toggle("active");
        lastMenuActive.classList.toggle("active");
        lastMenuActive = menuAjoutAutre;
    }
}

