<?php

$server = "192.168.237.183";
$username = "mikk";
$password = "1234";
$db = "projekt3";

$conn = mysqli_connect($server, $username, $password, $db);

if (!$conn) {
    die("Failed to connect" . mysqli_connect_error());
}