<?

// gementes

	$query  = "SELECT ";
	$query .= " id,name ";
	$query .= "FROM ";
	$query .= " gemeente ";
	$query .= "ORDER BY ";
	$query .= " name ";

	$result=mysql($WebEngine_database,$query);

	echo "<SELECT NAME=this_gemeente_id SIZE=1>\n";
	echo "<OPTION VALUE=0>alle gemeenten";

	while($row = mysql_fetch_assoc($result)):
		$id=$row["id"];
		$name=stripslashes($row["name"]);

		echo "<OPTION VALUE=\"$id\" ";
		echo ">$name\n";

	endwhile;

	echo "</SELECT>\n";

	// indicatoren

	$query  = "SELECT ";
	$query .= " id,title AS name ";
	$query .= "FROM ";
	$query .= " indicator ";
	$query .= "WHERE ";
	$query .= " id <> 8  "; // home
	$query .= "ORDER BY ";
	$query .= " name ";

	$result=mysql($WebEngine_database,$query);

	echo "<SELECT NAME=indicator SIZE=1>\n";
	while($row = mysql_fetch_assoc($result)):
		$id=$row["id"];
		$name=strtolower(stripslashes($row["name"]));

		echo "<OPTION VALUE=\"$id\" ";
		echo ">$name\n";

	endwhile;

	echo "</SELECT>\n";

?>
