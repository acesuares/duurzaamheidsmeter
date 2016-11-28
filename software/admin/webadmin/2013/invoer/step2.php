<?php
// licensed under GPL v2. (C) 2008 Ace Suares, Willemstad
	/*
		Copyright (C) 2008 Ace Suares, Gravenstraat 4, Otrobanda, Willemstad, Curaçao, Nehterlands Antilles
		http://www.suares.an
		
		This program is free software; you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation; either version 2 of the License, or
		(at your option) any later version.
		
		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.
		
		You should have received a copy of the GNU General Public License
		along with this program; if not, write to the Free Software
		Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	*/
// init
	include( "init.inc" );
// html header
	echo "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n
					<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n
					<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n
					<head>\n";
	echo "<title>{$settings->sitename} Stap 2</title>\n
           <link rel=\"stylesheet\" type=\"text/css\" href=\"{$settings->stylesheet_url}\">\n
           </head>\n";
// body
	echo "<body>\n";
// sanitize gemeente
	$gemeente_id = intval( $_POST[ "gemeente_id" ] );
	$query = "SELECT 	G.id, G.name AS gemeente,
										P.name AS provincie
						FROM 		gemeente G, provincie P 
						WHERE 	G.provincie_id = P.id
							AND 	G.id = '$gemeente_id' ";
	$result = mysql_query( $query );
	if ( mysql_num_rows( $result ) != 1 ):
		echo "Er is iets fout gegaan. Ga terug en probeer opnieuw";
		echo "</body>\n";
		echo "</html>\n";
		exit;
	endif;
// get the values for this gemeente
 	$row = mysql_fetch_object( $result );
	$gemeente = stripslashes( $row->gemeente );
	$provincie = stripslashes( $row->provincie );
// get the database code
	$databasecode = "";
	for ( $i = 1; $i <= 6; $i++ ) {
		$databasecode .= $_POST[ "databasecode$i" ];
	}
// header
	echo "<h1>STAP 2: Controleer de gegevens.</h1>\n";
	echo "<h2>" . strtoupper( "$gemeente ($provincie)" ) . "</h2>\n";
// form begin
	echo "<form action=\"step3.php\" method=\"post\">\n";
	echo "<input type=\"submit\">";
	echo "<input type=\"hidden\" name=\"gemeente_id\" value=\"$gemeente_id\">\n";
// antwoorden
	echo "<h3>V R A G E N &amp; A N T W O O R D E N</h3>\n";
	echo "<table>\n";
	$itchy = 0;
	$new_databasecode = "";
	foreach ( explode( ";", $databasecode ) as $key => $val) {
		list( $vraag_id, $answer ) = explode( "," , $val );
		if ( $vraag_id != "" ): // this is to avoid trailing ';' !!
			echo "<tr>\n";
			$vraag_id = intval( $vraag_id );
			$query = "SELECT 	LEFT( V.title_nl, 40 ) AS title,
												V.score,
												I.title_nl AS indicator
								FROM 		vraag V, indicator I 
								WHERE 	V.id = '$vraag_id' AND V.indicator_id = I.id ";
			$result = mysql_query( $query );
			if ( mysql_num_rows( $result ) != 1 ):
				echo "<td colspan=\"4\">Deze vraag bestaat niet. Die sla ik over...</td>\n";
			else:
				$new_databasecode .= strval( $vraag_id ); 
				$row = mysql_fetch_object( $result );
				$vraag_title = stripslashes( $row->title );
				$indicator_title = stripslashes( $row->indicator );
				$itchy++;
				echo "<td>$itchy</td>\n";
				echo "<td>$vraag_title</td>\n";
				if ( $answer == 'Y' ):
					echo "<td>ja</td>\n";
					$new_databasecode .= ",Y;";
				else:
					echo "<td>nee</td>\n";
					$new_databasecode .= ",N;";
				endif;
				echo "<td>$indicator_title</td>\n";
			endif;
			echo "<td>$vraag_id</td>\n";
			echo "</tr>\n";
		endif;
	}
	echo "</table>\n";
	echo "<input type=\"hidden\" name=\"databasecode\" value=\"$new_databasecode\">\n";
	echo "Nieuwe Databasecode, waar alle niet-bestaande vragen uit verdwenen zijn:<br />$new_databasecode<br /><br />";
	echo "<input type=\"submit\">";
	echo "</form>\n";
	echo "</body>\n";
	echo "</html>\n";
?>
