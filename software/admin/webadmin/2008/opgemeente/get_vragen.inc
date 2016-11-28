<?
	// make an array vragen[vraag_id]=vraag.naam

	$vraag = array();
	$score = array();
	$query  = "SELECT V.id, V.name, V.score, I.name as indicator ";
	$query .= " FROM vraag V, indicator I ";
	$query .= " WHERE V.indicator_id=I.id ";
	if ($this_indicator_id != 0):
		$query .= " AND V.indicator_id='$this_indicator_id'";
	endif;
	$query .= " AND V.active='Y' ";
	$query .= " ORDER BY V.name ";

	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_array($result)):
		$vraag_id = $row["id"];
		$vraag[$vraag_id] = $row["name"];
		$score[$vraag_id] = $row["score"];
		$indicator[$vraag_id] = $row["indicator"];
		// for that one special question: does gemeente participate ?
		// we need this conversion
		if ( $score[$vraag_id] == 0 ):
			$score[$vraag_id] = -1;
		endif;
	endwhile;
?>
