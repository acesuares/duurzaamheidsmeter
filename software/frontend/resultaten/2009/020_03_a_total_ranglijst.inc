<?php
// licensed under GPL v2. (C) 2008-2009 Ace Suares, Willemstad
	/*
		Copyright (C) 2008-2009 Ace Suares, Gravenstraat 4, Otrobanda, Willemstad, Curaçao, Nehterlands Antilles
		http://www.suares.com
		
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
// save current onderwerp
	$current_onderwerp = $onderwerp;
// initialize array
	$gemeentes = array();
// for each onderwerp, get the scores.	
	foreach ( $indicators as $onderwerp => $indicator ){
		if ( $onderwerp != $strings[ "introductie" ] && $onderwerp != "totaalscores" ): 
			// get a list of gemeentes that have scores 
				$query  = "SELECT ";
				$query .= " G.name, ";
				// sum of the scores + correctiefactor, divided by the max score of that onderwerp
				$query .= " ( ( (sum( V.score ) + max( CF.correctie_factor ) ) / {$indicators[ $onderwerp ]->max_score} ) * 100 ) AS score_procent ";
				
				$query .= "FROM ";
				$query .= " gemeente G, ";
				$query .= " gemeente2vraag G2V, ";
				$query .= " vraag V, ";
				$query .= " correctie CF ";
				
				$query .= " WHERE ";
				$query .= "  G.name NOT LIKE 'Amsterdam %' AND "; // get rid of stadsdelen
				$query .= "  G2V.gemeente_id = G.id AND ";
				$query .= "  G2V.vraag_id = V.id AND ";
				
				$query .= "  CF.gemeente_id = G.id AND ";
				$query .= "  CF.indicator_id = '" .  $indicators[ $onderwerp ]->id . "' AND ";
				
				$query .= "  V.active='Y' AND ";
				$query .= "  V.indicator_id = '" .  $indicators[ $onderwerp ]->id . "' "; 
				$query .= " GROUP BY G.id ";
				$result = mysql_query( $query );
			// put the results in an array
				while ( $row = mysql_fetch_object( $result ) ) {
					// correct some weird names
						if ( in_array( $row->name, array( "s-Gravendeel", "s-Gravenhage", "s-Hertogenbosch" ) ) ):
							$row->name = "'" . $row->name;
						endif;
						if ( $row->name == "Het Bildt" ):
							$row->name = "het Bildt";
						endif;
					// insert the score for this onderwerp and total the score
					$gemeentes[ $row->name ]->$onderwerp = $row->score_procent;
					$gemeentes[ $row->name ]->total_score += $row->score_procent;
				}
		endif;
	}
// sort on total_score
	$gemeentes_sorted = array();
	foreach( $gemeentes as $gemeente_name => $gemeente ){
		if ( $gemeente->total_score > 0 ):
			$gemeente->name = $gemeente_name;
			$gemeentes_sorted[ ( 1000 + round( $gemeente->total_score ) ) . $gemeente_name ] = $gemeente;
		endif;
	}
	krsort( $gemeentes_sorted );

/* debug
	echo "<XMP>";
	print_r( $gemeentes_sorted );
	echo "</XMP>";
	echo "<XMP>";
	print_r($gemeentes);
	echo "</XMP>";
	*/
// restore current onderwerp
	$onderwerp = $current_onderwerp;
?>