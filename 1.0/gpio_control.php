<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Validate and sanitize user input
    $pin = filter_input(INPUT_POST, 'pin', FILTER_VALIDATE_INT);
    $action = filter_input(INPUT_POST, 'action', FILTER_SANITIZE_STRING);

    if ($pin !== false && ($action === 'unlock' || $action === 'lock')) {
        // Execute the command to control the GPIO pin
        if ($action == "unlock") {
            shell_exec("/usr/bin/gpio -g mode $pin out");
            shell_exec("/usr/bin/gpio -g write $pin 1");
            echo "Pin $pin is on";
        } elseif ($action == "lock") {
            shell_exec("/usr/bin/gpio -g write $pin 0");
            echo "Pin $pin is off";
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
