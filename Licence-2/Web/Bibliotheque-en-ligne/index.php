<?php
session_start();
if (!isset($_SESSION["connected"])) {
	$_SESSION["connected"] = False;
}
if (!isset($_SESSION["id"])) {
	$_SESSION["id"] = NULL;
}
if (!isset($_SESSION["admin"])) {
	$_SESSION["admin"] = False;
}
if (!isset($_SESSION["panier"])) {
	$_SESSION["panier"] = [];
}
$_SESSION["recherche_mot"] = null;
?>
<!DOCTYPE html>
<html lang="en">


<head>
	<meta charset="utf-8" />
	<title>Bibliothèque St Etienne</title>
	<meta name="generator" content="Geany 1.38" />
	<link href="css/nav.css" rel="stylesheet">
	<link rel = "stylesheet" href = "css/index.css"/>
	<script src = "js/slider.js"></script>
	<script src="https://kit.fontawesome.com/b01cc878ee.js" crossorigin="anonymous"></script>

</head>

<body>
	<div id = "haut">
		<nav>
			<div class="navflex">
			<p>St Etienne | BIBLIOTHEQUE</p>
			</div>
			<div class="navflex nav_mobile">
				<i class="fa-solid fa-xmark fa-2xl" id='mobile_btnX' onclick="toggleMenu()"></i>
				<h2 class='mobile_title_nav' id='first_mobile_title'>NAVIGATION</h2>
				<ul class='nav_liste'>
					<li><a href="#news">NOUVEAUTE</a></li>
					<li><a href="#coeur">COUP DE COEUR</a></li>
					<li><a href="#bientot">A VENIR</a></li>
					<li><a href="recherche.php">CATALOGUE</a></li>
				</ul>
				<h2 class='mobile_title_nav'>MON COMPTE</h2>
				<ul id="mobile_profil_list">
					<li>
						<i class="fa-solid fa-book"></i><a class = "emprunt_button" href='emprunt.php'>EMPRUNT(S)</a></li>
					<li>
						<i class="fa-solid fa-basket-shopping"></i><a class = "panier_button" href='panier.php'>PANIER</a></li>
						<?php
							if ($_SESSION["admin"]) {
								echo "
								<li>
									<i class='fa-solid fa-user'></i><a class = \"admin_button\" href = 'admin.php'>ADMIN</a></li>";
								}
								echo "<li>";
									if ($_SESSION['connected']) {
										echo "<i class='fa-solid fa-right-from-bracket'></i><a class ='connect_button' href ='deconnexion.php'>DECONNEXION</a></li>";
									} else {
										echo "<i class='fa-solid fa-right-from-bracket'></i><a class ='connect_button' href ='connexion.php'>CONNEXION</a></li>";
									}
								
						?>
				</ul>
			</div>
		
			<div class="navflex">
				<?php
					$nb_emprunt = count($_SESSION['panier']);
					if ($nb_emprunt > 9) {
					  echo "<a class = \"panier_button\" href='panier.php'><img src='assets/panier/plus.png' alt='bouton panier'></a>";
					} else {
					  echo "<a class = \"panier_button\" href='panier.php'><img src='assets/panier/$nb_emprunt.png' alt='bouton panier'></a>";
					}
				  ?>
				<div id="btn_compte" class="navflex" onclick="toggle()">
					<p>Mon Compte</p>
				</div>
			</div>
			<i id='burger' class="fa-solid fa-bars fa-2xl" onclick="toggleMenu()"></i>
		</nav>
		<div class="mobile-overlay" onclick="toggleMenu()"></div>
			<div id = "img-search">
				<ul class="profil-dropdown-list">
					<li>
						<i class="fa-solid fa-book"></i><a class = "emprunt_button" href='emprunt.php'>Emprunt(s)</a></li>
					<li>
						<i class="fa-solid fa-basket-shopping"></i><a class = "panier_button" href='panier.php'>Panier</a></li>
					<li>
						<?php
							if ($_SESSION["admin"]) {
								echo "
								<li>
									<i class='fa-solid fa-user'></i><a href = 'admin.php' class = \"admin_button\"><p>Admin</p></a></li>";
								}
								echo "<li>";
									if ($_SESSION['connected']) {
										echo "<i class='fa-solid fa-right-from-bracket'></i><a class ='connect_button' href ='deconnexion.php'><p>Deconnexion</p></a></li>";
									} else {
										echo "<i class='fa-solid fa-right-from-bracket'></i><a class ='connect_button' href ='connexion.php'><p>Connexion</p></a></li>";
									}
								
						?>
				</ul>
				<div id = "trues">
					<div id = "search">
						<form method = "post" action = "recherche.php">
							<input type = "text" name = "recherche" placeholder = "Tapez votre recherche" autocomplete="off"/>
							<button id = "sub" title = "envoyer" name = "search" type = "submit" ></button>
						</form>
					</div>
				</div>
			</div>
	</div>
        
    <a id = "retour-haut" href = "#haut"></a>
    <?php 
		try {
			$dsn = "mysql:host=localhost;dbname=bibliotheque";
			$connexion = new PDO($dsn, "root", "");
			}
		catch(PDOException $e) {
			exit('Erreur : '.$e->getMessage());
		}
		/*slider nouveautés*/
		$requete_news = "SELECT livre.LivreID, livre.Titre, livre.Parution,
					livre.Stock, livre.Description, auteur.Nom, auteur.Prenom,
					livre.Couverture
					FROM livre, auteur
					WHERE livre.AuteurID = auteur.AuteurID
						AND livre.DateSortieBiblio <= DATE(NOW())
					ORDER BY livre.DateSortieBiblio DESC
					LIMIT 8;";
		$results_news = $connexion->query($requete_news);
		$tab = $results_news->fetchAll();
		echo "<div id = \"home\">";
		if ($tab) {
			echo " <div id = \"news\" class = \"pres\" >
							<h1>Nouveauté</h1>
							<div class = \"slider-contain slider-1\">
								<div id = \"precedent\" >&lt;</div>
								<div id = \"suivant\" >&gt;</div>
								<div id = \"slider\">";
			
							foreach ($tab as $lignes) {
								echo "<form class=\"slide1 block book\" method = \"post\" action =\"livre.php\">
									<div class=\"book-content\">
										<div class=\"book-cover\">
											<img class = \" cover_livre\" src=\"assets/cover/{$lignes['Couverture']}\" alt=\"couverture du livre\">
										</div>
										<div class=\"book-summary\">
											<div class=\"book-title\">
												<h1>"; echo $lignes['Titre']; echo "</h1>";
												echo "<h2>"; echo $lignes['Prenom']; echo " "; echo $lignes['Nom']; echo " "; echo $lignes['Parution']; echo "</h2>";
											echo "</div>
											<div class = \"decript\">
											  <p>"; echo $lignes["Description"]; echo "</p>
											</div>
										</div>
										<div class = \"button_end\">
											<input type = \"hidden\" name = \"livre\" value = \"{$lignes['LivreID']}\"/>
											<input class = \"livre_submit\" type = \"submit\" value = \"détails\"/>
										</div>
										</div>
								</form>";
							}
							echo "</div>
							</div>
					</div>";
	}
	/*slider des coups de coeur (popularité)*/
		$requete_coeur = "SELECT livre.LivreID, livre.Titre, livre.Parution,
					livre.Stock, livre.Description, auteur.Nom, auteur.Prenom,
					livre.Couverture
					FROM livre, auteur
					WHERE livre.AuteurID = auteur.AuteurID
					ORDER BY HistEmprunt
					LIMIT 5;";
		$results_coeur = $connexion->query($requete_coeur);
		$tab2 = $results_coeur->fetchAll();
		if ($tab2) {
			echo "<div id = \"coeur\" class = \"pres\">
					<h1>Coup de Coeur</h1>
					<div class = \"slider-contain slider-1\">
						<div id = \"precedent2\" >&lt;</div>
						<div id = \"suivant2\" >&gt;</div>
						<div id = \"slider2\">";
					
					foreach ($tab2 as $lignes) {
						echo "<form class=\"slide2 block book\" method = \"post\" action =\"livre.php\">
							<div class=\"book-content\">
								<div class=\"book-cover\">
									<img class = \" cover_livre\" src=\"assets/cover/{$lignes['Couverture']}\" alt=\"couverture du livre\">
								</div>
								<div class=\"book-summary\">
									<div class=\"book-title\">
										<h1>"; echo $lignes['Titre']; echo "</h1>";
										echo "<h2>"; echo $lignes['Prenom']; echo " "; echo $lignes['Nom']; echo " "; echo $lignes['Parution']; echo "</h2>";
									echo "</div>
									<div class = \"decript\">
									  <p>"; echo $lignes["Description"]; echo "</p>
									</div>
								</div>
								<div class = \"button_end\">
											<input type = \"hidden\" name = \"livre\" value = \"{$lignes['LivreID']}\"/>
											<input class = \"livre_submit\" type = \"submit\" value = \"détails\"/>
								</div>
								</div>
						</form>";
					}
					echo "</div>
					</div>
		</div>";
		}
		
		/*slider sorties à venir*/
		$requete_venir = "SELECT livre.LivreID, livre.Titre, livre.Parution,
					livre.Stock, livre.Description, auteur.Nom, auteur.Prenom,
					livre.Couverture
					FROM livre, auteur
					WHERE livre.AuteurID = auteur.AuteurID
						AND livre.DateSortieBiblio > DATE(NOW())
					ORDER BY livre.DateSortieBiblio DESC
					LIMIT 8;";
		$results_venir = $connexion->query($requete_venir);
		$tab3 = $results_venir->fetchAll();
		if ($tab3) {
			echo "<div id = \"bientot\" class = \"pres\">
				<h1>A venir</h1>
				   <div class = \"slider-contain slider-1\">
						<div id = \"precedent3\" >&lt;</div>
						<div id = \"suivant3\" >&gt;</div>
						<div id = \"slider3\">";
					
					foreach ($tab3 as $lignes) {
						echo "<form class=\"slide3 block book\" method = \"post\" action =\"livre.php\">
							<div class=\"book-content\">
								<div class=\"book-cover\">
									<img class = \" cover_livre\" src=\"assets/cover/{$lignes['Couverture']}\" alt=\"couverture du livre\">
								</div>
								<div class=\"book-summary\">
									<div class=\"book-title\">
										<h1>"; echo $lignes['Titre']; echo "</h1>";
										echo "<h2>"; echo $lignes['Prenom']; echo " "; echo $lignes['Nom']; echo " "; echo $lignes['Parution']; echo "</h2>";
									echo "</div>
									<div class = \"decript\">
									  <p>"; echo $lignes["Description"]; echo "</p>
									</div>
								</div>
								<div class = \"button_end\">
											<input type = \"hidden\" name = \"livre\" value = \"{$lignes['LivreID']}\"/>
											<input class = \"livre_submit\" type = \"submit\" value = \"détails\"/>
								</div>
								</div>
						</form>";
					}
					echo "</div>
					</div>
			</div>";
		}
		echo "</div>";
    
    ?>

	

			

	 <script src="js/profil-dropdown.js"></script>
	 <script src="js/navMobile.js"></script>
</body>

</html>
