<?php
// blah
	$query = "SELECT sum(V.score) AS score,
       CF.correctie_factor AS correctiefactor,
       CF.correctie_reden AS correctiereden,
			 kl_datafromyear_id + 2001 AS kl_datafromyear, 
			 so_datafromyear_id + 2001 AS so_datafromyear,
			 rg_datafromyear_id + 2001 AS rg_datafromyear,
			 di_datafromyear_id + 2001 AS di_datafromyear,
			 di_datafromyear_id + 2001 AS ka_datafromyear,
       I.code,
       I.code_$lang AS code_lang
       FROM gemeente2vraag G2V,
       vraag V, indicator I,
       gemeente G,
       correctie CF
       WHERE G.id='$gemeente->id'
       AND V.active='Y'
       AND V.indicator_id = I.id
       AND V.indicator_id <> 8 
       AND G2V.vraag_id = V.id 
       AND G2V.gemeente_id = '$gemeente->id'
       AND CF.gemeente_id = '$gemeente->id'
       AND CF.indicator_id=I.id
       GROUP BY I.id
       ORDER BY I.name ";
  	$result = mysql_query( $query );
  	if ( mysql_num_rows( $result ) > 0 ):
		$out .= "<DIV ID=\"score\">\n";
  		$out .= "<TABLE>\n";
		$out .= "<TR ID=\"score-header-row\">\n";
		$out .= "<TD ID=\"score-header-indicator\">" . $strings[ "indicator" ] . "</TD>\n";
		$out .= "<TD ID=\"score-header-indicator\">" . $strings[ "datafromyear" ] . "</TD>\n";
		$out .= "<TD ID=\"score-header-score\">" . $strings[ "score" ] . "</TD>\n";
		$out .= "<TD ID=\"score-header-cfactor\">" . $strings[ "correctiefactor" ] . "</TD>\n";
		$out .= "<TD ID=\"score-header-scoreprocent\">" . $strings[ "score" ] . " (%)</TD>\n";
		$out .= "</TR>\n";
  		while( $row = mysql_fetch_object( $result ) ){
  			// save correctiefactor and _reden
  			if ( $row->code_lang == $onderwerp ):
  				$correctie = $row;
  			endif;
  			$out .= "<TR ID=\"score-row\">\n";
  			$out .= "<TD ID=\"score-indicator\">";
  			$out .= "<A HREF=\"{$settings->site_url}/$year/$lang/$row->code_lang/$view/$search\">";
  			$out .= $indicators[ $row->code_lang ]->title;
  			$out .= "</A>";
  			$out .= "</TD>\n";
  			$out .= "<TD ID=\"score-indicator\">";
				$itchy = $row->code . "_datafromyear";
  			$out .= $row->$itchy;
  			$out .= "</TD>\n";
  			$out .= "<TD ID=\"score-score\">";
  			$out .= $row->score;
  			$out .= "</TD>";
  			$out .= "<TD ID=\"score-cfactor\">";
  			$out .= $row->correctiefactor;
  			$out .= "</TD>\n";
  			$out .= "<TD ID=\"score-scoreprocent\">";
  			$out .= round ( ( $row->score + $row->correctiefactor ) / $indicators[ $row->code_lang ]->max_score * 100 ) . " %";
  			$out .= "</TD>\n";
  			$out .= "</TR>\n";
  		}
  		$out .= "</TABLE>\n";
		$out .= "</DIV>\n";
	endif;
?>