let profilDropDownList = document.querySelector(".profil-dropdown-list");
let btn = document.querySelector("#btn_compte");

const toggle = ()=> {
    if (btn !== null) {
        profilDropDownList.classList.toggle("active")
    }
};

window.addEventListener("click", function (e) {
    if (btn !== null) {
        if (!btn.contains(e.target)) profilDropDownList.classList.remove("active");
    }
    
});