<?

// Overzichten.

	// sortorder
	switch ($sortorder):
		case "inwoneraantal":
			$order_by = " G.inwoneraantal DESC, G.name, I.name ";
		break;
		case "provincie":
			$order_by = " P.name, G.name, I.name";
		break;
		case "totaalscore":
		break;
		default:
			$order_by = "G.name, I.name";
			$sortorder = "gemeente";
		break;
	endswitch;

	switch($choice):
		case "totaaloverzicht":

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
				$query .= "  V.indicator_id <> 10 AND ";  // extra watervragen

				$query .= "  G.provincie_id = P.id ";

				$query .= " GROUP BY G.id, I.name ";

				$query .= " ORDER BY $order_by ";

			endif;
		break;
		case "duurzaamheidsspiegel":
			$aantal_in_te_vullen = 5 ;

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
				$query .= "  HS.gemeente_id = G.id ";

				$query .= " GROUP BY G.id, I.name ";

				$query .= " ORDER BY HS.high_score, G.name, I.name";
			else:

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
				$query .= "  V.indicator_id <> 10 AND ";  // extra watervragen

				$query .= "  G.provincie_id = P.id ";

				$query .= " GROUP BY G.id, I.name ";

				$query .= " ORDER BY $order_by ";
			endif;
		break;
		case "wereldgemeente":
			$aantal_in_te_vullen = 2 ;

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
				$query .= " (CF.indicator_id = 1 OR "; // klimaat
				$query .= "  CF.indicator_id = 2) ";  // mondiaal
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
				$query .= "  (V.indicator_id = 1 OR ";  // klimaat
				$query .= "  V.indicator_id = 2) AND ";  // mondiaal

				$query .= "  G.provincie_id = P.id AND ";
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
				$query .= "  (V.indicator_id = 1 OR ";  // klimaat
				$query .= "  V.indicator_id = 2) AND ";  // mondiaal

				$query .= "  G.provincie_id = P.id ";

				$query .= " GROUP BY G.id, I.name ";

				$query .= " ORDER BY $order_by ";

			endif;
		break;
		default:
			header("Location: $PHP_SELF?spiegel=overzicht&choice=duurzaamheidsspiegel");
			exit;
		break;
	endswitch;


// display_scores
	include("overzicht_display_scores.inc");

?>