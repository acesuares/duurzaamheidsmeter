<?

// make an array gemeente[gemeente_id]=gemeente.naam
// and the other fileds.

	$gemeente = array();
	$provincie = array();

	$query  = "SELECT G.id as gemeente_id, G.name as gemeente_naam,";
	$query .= " P.name as provincie ";
	$query .= " FROM gemeente G, provincie P  ";
	$query .= " WHERE G.provincie_id = P.id ";

	if ($this_gemeente_id != 0):
		$query .= " AND G.id='$this_gemeente_id' ";
	endif;
	$query .= " ORDER BY G.name ";
	
 	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_array($result)):
		$gemeente_id = $row["gemeente_id"];

		$gemeente[$gemeente_id] = $row["gemeente_naam"];
		$provincie[$gemeente_id] = $row["provincie"];
	endwhile;

?>
