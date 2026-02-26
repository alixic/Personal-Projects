<!-- procesare.php -->

<!DOCTYPE html>
<html lang="ro">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Procesare Formular</title>
</head>
<body>
    <?php
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // Verificăm dacă există date trimise prin formular
        if (isset($_POST["name"]) && isset($_POST["email"])) {
            $name = $_POST["name"];
            $email = $_POST["email"];

            // Afișăm datele primite
            echo "<h1>Date primite:</h1>";
            echo "<p>Nume: " . htmlspecialchars($name) . "</p>";
            echo "<p>Email: " . htmlspecialchars($email) . "</p>";
        } else {
            echo "<p>Eroare: Nu s-au primit date valide.</p>";
        }
    } else {
        echo "<p>Eroare: Acces nepermis.</p>";
    }
    ?>
</body>
</html>
