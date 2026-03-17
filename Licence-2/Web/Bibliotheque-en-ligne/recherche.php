<?php
  session_start();
  if (!isset($_SESSION["recherche_mot"]) || isset($_POST['reset_mot'])) {
    $_SESSION["recherche_mot"] = null;
  }
?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>sans titre</title>
    <meta name="generator" content="Geany 2.0">
    <link href="css/nav.css" rel="stylesheet">
    <link href="css/recherche.css" rel="stylesheet">
    <script src="https://kit.fontawesome.com/b01cc878ee.js" crossorigin="anonymous"></script>
  </head>
  <?php
    //FONCTION PHP

    function str_format ($str) {
      return strtolower($str);
    }

    function afficher_livre ($ligne) {
      $cover_directory = 'assets/cover/';
      $path_default = 'assets/cover/default.jpg';
      
      $livreID = $ligne['LivreID'];
      $titre = $ligne['Titre'];
      $prenom = $ligne['Prenom'];
      $auteur = $ligne['Nom'];
      $parution = $ligne['Parution'];
      $desc = $ligne['Description'];
      $stock = $ligne['Stock'];
      $cover = $ligne['Couverture'];
      $emprunt = $ligne['Emprunt'];

      $path = $cover_directory . $cover;
      echo "<div class='block book'>
              <div class='book-content'>
                <div class='book-cover'>";
            
                if (file_exists($path)) {
                  echo "<img src='$path' alt='couverture du livre' style='width: 130px; height: 180px;'>";
                } else {
                  echo "<img src='$path_default' alt='couverture du livre' style='width: 130px; height: 180px;'>";
                }
        echo "</div>
                <div class='book-summary'>
                  <div class='book-title'>
                    <form method='POST' action='livre.php'>
                      <input type='hidden' name='livre' value='$livreID'>
                      <button class='input_titre' type='submit'>$titre</button>
                    </form>
                    <h2>$prenom $auteur - $parution</h2>
                  </div>
                  <div>
                    <p>$desc</p>
                  </div>
                </div>
              </div>";
              
              if ($emprunt != $stock) {
              echo "<div class='dispo-emprunter'>
                      <div class='flex'>
                        <div class='green-circle'></div>
                        <p>Exemplaire(s) disponible</p>
                      </div>

                      <form method='POST' action='ajout.php'>
                        <input type='hidden' name='emprunt' value='{$livreID}'>
                        <input class='emprunter-button' type='submit' value='Emprunter'>
                      </form>
                    </div>
                  </div>";
              } else {
                echo "<div class='dispo-emprunter'>
                        <div class='flex'>
                          <div class='red-circle'></div>
                          <p>Exemplaire indisponible</p>
                        </div>
                      
                        <div class='blocked-emprunter-button'>
                          <p>Emprunter</p>
                        </div>
                      </div>
                    </div>";
              }
      
    }
  ?>

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
							<input type = "text" name = "recherche" placeholder = "Tapez votre recherche" autocomplete="off">
							<button id = "sub" title = "envoyer" name = "search" type = "submit" ></button>
						</form>
					</div>
				</div>
			</div>
	</div>
    <a id = "retour-haut" href = "#haut"></a>

    
    <section id="container" class="flex">
      <section id="recherche">
        <form method="post" action="recherche.php" id="filter">
          <h1>AFFINEZ VOTRE RECHERCHE</h1>
          <div class="btn-container-filter">
              <input type="submit" value="Filtrer" name="filtre">
          </div>
          
          <div id="filtre-container">
            <div class="filtre-title" onclick="togglePublic()">
              <h2>PUBLIC VISE</h2>
              <img class="btn-arrow" id="btn-public" src="assets/fleche.png" alt="déroule, enroule le filtre">
            </div>

            <div class="filtre" id="filter-public">
              <div class="filter-container-checkbox">
                  <p><input class="input-checkbox" type="checkbox" name="filter_public[]" value="Toutpublic">Tout public</p>
                  <p><input class="input-checkbox" type="checkbox" name="filter_public[]" value="Ado">Ado</p>
                  <p><input class="input-checkbox" type="checkbox" name="filter_public[]" value="Adulte">Adulte</p>
              </div>
            </div>
            
            
            <div class="filtre-title" onclick="toggleAuteur()">
              <h2>AUTEUR</h2>
              <img class="btn-arrow" id="btn-auteur" src="assets/fleche.png" alt="déroule, enroule le filtre">
            </div>

            <div class="filtre" id="filter-auteur">
              <div class="filter-container-checkbox" style="display:flex; justify-content: space-around; flex-wrap: wrap;">
                  <input class="input-text" type="text" placeholder="Nom" name="auteur_nom">
                  <input class="input-text" type="text" placeholder="Prénom" name="auteur_prenom">
              </div>
            </div>


            <div class="filtre-title" onclick="toggleEditeur()">
              <h2>EDITEURS</h2>
              <img class="btn-arrow" id="btn-editeur" src="assets/fleche.png" alt="déroule, enroule le filtre">
            </div>

            <div class="filtre" id="filter-editeur">
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
                      echo "<div class='filter-container-checkbox'>";
                      foreach ($tab as $ligne) {
                          $value = $ligne['EditeurID'];
                          echo "<p><input class='input-checkbox' type='checkbox' name='filter_editeur[]' value='$value'>$value</p>";
                      }
                      echo "</div>";
                  } else {
                      echo "<p>Aucun Editeur enregistré</p>";
                  }
              ?>
            </div>


            <div class="filtre-title" onclick="toggleDate()">
              <h2>DATE</h2>
              <img class="btn-arrow" id="btn-date" src="assets/fleche.png" alt="déroule, enroule le filtre">
            </div>

            <div class="filtre" id="filter-date">
              <div class="filter-container-checkbox" style="display:flex; justify-content: space-around; flex-wrap: wrap;">
                  <input class="input-text" type="text" placeholder="A partir de" name="date_debut">
                  <input class="input-text" type="text" placeholder="Jusqu'à" name="date_fin">
              </div>
            </div>


            <div class="filtre-title" onclick="toggleCategorie()">
              <h2>CATEGORIES</h2>
              <img class="btn-arrow" id="btn-categorie" src="assets/fleche.png" alt="déroule, enroule le filtre">
            </div>

            <div class="filtre" id="filter-categorie">
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
                        echo "<div class='filter-container-checkbox'>";
                        foreach ($tab as $ligne) {
                            $value = $ligne['CategorieID'];
                            echo "<p><input class='input-checkbox' type='checkbox' name='filter_categorie[]' value='$value'>$value</p>";
                        }
                        echo "</div>";
                    } else {
                        echo "<p>Aucune Catégorie enregistré</p>";
                    }
                ?>
            </div>


            <div class="filtre-title" onclick="toggleType()">
              <h2>TYPE OUVRAGE</h2>
              <img class="btn-arrow" id="btn-type" src="assets/fleche.png" alt="déroule, enroule le filtre">
            </div>

            <div class="filtre" id="filter-type">
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
                        echo "<div class='filter-container-checkbox'>";
                        foreach ($tab as $ligne) {
                            $value = $ligne['GenreID'];
                            echo "<p><input class='input-checkbox' type='checkbox' name='genre[]' value='{$value}'>$value</p>";
                        }
                        echo "</div>";
                    } else {
                        echo "<p>Aucun Type d'ouvrage enregistré</p>";
                    }
                ?>
            </div>
          </div>
        </form>
      </section>

      <section id="resultat-container">
          <div id="recap-container" class="block">
            <h2>VOTRE RECHERCHE</h2>
            <div id='recap_mot_supp'>
              <div id="recap">
                <?php         
                  if (isset($_POST['recherche']) && !empty($_POST['recherche'])){
                    $_SESSION['recherche_mot'] = $_POST['recherche'];
                    $recherche = $_POST['recherche'];
                    echo"<p>$recherche</p>";
                  } else if (!empty($_SESSION['recherche_mot'])){
                    $recherche = $_SESSION['recherche_mot'];
                    echo"<p>$recherche</p>";
                  } else {
                    echo "<p>&quot; &quot;</p>";
                  }
                ?>
              </div>
              <?php
                echo "<form method='POST' action='recherche.php'>
                        <input id='supp_mot' type='submit' value='effacer' name='reset_mot'>
                      </form>";
              ?>
            </div>
          </div>
          
          <div id="num_page"></div>
          <div id="book-container">
            <?php
              try {
                  $dsn = "mysql:host=localhost;dbname=bibliotheque";
                  $connexion = new PDO($dsn, "root", "");
              }
              catch(PDOException $e) {
                  exit('Erreur : '.$e->getMessage());
              }


              
              $sql = "SELECT DISTINCT livre.LivreID, livre.PublicID, livre.Titre, auteur.Nom, auteur.Prenom, livre.EditeurID, livre.Parution, livre.GenreID, livre.Stock, livre.Description, livre.Couverture, livre.Emprunt 
              FROM livre
              JOIN auteur ON livre.AuteurID = auteur.AuteurID
              JOIN livre_categorie ON livre.LivreID = livre_categorie.LivreID
              WHERE livre.AuteurID = auteur.AuteurID AND livre.LivreID = livre_categorie.LivreID";

              if (isset($_POST['filtre'])) {
                if (!empty($_POST['filter_public'])) {
                  $first = true;
                  $sql .= " AND (";
                  foreach ($_POST['filter_public'] as $value) {
                    if ($first) {
                      $sql .= "livre.PublicID = '{$value}'";
                      $first = false;
                    } else {
                      $sql .= " OR livre.PublicID = '{$value}'";
                    }
                  }
                  $sql .= ")";
                }

                if (!empty($_POST['auteur_nom'])) {
                    $value = $_POST['auteur_nom'];
                    $sql .= " AND Nom = '{$value}'";
                }
                if (!empty($_POST['auteur_prenom'])) {
                  $value = $_POST['auteur_prenom'];
                  $sql .= " AND Prenom = '{$value}'";
                }

                if (!empty($_POST['filter_editeur'])) {
                  $first = true;
                  $sql .= " AND (";
                  foreach ($_POST['filter_editeur'] as $value) {
                    if ($first) {
                      $sql .= "EditeurID = '{$value}'";
                      $first = false;
                    } else {
                      $sql .= " OR EditeurID = '{$value}'";
                    }
                  }
                  $sql .= ")";
                }

                if (!empty($_POST['date_debut'])) {
                  $value = $_POST['date_debut'];
                  $sql .= " AND Parution >= '{$value}'";
                }
                if (!empty($_POST['date_fin'])) {
                  $value = $_POST['date_fin'];
                  $sql .= " AND Parution <= '{$value}'";
                }

                if (!empty($_POST['filter_categorie'])) {
                    $first = true;
                    $sql .= " AND (";
                    foreach ($_POST['filter_categorie'] as $value) {
                      if ($first) {
                        $sql .= "CategorieID = '{$value}'";
                        $first = false;
                      } else {
                        $sql .= " OR CategorieID = '{$value}'";
                      }
                    }
                    $sql .= ")";
                }

                if (!empty($_POST['genre'])) {
                  $first = true;
                  $sql .= " AND (";
                  foreach ($_POST['genre'] as $value) {
                    if ($first) {
                      $sql .= "GenreID = '{$value}'";
                      $first = false;
                    } else {
                      $sql .= " OR GenreID = '{$value}'";
                    }
                  }
                  $sql .= ")";
                }

                if (!empty($_SESSION['recherche_mot'])) {
                  $value = $_SESSION['recherche_mot'];
                  $sql .= " AND Titre LIKE '%{$value}%'";
                }
              }

              // Utilisation de requête préparée pour éviter les injections SQL
              
              $result = $connexion->query($sql);
              $tab = $result->fetchAll();
              $afficher = 0;
              if (empty($tab)) {
                echo "<div class='block'>
                        <p id='no-result'>Aucun Résultat</p>
                      </div>";
              } else {
                  $isSearch = false;
                  if (isset($_POST['search']) || !empty($_POST['recherche'])) {
                    $recherche = str_format($_POST['recherche']);
                    $isSearch = true;
                  }
                  foreach ($tab as $ligne) {
                      if ($isSearch) {
                        $titre = str_format($ligne['Titre']);
                      if (str_contains($titre, $recherche)) {
                        afficher_livre($ligne);
                        $afficher++;
                      } else {
                        continue;
                      }
                    } else {
                      afficher_livre($ligne);
                      $afficher++;
                    } 
                  }
                  if ($afficher == 0) {
                    echo "<div class='block'>
                        <p id='no-result'>Aucun Résultat</p>
                      </div>";
                  }
              }
            ?>
          </div>
      </section>
      
    </section>
    
    <script src="js/profil-dropdown.js"></script>
    <script src="js/filter-dropdown.js"></script>
    <script src="js/navMobile.js"></script>
  </body>
</html>
