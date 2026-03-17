<?php
    session_start();
    if (isset($_POST['suppr'])) {
        try {
            $dsn = "mysql:host=localhost;dbname=bibliotheque";
            $connexion = new PDO($dsn, "root", "");
            }
        catch(PDOException $e) {
            exit('Erreur : '.$e->getMessage());
        }

        $EmpruntID = $_POST['EmpruntID'];
        $req  = "SELECT LivreID
				FROM emprunt
				WHERE EmpruntID = '{$EmpruntID}';";
		$res = $connexion->query($req);
		$tab = $res->fetch();
		$req2 = "UPDATE livre
				SET Emprunt = Emprunt - 1
				WHERE LivreID = {$tab['LivreID']};";
		$connexion->exec($req2);
        $sql = "DELETE FROM emprunt WHERE EmpruntID = '{$EmpruntID}'";
        $retour = $connexion->exec($sql);
    } else if (isset($_POST['prolong'])) {
        try {
            $dsn = "mysql:host=localhost;dbname=bibliotheque";
            $connexion = new PDO($dsn, "root", "");
            }
        catch(PDOException $e) {
            exit('Erreur : '.$e->getMessage());
        }

        $EmpruntID = $_POST['EmpruntID'];
    
        // Sélection de la date de fin de l'emprunt
        $sql = "SELECT Fin FROM emprunt WHERE EmpruntID = :EmpruntID";
        $stmt = $connexion->prepare($sql);
        $stmt->bindParam(':EmpruntID', $EmpruntID, PDO::PARAM_INT);
        $stmt->execute();
        $fin_date = $stmt->fetchColumn();
        
        // Ajout de 14 jours à la date de fin
        $fin_date = new DateTime($fin_date);
        $fin_date->modify('+14 days');
        
        // Formatage de la nouvelle date
        $string_date = $fin_date->format("Y-m-d");
        
        // Mise à jour de la date de fin de l'emprunt dans la base de données
        $sql = "UPDATE Emprunt SET Fin = :nouvelle_date WHERE EmpruntID = :EmpruntID";
        $stmt = $connexion->prepare($sql);
        $stmt->bindParam(':nouvelle_date', $string_date, PDO::PARAM_STR);
        $stmt->bindParam(':EmpruntID', $EmpruntID, PDO::PARAM_INT);
        $stmt->execute();
    }
    


?>
<!DOCTYPE html>
<html lang="en">

    <head>
        <title>Bibliothèque St Etienne</title>
        <meta http-equiv="content-type" content="text/html;charset=utf-8" />
        <meta name="generator" content="Geany 1.38" />
        <link href="css/nav.css" rel="stylesheet">
        <link href="css/emprunt.css" rel="stylesheet">
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
                <h1 class="titre">Mes emprunts</h1>
            </div>
            <?php
                if (!$_SESSION["connected"]) {
                    echo "
                    <div class=\"block\">
                        <p>Connectez-vous pour avoir accès à vos emprunt(s)</p>
                    </div>";
                } else {
                    try {
                        $dsn = "mysql:host=localhost;dbname=bibliotheque";
                        $connexion = new PDO($dsn, "root", "");
                        }
                    catch(PDOException $e) {
                        exit('Erreur : '.$e->getMessage());
                    }     
                        echo "
                        <div class=\"block\">";
                            $id = $_SESSION["id"];
                            $sql = "SELECT DISTINCT EmpruntID, emprunt.LivreID, Couverture, Titre, EditeurID, Nom, Prenom, Debut, Fin
                                    FROM livre, auteur, emprunt
                                    WHERE livre.AuteurID = auteur.AuteurID
                                        AND livre.LivreID = emprunt.LivreID";
                            $result = $connexion->query($sql);
                            $tab = $result->fetchall();

                            if (empty($tab)) {
                                echo "<p>Vous n'avez pas d'emprunt(s)</p>
                                </div>";
                            } else {

                            $today = new DateTime(date("Y-m-d"));
                            
                            
                            foreach ($tab as $ligne) {
                                $fin_date = new DateTime($ligne['Fin']);
                                $interval = $today->diff($fin_date);
                                echo "
                                <form method=\"post\" action=\"emprunt.php\" class=\"container\">";
                                    if ($fin_date->format('Y') <= $today->format('Y') && $fin_date->format('m') <= $today->format('m') && $fin_date->format('d') <= $today->format('d')) {
                                        echo "<div class='block_livre depasse'>";
                                    } else if ($interval->y == 0 && $interval->m == 0 && $interval->d == 1) {
                                        echo "<div class='block_livre limite'>";
                                    } else {
                                        echo "<div class='block_livre'>";
                                    }

                                    echo "<div class=\"flex_row\">
                                            <img src=\"assets/cover/$ligne[Couverture]\" alt=\"cover\" class=\"element cover\">
                                            <div class=\"flex_column element\">
                                                <p><strong>$ligne[Titre]</strong></p>
                                                <p>$ligne[Prenom] $ligne[Nom] / $ligne[EditeurID]</p>
                                                <p>Emprunté le: $ligne[Debut]</p>
                                                <p>A rendre pour: $ligne[Fin]</p>
                                            </div>
                                        </div>
                                        <div class=\"input_container\">
                                            <input type =\"hidden\" name='EmpruntID' value = \"$ligne[EmpruntID]\">
                                            <input type=\"submit\" name=\"suppr\" value=\"rendre\" class=\"btn_suppr element\">
                                            <input type=\"submit\" name=\"prolong\" value=\"prolonger\" class=\"btn_prolong element\">
                                        </div>
                                    </div>
                                </form>";
                            }
                        }
                  echo "</div>";
                    }
                
            ?>
        </section>
        <script src="js/profil-dropdown.js"></script>
        <script src="js/navMobile.js"></script>
    </body>

</html>
