<?php
session_start();
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
        width: 90%;
        margin: auto;
        margin-top: 100px;
        background-color: rgba(0, 0, 0, 0.5);
        padding: 20px; 
        border-radius: 10px; 
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.8); 
        color: #fff;
        }
        .status {
            margin-left: 2%;
            margin-top: 5px;
            width: 96.5%;
            height: 50px;
            text-align: center;
            background-color: rgb(84,222,49, 0.5);
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.8); 

        }
        .button-left{
            margin-left: 2%;
            margin-top: 10px;
            width: 48%;
            height: 300px;
            text-align: center;
            display: inline-block;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.8); 
            

        }
        
        .button-right{
            margin-top: 10px;
            width: 48%;
            height: 300px;
            text-align: center;
            display: inline-block; 
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.8);

        }

        .button-left:hover,
        .button-right:hover {
            background-color: rgba(255, 255, 255, 0.2); /* Change color on hover */
            cursor: pointer; /* Change cursor on hover */
        }

        .button-left:active,
        .button-right:active {
            background-color: rgba(255, 255, 255, 0.4); /* Change color when clicked */
        }

    </style>
</head>

<body>
    <div>
        <img src="aulogo.jpg" height="100px">
    </div>
    <div class="container">
        <form method="post" action="">
            <div class="status">
                <h1>Døren er åben</h1>
            </div>
            <div class="button-left" id="unlock-buton">
                <h1> Lås OP </h1>
            </div>
            <div class="button-right" id="lock-button">
                <h1> Lås døren </h1>
            </div>

        </form>
    </div>

    <script src="https://code.jquery.com/jquery-3.6.4.min.js"></script>
    
    <?php
    // ÆNDRE TIL MASKINE SERVER
    $gpio_control_path = "http://your-server/gpio_control.php";

    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        $action = $_POST["action"];

        // Communicate with the GPIO control script
        $response = file_get_contents($gpio_control_path, false, stream_context_create([
            'http' => [
                'method' => 'POST',
                'header' => 'Content-Type: application/x-www-form-urlencoded',
                'content' => http_build_query(['action' => $action]),
            ],
        ]));

        // Return the response from the GPIO control script
        echo $response;
    }
    ?>

    <script>
        $(document).ready(function () {
            $("#unlock-button").click(function () {
                // Send a signal to the server when the unlock button is clicked
                $.post("handle_button.php", { action: "unlock" }, function (data) {
                    console.log(data);
                    // Update status and change color if needed
                    $(".status h1").text("Døren er åben");
                    $(".status").css("background-color", "rgb(84, 222, 49, 0.5)");
                });
            });

            $("#lock-button").click(function () {
                // Send a signal to the server when the lock button is clicked
                $.post("handle_button.php", { action: "lock" }, function (data) {
                    console.log(data);
                    // Update status and change color if needed
                    $(".status h1").text("Døren er låst");
                    $(".status").css("background-color", "red");
                });
            });
        });
    </script>

</body>
</html>
