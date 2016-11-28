<? 
	include( "000_init.inc" );
// the URL how to get here is controlled by rewrite rules in apache.
// we don't check every possibility here because of that.
// parse uri
	$parm = explode( "/", $_SERVER['REQUEST_URI'], 5 );
	// 0 = /, 1 = resultaten,  we don't need them
	$year = $parm[ 2 ];
// sanitize year
	if ( $year == "" ) { $year = $default_year; }
	if ( in_array( $year, $years ) ):
		$query  = "SELECT content ";
		$query .= " FROM css ";
		$query .= " WHERE name='$year' ";
		$result = mysql_query( $query );
		$row = mysql_fetch_object( $result );
		echo $row->content;
	endif;
?>