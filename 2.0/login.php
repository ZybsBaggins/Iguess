<?php
session_start();
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <!-- ... (your existing HTML head content) ... -->
</head>
<body>
    LED Control:
    <form method="post" action="gpio.php">
        <input type="submit" value="ON" name="on">
        <input type="submit" value="OFF" name="off">
    </form>

    <?php
    // Validate and sanitize user input
    if (isset($_POST['on'])) {
        // Ensure that 18 is the correct GPIO pin number for your LED
        $gpio_on = shell_exec("/usr/local/bin/gpio -g write 18 1");
        if ($gpio_on === false) {
            echo "Error turning LED on";
        } else {
            echo "LED is on";
        }
    } elseif (isset($_POST['off'])) {
        // Ensure that 18 is the correct GPIO pin number for your LED
        $gpio_off = shell_exec("/usr/local/bin/gpio -g write 18 0");
        if ($gpio_off === false) {
            echo "Error turning LED off";
        } else {
            echo "LED is off";
        }
    }
    ?>
</body>
</html>
