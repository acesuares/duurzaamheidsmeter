<?
	$query  = " SELECT @vraagnummer:=@vraagnummer+1 vraagnummer,
							id, title_$lang AS title, score,
							IF( telt_mee_totaalscore = 'Y', score, 0 ) as score_voor_totaal,
							helptext_$lang AS helptext ";
	$query .= " FROM ( SELECT @vraagnummer:=0 ) VNUM, vraag V";
	$query .= " WHERE indicator_id = '" . $indicators[ $onderwerp ] ->id . "'";
	$query .= "  AND active = 'Y'";
	$query .= "  ORDER BY name";
	$result = mysql_query( $query );
	$vragen = array();
	while ( $row = mysql_fetch_object( $result ) ) {
		$vragen[ $row->vraagnummer ] = $row;
	}
/*	found the vraagnummer (vraagnummer) thingy here in the user comments,
	posted by Mark Malakanov on April 30 2006 3:42pm on this page:
	http://dev.mysql.com/doc/refman/5.0/en/user-variables.html
*/
?>