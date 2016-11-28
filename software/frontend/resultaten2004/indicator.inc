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

			$current_datafromyear_id =4;
// Indicator.

	// make sure our choice exists
	$query  = "SELECT ";
	$query .= " id,name,title,code ";
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

	$row = mysql_fetch_assoc($result);
	$indicator_code = $row["code"];
	
	// sortorder
	switch ($sortorder):
		case "inwoneraantal":
			$order_by = " G.inwoneraantal DESC, G.name ";
		break;
		case "provincie":
			$order_by = " P.name, G.name ";
		break;
		case "totaalscore":
			$order_by = " high_score DESC, G.name ";
		break;
		default:
			$order_by = "G.name ";
			$sortorder = "gemeente";
		break;
	endswitch;


$aantal_in_te_vullen = -1 ;

					$max_indicator_score = array();
					$max_indicator_score["kl"] = 70;
					$max_indicator_score["rg"] = 50;
					$max_indicator_score["so"] = 50;

// get the scores

$query  = "SELECT ";
$query .= " I.id AS indicator_id, ";
$query .= " I.title AS indicator, ";
/* indicator code */
$query .= " I.code AS indicator_code,";
$query .= " G.id AS gemeente_id, ";
$query .= " G.name AS gemeente, ";
$query .= " G.inwoneraantal AS inwoneraantal, ";
$query .= " G." . $indicator_code . "_datafromyear_id, ";
$query .= " G." . $indicator_code . "_procent_2003, ";
$query .= " P.id AS provincie_id, ";
$query .= " P.name AS provincie, ";
$query .= " sum(V.score) AS score, ";
$query .= " max(CF.correctie_factor) AS correctie_factor, ";
$query .= " IF ( G.${indicator_code}_datafromyear_id <> $current_datafromyear_id, ";
$query .= "  G.${indicator_code}_procent_2003, ";
$query .= " (sum(V.score) + max(CF.correctie_factor)) * 100 / $max_indicator_score[$indicator_code] ) AS high_score ";

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

// echo $query;

// display_scores
	include("overzicht_display_scores.inc");

// display_vragen

// get aantal gemeentes dat aan deze indicator meedeed (totaal)

$query  = "SELECT  ";
$query .= " count(DISTINCT G2V.gemeente_id) AS aantal_gemeentes ";

$query .= " FROM gemeente2vraag G2V, vraag V ";

$query .= " WHERE ";
$query .= "  G2V.vraag_id = V.id AND ";
$query .= "  V.indicator_id = '$choice' ";

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
$query .= "  V.indicator_id = '$choice' AND ";
$query .= "  G." . $indicator_code . "_datafromyear_id=$current_datafromyear_id ";
$query .= " GROUP BY V.indicator_id ";

$result=mysql($WebEngine_database,$query);

$row = mysql_fetch_assoc($result);
$aantal_gemeentes_2005 = 0 + $row["aantal_gemeentes"];

$query  = "SELECT ";
$query .= " V.title AS vraag, ";
$query .= " count(DISTINCT G2V.gemeente_id) AS aantal_ja, ";
$query .= " V.questionfromyear_id ";
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
		$html_out .= "<B>In 2005 deden $aantal_gemeentes_2005 gemeentes mee.<BR>In totaal deden er $aantal_gemeentes_totaal mee.<BR>Welk percentage van de gemeentes die meedoen aan deze vragenlijst antwoordde:</B>";
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
	$aantal_gemeentes = $aantal_gemeentes_totaal;
	if ( $questionfromyear_id == $current_datafromyear_id ):
		$aantal_gemeentes = $aantal_gemeentes_2005;
	endif;
	
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
	if ( $questionfromyear_id == $current_datafromyear_id ):
		$html_out .= "<FONT COLOR=\"#FF0000\">$vraagnummer</FONT>";
	else:
		$html_out .= "$vraagnummer";
	endif;
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
