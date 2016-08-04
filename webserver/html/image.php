<!DOCTYPE html>
<html>
<body>

<?php


	$index = $_GET['index'];

	$servername = "localhost";
	$username = "pi";
	$password = "raspberry";
	$dbname = "scai";
	
	echo '<div align="right">';
		echo "<a href=index.html> <<< Home  </a><br />";
		echo "<a href=history.php> <<< History </a>";
	echo '</div>';

	echo '<center>';
		if($index>1){
		$aux=$index-1;
		echo "<a href=image.php?index=$aux&rows_per_page=$rows_per_page> <<< back   </a>";
		}
		$aux=$index+1;
		echo"&nbsp; - &nbsp;";
		echo "<a href=image.php?index=$aux&rows_per_page=$rows_per_page>   next >>></a>";
	echo '</center>';

		try {
		$conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
		$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		$stmt = $conn->prepare("SELECT id,date,tipperInclination,sideInclination ,compass,temperature,pressure,positionX,positionY,timestamp, image FROM history where id = $index"); 
		$stmt->execute();
		$result = $stmt->fetch(PDO::FETCH_OBJ);

	//echo '$result



	echo "<table style='border: solid 1px black;'>";
	  echo "<tr><th>id</th>
			<th>date</th>
			<th>tipperInclination</th>
			<th>sideInclination</th>
			<th>compass</th>
			<th>temperature</th>
			<th>pressure</th>
			<th>positionX</th>
			<th>positionY</th>
			<th>timestamp</th>
			</tr>";
		
		echo "<td style='width: 150px; border: 1px solid black;'>$result->id</td>";
		echo "<td style='width: 150px; border: 1px solid black;'>$result->date</td>";
		echo "<td style='width: 150px; border: 1px solid black;'>$result->tipperInclination</td>";
		echo "<td style='width: 150px; border: 1px solid black;'>$result->sideInclination</td>";
		echo "<td style='width: 150px; border: 1px solid black;'>$result->compass</td>";
		echo "<td style='width: 150px; border: 1px solid black;'>$result->temperature</td>";
		echo "<td style='width: 150px; border: 1px solid black;'>$result->pressure</td>";
		echo "<td style='width: 150px; border: 1px solid black;'>$result->positionX</td>";
		echo "<td style='width: 150px; border: 1px solid black;'>$result->positionY</td>";
		echo "<td style='width: 150px; border: 1px solid black;'>$result->timestamp</td>";
	echo "</table>";


	echo '<center><img src="data:image/jpeg;base64,'.base64_encode( $result->image ).'"/></center>';

	
    
	}
catch(PDOException $e) {
    echo "Error: " . $e->getMessage();
}
$conn = null;

//$stmt->close();
//$conn->close();
	
?>  

</body>
</html>
