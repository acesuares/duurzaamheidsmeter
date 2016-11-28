<?

// Provincie.

	// make sure our choice exists
	$query  = "SELECT ";
	$query .= " id,name ";
	$query .= "FROM ";
	$query .= " provincie ";
	$query .= "WHERE ";
	$query .= " id='$choice' ";

	$result=mysql($WebEngine_database,$query);
	if (mysql_num_rows($result) != 1):
		header("Location: $PHP_SELF?spiegel=overzicht&choice=duurzaamheidsspiegel");
		exit;
	endif;


	// sortorder
	switch ($sortorder):
		case "inwoneraantal":
			$order_by = " G.inwoneraantal DESC, G.name, I.name ";
		break;
		case "totaalscore":
		break;
		default:
			$order_by = "G.name, I.name";
			$sortorder = "gemeente";
		break;
	endswitch;

$aantal_in_te_vullen = -1 ;

// get the scores

if ($sortorder == "totaalscore"):

	$query = "CREATE TEMPORARY TABLE indi_score ";
	$query .= "(gemeente_id SMALLINT UNSIGNED, ";
	$query .= " score SMALLINT UNSIGNED, ";
	$query .= " correctie_factor TINYINT";  // cf can be negative !
	$query .= ")type=heap ";

	$result=mysql($WebEngine_database,$query);

	$query = "INSERT INTO indi_score ";
	$query .= "SELECT ";
	$query .= " G2V.gemeente_id,";
	$query .= " SUM(V.score) as score,";
	$query .= " MAX(CF.correctie_factor) as correctie_factor ";
	$query .= "FROM";
	$query .= " gemeente2vraag G2V,";
	$query .= " vraag V,";
	$query .= " correctie CF ";
	$query .= "WHERE ";
	$query .= " G2V.vraag_id = V.id AND";
	$query .= " CF.gemeente_id = G2V.gemeente_id AND";
	$query .= " CF.indicator_id = V.indicator_id AND";
	$query .= " CF.indicator_id <> 10 ";
	$query .= "GROUP BY ";
	$query .= " G2V.gemeente_id,";
	$query .= " V.indicator_id ";
	$query .= "ORDER BY G2V.gemeente_id";

	$result=mysql($WebEngine_database,$query);


	$query = "CREATE TEMPORARY TABLE high_score ";
	$query .= "(gemeente_id SMALLINT UNSIGNED, ";
	$query .= " high_score INT "; // high score can be negative, in execptional cases !
	$query .= ")type=heap ";

	$result=mysql($WebEngine_database,$query);


	$query = "INSERT INTO high_score ";
	$query .= "SELECT ";
	$query .= " gemeente_id,";
	$query .= " SUM(score) + SUM(correctie_factor) as high_score ";
	$query .= "FROM";
	$query .= " indi_score ";
	$query .= "GROUP BY ";
	$query .= " gemeente_id ";
	$query .= "ORDER BY high_score";

	$result=mysql($WebEngine_database,$query);


	$query = "DROP TABLE indi_score";

	$result=mysql($WebEngine_database,$query);

	$query  = "SELECT ";
	$query .= " I.id AS indicator_id, ";
	$query .= " I.title AS indicator, ";
	$query .= " G.id AS gemeente_id, ";
	$query .= " G.name AS gemeente, ";
	$query .= " G.inwoneraantal AS inwoneraantal, ";
	$query .= " P.id AS provincie_id, ";
	$query .= " P.name AS provincie, ";
	$query .= " sum(V.score) AS score, ";
	$query .= " CF.correctie_factor AS correctie_factor ";

	$query .= "FROM ";
	$query .= " gemeente G, ";
	$query .= " provincie P, ";
	$query .= " gemeente2vraag G2V, ";
	$query .= " vraag V, ";
	$query .= " correctie CF, ";
	$query .= " indicator I, ";
	$query .= " high_score HS ";

	$query .= " WHERE ";
	$query .= "  G2V.gemeente_id = G.id AND ";
	$query .= "  G2V.vraag_id = V.id AND ";

	$query .= "  CF.gemeente_id = G.id AND ";
	$query .= "  CF.indicator_id = I.id AND ";

	$query .= "  V.active='Y' AND ";
	$query .= "  V.indicator_id = I.id AND ";
	$query .= "  V.indicator_id <> 8 AND ";  // HOME
	$query .= "  V.indicator_id <> 10 AND ";  // extra watervragen

	$query .= "  G.provincie_id = P.id AND ";
	$query .= "  P.id='$choice' AND ";
	$query .= "  HS.gemeente_id = G.id ";

	$query .= " GROUP BY G.id, I.name ";

	$query .= " ORDER BY HS.high_score, G.name, I.name";
