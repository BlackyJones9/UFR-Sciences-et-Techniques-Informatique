let publicDropDownList = document.querySelector("#filter-public");
let auteurDropDownList = document.querySelector("#filter-auteur");
let editeurDropDownList = document.querySelector("#filter-editeur");
let dateDropDownList = document.querySelector("#filter-date");
let categorieDropDownList = document.querySelector("#filter-categorie");
let typeDropDownList = document.querySelector("#filter-type");

let btnPublic = document.querySelector("#btn-public");
let btnAuteur = document.querySelector("#btn-auteur");
let btnEditeur = document.querySelector("#btn-editeur");
let btnDate = document.querySelector("#btn-date");
let btnCategorie = document.querySelector("#btn-categorie");
let btnType = document.querySelector("#btn-type");

const togglePublic = ()=> {
    publicDropDownList.classList.toggle("active");
    btnPublic.classList.toggle("active");
}
const toggleAuteur = ()=> {
    auteurDropDownList.classList.toggle("active");
    btnAuteur.classList.toggle("active");
}
const toggleEditeur = ()=> {
    editeurDropDownList.classList.toggle("active");
    btnEditeur.classList.toggle("active");
}
const toggleDate = ()=> {
    dateDropDownList.classList.toggle("active");
    btnDate.classList.toggle("active");
}
const toggleCategorie = ()=> {
    categorieDropDownList.classList.toggle("active");
    btnCategorie.classList.toggle("active");
}
const toggleType = ()=> {
    typeDropDownList.classList.toggle("active");
    btnType.classList.toggle("active");
}