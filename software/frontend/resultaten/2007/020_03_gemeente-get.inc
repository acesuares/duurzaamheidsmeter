<?php
		// get a list of all gemeentes, set score to -1. We also need this to make the maps. We count the number of gemenetes in each province, too.
			$query  = "SELECT ";
			$query .= " G.id, ";
			$query .= " G.name,  G.inwoneraantal,";
			$query .= " -1 AS score_procent, ";
			$query .= " 'onbekend' AS antwoord, ";
			$query .= " P.name AS provincie ";
			$query .= "FROM ";
			$query .= " gemeente G, provincie P ";
			$query .= " WHERE G.name NOT LIKE 'Amsterdam %' "; // get rid of stadsdelen
			$query .= " AND P.id = G.provincie_id ";
			$query .= " ORDER BY G.name ";
			$result = mysql_query( $query );
		// put the results in an array
			$gemeentes = array();
			$gemeente_ids = array();
			while ( $row = mysql_fetch_object( $result ) ) {
			
				if ( in_array( $row->name, array( "s-Gravendeel", "s-Gravenhage", "s-Hertogenbosch" ) ) ):
					$row->name = "'" . $row->name;
				endif;
			
				$gemeentes[ $row->name ] = $row;
				$gemeente_ids[ $row->id ] = $row->name . " (" . $row->provincie . ")"; // for vragenlijst_verwerk
			}
		// get a list of gemeentes that have scores, overwriting some of the previous records... 
			$query  = "SELECT ";
			$query .= " G.name, ";
			$query .= " (((sum(V.score) + max(CF.correctie_factor))/" . $indicators[ $onderwerp ]->max_score . ")*100) AS score_procent, ";
			$query .= " 'nee' AS antwoord ";
			
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
			
				if ( in_array( $row->name, array( "s-Gravendeel", "s-Gravenhage", "s-Hertogenbosch" ) ) ):
					$row->name = "'" . $row->name;
				endif;
				if ( $row->name == "Het Bildt" ):
					$row->name = "het Bildt";
				endif;
			
				$gemeentes[ $row->name ]->score_procent = $row->score_procent;
				$gemeentes[ $row->name ]->antwoord = $row->antwoord;
			}
?>