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
		header("Location: $PHP_SELF?spiegel=overzicht&choice=totaalkaart");
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
					$query .= " score SMALLINT, ";
					$query .= " score_procent DECIMAL(3,2), ";
					$query .= " correctie_factor TINYINT,";  // cf can be negative !
					$query .= " code varchar(2),";
					$query .= " kl_datafromyear_id smallint(6) unsigned,";
					$query .= " kl_procent_2003 decimal(3,2),";
					$query .= " so_datafromyear_id smallint(6) unsigned,";
					$query .= " so_procent_2003 decimal(3,2),";
					$query .= " rg_datafromyear_id smallint(6) unsigned,";
					$query .= " rg_procent_2003 decimal(3,2)";
					$query .= ")type=heap ";
	
					$result=mysql($WebEngine_database,$query);

					$query = "INSERT INTO indi_score ";
					$query .= "SELECT ";
					$query .= " G2V.gemeente_id,";
					$query .= " SUM(V.score) as score,";
					$query .= " 0, ";
					$query .= " MAX(CF.correctie_factor) as correctie_factor, ";
					$query .= " I.code, ";
					$query .= " G.kl_datafromyear_id, ";
					$query .= " G.kl_procent_2003, ";
					$query .= " G.so_datafromyear_id, ";
					$query .= " G.so_procent_2003, ";
					$query .= " G.rg_datafromyear_id, ";
					$query .= " G.rg_procent_2003 ";
					$query .= "FROM";
					$query .= " gemeente2vraag G2V,";
					$query .= " vraag V,";
					$query .= " correctie CF, ";
					$query .= " indicator I, ";
					$query .= " gemeente G ";
					$query .= "WHERE ";
					$query .= " G2V.vraag_id = V.id AND";
					$query .= " CF.gemeente_id = G2V.gemeente_id AND";
					$query .= " CF.indicator_id = V.indicator_id AND";
					$query .= " G.id = G2V.gemeente_id AND";
					$query .= " I.id = V.indicator_id ";
					$query .= "GROUP BY ";
					$query .= " G2V.gemeente_id,";
					$query .= " V.indicator_id ";
					$query .= "ORDER BY G2V.gemeente_id";
	
					$result=mysql($WebEngine_database,$query);
					
					// convert the 2005 scores to procent
					
					$max_indicator_score = array();
					$max_indicator_score["kl"] = 70;
					$max_indicator_score["rg"] = 50;
					$max_indicator_score["so"] = 50;
					foreach ( array("kl","so","rg") as $indicode ){
						$query  = "UPDATE indi_score ";
						$query .= "SET score_procent = ( score + correctie_factor ) * 100 / " . $max_indicator_score[$indicode]; 
						$query .= " WHERE code = '$indicode'";

						$result=mysql($WebEngine_database,$query);
					}

					// join 2003 and 2005 scores
					$current_datafromyear_id =4;
					foreach ( array("kl","so","rg") as $indicode ){
						$query  = "UPDATE indi_score ";
						$query .= "SET score_procent = IF( ${indicode}_datafromyear_id <> $current_datafromyear_id, ${indicode}_procent_2003,score_procent )";
						$query .= " WHERE code = '$indicode'";

						$result=mysql($WebEngine_database,$query);
					}

					$query = "CREATE TEMPORARY TABLE high_score ";
					$query .= "(gemeente_id SMALLINT UNSIGNED, ";
					$query .= " high_score_procent DECIMAL(3,2) ";
					$query .= ")type=heap ";

					$result=mysql($WebEngine_database,$query);

					$query = "INSERT INTO high_score ";
					$query .= "SELECT ";
					$query .= " gemeente_id,";
					$query .= " SUM(score_procent) / 3 AS high_score_procent ";
					$query .= "FROM";
					$query .= " indi_score ";
					$query .= "GROUP BY ";
					$query .= " gemeente_id ";
					$query .= "ORDER BY high_score_procent";
	
					$result=mysql($WebEngine_database,$query);

					$query = "DROP TABLE indi_score";
	
					$result=mysql($WebEngine_database,$query);

	$query  = "SELECT ";
	$query .= " I.id AS indicator_id, ";
	$query .= " I.title AS indicator, ";
/* indicator code */
	$query .= " I.code AS indicator_code,";

	$query .= " G.id AS gemeente_id, ";
	$query .= " G.name AS gemeente, ";
	$query .= " G.inwoneraantal AS inwoneraantal, ";
/* there are three indicator codes now */
	$query .= "G.so_datafromyear_id,";
	$query .= "G.kl_datafromyear_id,";
	$query .= "G.rg_datafromyear_id,";

	$query .= "G.so_procent_2003,";
	$query .= "G.kl_procent_2003,";
	$query .= "G.rg_procent_2003,";

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

	$query .= "  G.provincie_id = P.id AND ";
	$query .= "  P.id='$choice' AND ";
	$query .= "  HS.gemeente_id = G.id ";

	$query .= " GROUP BY G.id, I.name ";

	$query .= " ORDER BY HS.high_score_procent DESC, G.name, I.name";
else:

	$query  = "SELECT ";
	$query .= " I.id AS indicator_id, ";
	$query .= " I.title AS indicator, ";
/* indicator code */
	$query .= " I.code AS indicator_code,";

	$query .= " G.id AS gemeente_id, ";
	$query .= " G.name AS gemeente, ";
	$query .= " G.inwoneraantal AS inwoneraantal, ";
/* there are three indicator codes now */
	$query .= "G.so_datafromyear_id,";
	$query .= "G.kl_datafromyear_id,";
	$query .= "G.rg_datafromyear_id,";

	$query .= "G.so_procent_2003,";
	$query .= "G.kl_procent_2003,";
	$query .= "G.rg_procent_2003,";

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
