<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $pin = $_POST["pin"];
    $action = $_POST["action"];

    // Execute the command to control the GPIO pin
    if ($action == "unlock") {
        exec("gpio write $pin 1");
        echo "Unlocked";
    } elseif ($action == "lock") {
        exec("gpio write $pin 0");
        echo "Locked";
    }
}
?>
