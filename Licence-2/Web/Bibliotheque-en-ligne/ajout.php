<?php
session_start();
if (!in_array($_POST["emprunt"], $_SESSION["panier"])) {
	array_push($_SESSION["panier"], $_POST["emprunt"]);
}
header('Location: index.php');
exit;
?>
<!DOCTYPE html>
<html lang="en">

<head>
	<title>sans titre</title>
	<meta http-equiv="content-type" content="text/html;charset=utf-8" />
	<meta name="generator" content="Geany 1.38" />
</head>

<body>
	
</body>

</html>
