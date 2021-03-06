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
	echo "<title>{$settings->sitename} Stap 3</title>\n
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
// link to next gemeente
	echo "<br /><a href=\"index.php\">Volgende Gemeente</a><br />\n";
// header 
	echo "<h1>U hebt de volgende gegevens ingevoerd:</h1>\n";
	echo "<h2>" . strtoupper( "$gemeente ($provincie)" ) . "</h2>\n";
// correctiefactoren
	echo "<h3>C O R R E C T I E F A C T O R E N</h3>\n";
	echo "<table>\n";
	// get the maxcorr per indicator
		$query = "SELECT 	id, title_nl AS title,
											maxcorr 
							FROM 		indicator 
							WHERE 	id <> 8 and id <> 15
							ORDER BY name ASC";  // id=8 is home-button 15 = totaalscores!
		$indicator_result = mysql_query( $query );
	while( $indicator_row = mysql_fetch_object( $indicator_result ) ){
		$indicator_id = $indicator_row->id;
		$indicator_title = stripslashes( $indicator_row->title );
		// insert or replace the new values into the database
			$query = "REPLACE INTO correctie VALUES(";
			$query  .= "'".$corr_id[$indicator_id]."',";
			$query  .= "'".$gemeente_id."',";
			$query  .= "'".$indicator_id."',";
			$query  .= "'".$factor[$indicator_id]."',";
			$query  .= "'".$reden[$indicator_id]."')";
			$replace_result = mysql_query( $query );
		// get the correctiefactoren and redenen
			$query = "SELECT 	id, correctie_factor, correctie_reden 
								FROM 		correctie
								WHERE		gemeente_id = '$gemeente_id' 
									AND 	indicator_id = '$indicator_id' ";
			$correctie_result = mysql_query( $query );
			if ( mysql_num_rows( $correctie_result ) != 1 ):
				echo "<tr><td>Serieus Database Probleem. Bel of Mail Ace.</td></tr>\n";
				echo "</table>\n";
				echo "</body>\n";
				echo "</html>\n";
				exit;
			endif;
			$correctie_row = mysql_fetch_object( $correctie_result );
			$correctie_id = $correctie_row->id;
			$correctie_factor = $correctie_row->correctie_factor;
			$correctie_reden = stripslashes( $correctie_row->correctie_reden );
		// show them
			echo "<tr>\n";
			echo "<td>$indicator_title</td><td>$correctie_factor</td>";
			echo "</tr>\n";
			echo "<tr>\n";     
			echo "<td colspan=\"2\">$correctie_reden</td>\n";
			echo "</tr>\n";
	}
	echo "</table>\n";
// link to next gemeente
	echo "<br /><a href=\"index.php\">Volgende Gemeente</a><br />\n";
// end html
	echo "</body>\n";
	echo "</html>\n";
?>