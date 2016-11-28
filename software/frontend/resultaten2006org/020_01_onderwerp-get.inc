<?php
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
		$query .= " GROUP BY V.indicator_id";
		$query .= " ORDER BY I.name";
		$result = mysql_query( $query );
		while( $row = mysql_fetch_object( $result ) ){
			$indicators[ $row->code_lang ] = $row;
			$onderwerpen[] = $row->code_lang;
		}
	// default onderwerp
		$default_onderwerp = $strings[ "introductie" ];
?>