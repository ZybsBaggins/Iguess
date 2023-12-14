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

</body>
</html>
