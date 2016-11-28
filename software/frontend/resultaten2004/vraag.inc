<?

// get aantal gemeentes dat aan deze indicator meedeed (totaal)

$query  = "SELECT  ";
$query .= " count(DISTINCT G2V.gemeente_id) AS aantal_gemeentes ";

$query .= " FROM gemeente2vraag G2V, vraag V ";

$query .= " WHERE ";
$query .= "  G2V.vraag_id = V.id AND ";
$query .= "  V.indicator_id = '$indicator_id' ";

$query .= " GROUP BY V.indicator_id ";

$result=mysql($WebEngine_database,$query);

$row = mysql_fetch_assoc($result);
$aantal_gemeentes_totaal = 0 + $row["aantal_gemeentes"];


// get aantal gemeentes dat aan deze indicator meedeed (alleen 2005)

$query  = "SELECT  ";
$query .= " count(DISTINCT G2V.gemeente_id) AS aantal_gemeentes ";

$query .= " FROM gemeente2vraag G2V, vraag V, gemeente G ";

$query .= " WHERE ";
$query .= "  G2V.vraag_id = V.id AND ";
$query .= "  G2V.gemeente_id = G.id AND ";
$query .= "  V.indicator_id = '$indicator_id' AND ";
$query .= "  G." . $indicator_code . "_datafromyear_id=$current_datafromyear_id ";
$query .= " GROUP BY V.indicator_id ";

$result=mysql($WebEngine_database,$query);

$row = mysql_fetch_assoc($result);
$aantal_gemeentes_2005 = 0 + $row["aantal_gemeentes"];


$html_out .= "<TABLE BGCOLOR=\"#000000\" WIDTH=780 BORDER=0 CELLPADDING=5 CELLSPACING=1>";


		$html_out .= "<TR HEIGHT=\"28\">";
		$html_out .= "<TD ALIGN=LEFT VALIGN=TOP";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= "<FONT SIZE=2>";
		$html_out .= "<B>Indicator:</B>";
		$html_out .= "</FONT>";
		$html_out .= "</TD>";
		$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=TOP";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= "<FONT SIZE=2>";
		$html_out .= "<B>" . $this_indicator->title . "</B>";
		$html_out .= "</FONT>";
		$html_out .= "</TD>";
		$html_out .= "</TR>\n";

		$html_out .= "<TR HEIGHT=\"28\">";
		$html_out .= "<TD ALIGN=LEFT VALIGN=TOP";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= "<FONT SIZE=2>";
		$html_out .= "<B>Vraag:</B>";
		$html_out .= "</FONT>";
		$html_out .= "</TD>";
		$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=TOP";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= "<FONT SIZE=2>";
		$html_out .= "<B>" . $this_vraag->title . "</B>";
		$html_out .= "</FONT>";
		$html_out .= "</TD>";
		$html_out .= "</TR>\n";

		if ( $vraag_id != $doet_u_mee_2005_id && $vraag_id != $doet_u_mee_2003_id):
			$html_out .= "<TR HEIGHT=\"28\">";
			$html_out .= "<TD ALIGN=LEFT VALIGN=TOP";
			$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
			$html_out .= "&nbsp;";
			$html_out .= "</TD>";
			$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=TOP";
			$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
			$html_out .= $sorteerbalk_fontface[$spiegel];
			$html_out .= $sorteerbalk_fontcolor[$spiegel];
			$html_out .= "<FONT SIZE=2>";
			if ( $this_vraag->questionfromyear_id == $current_datafromyear_id ):
				$html_out .= "<B>Deze vraag werd alleen in 2005 gesteld. Er deden $aantal_gemeentes_2005 gemeentes mee.</B>";
			else:
				$html_out .= "<B>Deze vraag werd zowel in 2003 als in 2005 gesteld. Er deden $aantal_gemeentes_totaal gemeentes mee.</B>";
			endif;
			$html_out .= "</FONT>";
			$html_out .= "</TD>";
	
			$html_out .= "</TR>\n";
		endif;

$query  = "SELECT ";
$query .= " count(gemeente_id) AS aantal_ja ";
$query .= " FROM gemeente2vraag G2V ";

$query .= " WHERE ";
$query .= "  G2V.vraag_id = '$vraag_id' ";

$result=mysql($WebEngine_database,$query);
$row = mysql_fetch_assoc($result);

$aantal_gemeentes = $aantal_gemeentes_totaal;
if ( $this_vraag->questionfromyear_id == $current_datafromyear_id ):
	$aantal_gemeentes = $aantal_gemeentes_2005;
endif;
	
$procent_ja = round( ( $row[ "aantal_ja" ] / $aantal_gemeentes ) * 100, 0 );
$procent_nee = 100 - $procent_ja;

$regel_bgcolor = "FFFFFF";

	$html_out .= "<TR>";

	$html_out .= "<TD ALIGN=LEFT VALIGN=TOP";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];

	$html_out .= "<B>Score:</B>";

	$html_out .= "</TD>\n";

	$html_out .= "<TD WIDTH=\"70%\"";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "&nbsp;";
	$html_out .= "</TD>";

	$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>ja</B>";
	$html_out .= "</TD>";

	$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>nee</B>";
	$html_out .= "</TD>";

	$html_out .= "</TR>";
		
	$html_out .= "<TR>";

	$html_out .= "<TD COLSPAN=2";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $t_fontface[$spiegel];
	$html_out .= $t_fontcolor[$spiegel];
	$html_out .= $t_fontsize[$spiegel];

	$html_out .= "&nbsp;";

	$html_out .= "</TD>\n";

	$html_out .= "<TD ALIGN=RIGHT VALIGN=TOP";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $t_fontface[$spiegel];
	$html_out .= $t_fontcolor[$spiegel];
	$html_out .= $t_fontsize[$spiegel];

	$html_out .= "<B>$procent_ja %</B>";

	$html_out .= "</TD>\n";

	$html_out .= "<TD ALIGN=RIGHT VALIGN=TOP";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $t_fontface[$spiegel];
	$html_out .= $t_fontcolor[$spiegel];
	$html_out .= $t_fontsize[$spiegel];

	$html_out .= "$procent_nee %";

	$html_out .= "</TD>\n";


	$html_out .= "</TR>\n";


// spacer

$html_out .= "<TR HEIGHT=\"28\">";

$html_out .= "<TD COLSPAN=4 ALIGN=LEFT VALIGN=CENTER";
$html_out .= " BGCOLOR=\"#FFFFFF\">";
$html_out .= "&nbsp;";
$html_out .= "</TD>";

$html_out .= "</TR>\n";

// helptext
	$html_out .= "<TR>";

	$html_out .= "<TD ALIGN=LEFT VALIGN=TOP COLSPAN=4";
	$html_out .= " BGCOLOR=\"#FFFFFF\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= "<FONT COLOR=\"#000000\" SIZE=\"2\"><B>";
	$html_out .= $this_vraag->helptext;
	$html_out .= "</B></FONT>";

	$html_out .= "</TD>\n";


	$html_out .= "</TR>";


$html_out .= "</TABLE>\n";

?>
