<?php
// licensed under GPL v2. (C) 2008 Ace Suares, Willemstad
	/*
		Copyright (C) 2008 Ace Suares, Gravenstraat 4, Otrobanda, Willemstad, Curaçao, Nehterlands Antilles
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