<?

// Gemeente

// get the scores

$query  = "SELECT ";
$query .= "G.name AS gemeente,";
$query .= "G.inwoneraantal,";
$query .= "G.adres,";
$query .= "G.postcode,";
$query .= "G.plaats,";
$query .= "G.tel,";
$query .= "G.fax,";
$query .= "G.email,";
$query .= "G.website,";
$query .= "G.description,";
$query .= "IF(G.foto<>'','yes','no') AS foto, ";
$query .= " P.name AS provincie, ";
$query .= " P.id AS provincie_id,";
$query .= " I.title AS indicator,";
$query .= " I.id AS indicator_id,";
$query .= " sum(V.score) AS score, ";
$query .= " CF.correctie_factor AS correctiefactor ";

$query .= " FROM gemeente2vraag G2V, ";
$query .= " vraag V, indicator I, ";
$query .= " gemeente G, provincie P, ";
$query .= " correctie CF ";

$query .= " WHERE G.id='$choice' ";
$query .= " AND P.id = G.provincie_id ";
$query .= " AND V.active='Y' ";
$query .= " AND V.indicator_id = I.id ";
$query .= " AND V.indicator_id <> 8 "; // HOME
$query .= " AND G2V.vraag_id = V.id ";
$query .= " AND G2V.gemeente_id = '$choice' ";
$query .= " AND CF.gemeente_id = '$choice' ";
$query .= " AND CF.indicator_id=I.id ";

$query .= " GROUP BY I.id ";
$query .= " ORDER BY I.name ";

$result=mysql($WebEngine_database,$query);

	// check for existence
	if (mysql_num_rows($result) == 0):
		header("Location: $PHP_SELF?spiegel=overzicht&choice=totaaloverzicht");
		exit;
	endif;

// display_scores
// get max scores per indicator, as well as some gif's
	include("max_scores.inc");

// display scores (for gemeente)

	$html_out .= "<TABLE WIDTH=780 BORDER=0 CELLPADDING=0 CELLSPACING=0>";