else:

	$query  = "SELECT ";
	$query .= " I.id AS indicator_id, ";
	$query .= " I.title AS indicator, ";
	$query .= " G.id AS gemeente_id, ";
	$query .= " G.name AS gemeente, ";
	$query .= " G.inwoneraantal AS inwoneraantal, ";
	$query .= " P.id AS provincie_id, ";
	$query .= " P.name AS provincie, ";
	$query .= " sum(V.score) AS score, ";
	$query .= " CF.correctie_factor AS correctie_factor ";

	$query .= "FROM ";
	$query .= " gemeente G, ";
	$query .= " provincie P, ";
	$query .= " gemeente2vraag G2V, ";
	$query .= " vraag V, correctie CF, ";
	$query .= " indicator I ";

	$query .= " WHERE ";
	$query .= "  G2V.gemeente_id = G.id AND ";
	$query .= "  G2V.vraag_id = V.id AND ";

	$query .= "  CF.gemeente_id = G.id AND ";
	$query .= "  CF.indicator_id = I.id AND ";

	$query .= "  V.active='Y' AND ";
	$query .= "  V.indicator_id = I.id AND ";
	$query .= "  V.indicator_id <> 8 AND ";  // HOME
	$query .= "  V.indicator_id <> 10 AND ";  // extra water

	$query .= "  G.provincie_id = P.id AND ";
	$query .= "  P.id='$choice' ";

	$query .= " GROUP BY G.id, I.name ";

	$query .= " ORDER BY $order_by ";

endif;

// display_scores
	include("overzicht_display_scores.inc");

// display_gegevens
	// provincie gegevens

	$query  = "SELECT ";
	$query .= "name,inwoneraantal,adres,postcode,plaats,tel,";
	$query .= "fax, email, website, description,";
	$query .= "IF(foto<>'','yes','no') AS foto ";

	$query .= " FROM provincie ";
	$query .= " WHERE id='$choice' ";

	$result=mysql($WebEngine_database,$query);

	// get the first (and only) row
	$row = mysql_fetch_assoc($result);

	// get values
	foreach ($row as $key => $value){
		$$key=$value;
	}

	if ( $foto == "yes" ):
		$foto_html="<IMG SRC=\"graphic.php?table_name=provincie&field_name=foto&id=$choice\" BORDER=\"0\" HSPACE=\"20\" ALIGN=\"RIGHT\" WIDTH=\"200\">";
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
	$html_out .= "$foto_html<B>Provincie $name</B> (". number_format ($inwoneraantal, 0, ",", ".")
." inwoners)";
	$html_out .= "<BR>";
	if ($adres.$postocde.$plaats != ""):
		$html_out .= $adres.", ".$postocde." ".$plaats;
		$html_out .= "<BR>";
	endif;
	if ($tel != ""):
		$html_out .= "telefoonnummer: ".$tel.", ";
	endif;
	if ($fax != ""):
		$html_out .= "faxnummer: ".$fax;
	endif;
	$html_out .= "<BR>";
	if ($email != ""):
		$html_out .= "<A HREF=mailto:".$email.">".$email."</A>";
		$html_out .= "<BR>";
	endif;
	if ($website != ""):
		$html_out .= "<A HREF=\"".$website."\" TARGET=_web_$gemeente_id>".$website."</A>";
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

?>