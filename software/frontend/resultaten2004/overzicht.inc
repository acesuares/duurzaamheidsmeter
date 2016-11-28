<?
// Overzichten.en kaarten
	switch( $choice ):
		case "totaaloverzicht":
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
			// get the scores
				if ($sortorder == "totaalscore"):
	
					$query = "CREATE TEMPORARY TABLE indi_score ";
					$query .= "(gemeente_id SMALLINT UNSIGNED, ";
					$query .= " score SMALLINT, ";
					$query .= " score_procent DECIMAL(5,2), ";
					$query .= " correctie_factor TINYINT,";  // cf can be negative !
					$query .= " code varchar(2),";
					$query .= " kl_datafromyear_id smallint(6) unsigned,";
					$query .= " kl_procent_2003 decimal(5,2),";
					$query .= " so_datafromyear_id smallint(6) unsigned,";
					$query .= " so_procent_2003 decimal(5,2),";
					$query .= " rg_datafromyear_id smallint(6) unsigned,";
					$query .= " rg_procent_2003 decimal(5,2)";
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
						$query .= "SET score_procent = IF( ${indicode}_datafromyear_id > $current_datafromyear_id, ${indicode}_procent_2003,score_procent )";
						$query .= " WHERE code = '$indicode'";

						$result=mysql($WebEngine_database,$query);
					}
					$query = "CREATE TEMPORARY TABLE high_score ";
					$query .= "(gemeente_id SMALLINT UNSIGNED, ";
					$query .= " high_score_procent DECIMAL(5,2) ";
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

					$query .= " I.code AS indicator_code,";

					$query .= " G.id AS gemeente_id, ";
					$query .= " G.name AS gemeente, ";
					$query .= " G.inwoneraantal AS inwoneraantal, ";

					$query .= "G.so_datafromyear_id,";
					$query .= "G.kl_datafromyear_id,";
					$query .= "G.rg_datafromyear_id,";

					$query .= "G.so_procent_2003,";
					$query .= "G.kl_procent_2003,";
					$query .= "G.rg_procent_2003,";
	
					$query .= " P.id AS provincie_id, ";
					$query .= " P.name AS provincie, ";
					$query .= " sum(V.score) AS score, ";
					$query .= " HS.high_score_procent AS hsp,";
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
					$query .= "  HS.gemeente_id = G.id ";
	
					$query .= " GROUP BY G.id, I.name ";
	
					$query .= " ORDER BY hsp DESC, G.name, I.name";

