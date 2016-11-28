<?

// Indicator.

	// make sure our choice exists
	$query  = "SELECT ";
	$query .= " id,name,title ";
	$query .= "FROM ";
	$query .= " indicator ";
	$query .= "WHERE ";
	$query .= " id='$choice' AND ";
	$query .= " id<>8 "; // HOME

	$result=mysql($WebEngine_database,$query);
	if (mysql_num_rows($result) != 1):
		header("Location: $PHP_SELF?spiegel=overzicht&choice=totaaloverzicht");
		exit;
	endif;

	// sortorder
	switch ($sortorder):
		case "inwoneraantal":
			$order_by = " G.inwoneraantal DESC, G.name ";
		break;
		case "provincie":
			$order_by = " P.name, G.name ";
		break;
		case "totaalscore":
			$order_by = " high_score, G.name ";
		break;
		default:
			$order_by = "G.name ";
			$sortorder = "gemeente";
		break;
	endswitch;


$aantal_in_te_vullen = -1 ;

// get the scores

$query  = "SELECT ";
$query .= " I.id AS indicator_id, ";
$query .= " I.title AS indicator, ";
$query .= " G.id AS gemeente_id, ";
$query .= " G.name AS gemeente, ";
$query .= " G.inwoneraantal AS inwoneraantal, ";
$query .= " P.id AS provincie_id, ";
$query .= " P.name AS provincie, ";
$query .= " sum(V.score) AS score, ";
$query .= " max(CF.correctie_factor) AS correctie_factor, ";
$query .= " (sum(V.score) + max(CF.correctie_factor)) AS high_score ";

$query .= "FROM ";
$query .= " gemeente G, ";
$query .= " provincie P, ";
$query .= " gemeente2vraag G2V, ";
$query .= " vraag V, ";
$query .= " indicator I, ";
$query .= " correctie CF ";

$query .= " WHERE ";
$query .= "  G2V.gemeente_id = G.id AND ";
$query .= "  G2V.vraag_id = V.id AND ";

$query .= "  CF.gemeente_id = G.id AND ";
$query .= "  CF.indicator_id = '$choice' AND ";

$query .= "  V.active='Y' AND ";
$query .= "  V.indicator_id = I.id AND "; 
$query .= "  V.indicator_id = '$choice' AND "; 

$query .= "  G.provincie_id = P.id ";
	
$query .= " GROUP BY G.id ";

$query .= " ORDER BY $order_by ";

// display_scores
	include("overzicht_display_scores.inc");

// display_vragen
// get aantal gemeentes dat aan deze indicator meedeed

$query  = "SELECT  ";
$query .= " count(DISTINCT G2V.gemeente_id) AS aantal_gemeentes ";

$query .= " FROM gemeente2vraag G2V, vraag V ";

$query .= " WHERE ";
$query .= "  G2V.vraag_id = V.id AND ";
$query .= "  V.indicator_id = '$choice' ";

$query .= " GROUP BY V.indicator_id ";

$result=mysql($WebEngine_database,$query);

$row = mysql_fetch_assoc($result);
$aantal_gemeentes = 0 + $row["aantal_gemeentes"];


$query  = "SELECT ";
$query .= " V.title AS vraag, ";
$query .= " count(DISTINCT G2V.gemeente_id) AS aantal_ja ";

$query .= " FROM gemeente2vraag G2V, vraag V ";

$query .= " WHERE ";
$query .= "  G2V.vraag_id = V.id AND ";
$query .= "  V.indicator_id = '$choice' ";

$query .= " GROUP BY G2V.vraag_id ";

$query .= " ORDER BY V.name ";

$html_out .= "<TABLE BGCOLOR=\"#000000\" WIDTH=780 BORDER=0 CELLPADDING=5 CELLSPACING=1>";

$html_out .= "<TR HEIGHT=\"1\">";

$html_out .= t_spacer(20,$sorteerbalk_bgcolor[$spiegel]);
$html_out .= t_spacer(480,$sorteerbalk_bgcolor[$spiegel]);
$html_out .= t_spacer(70,$sorteerbalk_bgcolor[$spiegel]);
$html_out .= t_spacer(70,$sorteerbalk_bgcolor[$spiegel]);

$html_out .= "</TR>";

		$html_out .= "<TR HEIGHT=\"28\">";
		$html_out .= t_spacer(20,$sorteerbalk_bgcolor[$spiegel]);
		$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= "<FONT SIZE=4>";
		$html_out .= "<B>Vragenlijst ".strtoupper($indicator)."</B>";
		$html_out .= "</FONT>";
		$html_out .= "</TD>";
		$html_out .= "</TR>\n";

		
		$html_out .= "<TR HEIGHT=\"28\">";
		$html_out .= t_spacer(20,$sorteerbalk_bgcolor[$spiegel]);
		$html_out .= "<TD ALIGN=LEFT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= "<FONT SIZE=2>";
		$html_out .= "<B>Welk percentage van de $aantal_gemeentes aan deze vragenlijst deelnemende gemeentes antwoordde:</B>";
		$html_out .= "</FONT>";
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

		$html_out .= "</TR>\n";


$vraagnummer = 0;

$result=mysql($WebEngine_database,$query);

while ($row = mysql_fetch_assoc($result)):

	// get values
	foreach ($row as $key => $value){
		$$key=$value;
	}

	$procent_ja = round (($aantal_ja/$aantal_gemeentes) * 100,0);
	$procent_nee = 100 - $procent_ja;

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

endwhile;

// spacer

$html_out .= "<TR HEIGHT=\"28\">";

$html_out .= "<TD COLSPAN=4 ALIGN=LEFT VALIGN=CENTER";
$html_out .= " BGCOLOR=\"#FFFFFF\">";
$html_out .= "&nbsp;";
$html_out .= "</TD>";

$html_out .= "</TR>\n";


$html_out .= "</TABLE>\n";

?>
