window.onload = function() {
		var num = 0;
        let elem = document.getElementsByClassName("slide1");
        var nb = elem.length;
        
        function changeSlide(n, sliderid) {
			if (n == -1) {
				num -= 1;
				if (num < 0) {
					num = nb - 1;
				}
			}
			if (n == 1) {
				num += 1;
				if (num == nb) {
					num = 0;
				}
			}
			let a = document.getElementById(sliderid);
			a.style.transform = "translateX(" + -100 * num + "%)";
			a.style.transitionDuration = "0.5s";
			return;		
		}
		
		document.getElementById("precedent").addEventListener("click",function(){changeSlide(-1,"slider");});
		document.getElementById("suivant").addEventListener("click",function(){changeSlide(1,"slider");});
		
		setInterval(function () {changeSlide(1, "slider");}, 5000);
		
		
		var num3 = 0;
        let elem3 = document.getElementsByClassName("slide3");
        var nb3 = elem3.length;  
        function changeSlide3(n, sliderid) {
			if (n == -1) {
				num3 -= 1;
				if (num3 < 0) {
					num3 = nb3 - 1;
				}
			}
			if (n == 1) {
				num3 += 1;
				if (num3 == nb3) {
					num3 = 0;
				}
			}
			let a = document.getElementById(sliderid);
			a.style.transform = "translateX(" + -100 * num3 + "%)";
			a.style.transitionDuration = "0.5s";
			return;		
		}
		document.getElementById("precedent3").addEventListener("click",function(){changeSlide3(-1,"slider3");});
		document.getElementById("suivant3").addEventListener("click",function(){changeSlide3(1,"slider3");});
		
		setInterval(function () {changeSlide3(1, "slider3");}, 5000);
		
		var num2 = 0;
        let elem2 = document.getElementsByClassName("slide2");
        var nb2 = elem2.length;
        function changeSlide2(n, sliderid) {
			if (n == -1) {
				num2 -= 1;
				if (num2 < 0) {
					num2 = nb2 - 1;
				}
			}
			if (n == 1) {
				num2 += 1;
				if (num2 == nb2) {
					num2 = 0;
				}
			}
			let a = document.getElementById(sliderid);
			a.style.transform = "translateX(" + -100 * num2 + "%)";
			a.style.transitionDuration = "0.5s";
			return;		
		}
		document.getElementById("precedent2").addEventListener("click",function(){changeSlide2(-1,"slider2");});
		document.getElementById("suivant2").addEventListener("click",function(){changeSlide2(1,"slider2");});
		
		setInterval(function () {changeSlide2(1, "slider2");}, 5000);
}

