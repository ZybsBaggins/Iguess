<?php

include "db.php";

session_start();

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (
        isset($_POST["navn"]) &&
        isset($_POST["brugernavn"]) &&
        isset($_POST["password"])
    ) {
        $navn = $_POST["navn"];
        $brugernavn = $_POST["brugernavn"];
        $password = $_POST["password"];

        $hashpw = password_hash($password, PASSWORD_DEFAULT);
        $sql = "INSERT INTO brugere (navn, brugernavn, password) VALUES ('$navn', '$brugernavn', '$hashpw')";
        $run_query = mysqli_query($conn, $sql);

        // Check if the query was successful
        if ($run_query) {
            echo "Registration successful";
            // Redirect to login page
            header("Location: index.php");
            exit(); 
        } else {
            echo "Query failed: " . mysqli_error($conn);
        }
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Registration Form</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: rgba(0, 0, 0, 0.3); 
        }
        .container {
        width: 300px;
        margin: auto;
        margin-top: 100px;
        background-color: rgba(0, 0, 0, 0.5);
        padding: 20px; 
        border-radius: 10px; 
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.8);
        color: #fff;
        }
        .input-box {
            margin-bottom: 20px;
        }
    </style>
</head>
<body>
    <div>
        <img src="aulogo.jpg" height="100px">
    </div>
    <div class="container">
        <form method="post" action="">
            <div class="input-box">
                <label for="navn">Name:</label>
                <input type="text" name="navn" id="navn" required>
            </div>
            <div class="input-box">
                <label for="brugernavn">Username:</label>
                <input type="text" name="brugernavn" id="brugernavn" required>
            </div>
            <div class="input-box">
                <label for="password">Password:</label>
                <input type="password" name="password" id="password" required>
            </div>
            <button type="submit">Register</button>
        </form>
    </div>
</body>
</html>