<?php
		$out .= "<div id=\"vragenlijst\">\n";
		$out .= "De online in te vullen vragenlijsten voor 2006 
							zijn niet langer beschikbaar. Recente vragenlijsten 
							zijn beschikbaar via de <a href=\"http://www.duurzaamheidsmeter.nl\">
							<b>Hoofdsite</b></a> van de Duurzaamheidsmeter.";
		$out .= "<br /><br /><br /><br /></div>\n";
		/* 

		$query  = "SELECT V.id, ";
		$query .= " V.title_$lang AS vraag, V.score,
								IF( V.telt_mee_totaalscore = 'Y', V.score, 0 ) as score_voor_totaal,
								V.helptext_$lang AS helptext, ";
		$query .= " @vraagnummer:=@vraagnummer+1 vraagnummer ";
		$query .= " FROM ( SELECT @vraagnummer:=0 ) VNUM, vraag V ";
		$query .= " WHERE ";
		$query .= " V.active='Y' ";
		$query .= " AND V.indicator_id = '{$indicators[ $onderwerp ]->id}' ";
		$query .= " ORDER BY V.name ";
		$result = mysql_query( $query );
		
		$max_score = 0;
		
		//$out .= "<FORM METHOD=\"POST\" ACTION=\"{$settings->site_url}/$year/$lang/$onderwerp/$view/{$strings[ 'verwerk' ]}\">\n";
		$out .= "<TABLE>\n";
		$out .= "<TR>\n";
		$out .= "<TD ID=\"vl-vraagnummer\"><strong>" . $strings[ "nr." ] . "</strong></TD>\n";
		$out .= "<TD ID=\"vl-vraag\"><strong>" . $strings[ "vraag" ] . "</strong></TD>\n";
		$out .= "<TD ID=\"vl-score\"><strong>" . $strings[ "score" ] . "</strong></TD>\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "</TR>\n";
		while ( $row = mysql_fetch_object( $result ) ):
			$out .= "<TR>\n";
			$out .= "<TD ID=\"vl-vraagnummer\">$row->vraagnummer.</TD>\n";
			$out .= "<TD ID=\"vl-vraag\"><A HREF=\"javascript:void(0)\">$row->vraag<SPAN>&nbsp;&nbsp;$row->helptext</SPAN></A></TD>\n";
			$out .= "<TD ID=\"vl-score\">";
			$out .= $row->score;
			$max_score += $row->score_voor_totaal;
			$out .= "</TD>\n";
			$out .= "<TD ID=\"vl-ja\">";
			$out .= "<INPUT TYPE=\"RADIO\" NAME=\"vraag[{$row->id}]\" VALUE=\"Y\">ja\n";
			$out .= "</TD>\n";
			$out .= "<TD ID=\"vl-nee\">";
			$out .= "<INPUT TYPE=\"RADIO\" NAME=\"vraag[{$row->id}]\" CHECKED VALUE=\"N\">nee\n";
			$out .= "</TD>\n";
			$out .= "</TR>\n";
		endwhile;
	// MAX SCORE 
		$out .= "<TR>\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "<TD ID=\"vl-maxscore-tekst\">Maximum Score</TD>\n";
		$out .= "<TD ID=\"vl-maxscore-score\">$max_score</TD>\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "<TD>&nbsp;</TD>\n";
		$out .= "</TR>\n";
		$out .= "</TABLE>\n";
		$out .= "</DIV>\n";
	// invulformulier gemeente
		$out .= "<DIV ID=\"invul-area\">\n";
	// invul helptekst
		$out .= "<P ID=\"invul-help\">\n";
		$out .= "Als u de vragen hierboven hebt beantwoord, s.v.p ook de 
onderstaande gegevens invullen.";
		$out .= "</P>\n";
	// invul gemeente dropdown
		$out .= "<P ID=\"invul-dropdown\">\n";
		$out .= "<SELECT NAME=\"gemeente_id\" SIZE=1>\n";
		$out .= "<OPTION VALUE=\"0\">";
		$out .= $strings[ "kiesgemeente-invul" ];
		$out .= "</OPTION>\n";
		foreach( $gemeentes as $gemeente_name=>$gemeente ):
			$out .= "<OPTION VALUE=\"{$gemeente->id}\">$gemeente_name ({$gemeente->provincie})</OPTION>\n";
		endforeach;
		$out .= "</SELECT>\n";
		$out .= "</P>\n";
	// invuller gegevens
		$out .= "<P ID=\"invul-invuller-help\">\n";
		$out .= "Gegevens invuller:";
		$out .= "</P>\n";
		$out .= "<P ID=\"invul-invuller\">\n";
		$out .= "<TABLE>\n";
		$out .= "<TR>\n";
		$out .= "<TD ID=\"invul-header\">&nbsp;</TD>\n";
		$out .= "<TD ID=\"invul-inputtext\">";
		$out .= "<SELECT NAME=\"invuller_aanspreektitel\" SIZE=1>\n";
		$out .= "<OPTION VALUE=\"Dhr/Mevr\" SELECTED>Dhr/Mevr</OPTION>\n";
		$out .= "<OPTION VALUE=\"Dhr\">Dhr</OPTION>\n";
		$out .= "<OPTION VALUE=\"Mevr\">Mevr</OPTION>\n";
		$out .= "</SELECT>\n";
		$out .= "</TR>\n";
		foreach ( $invuller_formfields as $formfield_name => $formfield_description ){
			$out .= "<TR>\n";
			$out .= "<TD ID=\"invul-header\">$formfield_description</TD>\n";
			$out .= "<TD ID=\"invul-inputtext\">";
			$out .= "<INPUT TYPE=\"TEXT\" SIZE=\"60\" NAME=\"$formfield_name\"></TD>\n";
			$out .= "</TR>\n";
		}
		$out .= "</TABLE>\n";
		$out .= "</P>\n";
	// contactambtenaar
		$out .= "<P ID=\"invul-contactambtenaar-help\">\n";
		$out .= "Gegevens gemeentelijke contactpersoon (indien anders dan de invuller):";
		$out .= "</P>\n";
		$out .= "<P ID=\"invul-contactambtenaar\">\n";
		$out .= "<TABLE>\n";
		$out .= "<TR>\n";
		$out .= "<TD ID=\"invul-header\">Naam contactambtenaar</TD>\n";
		$out .= "<TD ID=\"invul-inputtext\">";
		$out .= "<INPUT TYPE=\"TEXT\" SIZE=\"60\" NAME=\"contact_ambtenaar_naam\"></TD>\n";
		$out .= "</TR>\n";
		$out .= "<TR>\n";
		$out .= "<TD ID=\"invul-header\">Telefoon contactambtenaar</TD>\n";
		$out .= "<TD ID=\"invul-inputtext\">";
		$out .= "<INPUT TYPE=\"TEXT\" SIZE=\"60\" NAME=\"contact_ambtenaar_tel\"></TD>\n";
		$out .= "</TR>\n";
		$out .= "</TABLE>\n";
		$out .= "</P>\n";
	// correctiegedoe
		$out .= "<P ID=\"invul-correctie-help\">\n";
		$out .= "Toe te passen correctie (zie handleiding):";
		$out .= "</P>\n";
		$out .= "<P ID=\"invul-correctiefactor\">\n";
		$out .= "<TABLE>\n";
		$out .= "<TR>\n";
		$out .= "<TD ID=\"invul-header\">Correctiefactor</TD>\n";
		$out .= "<TD ID=\"invul-inputtext\">";
		$out .= "<SELECT NAME=\"correctiefactor\">\n";
		for ( $itchy = -( $indicators[ $onderwerp ]->maxcorr ); $itchy <= $indicators[ $onderwerp ]->maxcorr; $itchy++ ){
			$out .= "<OPTION VALUE=\"$itchy\" ";
			if ( 0 == $itchy) { $out .= " SELECTED"; }
			$out .= ">$itchy</OPTION>\n";
		}
		$out .= "</SELECT> punten.</TD>\n";
		$out .= "</TR>\n";
		$out .= "<TR>\n";
		$out .= "<TD ID=\"invul-header\">Als u de score corrigeert, vul hier de reden in.</TD>\n";
		$out .= "<TD ID=\"invul-inputtext\">";
		$out .= "<TEXTAREA ROWS=20 COLS=60 NAME=\"correctiereden\"></TEXTAREA></TD>\n";
		$out .= "</TR>\n";
		$out .= "</TABLE>\n";
		$out .= "</P>\n";
	// submit
		$out .= "<P ID=\"invul-submit-help\">\n";
		$out .= "Heeft u alles ingevuld?
Wij adviseren u dit formulier te printen voor eigen gebruik.
Klik vervolgens op 'verzenden' om uw gegevens te versturen.";
		$out .= "</P>\n";
		$out .= "<P ID=\"invul-submit\">\n";
		$out .= "<INPUT TYPE=\"SUBMIT\" VALUE=\"verzenden\"></TD>\n";
		$out .= "</P>\n";
		
		$out .= "</DIV>\n";
		//$out .= "</FORM>\n";
*/
?>