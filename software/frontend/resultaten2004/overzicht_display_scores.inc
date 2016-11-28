<?
/*
select * from datafromyear;
+----+------+-------+
| id | name | title |
+----+------+-------+
|  1 | ---- | ----  |
|  2 | 2002 | 2002  |
|  3 | 2003 | 2003  |
|  4 | 2005 | 2005  |
+----+------+-------+
4 rows in set (0.00 sec)
*/

$current_datafromyear_id = 4;

// display scores (for provincie, indicator or overzichten)

// get max scores per indicator, as well as some gif's
	include("max_scores.inc");

//	sorteerbalk
	$html_out .= "<TABLE WIDTH=780 BORDER=0 CELLPADDING=0 CELLSPACING=0>";

	$html_out .= t_spacer(10,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(170,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(375,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(110,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(100,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= "<TR HEIGHT=\"28\">";

	$html_out .= t_spacer(10,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= "<TD WIDTH=170 ALIGN=LEFT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= "<A HREF=\"$PHP_SELF?spiegel=$spiegel&choice=$choice\" STYLE=\"text-decoration: none\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>Gemeente</B>";
	$html_out .= "<IMG SRC=/images/pijl.gif WIDTH=12 HEIGHT=7 ";
	$html_out .= "BORDER=0 HSPACE=10 VSPACE=2 ";
	$html_out .= "ALT=\"Sorteer op Gemeente\">";
	$html_out .= "</A>";
	$html_out .= "</TD>";

	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= "<TD ALIGN=CENTER VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= "<A HREF=\"$PHP_SELF?spiegel=$spiegel&choice=$choice&sortorder=totaalscore\" STYLE=\"text-decoration: none\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>Totaalscore</B>";
	$html_out .= "<IMG SRC=/images/pijl.gif WIDTH=12 HEIGHT=7 ";
	$html_out .= "BORDER=0 HSPACE=10 VSPACE=2 ";
	$html_out .= "ALT=\"Sorteer op Score\">";
	$html_out .= "</A>";
	$html_out .= "</TD>";

	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= "<TD WIDTH=100 ALIGN=LEFT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	if ( $spiegel != "provincie"):
		$html_out .= "<A HREF=\"$PHP_SELF?spiegel=$spiegel&choice=$choice&sortorder=provincie\" STYLE=\"text-decoration: none\">";
	endif;
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>Provincie</B>";
	if ( $spiegel != "provincie"):
		$html_out .= "<IMG SRC=/images/pijl.gif WIDTH=12 HEIGHT=7 ";
		$html_out .= "BORDER=0 HSPACE=10 VSPACE=2 ";
		$html_out .= "ALT=\"Sorteer op Provincie\">";
		$html_out .= "</A>";
	endif;
	$html_out .= "</TD>";

	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= "<TD WIDTH=100 ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= "<A HREF=\"$PHP_SELF?spiegel=$spiegel&choice=$choice&sortorder=inwoneraantal\" STYLE=\"text-decoration: none\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>Inwoners</B>";
	$html_out .= "<IMG SRC=/images/pijl.gif WIDTH=12 HEIGHT=7 ";
	$html_out .= "BORDER=0 HSPACE=10 VSPACE=2 ";
	$html_out .= "ALT=\"Sorteer op Inwoneraantal\">";
	$html_out .= "</A>";
	$html_out .= "</TD>";

	$html_out .= "</TR>";
	$html_out .= "</TABLE>";
//	einde sorteerbalk


//	init
	$flipcolor_counter = 0;
	$current_gemeente = -1;

// we've made a query looong ago... let's see some results now !
//echo $query;
	$result=mysql($WebEngine_database,$query);
//	echo mysql_errno() . ": " . mysql_error() . "\n";


/*
while($row = mysql_fetch_assoc($result)):

$stuff2[$row['gemeente_id']]=$row;
endwhile;
echo "<XMP>";
print_r($stuff2);
echo "</XMP>";	
*/

	while($row = mysql_fetch_assoc($result)):
		// get values
		foreach ($row as $key => $value){
			$$key=$value;
			// echo "$key -> $value<BR>";
		}

		$corrected_score = 0 + $score + $correctie_factor;
		if ( $corrected_score < 0 ) { $corrected_score=0; }

		if ($current_gemeente != $gemeente_id):
			if ($current_gemeente != -1 ):
				if ($aantal_ingevuld >= $aantal_in_te_vullen):

					$html_out .= $temp_html_out;

					include("overzicht_close_gemeente.inc");

				endif;
			endif;
			// nieuwe gemeente !

			$temp_html_out = "";
			$totaal_score_procent = 0;

			if ( ($flipcolor_counter % 2) == 0 ):
				$regel_bgcolor = "FFFFFF";
			else:
				$regel_bgcolor = $t_bgcolor[$spiegel];
			endif;

			$temp_html_out .= "<TABLE WIDTH=\"780\" BORDER=\"0\" CELLPADDING=\"0\" CELLSPACING=\"0\">";
			$temp_html_out .= "<TR HEIGHT=\"25\">";

			$temp_html_out .= t_spacer(10,$regel_bgcolor);

			$temp_html_out .= "<TD WIDTH=\"170\" ALIGN=\"LEFT\" VALIGN=\"CENTER\"";
			$temp_html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
			$temp_html_out .= "<A HREF=\"$PHP_SELF?spiegel=gemeente&choice=$gemeente_id\" STYLE=\"text-decoration: none\">";
			$temp_html_out .= $t_fontface[$spiegel];
			$temp_html_out .= $t_fontcolor[$spiegel];
			$temp_html_out .= $t_fontsize[$spiegel];
			$temp_html_out .= "<B>$gemeente</B>";
			$temp_html_out .= "</A>";
			$temp_html_out .= "</TD>";

			$temp_html_out .= t_spacer(5,$regel_bgcolor);

			$temp_html_out .= "<TD WIDTH=\"300\" ALIGN=\"LEFT\" VALIGN=\"CENTER\"";
			$temp_html_out .= " BGCOLOR=\"#$regel_bgcolor\">";

			// set the flag
			$current_gemeente = $gemeente_id;

			// save this province for closing it later
			$previous_provincie=$provincie;
			$previous_provincie_id=$provincie_id;
			$previous_inwoneraantal=$inwoneraantal;

			// new corrected_totaal_score
			$corrected_totaal_score = 0;
			$max_totaal_score = 0;
			// start counting how many vragenlijsten they joined
			$aantal_ingevuld = 0;
		endif;
			$previous_score_procent = 0;
			// determine the status of the indicator bar depending on the year.
			if ( ${$indicator_code . "_datafromyear_id"} < $current_datafromyear_id ):
				$score_procent = ${$indicator_code . "_procent_2003"};
				if ( $score_procent > 0 ):
					$previous_score_procent = $score_procent;
					$year_status = "uit";
					// now we also need to take the 2003 percentage in account.
					// we need to display one indicator_bar
					$temp_html_out .= "<A HREF=\"$PHP_SELF?spiegel=indicator&choice=$indicator_id\">";
					$temp_html_out .= "<IMG SRC=\"" . $score_gif[$indicator][$year_status] . "\" ";
					$temp_html_out .= " WIDTH=\"". $score_procent ."\" ";
					$temp_html_out .= " ALT=\"$indicator: " . sprintf( "%01.1f%%", $score_procent ) . " (gegevens uit 2003)\" ";
					$temp_html_out .= " TITLE=\"$indicator: " . sprintf( "%01.1f%%", $score_procent ) . " (gegevens uit 2003)\" ";
					$temp_html_out .= " HEIGHT=\"10\" BORDER=\"0\"></A>";
				endif;
			else:
				if ($corrected_score > 0):
					$corrected_totaal_score += $corrected_score;
					$max_totaal_score += $max_score[$indicator];
					$year_status = "aan";
					// we need to display one indicator_bar
					$score_procent = 	round (($corrected_score/$max_score[$indicator]) * 100,1);
					$temp_html_out .= "<A HREF=\"$PHP_SELF?spiegel=indicator&choice=$indicator_id\">";
					$temp_html_out .= "<IMG SRC=\"" . $score_gif[$indicator][$year_status] . "\" ";
					$temp_html_out .= " WIDTH=\"". $score_procent ."\" ";
					$temp_html_out .= " ALT=\"$indicator: score: $score;";
					$temp_html_out .= " correctie: $correctie_factor; eindscore: $corrected_score";
					$temp_html_out .= " ($score_procent% van " . $max_score[$indicator] . ")\" ";
					$temp_html_out .= " TITLE=\"$indicator: score: $score;";
					$temp_html_out .= " correctie: $correctie_factor; eindscore: $corrected_score";
					$temp_html_out .= " ($score_procent% van " . $max_score[$indicator] . ")\" ";
					$temp_html_out .= " HEIGHT=\"10\" BORDER=\"0\"></A>";
				endif;
			endif;
			$totaal_score_procent += $score_procent;
		$aantal_ingevuld++;
	endwhile;

	if ($aantal_ingevuld >= $aantal_in_te_vullen):

		$html_out .= $temp_html_out;
		include("overzicht_close_gemeente.inc");

	endif;
	
	$html_out .= "<BR><BR>";
	// we can drop the High Score table now.
	$query = "DROP TABLE IF EXISTS high_score";
	$result=mysql($WebEngine_database,$query);

?>