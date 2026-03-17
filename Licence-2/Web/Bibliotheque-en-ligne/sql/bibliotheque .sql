-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Hôte : 127.0.0.1:3306
-- Généré le : mar. 14 mai 2024 à 13:58
-- Version du serveur : 8.0.31
-- Version de PHP : 8.0.26

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données : `bibliotheque`
--

-- --------------------------------------------------------

--
-- Structure de la table `auteur`
--

DROP TABLE IF EXISTS `auteur`;
CREATE TABLE IF NOT EXISTS `auteur` (
  `AuteurID` int NOT NULL AUTO_INCREMENT,
  `Nom` varchar(255) DEFAULT NULL,
  `Prenom` varchar(255) DEFAULT NULL,
  `Naissance` date DEFAULT NULL,
  `Ville` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`AuteurID`)
) ENGINE=InnoDB;

--
-- Déchargement des données de la table `auteur`
--

INSERT INTO `auteur` (`AuteurID`, `Nom`, `Prenom`, `Naissance`, `Ville`) VALUES
(1, 'Zola', 'Emile', '0000-00-00', 'Paris'),
(2, 'Camus', 'Albert', '1500-12-05', 'Paris'),
(3, 'Hugo', 'Victor', '1702-06-04', 'Rouen');

-- --------------------------------------------------------

--
-- Structure de la table `categorie`
--

DROP TABLE IF EXISTS `categorie`;
CREATE TABLE IF NOT EXISTS `categorie` (
  `CategorieID` varchar(255) NOT NULL,
  PRIMARY KEY (`CategorieID`)
) ENGINE=InnoDB;

--
-- Déchargement des données de la table `categorie`
--

INSERT INTO `categorie` (`CategorieID`) VALUES
('drama'),
('fantasy'),
('horreur'),
('science-fiction'),
('suspence');

-- --------------------------------------------------------

--
-- Structure de la table `editeur`
--

DROP TABLE IF EXISTS `editeur`;
CREATE TABLE IF NOT EXISTS `editeur` (
  `EditeurID` varchar(255) NOT NULL,
  `logo` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`EditeurID`)
) ENGINE=InnoDB;

--
-- Déchargement des données de la table `editeur`
--

INSERT INTO `editeur` (`EditeurID`, `logo`) VALUES
('1', 'tkt'),
('bragelonne', 'lumen'),
('Glénat', NULL),
('lumen', NULL),
('Pika', NULL);

-- --------------------------------------------------------

--
-- Structure de la table `emprunt`
--

DROP TABLE IF EXISTS `emprunt`;
CREATE TABLE IF NOT EXISTS `emprunt` (
  `EmpruntID` int NOT NULL AUTO_INCREMENT,
  `LivreID` int DEFAULT NULL,
  `UtilisateurID` int DEFAULT NULL,
  `Debut` date DEFAULT NULL,
  `Fin` date DEFAULT NULL,
  PRIMARY KEY (`EmpruntID`),
  KEY `LivreID` (`LivreID`),
  KEY `UtilisateurID` (`UtilisateurID`)
) ENGINE=InnoDB;


-- --------------------------------------------------------

--
-- Structure de la table `livre`
--

DROP TABLE IF EXISTS `livre`;
CREATE TABLE IF NOT EXISTS `livre` (
  `LivreID` int NOT NULL AUTO_INCREMENT,
  `PublicID` varchar(255) DEFAULT NULL,
  `Titre` varchar(255) DEFAULT NULL,
  `AuteurID` int DEFAULT NULL,
  `EditeurID` varchar(255) DEFAULT NULL,
  `Parution` date DEFAULT NULL,
  `GenreID` varchar(255) DEFAULT NULL,
  `Stock` int DEFAULT NULL,
  `Emprunt` int DEFAULT 0,
  `DateSortieBiblio` date DEFAULT NULL,
  `Description` varchar(250) NOT NULL,
  `Couverture` varchar(250) NOT NULL,
  `HistEmprunt` int DEFAULT 0 NOT NULL,
  PRIMARY KEY (`LivreID`),
  KEY `PublicID` (`PublicID`),
  KEY `AuteurID` (`AuteurID`),
  KEY `GenreID` (`GenreID`),
  KEY `EditeurID` (`EditeurID`)
) ENGINE=InnoDB;

