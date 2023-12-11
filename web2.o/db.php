<?php

$server = "localhost";
$username = "root";
$password = "";
$db = "projekt3";

$conn = mysqli_connect($server, $username, $password, $db);

if (!$conn) {
    die("Failed to connect" . mysqli_connect_error());
}