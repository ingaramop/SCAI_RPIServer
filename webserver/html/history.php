<!DOCTYPE html>
<html>
<body>

<?php

	echo '<div align="right">';
	echo "<a href=index.html> <<< Home  </a><br />";
	echo "<a href=history.php> <<< Latest Entry </a>";
	echo '</div>';

	//Check for valid rows_per_page parameter or assign a static value
	$rows_per_page = $_GET['rows_per_page'];

	if (isset($rows_per_page)) {
		if (ctype_digit( $rows_per_page )==false){
			 $rows_per_page = 10;
		}
	}
	else{
		$rows_per_page = 10;
	}
	echo "rows per page: $rows_per_page ";

	//Check for valid start_index parameter or assign a static value
	$start_index = $_GET['start_index'];

	if (isset($start_index)) {
		if (ctype_digit( $start_index )==false){
			 $start_index = 0;
			if($start_index < 0){
				$start_index =0;		
			}
		}
	}
	else{
		$start_index = 0;
	}
	echo "start index: $start_index ";

	//start table
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
			<th>image</th>
			</tr>";

	class TableRows extends RecursiveIteratorIterator { 
		 function __construct($it) { 
		     parent::__construct($it, self::LEAVES_ONLY); 
		 }

		 function current() {
		     return "<td style='width: 150px; border: 1px solid black;'>" . parent::current(). "</td>";
		 }

		 function beginChildren() { 
		     echo "<tr>"; 
		 } 

		 function endChildren() { 
		     echo "</tr>" . "\n";
		 } 
	} 

	$servername = "localhost";
	$username = "pi";
	$password = "raspberry";
	$dbname = "scai";

	try {
		$conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
		$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
		$link1 = "<a href=image.php?index=";
		$link2 = "> link </a>";
		$stmt = $conn->prepare("select id,date,tipperInclination,sideInclination ,compass,temperature,pressure,positionX,positionY,timestamp, CONCAT('$link1', id, '$link2') as link from history  order by id desc limit $start_index, $rows_per_page "); 
		$stmt->execute();

		 // set the resulting array to associative
		 $result = $stmt->setFetchMode(PDO::FETCH_ASSOC);

		 foreach(new TableRows(new RecursiveArrayIterator($stmt->fetchAll())) as $k=>$v) { 
		     echo $v;
		 }
		
	}
	catch(PDOException $e) {
		 echo "Error: " . $e->getMessage();
	}
	$conn = null;
	echo "</table>";

	//$stmt->close();
	//$conn->close();

	if($start_index>0){
		$aux=$start_index-$rows_per_page;
		echo "<a href=history.php?start_index=$aux&rows_per_page=$rows_per_page> <<< back   </a>";
	}
	$aux2=$start_index+$rows_per_page;
	echo"&nbsp; - &nbsp;";
	echo "<a href=history.php?start_index=$aux2&rows_per_page=$rows_per_page>   next >>></a>";
	
?>  

</body>
</html>
