<?php
session_start();
?>
<!DOCTYPE html>
<html lang="en">

<head>
	<meta charset="utf-8" />
	<title>Connexion | Bibliothèque St Etienne</title>
	<meta name="generator" content="Geany 1.38" />
	<link href="css/nav.css" rel="stylesheet">
	<link href="css/connexion.css" rel="stylesheet">
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
			<div id="img-search" style="height: 200px; background-position: 0px -100px;">
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
		echo
		"<div id = \"contains\">
		<div id = \"divco\">
			<h1>Connexion</h1>
			<form id = \"co\" method = \"post\" action = \"connexion.php\">
				<input type = \"text\" name = \"num\" placeholder = \"numéro de carte\" autocomplete=\"off\">
				<input type = \"text\" name = \"mdp\" placeholder = \"mot de passe\" autocomplete=\"off\">
				<input id = \"submit\" type = \"submit\" value = \"se connecter\">
			</form>";
		
		if (!empty($_POST["num"]) && !empty($_POST["mdp"])) {
			try {
				$dsn = "mysql:host=localhost;dbname=bibliotheque";
				$connexion = new PDO($dsn, "root", "");
				}
			catch(PDOException $e) {
				exit('Erreur : '.$e->getMessage());
			}
			$id = $_POST["num"];
			$requete = "SELECT mdp, UtilisateurID, rang
						FROM utilisateur
						WHERE UtilisateurID = '$id';";
			$res = $connexion->query($requete);
			$lign = $res->fetch();
			if ($lign) {
				if (password_verify($_POST["mdp"], $lign["mdp"])) {
					$_SESSION["id"] = $id;
					$_SESSION["connected"] = True;
					if ($lign["rang"] == 1) {
						$_SESSION["admin"] = True;
					}
					if ($_SESSION["connected"]) {
						echo "<p id = \"connect\">Vous êtes connecté</p>";
					}
				} else {
					echo "<p id = \"connect\">Mot de passe incorrect</p>";
				}
			} else {
				echo "<p id = \"connect\">Ce numéro n'existe pas</p>";
			}
		}
		echo "</div></div>";
	?>

  <script src="js/profil-dropdown.js"></script>
  <script src="js/navMobile.js"></script>
</body>

</html>
