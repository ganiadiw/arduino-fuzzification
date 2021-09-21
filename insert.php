<?php
    $dbhost = "localhost";
    $username = "root";
    $password = "";
    $dbname = "fuzzification";

    if(isset($_GET["suhu"]) || isset($_GET["kelembaban"]) || isset($_GET["lama_waktu"])){
        $suhu = $_GET["suhu"];
        $kelembaban = $_GET["kelembaban"];
        $lama_waktu = $_GET["lama_waktu"];
    }

    $conn = new mysqli ($dbhost, $username, $password, $dbname);

    if ($conn->connect_error){
        die("Connection Failed:" . $conn->connect_error);
    }

    $query = "INSERT INTO green_houses (suhu, kelembaban, lama_waktu) VALUES ('$suhu', '$kelembaban', '$lama_waktu')";

    if ($conn->query($query) === true){
        echo "New record created succesfully";
    } else {
        echo "Error: " . $query . "<br>" . $conn->error;
    }

    $conn->close();
    exit();
?>