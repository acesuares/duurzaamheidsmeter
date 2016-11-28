<?
	// make an array indicators

	$indicators = array();

	$query  = "SELECT I.id, I.title AS indicator ";
	$query .= " FROM indicator I ";
	$query .= " WHERE I.id <> 8 and id <> 15";
	$query .= " ORDER BY I.name ";

	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object($result)):
		$indicators[$row->id] = $row->indicator;
	endwhile;
?>
