<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Validate and sanitize user input
    $pin = filter_input(INPUT_POST, 'pin', FILTER_VALIDATE_INT);
    $action = filter_input(INPUT_POST, 'action', FILTER_SANITIZE_STRING);

    if ($pin !== false && ($action === 'unlock' || $action === 'lock')) {
        // Execute the command to control the GPIO pin
        if ($action == "unlock") {
            exec("echo in > /sys/class/gpio/gpio18/direction write $pin 24", $output, $return_var);
            echo "Unlocked";
        } elseif ($action == "lock") {
            exec("echo out > /sys/class/gpio/gpio18/direction write $pin 16", $output, $return_var);
            echo "Locked";
        }
    } else {
        // Invalid input, handle error or log it
        echo "Invalid input";
    }

    // if ($pin !== false && ($action === 'unlock' || $action === 'lock')) {
    //     // Execute the command to control the GPIO pin
    //     if ($action == "unlock") {
    //         exec("python3 /usr/bin/gpio write $pin 24", $output, $return_var);
    //         echo "Unlocked";
    //     } elseif ($action == "lock") {
    //         exec("python3 /usr/bin/gpio write $pin 16", $output, $return_var);
    //         echo "Locked";
    //     }
    // } else {
    //     // Invalid input, handle error or log it
    //     echo "Invalid input";
    // }
}
?>
