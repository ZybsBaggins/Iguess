<?php  

include "db.php";

session_start();

if(isset($_POST["brugernavn"]) && isset($_POST["password"])){
    $brugernavn = $_POST["brugernavn"];
    $password = $_POST["password"];

    // Assuming $conn is your database connection
    $sql = "SELECT * FROM brugere WHERE brugernavn = '$brugernavn' AND password = '$password'";
    $run_query = mysqli_query($conn, $sql);

    // Check if the query was successful
    if($run_query){
        // Check if there is a matching user in the database
        if(mysqli_num_rows($run_query) > 0){
            echo "success";

            header("Location: index.php");
            exit(); // Make sure to exit after a header redirect
        } else {
            echo "Invalid username or password";
        }
    } else {
        echo "Query failed: " . mysqli_error($conn);
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login Form</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: rgba(0, 0, 0, 0.3); /* Black with 80% opacity */
        }
        .container {
        width: 300px;
        margin: auto;
        margin-top: 50px;
        background-color: rgba(0, 0, 0, 0.5); /* Black with 80% opacity */
        padding: 20px; /* Add padding for content inside the container */
        border-radius: 10px; /* Add rounded corners */
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.8); /* Add a subtle box shadow */
        color: #fff;
        }
    </style>
</head>
<body>
    <div>
        <img src="aulogo.jpg" height="100px">
    </div>
    <div class="container">
        <form method="post" action="">
            <div>
                <h1> Dørlås Login
            </div>  
            <div class="input-box">
                <label for="brugernavn">Username:</label>
                <input type="text" name="brugernavn" id="brugernavn" required>
            </div>
            <div class="input-box">
                <label for="password">Password:</label>
                <input type="password" name="password" id="password" required>
            </div>
            <button type="submit">Submit</button>
        </form>


        <button onclick="location.href='register.php'">Register</button>
    </div>
</body>
</html>
