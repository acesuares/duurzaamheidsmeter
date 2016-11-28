<?

	// chooser_bar.inc

	// outer table

	$chooser_out .= "<div id='chooser-2003'>\n";

	// overzicht

	$chooser_out .= "<div id='link-2003-totaalovericht' class='link-2003'>\n";
	$chooser_out .= "<ul>\n";

	$chooser_out .= "<li><strong>overzicht</strong></li>\n";
	$chooser_out .= "<li>&nbsp;</li>\n";
	$chooser_out .= "<li><a href='index.php?spiegel=overzicht&choice=totaaloverzicht'>totaaloverzicht</a></li>";

	$chooser_out .= "</ul>\n";
	$chooser_out .= "</div>\n";

	// provincies


	$query  = "SELECT ";
	$query .= " id,name ";
	$query .= "FROM ";
	$query .= " provincie ";
//	$query .= "WHERE ";
//	$query .= " active='Y' ";
	$query .= "ORDER BY ";
	$query .= " name ";

	$result=mysql($WebEngine_database,$query);

	$chooser_out .= "<div id='link-2003-provincies' class='link-2003'>\n";
	$chooser_out .= "<ul>\n";


	$chooser_out .= "<li><strong>provincie</strong></li>\n";
	$chooser_out .= "<li>&nbsp;</li>\n";
	while($row = mysql_fetch_assoc($result)):
		$id=$row["id"];
		$name=stripslashes($row["name"]);
	        $chooser_out .= "<li><a href='index.php?spiegel=provincie&choice=$id'>$name</a></li>\n";
	endwhile;

	$chooser_out .= "</ul>\n";
	$chooser_out .= "</div>\n";

	
	// gemeentes (alleen die vragen beantwoord hebben)

	$query  = "SELECT ";
	$query .= " G.id,G.name ";
	$query .= "FROM ";
	$query .= " gemeente G, gemeente2vraag G2V ";
	$query .= "WHERE ";
	$query .= " G2V.gemeente_id = G.id ";
	$query .= "GROUP BY ";
	$query .= " G.id ";
	$query .= "ORDER BY ";
	$query .= " G.name ";

	$result=mysql($WebEngine_database,$query);

	$chooser_out .= "<div id='link-2003-gemeente' class='link-2003'>\n";
	$chooser_out .= "<ul>\n";

	$chooser_out .= "<li><strong>gemeente</strong></li>\n";
	$chooser_out .= "<li>&nbsp;</li>\n";
	while($row = mysql_fetch_assoc($result)):
		$id=$row["id"];
		$name=stripslashes($row["name"]);

	        $chooser_out .= "<li><a href='index.php?spiegel=gemeente&choice=$id'>$name</a></li>\n";

	endwhile;

	$chooser_out .= "</ul>\n";
	$chooser_out .= "</div>\n";


	// indicatoren

	$query  = "SELECT ";
	$query .= " id,title AS name ";
	$query .= "FROM ";
	$query .= " indicator ";
	$query .= "WHERE ";
	$query .= " id <> 8  "; // home
	$query .= "ORDER BY ";
	$query .= " name ";

	$result=mysql($WebEngine_database,$query);

	$chooser_out .= "<div id='link-2003-indicator' class='link-2003'>\n";
	$chooser_out .= "<ul>\n";

	$chooser_out .= "<li><strong>indicator</strong></li>\n";
	$chooser_out .= "<li>&nbsp;</li>\n";
	while($row = mysql_fetch_assoc($result)):
		$id=$row["id"];
		$name=strtolower(stripslashes($row["name"]));
	        $chooser_out .= "<li><a href='index.php?spiegel=indicator&choice=$id'>$name</a></li>\n";
	endwhile;

	$chooser_out .= "</ul>\n";
	$chooser_out .= "</div>\n";

	$chooser_out .= "</div>\n";


   $html_out = $html_out . $chooser_out;

?>
