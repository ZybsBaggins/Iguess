<?php
session_start();

if ($_SERVER["REQUEST_METHOD"] === "POST") {
    if (isset($_POST['on'])) {
        // Set the GPIO pin to HIGH (ON)
        shell_exec("/usr/bin/gpio -g write 18 1");
        sleep(5);
        shell_exec("/usr/bin/gpio -g write 18 0");
        
    } elseif (isset($_POST['off'])) {
        // Set the GPIO pin to LOW (OFF)
        shell_exec("/usr/bin/gpio -g write 18 0");
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
        <form method="post" action="login.php">
            <div class="status">
                <h1>
                    <?php
                    // Read and display the current state of the GPIO pin
                    $gpio_state = shell_exec("/usr/bin/gpio -g read 18");
                    if ($gpio_state == 1) {
                        echo "Døren er låst";
                    } else {
                        echo "Døren er åben";
                    }
                    ?>
                </h1>
            </div>
            <div class="button-left">
                <input type="submit" value="OFF" name="off">
                <h1> Lås OP </h1>
            </div>
            <div class="button-right">
                <input type="submit" value="ON" name="on">
                <h1> Lås døren </h1>
            </div>
        </form>
    </div>

    <script src="https://code.jquery.com/jquery-3.6.4.min.js"></script>
    
    <script>
        $(document).ready(function () {
            $(".button-left").click(function () {       
                // Update status and change color
                $(".status h1").text("Døren er låst");
                $(".status").css("background-color", "red");
            });

            $(".button-right").click(function () {
                // Update status and change color
                $(".status h1").text("Døren er åben");
                $(".status").css("background-color", "rgb(84, 222, 49, 0.5)");
            });
        });
    </script>
</body>
</html>