// sorteerbalk (waar niks te sorteren valt, dus deze keer geen pijltjes !)
	$html_out .= "<TR HEIGHT=\"1\">";

	$html_out .= t_spacer(10,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(150,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(350,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(70,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(70,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(100,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= t_spacer(10,$sorteerbalk_bgcolor[$spiegel]);
	$html_out .= "</TR>";

	$html_out .= "<TR HEIGHT=\"28\">";

	$html_out .= t_spacer(10,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= "<TD WIDTH=150 ALIGN=LEFT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>Indicator</B>";
	$html_out .= "</TD>";

	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= t_spacer(350,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= "<TD WIDTH=70 ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>Score</B>";
	$html_out .= "</TD>";

	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= "<TD WIDTH=70 ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>Correctie</B>";
	$html_out .= "</TD>";

	$html_out .= t_spacer(5,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= "<TD WIDTH=100 ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>Score (%)</B>";
	$html_out .= "</TD>";

	$html_out .= t_spacer(10,$sorteerbalk_bgcolor[$spiegel]);

	$html_out .= "</TR>\n";
// einde sorteerbalk


	$img_bar_out = "";
	$totaal_score = 0 ;
	$totaal_correctie = 0 ;
	$totaal_score_procent = 0 ;
	$flipcolor_counter = 0;


	while ($row = mysql_fetch_assoc($result)):

		// get values
		foreach ($row as $key => $value){
			$$key=$value;
		}

		$corrected_score = 0 + $score + $correctiefactor;
		$totaal_score += $score;
		$totaal_correctie += $correctiefactor;
		$score_procent = 	round (($corrected_score/$max_score[$indicator]) * 100,1);

		if ( ($flipcolor_counter % 2) == 0 ):
			$regel_bgcolor = "FFFFFF";
		else:
			$regel_bgcolor = $t_bgcolor[$spiegel];
		endif;

		$html_out .= "<TR HEIGHT=\"25\">";

		$html_out .= t_spacer(10,$regel_bgcolor);

		$html_out .= "<TD WIDTH=150 ALIGN=LEFT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
		$html_out .= "<A HREF=\"$PHP_SELF?spiegel=indicator&choice=$indicator_id\" STYLE=\"text-decoration: none\">";
		$html_out .= $t_fontface[$spiegel];
		$html_out .= $t_fontcolor[$spiegel];
		$html_out .= $t_fontsize[$spiegel];
		$html_out .= $indicator;
		$html_out .= "</A>";
		$html_out .= "</TD>";

		$html_out .= t_spacer(5,$regel_bgcolor);

		$temp_img_out  = "";
		$temp_img_out .= "<A HREF=\"$PHP_SELF?spiegel=indicator&choice=$indicator_id\">";
		$temp_img_out .= "<IMG SRC=\"" . $score_gif[$indicator] . "\" ";
		$temp_img_out .= " WIDTH=\"". ($corrected_score * 2) ."\" ";
		$temp_img_out .= " ALT=\"$indicator: score: $score;";
		$temp_img_out .= " correctie: $correctiefactor; eindscore: $corrected_score";
		$temp_img_out .= " ($score_procent% van " . $max_score[$indicator] . ")\" ";
		$temp_img_out .= " HEIGHT=\"10\" BORDER=\"0\"></A>";

		$img_bar_out .= $temp_img_out;

		$html_out .= "<TD WIDTH=350 ALIGN=LEFT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
		$html_out .= $temp_img_out;
		$html_out .= "</TD>\n";

		$html_out .= t_spacer(5,$regel_bgcolor);

		$html_out .= "<TD WIDTH=70 ALIGN=RIGHT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
		$html_out .= $t_fontface[$spiegel];
		$html_out .= $t_fontcolor[$spiegel];
		$html_out .= $t_fontsize[$spiegel];
		$html_out .= $score;
		$html_out .= "</TD>";

		$html_out .= t_spacer(5,$regel_bgcolor);

		$html_out .= "<TD WIDTH=70 ALIGN=RIGHT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
		$html_out .= $t_fontface[$spiegel];
		$html_out .= $t_fontcolor[$spiegel];
		$html_out .= $t_fontsize[$spiegel];
		$html_out .= $correctiefactor;
		$html_out .= "</TD>";

		$html_out .= t_spacer(5,$regel_bgcolor);

		$html_out .= "<TD WIDTH=100 ALIGN=RIGHT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
		$html_out .= $t_fontface[$spiegel];
		$html_out .= $t_fontcolor[$spiegel];
		$html_out .= $t_fontsize[$spiegel];
		$html_out .= "$score_procent %";
		$html_out .= "</TD>";

		$html_out .= t_spacer(10,$regel_bgcolor);

		$html_out .= "</TR>\n";

		$flipcolor_counter++;

	endwhile;


	//TOTAAL SCORES
	$totaal_score_procent =	round ((($totaal_score + $totaal_correctie)/$totaal_max_score) * 100,1);

	$regel_bgcolor = $sorteerbalk_bgcolor[$spiegel];

	$html_out .= "<TR HEIGHT=\"25\">";

	$html_out .= t_spacer(10,$regel_bgcolor);

	$html_out .= "<TD WIDTH=150 ALIGN=LEFT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>TOTAAL</B>";
	$html_out .= "</TD>";

	$html_out .= t_spacer(5,$regel_bgcolor);

	$html_out .= "<TD WIDTH=350 ALIGN=LEFT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $img_bar_out;
	$html_out .= "</TD>\n";

	$html_out .= t_spacer(5,$regel_bgcolor);

	$html_out .= "<TD WIDTH=70 ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= $totaal_score;
	$html_out .= "</TD>";

	$html_out .= t_spacer(5,$regel_bgcolor);

	$html_out .= "<TD WIDTH=70 ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= $totaal_correctie;
	$html_out .= "</TD>";

	$html_out .= t_spacer(5,$regel_bgcolor);

	$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "$totaal_score_procent %";
	$html_out .= "</TD>";

	$html_out .= t_spacer(10,$regel_bgcolor);

	$html_out .= "</TR>\n";

	$html_out .= "<TR HEIGHT=\"28\">\n";
	$html_out .= "<TD COLSPAN=11 BGCOLOR=\"#FFFFFF\">";
	$html_out .= "<IMG SRC=/images/clear.gif WIDTH=10 HEIGHT=28 BORDER=0>";
	$html_out .= "</TD>";
	$html_out .= "</TR>\n";


	$html_out .= "</TABLE>\n";


// display_gegevens
	if ( $foto == "yes" ):
		$foto_html="<IMG SRC=\"graphic.php?table_name=gemeente&field_name=foto&id=$choice\" BORDER=\"0\" HSPACE=\"20\" ALIGN=\"RIGHT\" WIDTH=\"200\">";
	else:
		$foto_html="";
	endif;

	$html_out .= "<TABLE BGCOLOR=\"".$sorteerbalk_bgcolor[$spiegel]."\" WIDTH=780 BORDER=0 CELLSPACING=2 CELLPADDING=0>\n";
	$html_out .= "<TR><TD>\n";

	$html_out .= "<TABLE BGCOLOR=\"#FFFFFF\" WIDTH=776 BORDER=0 CELLSPACING=0 CELLPADDING=12>\n";
	$html_out .= "<TR><TD>\n";
	$html_out .= $t_fontface[$spiegel];
	$html_out .= $t_fontcolor[$spiegel];
	$html_out .= $t_fontsize[$spiegel];
	$html_out .= "$foto_html<B>Gemeente $gemeente</B>, provincie $provincie (". number_format ($inwoneraantal, 0, ",", ".")." inwoners)";
	$html_out .= "<BR>";
	if ($adres.$postocde.$plaats != ""):
		$html_out .= "adres: $adres, $postocde $plaats";
		$html_out .= "<BR>";
	endif;
	if ($tel != ""):
		$html_out .= "telefoonnummer: $tel";
		$html_out .= "<BR>";
	endif;
	if ($fax != ""):
		$html_out .= "faxnummer: $fax";
		$html_out .= "<BR>";
	endif;
	$html_out .= "<BR>";
	if ($email != ""):
		$html_out .= "e-mail: <A HREF=mailto:".$email.">".$email."</A>";
		$html_out .= "<BR>";
	endif;
	if ($website != ""):
		$html_out .= "website: <A HREF=\"".$website."\" TARGET=_web_$gemeente_id>".$website."</A>";
		$html_out .= "<BR>";
	endif;
	$html_out .= "<BR>";
	
	$html_out .= "<BR>";

	if ($description != ""):
		$html_out .= nl2br($description);
		$html_out .= "<BR>";
	endif;

	$html_out .= "</TD></TR>\n";
	$html_out .= "</TABLE>\n";

	$html_out .= "</TD></TR>\n";
	$html_out .= "</TABLE>\n";

	$html_out .= "<BR>\n";

// display_vragen
// get the vragen that are answered with yes

$query  = "SELECT ";
$query .= " vraag_id ";

$query .= " FROM gemeente2vraag ";

$query .= " WHERE ";
$query .= " gemeente_id = '$choice' ";

$query .= " ORDER BY vraag_id ";


$result=mysql($WebEngine_database,$query);

// check for existence
if (mysql_num_rows($result) == 0):
	header("Location: $PHP_SELF?spiegel=overzicht&choice=totaaloverzicht");
	exit;
endif;

// get the beantwoorde vragen id's in an array.
$beantwoorde_vragen = array();
while ($row = mysql_fetch_assoc($result)):
	$beantwoorde_vragen[] = $row["vraag_id"];
endwhile;

// get the indicatoren that they used

$query  = "SELECT DISTINCT";
$query .= " V.indicator_id ";

$query .= " FROM gemeente2vraag G2V, vraag V ";

$query .= " WHERE ";
$query .= " G2V.gemeente_id = '$choice' AND ";
$query .= " G2V.vraag_id = V.id ";


$query .= " ORDER BY V.indicator_id ";

$result=mysql($WebEngine_database,$query);

// get the indicator id's and put them in a mysql range
$gebruikte_indicatoren = "(";

while ($row = mysql_fetch_assoc($result)):
	$gebruikte_indicatoren .= "'".$row["indicator_id"]."',";
endwhile;

// strip the last ','
$gebruikte_indicatoren = substr($gebruikte_indicatoren,0,strlen($gebruikte_indicatoren)-1).")";


// get all vragen (within the used indicators!)

$query  = "SELECT ";
$query .= " V.id AS vraag_id, V.title AS vraag, V.score, ";
$query .= " I.id AS indicator_id, I.title as indicator, I.code as code ";

$query .= " FROM vraag V, indicator I ";

$query .= " WHERE ";
$query .= " V.active='Y' ";
$query .= " AND V.indicator_id = I.id ";
$query .= " AND V.indicator_id IN $gebruikte_indicatoren ";

$query .= " ORDER BY I.name, V.name ";


$html_out .= "<TABLE BGCOLOR=\"#000000\" WIDTH=780 BORDER=0 CELLPADDING=5 CELLSPACING=1>";


$html_out .= "<TR HEIGHT=\"2\">";

$html_out .= t_spacer(20,$sorteerbalk_bgcolor[$spiegel]);
$html_out .= t_spacer(470,$sorteerbalk_bgcolor[$spiegel]);
$html_out .= t_spacer(40,$sorteerbalk_bgcolor[$spiegel]);
$html_out .= t_spacer(40,$sorteerbalk_bgcolor[$spiegel]);
$html_out .= t_spacer(40,$sorteerbalk_bgcolor[$spiegel]);
$html_out .= "</TR>";

$vraagnummer = 0;
$current_indicator = -1;
$totaal_score = 0;

$result=mysql($WebEngine_database,$query);


while ($row = mysql_fetch_assoc($result)):

	// get values
	foreach ($row as $key => $value){
		$$key=$value;
	}

	if ( $current_indicator != $indicator_id ):
		if ( $current_indicator != -1 ):
			include("gemeente_close_indicator.inc");
		endif;
		// new indicator
			$html_out .= "<TR HEIGHT=\"28\">";
	
			$html_out .= t_spacer(20,$sorteerbalk_bgcolor[$spiegel]);
	
			$html_out .= "<TD ALIGN=LEFT VALIGN=CENTER";
			$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
			$html_out .= $sorteerbalk_fontface[$spiegel];
			$html_out .= $sorteerbalk_fontcolor[$spiegel];
			$html_out .= "<FONT SIZE=4>";
			$html_out .= "<B>".strtoupper($indicator)." - Vragenlijst gemeente $gemeente</B>";
			$html_out .= "</FONT>";
			$html_out .= "</TD>";
	
			$html_out .= t_spacer(40,$sorteerbalk_bgcolor[$spiegel]);
	
	
			$html_out .= t_spacer(40,$sorteerbalk_bgcolor[$spiegel]);
	
			$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
			$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
			$html_out .= $sorteerbalk_fontface[$spiegel];
			$html_out .= $sorteerbalk_fontcolor[$spiegel];
			$html_out .= $sorteerbalk_fontsize[$spiegel];
			$html_out .= "<B>score</B>";
			$html_out .= "</TD>";
	
			$html_out .= "</TR>\n";

		// invuller / contactpersoon
			$query  = "SELECT ";
			$query .= " CONCAT_WS(' ', A.name, ${code}_voornaam, ${code}_achternaam) AS volle_naam, ";
			$query .= " IF( ${code}_gemeenteadres='Y', ${code}_adres, adres) AS adres, ";
			$query .= " IF( ${code}_gemeenteadres='Y', CONCAT_WS(' ',${code}_postcode, ${code}_plaats), CONCAT_WS(' ',postcode, plaats) ) AS pcplaats, ";
			$query .= " ${code}_tel AS telefoonnummer, ";
			$query .= " ${code}_email AS email, ";
			$query .= " DFY.title AS datafromyear ";
			$query .= "FROM ";
			$query .= " gemeente G, aanspreektitel A, datafromyear DFY ";
			$query .= " WHERE ";
			$query .= " G.id = '$choice' ";
			$query .= " AND G.${code}_aanspreektitel_id = A.id ";
			$query .= " AND G.${code}_datafromyear_id = DFY.id ";
			$invuller_result=mysql($WebEngine_database,$query);
			$invuller_row = mysql_fetch_object($invuller_result);
			$html_out .= "<TR HEIGHT=\"28\">";
			$html_out .= t_spacer(20,"FFFFFF");
			$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=CENTER";
			$html_out .= " BGCOLOR=\"#FFFFFF\">";
			$html_out .= $t_fontface[$spiegel];
			$html_out .= $t_fontcolor[$spiegel];
			$html_out .= $t_fontsize[$spiegel];
			$html_out .= "contactpersoon: ";
			$html_out .= $invuller_row->volle_naam;
			$html_out .= "<BR>";
			$html_out .= "adres: ";
			$html_out .= $invuller_row->adres;
			$html_out .= "; ";
			$html_out .= $invuller_row->pcplaats;
			if ($invuller_row->telefoonnummer):
				$html_out .= "<BR>";
				$html_out .= "telefoon: ";
				$html_out .= $invuller_row->telefoonnummer;
			endif;
			if ($invuller_row->email):
				$html_out .= "<BR>";
				$html_out .= "e-mail: ";
				$html_out .= "<A HREF=mailto:" . $invuller_row->email . ">";
					$html_out .= $invuller_row->email;
				$html_out .= "</A>";
			endif;
			$html_out .= "<BR>";
			$html_out .= "Gegevens stammen uit: ";
			$html_out .= $invuller_row->datafromyear;
			$html_out .= "<BR>";
			$html_out .= "</TD>";
			$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
			$html_out .= " BGCOLOR=\"#FFFFFF\">";
			$html_out .= $t_fontface[$spiegel];
			$html_out .= $t_fontcolor[$spiegel];
			$html_out .= $t_fontsize[$spiegel];
			$html_out .= "&nbsp;";
			$html_out .= "</TD>";
			$html_out .= "</TR>\n";

		// some variables
			$vraagnummer = 0;
			$current_indicator = $indicator_id;
			$this_indicator_score = 0;
			$flipcolor_counter = 0;
	endif;

	// volgende vraag
	$vraagnummer++;

	$regel_bgcolor = "FFFFFF";

	$html_out .= "<TR>";


	$html_out .= "<TD ALIGN=RIGHT VALIGN=TOP";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $t_fontface[$spiegel];
	$html_out .= $t_fontcolor[$spiegel];
	$html_out .= $t_fontsize[$spiegel];
	$html_out .= "$vraagnummer";
	$html_out .= "</TD>";


	$html_out .= "<TD ALIGN=LEFT VALIGN=TOP";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $t_fontface[$spiegel];
	$html_out .= $t_fontcolor[$spiegel];
	$html_out .= $t_fontsize[$spiegel];
	$html_out .= "$vraag";
	$html_out .= "</TD>";


	$html_out .= "<TD ALIGN=RIGHT VALIGN=TOP";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $t_fontface[$spiegel];
	$html_out .= $t_fontcolor[$spiegel];
	$html_out .= $t_fontsize[$spiegel];

	if ( in_array($vraag_id,$beantwoorde_vragen) ):
		$html_out .= "ja";
	else:
		$html_out .=  "nee";
	endif;

	$html_out .= "</TD>\n";


	$html_out .= "<TD ALIGN=RIGHT VALIGN=TOP";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $t_fontface[$spiegel];
	$html_out .= $t_fontcolor[$spiegel];
	$html_out .= $t_fontsize[$spiegel];
	$html_out .= $score;
	$html_out .= "</TD>\n";

	$html_out .= "<TD ALIGN=RIGHT VALIGN=TOP";
	$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
	$html_out .= $t_fontface[$spiegel];
	$html_out .= $t_fontcolor[$spiegel];
	$html_out .= $t_fontsize[$spiegel];

	if ( in_array($vraag_id,$beantwoorde_vragen) ):
		$html_out .= "<B>$score</B>";
		$this_indicator_score += $score;
		$totaal_score += $score;
	else:
		$html_out .=  "<B>0</B>";
	endif;

	$html_out .= "</TD>\n";


	$html_out .= "</TR>\n";

	$flipcolor_counter++;

endwhile;

include("gemeente_close_indicator.inc");


// TOTAALSCORE VOOR ALLE INDICATOREN

$html_out .= "<TR HEIGHT=\"28\">";

$html_out .= t_spacer(20,$sorteerbalk_bgcolor[$spiegel]);

$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=CENTER";
$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
$html_out .= $sorteerbalk_fontface[$spiegel];
$html_out .= $sorteerbalk_fontcolor[$spiegel];
$html_out .= $sorteerbalk_fontsize[$spiegel];
$html_out .= "<FONT SIZE=+1><B>TOTAALSCORE</B></FONT>";
$html_out .= "</TD>";


$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
$html_out .= $sorteerbalk_fontface[$spiegel];
$html_out .= $sorteerbalk_fontcolor[$spiegel];
$html_out .= $sorteerbalk_fontsize[$spiegel];
$html_out .= "<FONT SIZE=+1><B>$totaal_score</B></FONT>";
$html_out .= "</TD>";


$html_out .= "</TR>\n";

// spacer

$html_out .= "<TR HEIGHT=\"28\">";

$html_out .= t_spacer(20,"FFFFFF");

$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=CENTER";
$html_out .= " BGCOLOR=\"#FFFFFF\">";
$html_out .= "&nbsp;";
$html_out .= "</TD>";


$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
$html_out .= " BGCOLOR=\"#FFFFFF\">";
$html_out .= "&nbsp;";
$html_out .= "</TD>";


$html_out .= "</TR>\n";


$html_out .= "</TABLE>\n";


?>
