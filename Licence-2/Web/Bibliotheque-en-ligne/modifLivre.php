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
							
									$sql = "UPDATE livre SET Titre = :Titre, Stock = :Stock, Description = :Description WHERE LivreID = :LivreID";
									$stmt = $connexion->prepare($sql);
									$stmt->bindParam(':Titre', $_POST['titre']);
									$stmt->bindParam(':Stock', $_POST['stock']);
									$stmt->bindParam(':Description', $_POST['desc']);
									$stmt->bindParam(':LivreID', $_POST['LivreID']);
									if ($stmt->execute()) {
									echo "	<div class=\"msq_sql_succes\">
												<p>Livre modifié avec succès !</p>
											</div>";
									} else {
									echo "	<div class=\"msq_sql_echec\">
												<p>Erreur lors de la modification du Livre !</p>
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

							$sql = "SELECT Titre, Stock, Description 
									FROM livre
									WHERE LivreID = '{$_POST['LivreID']}'";
							$result = $connexion->query($sql);
							$tab = $result->fetch();
							$LivreID = $_POST['LivreID'];
					echo "
						<div class='block'>
							<form class='container' method='POST' action='modifLivre.php'>
								<div class='flex'>
									<div class='input'>
										<p>Titre</p>
										<input type='text' name='titre' value='$tab[Titre]' required autocomplete='off'>
									</div>
									<div class='input'>
										<p>Stock</p>
										<input type='number' name='stock' min='-100' step='1' value='$tab[Stock]' oninput='validEntier(this)' required autocomplete='off'>
									</div>
								</div>
								<div class='flex_column'>
									<h2>Description</h2>
									<textarea class='input' name='desc' rows='6' cols='50' required>$tab[Description]</textarea>
								</div>
								<div id='fixed'>
									<input type='hidden' name='LivreID' value='$LivreID'>
									<input class='btn_valider' type='submit' name='valid_modif' value='valider'>
								</div>
							</form>
							<form method='POST' action='admin.php'>
								<input type='hidden' name='LivreID' value='$LivreID'>
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