/*$result=mysql($WebEngine_database,$query);
$iii=0;
while($row=mysql_fetch_assoc($result)):
$iii++;
$stuff3[$iii]=$row;
endwhile;
echo "<XMP>";
print_r($stuff3);
echo "</XMP>";
*/

				else:
					$query  = "SELECT ";
					$query .= " I.id AS indicator_id, ";
					$query .= " I.title AS indicator, ";

					$query .= " I.code AS indicator_code,";
	
					$query .= " G.id AS gemeente_id, ";
					$query .= " G.name AS gemeente, ";
					$query .= " G.inwoneraantal AS inwoneraantal, ";

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
	
					$query .= "  G.provincie_id = P.id ";
	
					$query .= " GROUP BY G.id, I.name ";
	
					$query .= " ORDER BY $order_by ";
					// echo $query;
				endif;
			include( "overzicht_display_scores.inc" );
		break;
		case "totaalkaart":
			$indicator_codes = array ( "so", "rg", "kl" );
			$indicator_ids = array ( 3, 4, 1 );
			$indicator_max_scores = array ( 50, 50, 70 );
			$current_datafromyear_id =4;
			$kaart_title = "Totaalscore 2003/2005";
			$kaart_text = "<B>Totaalresultaten Lokale 
 Duurzaamheidsmeter.</B><BR><BR>
 U kunt op de landkaart klikken om de specifieke gegevens van een 
 gemeente op te roepen. Onder de kaart verschijnt dan een tabel met 
 informatie over hoe de score van deze gemeente is opgebouwd.<BR><BR>
 Per gemeente wordt aangegeven wanneer en door wie de gegevens zijn aangeleverd.
 <BR><BR>Aanvullingen en commentaren zijn van harte welkom via <A HREF=\"mailto:info@la21.nl\">info@la21.nl</A>";
			include( "kaart.inc" );
		break;
		case "kl_20032005":
			$indicator_codes = array ( "kl" );
			$indicator_ids = array ( 1 );
			$indicator_max_scores = array ( 70 );
			$current_datafromyear_id =4;
			$kaart_title = "Klimaat &amp; Water 2003/2005";
			$kaart_text = "<B>Totaalscores op het gebied van Klimaat -en Waterbeleid.</B><BR><BR>
 Een deel van het klimaatbeleid richt zich op het verminderen van de 
 productie van broeikasgassen. Maar aanpassing aan een veranderend 
 klimaat, bijvoorbeeld op het gebied van neerslag en afvoer van water, is 
 ook belangrijk. Deze kaart laat de prestaties van Nederlandse gemeenten 
 op dat gebied zien. Als u op een gemeente klikt krijgt u de 
 gedetailleerde opbouw/antwoorden van de betreffende gemeente in beeld.";
 		include( "kaart.inc" );
		break;
		case "rg_20032005":
			$indicator_codes = array ( "rg" );
			$indicator_ids = array ( 4 );
			$indicator_max_scores = array ( 50 );
			$current_datafromyear_id =4;
			$kaart_title = "Duurzaam &amp; Ondernemen 2003/2005";
			$kaart_text = "<B>Totaalscores op het gebied van Duurzaam Ondernemen</B><BR><BR>
 Duurzaam ondernemen gaat over zaken als duurzaam inkopen, maar ook over 
 hoe je met je omgeving om gaat. De overheid is ook een ondernemer en de 
 kaart laat zien hoe 'duurzaam' de gemeenten daarbij opereren. Als u op 
 een gemeente klikt krijgt u de gedetailleerde opbouw/antwoorden van de 
 betreffende gemeente in beeld. De eerste groep vragen gaat specifiek 
 over hoe duurzaam het gemeentelijk inkoopbeleid is. De tweede groep 
 vragen over ruimte en groen en natuur en milieu educatie.";
			include( "kaart.inc" );
		break;
		case "so_20032005":
			$indicator_codes = array ( "so" );
			$indicator_ids = array ( 3 );
			$indicator_max_scores = array ( 50 );
			$current_datafromyear_id =4;
			$kaart_title = "Sociaal &amp; Mondiaal 2003/2005";
			$kaart_text = "<B>Totaalscores op het gebied van Mondiaal en Sociaal beleid</B><BR><BR> 
 De 'P' van Participatie en People staat centraal in deze kaart die 
 prestaties van gemeenten op het gebied van internationale samenwerking, 
 participatie en emancipatie laat zien. Een aantal vragen geven ook een 
 indicatie van het armoedebeleid van de gemeente. Als u op een gemeente 
 klikt krijgt u de getailleerde opbouw/antwoorden van de betreffende 
 gemeente te zien.";
			include( "kaart.inc" );
		break;
		default:
			header("Location: $PHP_SELF?spiegel=overzicht&choice=totaalkaart");
			exit;
		break;
	endswitch;
	/*
	mysql> select id,name,code from indicator;
+----+---------------------------+------+
| id | name                      | code |
+----+---------------------------+------+
|  1 | 002 klimaat & water       | kl   |
|  2 | 003 mondiaal              | lm   |
|  3 | 005 sociaal & mondiaal    | so   |
|  4 | 004 duurzaam & ondernemen | rg   |
|  5 | 006 water                 | wa   |
|  8 | 001 HOME (de echte)       |      |
+----+---------------------------+------+
6 rows in set (0.00 sec)
mysql> SELECT sum(V.score) AS max_score, I.title as indicator, I.id as indicator_id FROM vraag V, indicator I WHERE V.active='Y' AND V.indicator_id = I.id AND I.id <> 8 GROUP BY V.indicator_id;
+-----------+-----------------------+--------------+
| max_score | indicator             | indicator_id |
+-----------+-----------------------+--------------+
|        70 | Klimaat & Water       |            1 |
|        50 | Sociaal & Mondiaal    |            3 |
|        50 | Duurzaam & Ondernemen |            4 |
+-----------+-----------------------+--------------+
3 rows in set (0.02 sec)

*/
?>
