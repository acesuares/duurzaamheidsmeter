<?
// put in array G2V the answered questions for this_indicator

	$G2V = array();

	$query  = "SELECT G2V.gemeente_id, V.id as vraag_id ";
	$query .= " FROM gemeente2vraag G2V, vraag V ";
	$query .= " WHERE G2V.vraag_id = V.id ";
	if ($this_indicator_id != 0):
		$query .= " AND V.indicator_id='$this_indicator_id' ";
	endif;
	if ($this_gemeente_id != 0):
		$query .= " AND G2V.gemeente_id='$this_gemeente_id'";
	endif;

	$query .= " ORDER BY gemeente_id ";

	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_array($result)):
		$vraag_id = $row["vraag_id"];
		$gemeente_id = $row["gemeente_id"];
		$G2V[$gemeente_id][] = $vraag_id;
	endwhile;
?>