--
-- Déchargement des données de la table `livre`
--

INSERT INTO `livre` (`LivreID`, `PublicID`, `Titre`, `AuteurID`, `EditeurID`, `Parution`, `GenreID`, `Stock`, `Emprunt`, `DateSortieBiblio`, `Description`, `Couverture`) VALUES
(2, 'Adulte', 'LA VIE', 1, 'Pika', '0000-00-00', 'comics', 1, 1, '2000-12-31', 'La mort et la vie sont liés. Et l\'un comme l\'autre arrivent le même nombre de fois.', 'vie.jpg'),
(3, 'Ado', 'La peste', 2, 'Glénat', '1482-07-09', 'roman', 2, 1, '2004-05-11', 'La peste a tué pleins de gens :(', 'peste.jpg'),
(1, 'Adulte', 'LA MORT', 1, 'Glénat', '2000-03-04', 'roman', 1, 1, '2002-04-05', 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam condimentum elit ac magna molestie, eu bibendum  purus eget...', 'mort.png'),
(4, 'Ado', 'Les fleurs du mal', 3, 'bragelonne', '1802-12-31', 'magazine', 1, 0, '2000-11-30', 'La fille est morte si vous n\'aviez pas compris.', 'fleurs.jpg'),
(5, 'ToutPublic', 'LE FUTUR', 2, 'Glénat', '2000-03-05', 'bd', 1, 0, '2025-06-14', 'Ce livre parle du futur.', 'futur.jpg'),
(6, 'ToutPublic', 'Un monde sans fin et sans espoir', 3, 'lumen', '1802-12-30', 'manga', 1, 0, '2024-06-04', 'On va dans le mur.', 'apocalypse.jpg'),
(7, 'ToutPublic', 'Dora ninja', 2, 'lumen', '2024-08-12', 'art', 2, 0, '2024-08-13', 'Dora est devenue une ninja maléfique prête à tuer tout ce qui bouge. Malheureusement elle s\'est fait voler son goûter et elle est prête à tout pour le récupérer. Chippeur n\'a qu\'à bien se tenir !', 'dora.jpg'),
(8, 'ToutPublic', 'Un  sans espoir', 3, 'lumen', '1802-12-30', 'manga', 0, 0, '2024-06-04', 'On va dans le ciel.', 'apocalypse.jpg');
-- --------------------------------------------------------

--
-- Structure de la table `livre_categorie`
--

DROP TABLE IF EXISTS `livre_categorie`;
CREATE TABLE IF NOT EXISTS `livre_categorie` (
  `CategorieID` varchar(255) DEFAULT NULL,
  `LivreID` int DEFAULT NULL,
  KEY `CategorieID` (`CategorieID`),
  KEY `LivreID` (`LivreID`)
) ENGINE=InnoDB;

--
-- Déchargement des données de la table `livre_categorie`
--

INSERT INTO `livre_categorie` (`CategorieID`, `LivreID`) VALUES
('science-fiction', 5),
('drama', 4),
('horreur', 2),
('fantasy', 1),
('fantasy', 5),
('suspence', 3),
('suspence', 6),
('horreur', 7);

-- --------------------------------------------------------

--
-- Structure de la table `public_vise`
--

DROP TABLE IF EXISTS `public_vise`;
CREATE TABLE IF NOT EXISTS `public_vise` (
  `PublicID` varchar(255) NOT NULL,
  PRIMARY KEY (`PublicID`)
) ENGINE=InnoDB;

--
-- Déchargement des données de la table `public_vise`
--

INSERT INTO `public_vise` (`PublicID`) VALUES
('ToutPublic'),
('Ado'),
('Adulte');

-- --------------------------------------------------------

--
-- Structure de la table `rang`
--

DROP TABLE IF EXISTS `rang`;
CREATE TABLE IF NOT EXISTS `rang` (
  `RangID` int NOT NULL,
  `MaxEmprunt` int DEFAULT NULL,
  `MaxDelai` int DEFAULT NULL,
  PRIMARY KEY (`RangID`)
) ENGINE=InnoDB;

--
-- Déchargement des données de la table `rang`
--

INSERT INTO `rang` (`RangID`, `MaxEmprunt`, `MaxDelai`) VALUES
(1, 10, 10),
(2, 2, 2);

-- --------------------------------------------------------

--
-- Structure de la table `type_ouvrage`
--

DROP TABLE IF EXISTS `type_ouvrage`;
CREATE TABLE IF NOT EXISTS `type_ouvrage` (
  `GenreID` varchar(255) NOT NULL,
  PRIMARY KEY (`GenreID`)
) ENGINE=InnoDB;

--
-- Déchargement des données de la table `type_ouvrage`
--

INSERT INTO `type_ouvrage` (`GenreID`) VALUES
('1'),
('bd'),
('comics'),
('art'),
('magazine'),
('manga'),
('roman');

-- --------------------------------------------------------

--
-- Structure de la table `utilisateur`
--

DROP TABLE IF EXISTS `utilisateur`;
CREATE TABLE IF NOT EXISTS `utilisateur` (
  `UtilisateurID` int NOT NULL AUTO_INCREMENT,
  `Nom` varchar(255) DEFAULT NULL,
  `Prenom` varchar(255) DEFAULT NULL,
  `Naissance` date DEFAULT NULL,
  `mail` varchar(255) DEFAULT NULL,
  `rang` int DEFAULT 2 NULL,
  `mdp` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`UtilisateurID`),
  KEY `rang` (`rang`)
) ENGINE=InnoDB;

