<?php
    session_start();
    $_SESSION["recherche_mot"] = [];
    if (!$_SESSION["admin"]) {
		header('Location: index.php');
		exit;
	}
?>

<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="utf-8" />
        <title>Bibliothèque St Etienne</title>
        <meta name="generator" content="Geany 1.38" />
        <link href="css/nav.css" rel="stylesheet">
        <link href="css/admin.css" rel="stylesheet">
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


        <div id="titre-page">
            <p>ADMINISTRATION</p>
        </div>

        <div id="container">
            <div id="liste">
                <div id="liste-container">
                    

                    <div class="liste-title" onclick="toggleAjouter()">
                        <h2>AJOUTER</h2>
                        <img class="btn-arrow active" id="btn-ajouter" src="assets/fleche.png" alt="déroule, enroule le filtre">
                    </div>
                    <div class="liste active" id="liste-ajouter">
                        <div class="liste-container-checkbox">
                            <p id="ajouterLivre" class="active" onclick="toggleAjouterLivre()">Livres</p>
                            <p id="ajouterUtilisateur" onclick="toggleAjouterUtilisateur()">Utilisateurs</p>
                            <p id="ajouterAuteur" onclick="toggleAjouterAuteur()">Auteurs</p>
                            <p id="ajouterAutre" onclick="toggleAjouterAutre()">Autre</p>
                        </div>
                    </div>

                    <div class="liste-title" onclick="toggleGerer()">
                        <h2>GERER</h2>
                        <img class="btn-arrow active" id="btn-gerer" src="assets/fleche.png" alt="déroule, enroule le filtre">
                    </div>
                    <div class="liste active" id="liste-gerer">
                        <div class="liste-container-checkbox">
                            <p id="gererLivre" onclick="toggleGererLivre()">Livres</p>
                            <p id="gererUtilisateur" onclick="toggleGererUtilisateur()">Utilisateurs</p>
                        </div>
                    </div>

                    
                </div> 
            </div>
            
                <!-- AJOUT LIVRE !-->
                <section class="ajout-livre active">
                    <div class="titre-element">
                        <p>AJOUT - LIVRE</p>
                    </div>
                        <?php
                            if (isset($_POST["ajout-livre"])) {
                                try {
                                    $dsn = "mysql:host=localhost;dbname=bibliotheque";
                                    $connexion = new PDO($dsn, "root", "");
                                }
                                catch(PDOException $e) {
                                    exit('Erreur : '.$e->getMessage());
                                }
                                
                                $sql_get = "SELECT LivreID FROM livre ORDER BY LivreID DESC LIMIT 1";
                                $result = $connexion->query($sql_get);
                                $tab = $result->fetchAll();
                                foreach ($tab as $ligne) {
                                    $livreID = $ligne['LivreID'];
                                }

                                $livreID += 1;

                                $file = $_FILES['cover'];
                                $upload_directory = 'assets/cover/';
                                
                                // Chemin complet du fichier
                                $target_file = $upload_directory . basename($file['name']);
                                if(!empty($_POST['categorie'])) {
                                    if(move_uploaded_file($file['tmp_name'], $target_file)) {
                                        // Utilisation de la variable correcte pour le nom du fichier dans la requête SQL
                                        $logo_name = $_FILES['cover']['name'];
                                    
                                        // Utilisation de requête préparée pour éviter les injections SQL
                                        $sql = "INSERT INTO livre (LivreID, PublicID, Titre, AuteurID, EditeurID, Parution, GenreID, Stock, DateSortieBiblio, Description, Couverture)
                                                VALUES (:livreID, :PublicID, :Titre, :AuteurID, :EditeurID, :Parution, :GenreID, :Stock, :DateSortieBiblio, :Description, :Couverture)";

                                        $stmt = $connexion->prepare($sql);
                                        $stmt->bindParam(':livreID', $livreID);
                                        $stmt->bindParam(':PublicID', $_POST['public']);
                                        $stmt->bindParam(':Titre', $_POST["titre"]);
                                        $stmt->bindParam(':AuteurID', $_POST["auteur"]);
                                        $stmt->bindParam(':EditeurID', $_POST["editeur"]);
                                        $stmt->bindParam(':Parution', $_POST['parution']);
                                        $stmt->bindParam(':GenreID', $_POST['genre']);
                                        $stmt->bindParam(':Stock', $_POST['stock']);
                                        $stmt->bindParam(':DateSortieBiblio', $_POST['sortie_biblio']);
                                        $stmt->bindParam(':Description', $_POST['description']);
                                        $stmt->bindParam(':Couverture', $logo_name);
                                        $retour = $stmt->execute();

                                        if ($retour) {
                                            echo "<div class=\"msq_sql_succes\">
                                                    <p>Livre ajouté avec succès !</p>
                                                </div>";
                                        } else {
                                            echo "<div class=\"msq_sql_echec\">
                                                    <p>Erreur lors de l'ajout du livre !</p>
                                                </div>";
                                        }
                                        
                                        foreach ($_POST['categorie'] as $categorie) {
                                            $sql = "INSERT INTO livre_categorie (CategorieID, LivreID)
                                                    VALUES (:CategorieID, :LivreID)";
                                            $stmt = $connexion->prepare($sql);
                                            $stmt->bindParam(':CategorieID', $categorie);
                                            $stmt->bindParam(':LivreID', $livreID);
                                            $retour = $stmt->execute();
                                            
                                            
                                            if (!$retour) {
                                                echo "<div class=\"msq_sql_echec\">
                                                        <p>Erreur lors de l'ajout de de la catégorie: $categorie</p>
                                                    </div>";
                                            } 
                                        }
                                    } else { echo "<div class=\"msq_sql_echec\">
                                                <p>Erreur lors du téléchargement de l'image</p>
                                            </div>";
                                    }
                                } else {
                                    echo "<div class=\"msq_sql_echec\">
                                            <p>Veuillez remplir toutes les cases</p>
                                          </div>";
                                }
                            } 
                        ?>
                    
                    <form method="POST" action="admin.php" enctype="multipart/form-data">
                        <div class="form-ajout">
                            <div class="flex">
                                <div class="input">
                                    <p>Titre</p>
                                    <input type="text" name="titre" required autocomplete="off">
                                </div>
                                <div class="input file">
                                    <p>Couverture</p>
                                    <input type="file" name="cover" required accept="image/png, image/jpeg">
                                </div>
                            </div>
                            <div class="flex">
                                <div class="input">
                                    <p>Parution</p>
                                    <input type="date" name="parution" required>
                                </div>
                                <div class="input">
                                    <p>Sortie en bibliothèque</p>
                                    <input type="date" name="sortie_biblio" required>
                                </div>
                            </div>
                            <div class="input">
                                <p>Stock</p>
                                <input type="number" name="stock" min="-100" step="1" oninput="validEntier(this)" required>
                            </div>
                            <div class="flex-column">
                                <div class="flex">
                                    <div class="flex-column">
                                        <h2>Public visé</h2>
                                        <div class="selectionne">

                                        </div>
                                        <select class="select-container selected" name="public" multiple required onchange="changeBackground(this)">
                                            <option value="Tout Public">Tout Public</option>
                                            <option value="Ado">Ado</option>
                                            <option value="Adulte">Adulte</option>
                                        </select>
                                    </div>
                                    <div class="flex-column">
                                        <h2>Editeur</h2>
                                        <div class="selectionne">
                                        </div>
                                        <div class="checkbox-container">
                                            <?php
                                                try {
                                                    $dsn = "mysql:host=localhost;dbname=bibliotheque";
                                                    $connexion = new PDO($dsn, "root", "");
                                                }
                                                catch(PDOException $e) {
                                                    exit('Erreur : '.$e->getMessage());
                                                }
                                                $sql_get = "SELECT EditeurID FROM editeur"; 
                                                $result = $connexion->query($sql_get);
                                                $tab = $result->fetchAll();
                                                
                                                if (!empty($tab)) {
                                                    echo "<select class=\"select-content selected\" name=\"editeur\" multiple required>";
                                                    foreach ($tab as $ligne) {
                                                        $value = $ligne['EditeurID'];
                                                        echo "<option value=\"$value\">$value</option>";
                                                    }
                                                    echo "</select>";
                                                } else {
                                                    echo "<div class=\"checkbox-content\"><p>Aucun Editeur enregistrer,</br> veuillez en ajouter</p></div>";
                                                }
                                            ?>
                                        </div>
                                    </div>
                                    <div class="flex-column">
                                        <h2>Auteur</h2>
                                        <div id="auteur" class="selectionne">
                                        </div>
                                        <div class="checkbox-container">
                                            <?php
                                                try {
                                                    $dsn = "mysql:host=localhost;dbname=bibliotheque";
                                                    $connexion = new PDO($dsn, "root", "");
                                                }
                                                catch(PDOException $e) {
                                                    exit('Erreur : '.$e->getMessage());
                                                }
                                                $sql_get = "SELECT AuteurID, Nom, Prenom, Naissance FROM auteur"; 
                                                $result = $connexion->query($sql_get);
                                                $tab = $result->fetchAll();
                                                
                                                if (!empty($tab)) {
                                                    echo "<select class=\"select-content selected\" name=\"auteur\" multiple required>";
                                                    foreach ($tab as $ligne) {
                                                        $value = $ligne['AuteurID'];
                                                        $nom = $ligne['Nom'];
                                                        $prenom = $ligne['Prenom'];
                                                        $naissance = $ligne['Naissance'];
                                                            echo "<option value=\"$value\">$nom $prenom ($naissance)</option>";
                                                    }
                                                    echo "</select>";
                                                } else {
                                                    echo "<div class=\"checkbox-content\"><p>Aucun Auteur enregistrer,</br> veuillez en ajouter</p></div>";
                                                }
                                            ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="flex">
                                    <div class="flex-column">
                                        <h2>Genre</h2>
                                        <div class="selectionne">
                                            
                                        </div>
                                        <div class="checkbox-container">
                                            <?php
                                                try {
                                                    $dsn = "mysql:host=localhost;dbname=bibliotheque";
                                                    $connexion = new PDO($dsn, "root", "");
                                                }
                                                catch(PDOException $e) {
                                                    exit('Erreur : '.$e->getMessage());
                                                }
                                                $sql_get = "SELECT GenreID FROM type_ouvrage"; 
                                                $result = $connexion->query($sql_get);
                                                $tab = $result->fetchAll();
                                                
                                                if (!empty($tab)) {
                                                    echo "<select class=\"select-content selected\" name=\"genre\" multiple required>";
                                                    foreach ($tab as $ligne) {
                                                        $value = $ligne['GenreID'];
                                                        echo "<option value=\"$value\">$value</option>";
                                                    }
                                                    echo "</select>";
                                                } else {
                                                    echo "<div class=\"checkbox-content\"><p>Aucun Genre enregistrer,</br> veuillez en ajouter</p></div>";
                                                }
                                            ?>
                                        </div>
                                    </div>
                                    <div class="flex-column">
                                        <h2>Categorie</h2>
                                        <div class="selectionne">
                                            
                                        </div>
                                        <div class="checkbox-container">
                                            <?php
                                                try {
                                                    $dsn = "mysql:host=localhost;dbname=bibliotheque";
                                                    $connexion = new PDO($dsn, "root", "");
                                                }
                                                catch(PDOException $e) {
                                                    exit('Erreur : '.$e->getMessage());
                                                }
                                                $sql_get = "SELECT CategorieID FROM categorie"; 
                                                $result = $connexion->query($sql_get);
                                                $tab = $result->fetchAll();
                                                
                                                if (!empty($tab)) {
                                                    foreach ($tab as $ligne) {
                                                        $value = $ligne['CategorieID'];
                                                        echo "<div class=\"checkbox-content\"><input type=\"checkbox\" name=\"categorie[]\" value=\"$value\"/><p>$value</p></div>";
                                                    }
                                                } else {
                                                    echo "<div class=\"checkbox-content\"><p>Aucune Categorie enregistrer,</br> veuillez en ajouter</p></div>";
                                                }
                                            ?>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="flex-column">
                                <h2>Description</h2>
                                <textarea name="description" rows="6" cols="50" required></textarea>
                            </div>
                            
                            <input type="submit" value="Ajouter" class="btn-form">
                            <input type="hidden" name="ajout-livre" value="ok">
                        </div>
                    </form>
                </section>


                <section class="ajout-utilisateur">
                    <div class="titre-element">
                            <p>AJOUT - UTILISATEUR</p>
                    </div>
                    <?php
                        if (isset($_POST["ajout-utilisateur"])) {
                            try {
                                $dsn = "mysql:host=localhost;dbname=bibliotheque";
                                $connexion = new PDO($dsn, "root", "");
                            }
                            catch(PDOException $e) {
                                exit('Erreur : '.$e->getMessage());
                            }
                            
                            $passHash = password_hash($_POST["mdp"], PASSWORD_DEFAULT);
                            
                            $sql = "INSERT INTO utilisateur (Nom, Prenom, Naissance, Mail, Mdp) 
                                            VALUES ('{$_POST["nom"]}', '{$_POST["prenom"]}', '{$_POST["date"]}', '{$_POST["mail"]}', '{$passHash}')";
                            $retour = $connexion->exec($sql);
                            if ($retour) {
                                echo "<div class=\"msq_sql_succes\">
                                        <p>Utilisateur ajouté avec succès !</p>
                                    </div>";
                            } else {
                                echo "<div class=\"msq_sql_echec\">
                                        <p>Erreur lors de l'ajout de l'utilisateur !</p>
                                    </div>";
                            }
                        }
                    ?>
                    <form method="POST" action="admin.php">
                        <div class="form-ajout">
                            <div class="flex">
                                <div class="input">
                                    <p>Nom</p>
                                    <input type="text" name="nom" required autocomplete="off">
                                </div>
                                <div class="input">
                                    <p>Prenom</p>
                                    <input type="text" name="prenom" required autocomplete="off"> 
                                </div>
                            </div>

                            <div class="flex">
                                <div class="input">
                                    <p>Mail</p>
                                    <input type="email" name="mail" required autocomplete="off">
                                </div>
                                <div class="input">
                                    <p>Date de naissance</p>
                                    <input type="date" name="date" required>
                                </div>
                            </div>
                            <div class="input">
                                <p>Mot de passe</p>
                                <input type="password" name="mdp" required autocomplete='off'>
                            </div>
                            <input type="submit" value="Ajouter" class="btn-form">
                            <input type="hidden" name="ajout-utilisateur" value="ok">
                        </div>
                    </form>
                </section>

                <section class="ajout-auteur">
                    <div class="titre-element">
                        <p>AJOUT - AUTEURS</p>
                    </div>
                    <?php
                        if (isset($_POST["ajout-auteur"])) {
                            try {
                                $dsn = "mysql:host=localhost;dbname=bibliotheque";
                                $connexion = new PDO($dsn, "root", "");
                            }
                            catch(PDOException $e) {
                                exit('Erreur : '.$e->getMessage());
                            }
                            
                            $sql = "INSERT INTO auteur (Nom, Prenom, Naissance, Ville) 
                                    VALUES ('{$_POST["nom"]}', '{$_POST["prenom"]}', '{$_POST["date"]}', '{$_POST["ville"]}')";
                            $retour = $connexion->exec($sql);
                            
                            if ($retour) {
                                echo "<div class=\"msq_sql_succes\">
                                        <p>Auteur ajouté avec succès !</p>
                                    </div>";
                            } else {
                                echo "<div class=\"msq_sql_echec\">
                                        <p>Erreur lors de l'ajout de l'Auteur !</p>
                                    </div>";
                            }
                        }
                    ?>
                    <form method="POST" action="admin.php">
                        <div class="form-ajout">
                            <div class="flex">
                                <div class="input">
                                    <p>Nom</p>
                                    <input type="text" name="nom" required autocomplete="off">
                                </div>
                                <div class="input">
                                    <p>Prenom</p>
                                    <input type="text" name="prenom" required autocomplete="off">
                                </div>
                            </div>

                            <div class="flex">
                                <div class="input">
                                    <p>Date de naissance</p>
                                    <input type="date" name="date" required>
                                </div>
                                <div class="input">
                                    <p>Ville</p>
                                    <input type="text" name="ville" required autocomplete="off">
                                </div>
                            </div>
                            <input type="submit" value="Ajouter" class="btn-form">
                            <input type="hidden" name="ajout-auteur" value="ok">
                        </div>
                    </form>
                </section>

                <section class="ajout-autre">
                    <div class="titre-element">
                            <p>AJOUT - Editeur / Genre / Categorie</p>
                    </div>
                    <?php
                        if (isset($_POST["ajout-editeur"]) || isset($_POST["ajout-Genre"]) || isset($_POST["ajout-categorie"])) {
                            try {
                                $dsn = "mysql:host=localhost;dbname=bibliotheque";
                                $connexion = new PDO($dsn, "root", "");
                            }
                            catch(PDOException $e) {
                                exit('Erreur : '.$e->getMessage());
                            }
                            
                            if (isset($_POST["ajout-editeur"]) && isset($_FILES['cover']) && !empty($_FILES['cover']['name'])) {
                                $sql_get = "SELECT EditeurID FROM editeur";
                                $result = $connexion->query($sql_get);
                                $tab = $result->fetchAll();
                                
                                $colonne = array();
                                foreach ($tab as $ligne) {
                                    $colonne[] = $ligne['EditeurID'];
                                }
                                if (!in_array($_POST["nom"], $colonne)) {
                                    $file = $_FILES['cover'];
                                    
                                    $upload_directory = 'assets/logo_editor/';
                                    
                                    // Chemin complet du fichier
                                    $target_file = $upload_directory . basename($file['name']);
                                    if(move_uploaded_file($file['tmp_name'], $target_file)) {
                                        // Utilisation de la variable correcte pour le nom du fichier dans la requête SQL
                                        $logo_name = $_FILES['cover']['name'];
                                    
                                        // Utilisation de requête préparée pour éviter les injections SQL
                                        $sql = "INSERT INTO editeur (EditeurID, Logo) VALUES (:nom, :logo)";
                                        $stmt = $connexion->prepare($sql);
                                        $stmt->bindParam(':nom', $_POST["nom"]);
                                        $stmt->bindParam(':logo', $logo_name);
                                        $retour = $stmt->execute();
                                    
                                        if ($retour) {
                                            echo "<div class=\"msq_sql_succes\">
                                                    <p>Auteur ajouté avec succès !</p>
                                                </div>";
                                        } else {
                                            echo "<div class=\"msq_sql_echec\">
                                                    <p>Erreur lors de l'ajout de l'Auteur !</p>
                                                </div>";
                                        }
                                    } else {
                                        echo "<div class=\"msq_sql_echec\">
                                                <p>Une erreur s'est produite lors du téléchargement de l'image.</p>
                                              </div>";
                                    }
                                    
                                    
                                } else {
                                    echo "<div class=\"msq_sql_echec\">
                                            <p>Erreur le l'editeur est déjà enregistrer !</p>
                                        </div>";
                                }
                            }
                            if (isset($_POST["ajout-Genre"])) {
                                $sql_get = "SELECT GenreID FROM type_ouvrage";
                                $result = $connexion->query($sql_get);
                                $tab = $result->fetchAll();
                                
                                $colonne = array();
                                foreach ($tab as $ligne) {
                                    $colonne[] = $ligne['GenreID'];
                                }
                                if (!in_array($_POST["Genre"], $colonne)) {
                                    $sql = "INSERT INTO type_ouvrage (GenreID) 
                                    VALUES ('{$_POST["Genre"]}')";
                                    $retour = $connexion->exec($sql);
                                    if ($retour) {
                                        echo "<div class=\"msq_sql_succes\">
                                                <p>Genre ajouté avec succès !</p>
                                            </div>";
                                    } else {
                                        echo "<div class=\"msq_sql_echec\">
                                                <p>Erreur lors de l'ajout du Genre !</p>
                                            </div>";
                                    }
                                } else {
                                    echo "<div class=\"msq_sql_echec\">
                                            <p>Erreur le Genre est déjà enregistrer !</p>
                                          </div>";
                                }
                                
                            }
                            if (isset($_POST["ajout-categorie"])) {
                                $sql_get = "SELECT CategorieID FROM categorie";
                                $result = $connexion->query($sql_get);
                                $tab = $result->fetchAll();
                                
                                $colonne = array();
                                foreach ($tab as $ligne) {
                                    $colonne[] = $ligne['CategorieID'];
                                }
                                if (!in_array($_POST["categorie"], $colonne)) {
                                    $sql = "INSERT INTO categorie (CategorieID) 
                                    VALUES ('{$_POST["categorie"]}')";
                                    $retour = $connexion->exec($sql);
                                    if ($retour) {
                                        echo "<div class=\"msq_sql_succes\">
                                                <p>Categorie ajouté avec succès !</p>
                                            </div>";
                                    } else {
                                        echo "<div class=\"msq_sql_echec\">
                                                <p>Erreur lors de l'ajout de la Categorie !</p>
                                            </div>";
                                    }
                                } else {
                                    echo "<div class=\"msq_sql_echec\">
                                            <p>Erreur la Categorie est déjà enregistrer !</p>
                                          </div>";
                                }
                            
                            }
                        }
                    ?>
                    <div class="flex">
                        <form method="POST" action="admin.php" enctype="multipart/form-data">
                            <div class="form-ajout">
                                <div>
                                    <div class="titre-element">
                                        <p>Editeur</p>
                                    </div>
                                    <div class="input">
                                        <p>Nom</p>
                                        <input type="text" name="nom" required autocomplete="off">
                                    </div>
                                    <div class="input">
                                        <p>Logo</p>
                                        <input type="file" name="cover" accept="image/png, image/jpeg">
                                    </div>
                                </div>
                                <input type="submit" value="Ajouter" class="btn-form">
                                <input type="hidden" name="ajout-editeur" value="ok">
                            </div>
                        </form>
                        <form method="POST" action="admin.php">
                            <div class="form-ajout">
                                <div>
                                    <div class="titre-element">
                                        <p>Genre</p>
                                    </div>
                                    <div class="input">
                                        <input type="text" name="Genre" required autocomplete="off">
                                    </div>
                                </div>
                                <input type="submit" value="Ajouter" class="btn-form">
                                <input type="hidden" name="ajout-Genre" value="ok">
                            </div>
                        </form>
                        <form method="POST" action="admin.php">
                            <div class="form-ajout">
                                <div>
                                    <div class="titre-element">
                                        <p>Categorie</p>
                                    </div>
                                    <div class="input">
                                        <input type="text" name="categorie" required autocomplete="off">
                                    </div>
                                </div>
                                <input type="submit" value="Ajouter" class="btn-form">
                                <input type="hidden" name="ajout-categorie" value="ok">
                            </div>
                        </form>
                    </div>
                </section>

                <!-- SUPPRIMER LIVRE !-->
                
                <section class="gerer_livre">
                    <div class="block">
                        <h1 class="titre">Liste des Livres</h1>
                    </div>
                    <?php
                        if (!$_SESSION["connected"]) {
                            echo "
                            <div class=\"block bold\">
                                <p>Vous n'avez pas accès à cette page</p>
                            </div>";
                        } else {
                            try {
                                $dsn = "mysql:host=localhost;dbname=bibliotheque";
                                $connexion = new PDO($dsn, "root", "");
                                }
                            catch(PDOException $e) {
                                exit('Erreur : '.$e->getMessage());
                            }     

                            if (isset($_POST['suppr_livre'])) {
                                $LivreID = $_POST['LivreID'];
                                $sql = "DELETE FROM livre WHERE LivreID = $LivreID";
                                $retour = $connexion->exec($sql);
                            }
                            
                            echo "
                            <div class=\"block flex_column_center\">";
                                $sql = "SELECT DISTINCT LivreID, Couverture, Titre, EditeurID, Nom, Prenom
                                        FROM livre, auteur
                                        WHERE livre.AuteurID = auteur.AuteurID";
                                $result = $connexion->query($sql);
                                $tab = $result->fetchall();

                                if (empty($tab)) {
                                    echo "
                                        <div>
                                            <p>Il n'y a pas de livre</p>
                                        </div>";
                                } else {
                                
                                foreach ($tab as $ligne) {
                                    echo "
                                    <div class='block_livre'>
                                        <div class='flex_row' style='justify-content: space-between; width: 100%;'>
                                            <form method='post' action='admin.php' class='flex_row form_left'>
                                                <div class='flex_row'>
                                                    <img src='assets/cover/$ligne[Couverture]' alt='cover' class='element cover'>
                                                    <div class='flex_column element'>
                                                        <p><strong>$ligne[Titre]</strong></p>
                                                        <p>$ligne[Prenom] $ligne[Nom] / $ligne[EditeurID]</p>
                                                    </div>
                                                </div>
                                                <div class='input_container'>
                                                    <input type='hidden' name='LivreID' value='$ligne[LivreID]'>
                                                    <input type='submit' name='suppr_livre' value='supprimer' class='btn_suppr element'>
                                                </div>
                                            </form>
                                            
                                            <form method='POST' action='modifLivre.php' class='form_right'>
                                                <input type='hidden' name='LivreID' value='$ligne[LivreID]'>
                                                <input type='submit' name='modif' value='modifier' class='btn_modif element'>
                                            </form>
                                        </div>
                                    </div>";
                                        
                                        
                                    }
                                }
                        echo "</div>";
                            }
                        
                    ?>
                </section>

                <!-- SUPPRIMER UTILISATEUR !-->
                <section class="gerer_utilisateur">
                    <div class="block">
                        <h1 class="titre">Liste des Utilisateurs</h1>
                    </div>
                    <?php
                        if (!$_SESSION["connected"]) {
                            echo "
                            <div class=\"block bold\">
                                <p>Vous n'avez pas accès à cette page</p>
                            </div>";
                        } else {
                            try {
                                $dsn = "mysql:host=localhost;dbname=bibliotheque";
                                $connexion = new PDO($dsn, "root", "");
                                }
                            catch(PDOException $e) {
                                exit('Erreur : '.$e->getMessage());
                            }     

                            if (isset($_POST['suppr_utilisateur'])) {
                                $UtilisateurID = $_POST['UtilisateurID'];
                                $sql = "DELETE FROM utilisateur WHERE UtilisateurID = $UtilisateurID";
                                $retour = $connexion->exec($sql);
                            }
                            
                            echo "
                            <div class=\"block flex_column_center\">";
                                $sql = "SELECT DISTINCT UtilisateurID, Nom, Prenom, mail, rang
                                        FROM utilisateur";
                                $result = $connexion->query($sql);
                                $tab = $result->fetchall();

                                if (empty($tab)) {
                                    echo "
                                        <div>
                                            <p>Il n'y a pas d'Utilisateur</p>
                                        </div>";
                                } else {
                                
                                foreach ($tab as $ligne) {
                                    echo "
                                    <div class='block_livre'>
                                        <div class='flex_row' style='justify-content: space-between; width: 100%;'>
                                            <form method='post' action='admin.php' class='flex_row form_left'>
                                                <div class='flex_row'>
                                                    <div class='flex_column element'>
                                                        <p><strong>$ligne[Prenom] $ligne[Nom]</strong></p>
                                                        <p>$ligne[mail]</p>
                                                    </div>
                                                </div>
                                                <div class='input_container'>";
                                                if ($ligne['rang'] != 1) {
                                                    echo "
                                                    <input type='hidden' name='UtilisateurID' value='$ligne[UtilisateurID]'>
                                                    <input type='submit' name='suppr_utilisateur' value='supprimer' class='btn_suppr element'>";
                                                } else {
                                                    echo "
                                                    <p class='btn_suppr_desable element'>supprimer</p>";
                                                }
                                                    
                                          echo" </div>
                                            </form>
                                            
                                            <form method='POST' action='modifUtilisateur.php' class='form_right'>
                                                <input type='hidden' name='UtilisateurID' value='$ligne[UtilisateurID]'>
                                                <input type='submit' name='modif_utilisateur' value='modifier' class='btn_modif element'>
                                            </form>
                                        </div>
                                    </div>";
                                        
                                        
                                    }
                                }
                        echo "</div>";
                            }
                        
                    ?>
                </section>
            </div>
        




            

        <script src="js/profil-dropdown.js"></script>
        <script src="js/navMobile.js"></script>
        <script src="js/adminList-dropdown.js"></script>
        <script src="js/input_option.js"></script>
    </body>
</html>
