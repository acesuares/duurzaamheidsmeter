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
// indicators and onderwerpen (we need this in many places so let's retrieve them only once.)
	$indicators = array();
	$onderwerpen = array();
	// introductie
		$onderwerpen[] = $strings[ "introductie" ];
		$indicators[ $strings[ "introductie" ] ]->title = $strings[ "introductie" ];
		$indicators[ $strings[ "introductie" ] ]->code_lang = $strings[ "introductie" ];
		$indicators[ $strings[ "introductie" ] ]->code_en = "introduction";
		$indicators[ $strings[ "introductie" ] ]->code_nl = "introductie";
	// the rest
		$query  = "SELECT ";
		$query .= " I.id, I.title_$lang AS title, I.code, ";
		$query .= " I.code_$lang AS code_lang, ";
		$query .= " I.code_nl, I.code_en, maxcorr, ";
		$query .= " I.introductie_$lang AS introductie, ";
		$query .= " sum(IF( V.telt_mee_totaalscore = 'Y', V.score, 0 ) ) AS max_score ";
		$query .= " FROM ";
		$query .= " vraag V, indicator I ";
		$query .= " WHERE ";
		$query .= " V.active='Y' AND ";
		$query .= " I.active='Y' AND ";
		$query .= " V.indicator_id = I.id ";
		$query .= " AND I.id <> 8 "; // HOME
		$query .= " AND I.id <> 15 "; // TOTAALSCORES
		$query .= " GROUP BY V.indicator_id";
		$query .= " ORDER BY I.name";
		$result = mysql_query( $query );
		while( $row = mysql_fetch_object( $result ) ){
			$indicators[ $row->code_lang ] = $row;
			$onderwerpen[] = $row->code_lang;
		}
		/* echo "<XMP>";
		print_r($indicators);
		echo "</XMP>";
		*/
	// totaalscores
		$onderwerpen[] = $strings[ "totaalscores" ];
		$indicators[ $strings[ "totaalscores" ] ]->title = $strings[ "totaalscores" ];
		$indicators[ $strings[ "totaalscores" ] ]->code_lang = $strings[ "totaalscores" ];
		$indicators[ $strings[ "totaalscores" ] ]->code_en = "totalscores";
		$indicators[ $strings[ "totaalscores" ] ]->code_nl = "totaalscores";
	// default onderwerp
		$default_onderwerp = $strings[ "introductie" ];
?>