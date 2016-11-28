<?php
		// get a list of all provincies, set score to -1. We also need this to make the maps.
			$query  = "SELECT ";
			$query .= " P.id, ";
			$query .= " P.name,  P.inwoneraantal,";
			$query .= " P.adres, P.postcode,";
			$query .= " P.plaats, P.tel,";
			$query .= " P.fax, P.email,";
			$query .= " P.website, P.description,";
			$query .= " -1 AS score_procent ";
			$query .= "FROM ";
			$query .= " provincie P ";
			$query .= " ORDER BY P.name ";
			$result = mysql_query( $query );
		// put the results in an array
			$provincies = array();
			while ( $row = mysql_fetch_object( $result ) ) {
				$provincies[ $row->name ] = $row;
			}
		// average the scores of the gemeentes
			foreach ( $gemeentes as $gemeente_name=>$gemeente ):
				$provincies[ $gemeente->provincie ]->aantal_gemeentes++;
				$provincies[ $gemeente->provincie ]->{$gemeente->antwoord}++;
				if ( $gemeente->score_procent >= 0 ):
					$provincies[ $gemeente->provincie ]->aantal_gemeentes_dat_meedoet++;
					$provincies[ $gemeente->provincie ]->score_procent += $gemeente->score_procent;
				endif;
			endforeach;
			// calculate average
			foreach ( $provincies as $provincie_name=>$provincie ):
				if ( $provincie->aantal_gemeentes > 0 ):
					$provincies[ $provincie_name ]->procent_deelname= round( $provincie->aantal_gemeentes_dat_meedoet / $provincie->aantal_gemeentes * 100, 1);
				else:
					$provincies[ $provincie_name ]->procent_deelname = 0;
				endif;
				if ( $provincie->score_procent != -1 ):
					$provincies[ $provincie_name ]->score_procent = round( $provincie->score_procent / $provincie->aantal_gemeentes_dat_meedoet, 1 );
				endif;
				if ( $provincies[ $provincie_name ]->procent_deelname > 0 ):
					$provincies[ $provincie_name ]->gewogen_score_procent = round( $provincies[ $provincie_name ]->score_procent * ( $provincies[ $provincie_name ]->procent_deelname / 100 ), 1 );
				endif;
			endforeach;
?>