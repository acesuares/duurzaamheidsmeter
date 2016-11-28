<?
// put in array G2V the answered questions for this_indicator

	$correctie = array();

	$query  = "SELECT C.* ";
	$query .= " FROM correctie C ";
	$query .= " WHERE 1 = 1 ";
	if ($this_indicator_id != 0):
		$query .= " AND C.indicator_id='$this_indicator_id' ";
	endif;
	if ($this_gemeente_id != 0):
		$query .= " AND C.gemeente_id='$this_gemeente_id'";
	endif;

	$query .= " ORDER BY C.gemeente_id ";

	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object($result)):
		$correctie_factor[$row->gemeente_id][$row->indicator_id] = $row->correctie_factor;
		$correctie_reden[$row->gemeente_id][$row->indicator_id] = $row->correctie_reden;
	endwhile;
?>
