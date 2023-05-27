<?php

if (isset($_GET["time"]) && isset($_GET["temperature"]) && isset($_GET["pressure"]) && isset($_GET["humidity"])) {

   $time = $_GET["time"];
   $temperature = $_GET["temperature"];
   $pressure = $_GET["pressure"];
   $humidity = $_GET["humidity"];

   $servername = "localhost";
   $username = "root";
   // $password = "mysql_splash";
   $dbname = "weatherdb";
   $port = 3306;

   // Create connection
   $conn = new mysqli($servername, $username, null, $dbname, $port);
   // Check connection
   if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
   }

   $sql = "INSERT INTO weatherdata (time, temperature,pressure,humidity) VALUES ('$time', $temperature, $pressure, $humidity)";

   if ($conn->query($sql) === TRUE) {
      echo "New record created successfully";
   } else {
      echo "Error: " . $sql . " => " . $conn->error;
   }

   $conn->close();
} else {
   echo "weather data is not set";
}
?>