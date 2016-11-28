<?
	// make an array vragen[vraag_id]=vraag.naam

	$vragen = array();
	$query  = "SELECT V.id, V.name, V.score,
							V.aantal_ja_2002, V.aantal_ja_2003, V.questionfromyear_id + 2000 as questionfromyear, V.reference_question_2003, V.reference_question_2005,
							I.name as indicator ";
	$query .= " FROM vraag V, indicator I ";
	$query .= " WHERE V.indicator_id=I.id ";
	if ($this_indicator_id != 0):
		$query .= " AND V.indicator_id='$this_indicator_id'";
	endif;
	$query .= " AND V.active='Y' ";
	$query .= " ORDER BY V.name ";

	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object($result)):
		if ( $row->score == 0 ):
		  $row->score = -1;
		endif;
		$vragen[$row->id] = $row;
	endwhile;
?>
