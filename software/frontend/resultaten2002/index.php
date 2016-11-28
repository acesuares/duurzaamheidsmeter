<?

	// chooser.php

	include("init.inc");

	function t_spacer($width,$regel_bgcolor){
		$t_spacer  = "<TD WIDTH=\"$width\" BGCOLOR=\"#$regel_bgcolor\">";
		$t_spacer .= "<IMG SRC=\"/images/clear.gif\" BORDER=\"0\" WIDTH=\"$width\" HEIGHT=\"1\" HSPACE=\"0\" VSPACE=\"0\">";
		$t_spacer .= "</TD>";
		return $t_spacer;
	}
	$base_font			= "<FONT FACE=\"Verdana, Arial, Helvetica\">";
	$base_fontcolor	= "<FONT COLOR=\"#000000\">";
	$base_fontsize		= "<FONT SIZE=\"2\">";

	$html_out = "";


	// sitestyle
	$bgcolor["overzicht"]						= "003366";
	$sorteerbalk_bgcolor["overzicht"]		= "003366";
	$sorteerbalk_fontface["overzicht"]		= $base_font;
	$sorteerbalk_fontcolor["overzicht"]		= "<FONT COLOR=\"#FFFFFF\">";
	$sorteerbalk_fontsize["overzicht"]		= "<FONT SIZE=\"2\">";
	$t_bgcolor["overzicht"]						= "D2DFEA";
	$t_fontface["overzicht"]					= $base_font;
	$t_fontcolor["overzicht"]					= $base_fontcolor;
	$t_fontsize["overzicht"]					= $base_fontsize;

	$bgcolor["provincie"]						= "CC3300";
	$sorteerbalk_bgcolor["provincie"]		= "CC3300";
	$sorteerbalk_fontface["provincie"]		= $base_font;
	$sorteerbalk_fontcolor["provincie"]		= "<FONT COLOR=\"#FFFFFF\">";
	$sorteerbalk_fontsize["provincie"]		= "<FONT SIZE=\"2\">";
	$t_bgcolor["provincie"]						= "FFCCCC";
	$t_fontface["provincie"]					= $base_font;
	$t_fontcolor["provincie"]					= $base_fontcolor;
	$t_fontsize["provincie"]					= $base_fontsize;

	$bgcolor["gemeente"]							= "006600";
	$sorteerbalk_bgcolor["gemeente"]			= "006600";
	$sorteerbalk_fontface["gemeente"]		= $base_font;
	$sorteerbalk_fontcolor["gemeente"]		= "<FONT COLOR=\"#FFFFFF\">";
	$sorteerbalk_fontsize["gemeente"]		= "<FONT SIZE=\"2\">";
	$t_bgcolor["gemeente"]						= "99cc99";
	$t_fontface["gemeente"]						= $base_font;
	$t_fontcolor["gemeente"]					= $base_fontcolor;
	$t_fontsize["gemeente"]						= $base_fontsize;

	$sorteerbalk_bgcolor["indicator"]		= "ff9900";
	$bgcolor["indicator"]						= "ff9900";
	$sorteerbalk_fontface["indicator"]		= $base_font;
	$sorteerbalk_fontcolor["indicator"]		= "<FONT COLOR=\"#FFFFFF\">";
	$sorteerbalk_fontsize["indicator"]		= "<FONT SIZE=\"2\">";
	$t_bgcolor["indicator"]						= "ffcc99";
	$t_fontface["indicator"]					= $base_font;
	$t_fontcolor["indicator"]					= $base_fontcolor;
	$t_fontsize["indicator"]					= $base_fontsize;

	switch ($spiegel):
		case "overzicht":
		   include("overzicht.inc");
		break;
		case "provincie":
		   include("provincie.inc");
		break;
		case "gemeente":
		   include("gemeente.inc");
		break;
		case "indicator":
		   include("indicator.inc");
		break;
		default:
			header("Location: $PHP_SELF?spiegel=overzicht&choice=duurzaamheidsspiegel");
			exit;
		break;
	endswitch;
	// HTML headers

	// title_and_logo

	// create the title

	switch ($spiegel):
		case "overzicht":
			switch($choice):
				case "totaaloverzicht":
					$site_title = "Resultaten Spiegel 2002: <BR>Totaaloverzicht";
				break;
				case "duurzaamheidsspiegel":
					$site_title = "Resultaten Spiegel 2002: <BR>De Duurzaamheidsspiegel";
				break;
				case "wereldgemeente":
					$site_title = "Resultaten Spiegel 2002: <BR>De Wereldgemeente";
				break;
			endswitch;
		break;
		case "provincie":
			$site_title = "Resultaten Spiegel 2002: <BR>Provincie $provincie";
		break;
		case "gemeente":
			$site_title = "Resultaten Spiegel 2002: <BR>Gemeente $gemeente";
		break;
		case "indicator":
			$site_title = "Resultaten Spiegel 2002: <BR>Indicator: ".strtolower($indicator);
		break;
	endswitch;

	// display the title

	$title_out .= "<TABLE WIDTH=780 BGCOLOR=\"#FFFFFF\" CELLPADDING=0 CELLSPACING=0 BORDER=0>\n";
	
	$title_out .= "<TR HEIGHT=\"17\">\n";

	$title_out .= "<TD COLSPAN=5 BGCOLOR=\"#FFFFFF\">";
	$title_out .= "<IMG SRC=/images/clear.gif WIDTH=1 HEIGHT=1 BORDER=0>";
	$title_out .= "</TD>\n";

	$title_out .= "</TR>\n";

	$title_out .= "<TR HEIGHT=\"90\">\n";
	
	$title_out .= t_spacer(10,"FFFFFF");
	
	$title_out .= "<TD ALIGN=LEFT VALIGN=center>\n";
	$title_out .= $t_fontface[$spiegel];
	$title_out .= $t_fontcolor[$spiegel];
	$title_out .= "<FONT SIZE=\"5\">";

	$title_out .= "$site_title\n";

	$title_out .= "</TD>\n";

	$title_out .= t_spacer(10,"FFFFFF");

	$title_out .= "<TD ALIGN=RIGHT VALIGN=CENTER>";
        $title_out .= "<A HREF='http://www.duurzaamheidsmeter.nl'>";
        $title_out .= "<IMG SRC=/images/la21.gif WIDTH=70 HEIGHT=70 VSPACE=0 HSPACE=0 BORDER=0 ALT=\"Lokale Agenda 21\">";
        $title_out .= "...terug naar de hoofdsite...";
 //   	$title_out .= "<A HREF=http://www.la21.nl/ TARGET=_la21nl>";
	$title_out .= "</A>";
	$title_out .= "</TD>";
	
	$title_out .= t_spacer(10,"FFFFFF");

	$title_out .= "</TR>\n";
	$title_out .= "</TABLE>\n";

	// legenda

	$title_out .= "<TABLE WIDTH=780 BGCOLOR=\"FFFFFF\" CELLPADDING=0 CELLSPACING=2 BORDER=0>\n";

	$title_out .= "<TR HEIGHT=\"29\">\n";
	
	$title_out .= "<TD ALIGN=RIGHT VALIGN=CENTER>\n";
	$title_out .= $t_fontface[$spiegel];
	$title_out .= $t_fontcolor[$spiegel];
	$title_out .= "<FONT SIZE=\"1\">";

	foreach ($score_gif as $indicator_title => $img_src) {
		$title_out .= "<IMG SRC=\"$img_src\" WIDTH=10 HEIGHT=10 HSPACE=10 VSPACE=0 ALT=\"$indicator_title\">";
		$title_out .= strtolower($indicator_title);
	}
	$title_out .= "<IMG SRC=\"/images/clear.gif\" WIDTH=10 HEIGHT=10 HSPACE=0 VSPACE=0>";
		
	$title_out .= "</TD>\n";
	$title_out .= "</TR>\n";
	$title_out .= "</TABLE>\n";

   $html_out = $title_out . $html_out;

	//end title_and_logo_and_legenda

	include("chooser_bar.inc");

	
	// HTML header
	$header_out .= "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">";
      
	$header_out .= "<HTML><HEAD>\n";
	$header_out .= "<TITLE>".strip_tags($site_title)."</TITLE>\n";
        $header_out .= "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">";
        $header_out .= "
        	<style>
		  #chooser-2003 { width: 780px; }
	          .link-2003 {
                    width: 25%;
                    float: left;
                    text-align: left;
  		    font-size: smaller;
                    font-family: sans-serif;
                  }
	          .link-2003 ul { list-style: none; }
                  #link-2003-totaalovericht {}
                  #link-2003-provincies {}
          	  #link-2003-gemeente {}
                  #link-2003-indicator {}
	      </style>\n";
      	$header_out .= "</HEAD>\n";

	$header_out .= "<BODY ";
	$header_out .= "	BGCOLOR=\"#FFFFFF\" ";
	$header_out .= "	LINK=\"#000000\" ";
	$header_out .= "	VLINK=\"#000000\"  ";
	$header_out .= " ALINK=\"#000000\" ";
	$header_out .= "	TEXT=\"#000000\"  ";
	$header_out .= " MARGINHEIGHT=0 TOPMARGIN=0 MARGINWIDTH=0 LEFTMARGIN=0 RIGHTMARGIN=0 BOTTOMMARGIN=0 ";
	$header_out .= " >";
	$header_out .= "<CENTER>";

   $html_out = $header_out . $html_out;
	// end HTML header


	// HTML footer
	$html_out .= "<BR>";
	$html_out .= "<TABLE BGCOLOR=\"".$sorteerbalk_bgcolor[$spiegel]."\" WIDTH=780 BORDER=0 CELLSPACING=2 CELLPADDING=0>\n";
	$html_out .= "<TR><TD>\n";

	$html_out .= "<TABLE BGCOLOR=\"#FFFFFF\" WIDTH=776 BORDER=0 CELLSPACING=0 CELLPADDING=12>\n";
	$html_out .= "<TR><TD ALIGN=RIGHT>\n";
	$html_out .= $t_fontface[$spiegel];
	$html_out .= $t_fontcolor[$spiegel];
	$html_out .= $t_fontsize[$spiegel];
	$html_out .= "<B>Website LA21 en de Lokale Duurzaamheidsspiegel</B><BR>";
	$html_out .= "<FONT SIZE=1>";
	$html_out .= "inhoud:  <A HREF=mailto:cosnhn@cossen.nl STYLE=\"text-decoration: none\">Thijs de la Court</A> (COS Noord-Holland Noord)<BR>";
	$html_out .= "graphic design: ";
	$html_out .= "<A HREF=http://www.jetdesign.nl TARGET=_jetdesign STYLE=\"text-decoration: none\">Jet Design</A><BR>";
	$html_out .= "database development &amp; hosting: ";
	$html_out .= "<A HREF=http://www.suares.nl TARGET=_suares STYLE=\"text-decoration: none\">Ace Suares</A><BR>";
	$html_out .= "Aan deze website kunnen geen rechten worden ontleend.<BR><BR>";