--
-- Déchargement des données de la table `utilisateur`
--

INSERT INTO `utilisateur` (`UtilisateurID`, `Nom`, `Prenom`, `Naissance`, `mail`, `rang`, `mdp`) VALUES
(1503, 'Delaune', 'Armand', '2005-06-19', 'o', 1, '$2y$10$qwlMk1X6tn4f6ex/y9lQkO8mUFhzuTl.jLoECAIr53P0S1HWseN7a'),
(1504, 'UN', 'gars', '1999-05-07', 'o', 2, '$2y$10$0tOspiEkc3d5ElIv1BdFp.Pqe5301PJLt.PK7gDI7wdFfrWOE2sMW');

--
-- Contraintes pour les tables déchargées
--

--
-- Contraintes pour la table `emprunt`
--
ALTER TABLE `emprunt`
  ADD CONSTRAINT `emprunt_ibfk_1` FOREIGN KEY (`LivreID`) REFERENCES `livre` (`LivreID`) ON DELETE CASCADE,
  ADD CONSTRAINT `emprunt_ibfk_2` FOREIGN KEY (`UtilisateurID`) REFERENCES `utilisateur` (`UtilisateurID`) ON DELETE CASCADE;

--
-- Contraintes pour la table `livre`
--
ALTER TABLE `livre`
  ADD CONSTRAINT `livre_ibfk_1` FOREIGN KEY (`PublicID`) REFERENCES `public_vise` (`PublicID`) ON DELETE CASCADE,
  ADD CONSTRAINT `livre_ibfk_2` FOREIGN KEY (`AuteurID`) REFERENCES `auteur` (`AuteurID`) ON DELETE CASCADE,
  ADD CONSTRAINT `livre_ibfk_3` FOREIGN KEY (`GenreID`) REFERENCES `type_ouvrage` (`GenreID`) ON DELETE CASCADE,
  ADD CONSTRAINT `livre_ibfk_4` FOREIGN KEY (`EditeurID`) REFERENCES `editeur` (`EditeurID`) ON DELETE CASCADE;

--
-- Contraintes pour la table `livre_categorie`
--
ALTER TABLE `livre_categorie`
  ADD CONSTRAINT `livre_categorie_ibfk_1` FOREIGN KEY (`CategorieID`) REFERENCES `categorie` (`CategorieID`) ON DELETE CASCADE,
  ADD CONSTRAINT `livre_categorie_ibfk_2` FOREIGN KEY (`LivreID`) REFERENCES `livre` (`LivreID`) ON DELETE CASCADE;

--
-- Contraintes pour la table `utilisateur`
--
ALTER TABLE `utilisateur`
  ADD CONSTRAINT `utilisateur_ibfk_1` FOREIGN KEY (`rang`) REFERENCES `rang` (`RangID`) ON DELETE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
