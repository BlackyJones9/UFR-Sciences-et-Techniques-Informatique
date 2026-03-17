<?php
session_start();
if (!$_SESSION["admin"]) {
	header('Location: index.php');
	exit;
}
?>
<!DOCTYPE html>
<html lang='en'>

<head>
	<title>sans titre</title>
	<meta http-equiv='content-type' content='text/html;charset=utf-8' />
	<meta name='generator' content='Geany 1.38' />
	<link href='css/nav.css' rel='stylesheet'>
	<link href='css/modifLivreUtilisateur.css' rel='stylesheet'>
	<script src='https://kit.fontawesome.com/b01cc878ee.js' crossorigin='anonymous'></script>
</head>

	<body>
	<div id = 'haut'>
            <nav>
                <div class='navflex'>
                <p>St Etienne | BIBLIOTHEQUE</p>
                </div>
                <div class='navflex nav_mobile'>
                    <i class='fa-solid fa-xmark fa-2xl' id='mobile_btnX' onclick='toggleMenu()'></i>
                    <h2 class='mobile_title_nav' id='first_mobile_title'>NAVIGATION</h2>
                    <ul class='nav_liste'>
                        <li><a href='index.php'>NOUVEAUTE</a></li>
                        <li><a href='index.php'>COUP DE COEUR</a></li>
                        <li><a href='index.php'>A VENIR</a></li>
                        <li><a href='recherche.php'>CATALOGUE</a></li>
                    </ul>
                    <h2 class='mobile_title_nav'>MON COMPTE</h2>
                    <ul id='mobile_profil_list'>
                        <li>
                            <i class='fa-solid fa-book'></i><a href='#'>EMPRUNT(S)</a></li>
                        <li>
                            <i class='fa-solid fa-basket-shopping'></i><a href='#'>PANIER</a></li>
                            <?php
                                if ($_SESSION['admin']) {
                                    echo "
                                    <li>
                                        <i class='fa-solid fa-user'></i><a href = 'admin.php'>ADMIN</a></li>";
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
            
                <div class='navflex'>
                    <img src='assets/panier/0.png' alt='bouton panier' id='panier'>
                    <div id='btn_compte' class='navflex' onclick='toggle()'>
                        <p>Mon Compte</p>
                    </div>
                </div>
                <i id='burger' class='fa-solid fa-bars fa-2xl' onclick='toggleMenu()'></i>
            </nav>
            <div class='mobile-overlay' onclick='toggleMenu()'></div>
                <div>
                    <ul class='profil-dropdown-list'>
                        <li>
                            <i class='fa-solid fa-book'></i><p>Emprunt(s)</p></li>
                        <li>
                            <i class='fa-solid fa-basket-shopping'></i><p>Panier</p></li>
                        <li>
                            <?php
                                if ($_SESSION['admin']) {
                                    echo "
                                    <li>
                                        <i class='fa-solid fa-user'></i><a href = 'admin.php'><p>Admin</p></a></li>";
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
		<div id='container'>
			<div id="titre-page">
				<p>ADMINISTRATION</p>
			</div>
			<section class='flex'>							
				<div class='flex_column'>
					<div class="block">
						<h1 class="titre">Modifier un livre</h1>
					</div>
			
					<?php
						if (isset($_POST['valid_modif'])) {
							try {
								$dsn = "mysql:host=localhost;dbname=bibliotheque";
								$connexion = new PDO($dsn, "root", "");
								}
							catch(PDOException $e) {
								exit('Erreur : '.$e->getMessage());
							}
					
                            $sql ="";
                            $hasMdp = false;
                            if (empty($_POST['mdp'])) {
                                $sql = "UPDATE utilisateur SET Nom = :Nom, Prenom = :Prenom, Naissance = :Naissance, mail = :mail, rang = :rang 
                                WHERE UtilisateurID = :UtilisateurID";
                            } else {
                                $sql = "UPDATE utilisateur SET Nom = :Nom, Prenom = :Prenom, Naissance = :Naissance, mail = :mail, rang = :rang, mdp = :mdp
                                WHERE UtilisateurID = :UtilisateurID";
                                $noMdp = true;
                            }

                            $stmt = $connexion->prepare($sql);
							$stmt->bindParam(':Nom', $_POST['nom']);
							$stmt->bindParam(':Prenom', $_POST['prenom']);
							$stmt->bindParam(':Naissance', $_POST['date']);
                            $stmt->bindParam(':mail', $_POST['mail']);
							$stmt->bindParam(':rang', $_POST['rang']);

                            if($hasMdp) {
                                $stmt->bindParam(':mdp', $_POST['mdp']);
                            }
							$stmt->bindParam(':UtilisateurID', $_POST['UtilisateurID']);
							if ($stmt->execute()) {
							echo "	<div class=\"msq_sql_succes\">
										<p>Utilisateur modifié avec succès !</p>
									</div>";
							} else {
							echo "	<div class=\"msq_sql_echec\">
										<p>Erreur lors de la modification de l'Utilisateur !</p>
									</div>";
							}
						}

					
					try {
						$dsn = "mysql:host=localhost;dbname=bibliotheque";
						$connexion = new PDO($dsn, "root", "");
						}
					catch(PDOException $e) {
						exit('Erreur : '.$e->getMessage());
					}

					$sql = "SELECT Nom, Prenom, Naissance, mail, rang
							FROM utilisateur
							WHERE UtilisateurID = '{$_POST['UtilisateurID']}'";
					$result = $connexion->query($sql);
					$tab = $result->fetch();
					$UtilisateurID = $_POST['UtilisateurID'];
					echo "
						<div class='block'>
							<form class='container' method='POST' action='modifUtilisateur.php'>

                                <div class='flex'>
                                    <div class='input'>
                                        <p>Nom</p>
                                        <input type='text' name='nom' value='$tab[Nom]' required autocomplete='off'>
                                    </div>
                                    <div class='input'>
                                        <p>Prenom</p>
                                        <input type='text' name='prenom' value='$tab[Prenom]' required autocomplete='off'>
                                    </div>
                                </div>

                                <div class='flex'>
									<div class='input'>
										<p>Naissance</p>
										<input type='date' name='date' value='$tab[Naissance]' required>
									</div>
									<div class='input'>
										<p>Mail</p>
										<input type='email' name='mail' value='$tab[mail]' required autocomplete='off'>
									</div>
								</div>

                                <div class='flex'>
									<div class='input'>
										<p>Mot de passe</p>
										<input type='text' name='mdp' placeholder='mot de passe' autocomplete='off'>
									</div>
									<div class='input'>
										<p>Permission</p>
										<input type='number' name='rang' min='-100' step='1' value='$tab[rang]' oninput='validEntier(this)' required>
									</div>
								</div>
                                    

								<div id='fixed'>
									<input type='hidden' name='UtilisateurID' value='$UtilisateurID'>
									<input class='btn_valider' type='submit' name='valid_modif' value='valider'>
								</div>
							</form>
							<form method='POST' action='admin.php'>
								<input type='hidden' name='UtilisateurID' value='$UtilisateurID'>
								<input class='btn_retour' type='submit' name='valid_modif' value='retour'>
							</form>
						</div>";
					?>
			</div>
		</section>
        </div>
		<script src="js/profil-dropdown.js"></script>
        <script src="js/navMobile.js"></script>
		<script src="js/adminList-dropdown.js"></script>
		<script src="js/input_option.js"></script>
	</body>

</html>
