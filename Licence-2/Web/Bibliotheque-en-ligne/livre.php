<?php
	session_start();
?>
<!DOCTYPE html>
<html lang="en">


<head>
  <meta charset="utf-8" />
  <title>Livre | Bibliothèque St Etienne</title>
  <meta name="generator" content="Geany 1.38" />
  <link href="css/nav.css" rel="stylesheet">
  <link href="css/livre.css" rel="stylesheet">
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
					<li><a href="index.php">NOUVEAUTE</a></li>
					<li><a href="index.php">COUP DE COEUR</a></li>
					<li><a href="index.php">A VENIR</a></li>
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
			<div>
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
			</div>
	</div>
	
	
  <?php
	try {
		$dsn = "mysql:host=localhost;dbname=bibliotheque";
		$connexion = new PDO($dsn, "root", "");
		}
	catch(PDOException $e) {
		exit('Erreur : '.$e->getMessage());
	}
	if (isset($_POST['livre'])) {
		$requete_livre = "SELECT livre.Titre, livre.Parution,
					livre.Stock, livre.Description, auteur.Nom, auteur.Prenom,
					livre.Couverture, livre.EditeurID, livre.GenreID, livre.PublicID,
					livre.Emprunt, livre.DateSortieBiblio
					FROM livre, auteur
					WHERE livre.AuteurID = auteur.AuteurID
						AND livre.livreID = {$_POST['livre']};";

		$requete_categories = "SELECT CategorieID
							  FROM livre_categorie
							  where LivreID = {$_POST['livre']};";
		$results_livre = $connexion->query($requete_livre);
		$lign = $results_livre->fetch();
		$results_categories = $connexion->query($requete_categories);
		$tab = $results_categories->fetchAll();
  echo "
  <div id = \"desc\">
    <div id = \"desc2\">
      <div id = \"d1\">"; 
		  if ($lign) {
			echo" <img class = \" cover_livre\" src=\"assets/cover/{$lign['Couverture']}\" alt=\"couverture du livre\"/>";
	      }
  echo "
      </div>
      <div id = \"d2\">";
      echo "<div>
				<h1>{$lign['Titre']}<br>{$lign['Prenom']} {$lign['Nom']}</h1>
				<h2>{$lign['Parution']} {$lign['EditeurID']}</h2>
				<p>{$lign['GenreID']} {$lign['PublicID']}</p>
				<br>
			</div>
			<p id = \"synopsis\"> <strong>Synopsis : </strong><br>{$lign['Description']}</p>";
	echo "<div id = \"div_cate\">";
	if ($tab) {
			echo "
				<p id = \"title_cate\">Catégories</p>
				<div id = \"intern\">";
					foreach ($tab as $lignes) {
						echo "<p class = \"ligne_cate\">{$lignes['CategorieID']}</p>";
					}
		   echo"</div>";	
	}
	echo"</div>";
	
  echo "
      </div>
      <div id = \"d3\">";

      if ($lign) {	  
		  if ($lign['Stock'] <= $lign['Emprunt'] || strtotime($lign["DateSortieBiblio"]) >strtotime(date("Y-m-d"))) {
			   echo "<div class=\"dispo-emprunter\">
						<div class=\"emprunter-button\" id = \"grise\">
							<input type = \"hidden\" name = \"emprunt\" value = \"{$_POST['livre']}\"/>
							<input id = \"submit\" type = \"submit\" value = \"Emprunter\"/>
						</div>
						<div class=\"flex\">
							<div class=\"red-circle\">
							</div>
							<p>Exemplaire indisponible</p>
						</div>
					</div>";
		  } else {
			  echo "<div class=\"dispo-emprunter\">
						<form class=\"emprunter-button\" method = \"post\" action = \"ajout.php\">
							<input type = \"hidden\" name = \"emprunt\" value = \"{$_POST['livre']}\"/>
							<input id = \"submit\" type = \"submit\" value = \"Emprunter\"/>
						</form>
						<div class=\"flex\">
							<div class=\"green-circle\">
							</div>
							<p>Exemplaire(s) disponible</p>
						</div>
					</div>";
				   
		 }
	}
		
      
  echo "
      </div>
    </div>
  </div>";
}
  ?>

  <script src="js/profil-dropdown.js"></script>
  <script src="js/navMobile.js"></script>
</body>

</html>
