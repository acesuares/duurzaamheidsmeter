<?php
	if ( $gemeentes[ $gemeente->gemeente ]->score_procent < 0 ):	
		$out .= "<DIV ID=\"vl-title\">";
		$out .= "$gemeente->gemeente " . $strings[ "deed niet mee met de  vragenlijst" ] . " '" . $indicators[ $onderwerp ]->title . "'";
		$out .= "</DIV>\n";
	else:
		$query  = "SELECT V.id AS vraag_id, ";
		$query .= " V.title_$lang AS vraag, V.score, G2V.id AS determinator, ";
		$query .= " @vraagnummer:=@vraagnummer+1 vraagnummer ";
		$query .= " FROM ( SELECT @vraagnummer:=0 ) VNUM, vraag V LEFT JOIN gemeente2vraag G2V ";
		$query .= " ON G2V.vraag_id=V.id AND G2V.gemeente_id='$gemeente->id' ";
		$query .= " WHERE ";
		$query .= " V.active='Y' ";
		$query .= " AND V.indicator_id = '{$indicators[ $onderwerp ]->id}' ";
		$query .= " ORDER BY V.name ";
		$result = mysql_query( $query );
		
		$totaal_score = 0;
		
		$out .= "<H2 ID=\"vl-title\">";
		$out .= $indicators[ $onderwerp ]->title . " - " . $strings[ "vragenlijst gemeente" ] . " $gemeente->gemeente";
		$out .= "</H2>\n";
		
		$out .= "<DIV ID=\"vragenlijst\">\n";
		$out .= "<TABLE>\n";
		$out .= "<TR>\n";
		$out .= "<TD ID=\"vl-header-vraagnummer\">" . $strings[ "nr." ] . "</strong></TD>\n";
		$out .= "<TD ID=\"vl-header-vraag\">" . $strings[ "vraag" ] . "</TD>\n";
		$out .= "<TD ID=\"vl-header-jaofnee\">" . $strings[ "antwoord" ] . "</TD>\n";
		$out .= "<TD ID=\"vl-header-score\">" . $strings[ "score" ] . "</TD>\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "</TR>\n";
		while ( $row = mysql_fetch_object( $result ) ):
			$out .= "<TR>\n";
			$out .= "<TD ID=\"vl-vraagnummer\">$row->vraagnummer.</TD>\n";
			$out .= "<TD ID=\"vl-vraag\">";
  			$out .= "<A HREF=\"{$settings->site_url}/$year/$lang/$onderwerp/{$strings[ 'antwoorden' ]}/{$row->vraagnummer}/$search\">";
			$out .= $row->vraag;
			$out .= "</A></TD>\n";
			$out .= "<TD ID=\"vl-jaofnee\">";
			if ( $row->determinator ):
				$out .= "ja";
				$totaal_score += $row->score;
			else:
				$out .=  "nee";
			endif;
			$out .= "</TD>\n";
			$out .= "<TD ID=\"vl-score\">";
			if ( $row->determinator ):
				$out .= $row->score;
			else:
				$out .=  "&nbsp;";
			endif;
			$out .= "</TD>\n";
			$out .= "<TD>&nbsp;</TD>\n";
			$out .= "</TR>\n";
		endwhile;
	// TOTAALSCORE 
		$out .= "<TR ID=\"vl-totaalscore-row\">\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "<TD ID=\"vl-totaalscore-tekst\">Totaalscore</TD>\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "<TD ID=\"vl-totaalscore-score\">$totaal_score</TD>\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "</TR>\n";
	// Correctie
		if ( $correctie->correctiefactor <> 0 ):
			//factor
			$out .= "<TR ID=\"vl-cfactor-row\">\n";
			$out .= "<TD>&nbsp;</TD>\n";
			$out .= "<TD ID=\"vl-cfactor-tekst\">Correctiefactor</TD>\n";
			$out .= "<TD>&nbsp;</TD>\n";
			$out .= "<TD ID=\"vl-cfactor-score\">$correctie->correctiefactor</TD>\n";
			$out .= "<TD>&nbsp;</TD>\n";
			$out .= "</TR>\n";
			// gecorrigeerd
			$out .= "<TR ID=\"vl-cfactor-totaal-row\">\n";
			$out .= "<TD>&nbsp;</TD>\n";
			$out .= "<TD ID=\"vl-cfactor-totaal-tekst\">Gecorrigeerde Totaalscore</TD>\n";
			$out .= "<TD>&nbsp;</TD>\n";
			$out .= "<TD ID=\"vl-cfactor-totaal-score\">" . ( $totaal_score + $correctie->correctiefactor ) . "</TD>\n";
			$out .= "<TD>&nbsp;</TD>\n";
			$out .= "</TR>\n";
		endif;
		// gecorrigeerd in procenten
		$out .= "<TR ID=\"vl-gecorr-totaal-row\">\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "<TD ID=\"vl-gecorr-totaal-tekst\"><A HREF=\"javascript:void(0)\">Totaalscore (inclusief eventuele correcties) in procenten<SPAN>&nbsp;&nbsp;In principe kan een gemeente op vrijwel alle vragen 'ja' antwoorden. 
Voor elk positief antwoord zijn punten te behalen. De totaalscore (in 
procenten) geeft aan hoeveel procent positief beantwoord is. Uiteraard 
moet dit beeld gerelativeerd worden. We kunnen veel vragen niet stellen. 
Maar het resultaat is een goede indicatie van de prestatie van de gemeente.
</SPAN></A></TD>\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "<TD ID=\"vl-gecorr-totaal-score\">";
		$out .= round ( ( $totaal_score + $correctie->correctiefactor ) / $indicators[ $onderwerp ]->max_score * 100 );
		$out .= "</TD>\n";
		$out .= "<TD ID=\"vl-gecorr-totaal-procentteken\">&nbsp;%</TD>\n";
		$out .= "</TR>\n";
		// reden
		if ( $correctie->correctiereden <> "" ):
			$out .= "<TR ID=\"vl-creden-row\">\n";
			$out .= "<TD>&nbsp;</TD>\n";
			$out .= "<TD COLSPAN=4 ID=\"vl-creden-tekst\">" . $strings[ "Reden voor correctie" ] . ": $correctie->correctiereden</TD>\n";
			$out .= "</TR>\n";
		endif;
		$out .= "</TABLE>\n";
		$out .= "</DIV>\n";
	endif;
?>