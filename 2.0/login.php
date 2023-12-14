<?php
session_start();

require_once 'vendor/autoload.php'; // Include the autoloader for PiPHP GPIO

use PiPHP\GPIO\GPIO;
use PiPHP\GPIO\Pin\PinInterface;
use PiPHP\GPIO\Pin\InputPinInterface;
use PiPHP\GPIO\Pin\OutputPinInterface;

// Create a GPIO object
$gpio = new GPIO();

// Retrieve pin 18 and configure it as an output pin
$pin = $gpio->getOutputPin(18);

// Set the initial state of the pin (e.g., low)
$pin->setValue(PinInterface::VALUE_HIGH);

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
        <form method="get" action="gpio.php">
            <div class="status">
                <h1>Døren er åben</h1>
            <input type="submit" value="ON" name= "on">
            <input type="submit" value="OFF" name= "off">
            </div>

        </form>
    </div>

    <script src="https://code.jquery.com/jquery-3.6.4.min.js"></script>
    
    <?php
                $setmode17 = shell_exec("/usr/local/bin/gpio -g mode 18 out");
                if(isset($_GET['on'])){
                    $gpio_on = shell_exec("/usr/local/bin/gpio -g write 18 1");
                    echo "led on";
                }
                else if(isset($_GET['off'])){
                    $gpio_on = shell_exec("/usr/local/bin/gpio -g write 18 0");
                    echo "led off";
                }
    ?>
    
</body>
</html>