/* counters disabled as per 2006-05-23
	if ( $spiegel == "overzicht"):
		$counter_query="s:$spiegel;c:$choice";
	else:
		$counter_query="s:$spiegel;c:".(100000+$choice);
	endif;

	$query  = "SELECT hits ";
	$query .= "FROM counter ";
	$query .= "WHERE query='$counter_query'";

	$result=mysql($WebEngine_database,$query);

	if ( mysql_num_rows($result) == 0):
		$query  = "INSERT into counter VALUES ('$counter_query',1)";
		$result=mysql($WebEngine_database,$query);
		$page_views = 1;
	else:
		$row = mysql_fetch_assoc($result);
		$page_views = $row["hits"] + 1;
	
		$query  = "UPDATE counter ";
		$query .= "SET hits=hits+1 ";
		$query .= "WHERE query='$counter_query'";

		$result=mysql($WebEngine_database,$query);
	endif;

	$html_out .= $t_fontsize[$spiegel];
	$html_out .= "deze pagina is <B>".number_format ($page_views, 0, ",", ".")."</B> keer bekeken";
*/
	$html_out .= "</TD></TR>\n";
	$html_out .= "</TABLE>\n";

	$html_out .= "</TD></TR>\n";
	$html_out .= "</TABLE>\n";

	$html_out .= "<BR><BR><BR>";

	// end HTML footer

	
	$html_out .= "</CENTER></BODY></HTML>";

	echo $html_out;

?>
