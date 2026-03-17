<?php
    session_start();
	/*supprimer un livre du panier*/
	if (isset($_POST["sup"])) {
		unset($_SESSION["panier"][array_search($_POST["sup"], $_SESSION["panier"])]);
		sort($_SESSION["panier"]);
	}
?>

<!DOCTYPE html>
<html lang="en">


    <head>
        <meta charset="utf-8" />
        <title>Bibliothèque St Etienne</title>
        <meta name="generator" content="Geany 1.38" />
        <link href="css/nav.css" rel="stylesheet">
        <link rel = "stylesheet" href = "css/panier.css"/>
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

	<section class="container">
		<div class="block">
			<h1 class="titre">Mon panier</h1>
		</div>
	<?php
	/*test si l'utilisateur est connecté ou non*/
	if (!$_SESSION["connected"]) {
		echo "
		<div class=\"block\">
			<p>Connectez-vous pour avoir accès au panier</p>
		</div>";
	} else {
		try {
			$dsn = "mysql:host=localhost;dbname=bibliotheque";
			$connexion = new PDO($dsn, "root", "");
			}
		catch(PDOException $e) {
			exit('Erreur : '.$e->getMessage());
		}
		$id = $_SESSION["id"];
		
		$requete = "SELECT MaxEmprunt, MaxDelai
					FROM utilisateur, rang
					WHERE UtilisateurID = '$id'
						AND utilisateur.rang = rang.rangID;";
		$res = $connexion->query($requete);
		$lign = $res->fetch();
		$requete2 = "SELECT COUNT(EmpruntID)
					FROM emprunt
					WHERE UtilisateurID = '$id';";
		$res2 = $connexion->query($requete2);
		$lign2 = $res2->fetch();
		/*valider le panier*/
		if (isset($_POST["valid"])) {
			if ($lign["MaxEmprunt"] < $lign2["COUNT(EmpruntID)"] + count($_SESSION["panier"])) {
				echo "<div class=\"block\">
						<p>Limite d'emprunt dépassée</p>
					</div>";
			} else {
				$today = new DateTime(date("Y-m-d"));
				$nbJours =  $lign["MaxDelai"] *7;
				$fin = new DateTime(date("Y-m-d"));
				date_modify($fin, "+ $nbJours days");
				for ($k = 0; $k < count($_SESSION["panier"]); ++$k) {
					$td = $today->format('Y-m-d');
					$fn = $fin->format('Y-m-d');
					$requete_ajout = "INSERT INTO emprunt (LivreID, UtilisateurID, Debut, Fin)
										VALUES({$_SESSION["panier"][$k]}, $id, '{$td}', '{$fn}');";
					$connexion ->exec($requete_ajout);			
					$requete_ajout_livre = "UPDATE livre
											SET	Emprunt = Emprunt + 1,
												HistEmprunt = HistEmprunt + 1
											WHERE LivreID = {$_SESSION["panier"][$k]};";
					$connexion->exec($requete_ajout_livre);
												
				}
				$_SESSION["panier"] = [];
			}
			
		}
		/* test si panier vide ou non*/
		if ($_SESSION["panier"] == []) {
			echo "
				<div class=\"block\">
					<p>Votre panier est vide</p>
				</div>";
		} else {
			/*affiche le contenu du panier*/
			echo "
            <div id=\"panierblock\" class=\"block\">";
                foreach ($_SESSION["panier"] as $livre) {
					$requete3 = "SELECT Couverture, Titre, EditeurID, Nom, Prenom
								FROM livre, auteur
								WHERE livre.AuteurID = auteur.AuteurID
									AND LivreID = '$livre';";
					$res3 = $connexion->query($requete3);
					$lign3 = $res3->fetch();
					echo "
					<form method=\"post\" action=\"panier.php\" class=\"container\">
						<div class=\"block_livre\">
							<div class=\"flex_row\">
								<img src=\"assets/cover/{$lign3["Couverture"]}\" alt=\"cover\" class=\"element cover\">
								<div class=\"flex_column element\">
									<p><strong>{$lign3["Titre"]}</strong></p>
									<p>{$lign3["Prenom"]} {$lign3["Nom"]} / {$lign3["EditeurID"]}</p>
								</div>
							</div>
							<div class=\"input_container\">
								<input type =\"hidden\" name = \"sup\" value = \"$livre\">
								<input type=\"submit\" name=\"id_book\" value=\"supprimer\" class=\"btn_suppr element\">
							</div>
						</div>
                    </form>";
                }
				echo "
                <form method=\"post\" action=\"panier.php\" style=\"display: flex; justify-content: flex-end;\">
					<input type =\"hidden\" name = \"valid\" value = \"\">
                    <input type=\"submit\"  value=\"valider\" class=\"btn_valider\">
                </form>
            </div>";
        }
     }
            ?>
        </section>
        <script src="js/profil-dropdown.js"></script>
        <script src="js/navMobile.js"></script>
    </body>
</html>
