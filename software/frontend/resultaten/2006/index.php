<?php
// licensed under GPL v2. (C) 2006 Ace Suares, Amsterdam/Willemstad
	/*
		Copyright (C) 2006 Ace Suares, Keizersgracht 132, 1015 CW Amsterdam
		http://www.suares.com
		
		This program is free software; you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation; either version 2 of the License, or
		(at your option) any later version.
		
		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.
		
		You should have received a copy of the GNU General Public License
		along with this program; if not, write to the Free Software
		Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	*/	
// include database link and some other stuff
	include( "000_init.inc" );
	include( "010_functions.inc" );
	include( "020_parse-uri.inc" );
// HTML header
	$out = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n
           <HTML>\n
           <HEAD>\n
           <TITLE>{$strings[ 'Duurzaamheidsmeter' ]} $year</TITLE>\n
           <META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=ISO-8859-15\">\n
           <LINK REL=\"stylesheet\" TYPE=\"text/css\" HREF=\"{$settings->site_url}/$year/{$settings->stylesheet}\">\n
           </HEAD>\n";
// HTML body
	$out .= "<BODY>\n";
	$out .= "<DIV ID=\"page-area\">\n";
	include( "lang-switch.inc" );
	// title
	$out .= "<H1 ID=\"title-area\">{$strings[ 'Duurzaamheidsmeter' ]} $year</H1>\n";
	include( "tabs.inc");
	if ( $onderwerp == $strings["introductie" ] ):
		$query  = "SELECT ";
		$query .= " I.introductie_$lang AS inleiding ";
		$query .= " FROM indicator I ";
		$query .= " WHERE ";
		$query .= " I.id = 8 ";
		$result = mysql_query( $query );
		$row = mysql_fetch_object( $result );
		$out .= "<DIV ID=\"introductie-tekst\">\n";
		$out .= nl2br( stripslashes( $row->inleiding ) );
		$out .= "</DIV>\n";
	else:
		switch ( $view ):
			case $strings[ "uitleg" ]:
				// introductietekst
					$out .= "<DIV ID=\"introductie-tekst\">\n";
					$out .= nl2br( stripslashes ( $indicators[ $onderwerp ]->introductie ) );
					$out .= "</DIV>\n";
			break;
			case $strings[ "gemeente" ]:
				// show dropdown menu
					$out .= "<FORM ACTION=\"{$settings->site_url}/$year/$lang/$onderwerp/$view\" METHOD=\"POST\">\n";
					$out .= "<DIV ID=\"dropdown\">\n";
					$out .= "<SELECT NAME=\"search\" SIZE=1 ONCHANGE=\"submit()\">\n";
					$out .= "<OPTION>";
					$out .= $strings[ "kiesgemeente" ];
					$out .= "</OPTION>\n";
					foreach( $gemeentes as $gemeente_name=>$gemeente ):
						if ( $search == $gemeente_name ):
							$selected = " SELECTED";
						else:
							$selected = "";
						endif;
						if ( $gemeente->score_procent == -1 ):
							$score = "";
						else:
							$score = " (" . sprintf( "%.2f", $gemeente->score_procent ) . "%)";
						endif;
						$out .= "<OPTION VALUE=\"$gemeente_name\"$selected>$gemeente_name$score</OPTION>\n";
					endforeach;
					$out .= "</SELECT>\n";
					$out .= "<INPUT TYPE=\"SUBMIT\" VALUE=\"&gt;&gt;\">\n";
					$out .= "</DIV>\n";
					$out .= "</FORM>\n";
					// put the scores in an array
						$scores = array();
						$steps = 10;
						$steps_factor = 100 / $steps;
						foreach ( $gemeentes as $gemeente_name => $gemeente ){
							if ( $gemeente->score_procent >= 0 ):
								for ( $i = 1 ; $i <= $steps ; $i++){
									if ( ( floatval( $gemeente->score_procent ) - ( $steps_factor * $i ) ) <= 0 ):
										$scores[ $i ] .= "\"[GEMNAAM]\"=\"" . $gemeente->name . "\" OR ";
										break;
									endif;
								}
							elseif ( $gemeente->score_procent == -1 ):
								$scores[ 0 ] .= "\"[GEMNAAM]\"=\"" . $gemeente->name . "\" OR ";
							endif;
						}
				// make the map; show the map
					$base_layer = "scoregemeente";
					include( "100_makemap.inc" );
					include( "110_gemeente-showmap.inc" );
				// show gemeente_gegevens and contactpersoon and description
					if ( $search ):
						include( "200_gemeente-gegevens.inc" );
						include( "210_gemeente-scores.inc" );
						include( "220_gemeente-vragenlijst.inc" );
					endif;
			break;
			case $strings[ "provincie" ]:
					// put the scores in an array
						$scores = array();
						$steps = 20;
						$steps_factor = 100 / $steps;
						foreach ( $provincies as $provincie_name => $provincie ){
							if ( $provincie->score_procent >= 0 ):
								for ( $i = 1 ; $i <= $steps ; $i++){
									if ( ( floatval( $provincie->score_procent ) - ( $steps_factor * $i ) ) <= 0 ):
										$scores[ "$i" ] .= "'[PROVC_NM]'='" . $provincie->name . "' OR ";
										break;
									endif;
								}
							elseif ( $provincie->score_procent == -1 ):
								$scores[ "0" ] .= "'[PROVC_NM]'='" . $provincie->name . "' OR ";
							endif;
						}
					// make the map
						$base_layer = "scoreprovincie";
						include( "100_provincie_makemap.inc" );
					// display the provincie map
						$out .= "<DIV ID=\"map-area-topdivider\">";
						$out .= "</DIV>\n";
						$out .= "<DIV ID=\"map-area\">";
						$out .= "<DIV ID=\"map-large-provincie\">";
						$out .= "<IMG SRC=\"$image_url\" WIDTH=\"450\" HEIGHT=\"500\" BORDER=\"0\" ALT=\"\" TITLE=\"\">";
						$out .= "</DIV>\n";
					// show the list
						$out .= "<DIV ID=\"lijstje-provincie\">";
						$out .= "<TABLE>\n";
					//header
						$out .= "<TR>\n";
						$out .= "<TD ID=\"prov_naam\">{$strings[ "provincie" ]}</TD>\n";
						$out .= "<TD ID=\"prov_deelname\">{$strings[ "deelname" ]}</TD>\n";
						$out .= "<TD ID=\"prov_score\">{$strings[ "gemiddelde score" ]}</TD>\n";
						$out .= "</TR>\n";
					// scores per provincie
						foreach ( $provincies as $provincie_name => $provincie ){
							$out .= "<TR>\n";
							$out .= "<TD ID=\"prov_naam\">$provincie_name</TD>\n";
							if ( $provincie->score_procent  == -1 ):
								$out .= "<TD COLSPAN=\"2\" ID=\"prov_onbekend\">" . $strings[ "onbekend" ] . "</TD>\n";
							else:
								$out .= "<TD ID=\"prov_deelname\">" . number_format( $provincie->procent_deelname, 1, ",", "." ) . " %</TD>\n";
								$out .= "<TD ID=\"prov_score\">" . number_format( $provincie->score_procent, 1, ",", "." ) . " %</TD>\n";
							endif;
							$out .= "</TR>\n";
						}
						$out .= "</TABLE>\n";
						$out .= "</DIV>\n";
						$out .= "</DIV>\n";
			break;
			case $strings[ "antwoorden" ]:
				// if there is no question selected, show a list; else a map
					// compose url in case a gemeent has already been chosen ($search)
						if ( $search ):
							$out .= "<FORM ACTION=\"{$settings->site_url}/$year/$lang/$onderwerp/$view//$search\" METHOD=\"POST\">\n";
						else:
							$out .= "<FORM ACTION=\"{$settings->site_url}/$year/$lang/$onderwerp/$view\" METHOD=\"POST\">\n";
						endif;
					// show dropdown menu
						$out .= "<DIV ID=\"dropdown\">\n";
						$out .= "<SELECT NAME=\"search_vraagnummer\" SIZE=1 ONCHANGE=\"submit()\">\n";
						$out .= "<OPTION>";
						$out .= $strings[ "kieskaart" ];
						$out .= "</OPTION>\n";
						foreach( $vragen as $vraagnummer=>$vraag ):
							if ( $search_vraagnummer == $vraagnummer ):
								$selected = " SELECTED";
							else:
								$selected = "";
							endif;
							$out .= "<OPTION VALUE=\"$vraagnummer\"$selected>";
							$out .= substr( $vraagnummer . ". " . $vraag->title, 0, 125 );
							if ( strlen( substr( $vraagnummer . ". " . $vraag->title, 0, 125 ) ) > 124 ):
								$out .= "...";
							endif;
							$out .= "</OPTION>\n";
						endforeach;
						$out .= "</SELECT>\n";
						$out .= "<INPUT TYPE=\"SUBMIT\" VALUE=\"&gt;&gt;\">\n";
						$out .= "</DIV>\n";
						$out .= "</FORM>\n";
					// map or list of questions
						if ( $search_vraagnummer ):
							// map: get the gemeentes who answered yes
								$query  = "SELECT G.name ";
								$query .= "FROM ";
								$query .= " gemeente G, ";
								$query .= " gemeente2vraag G2V ";
								$query .= " WHERE ";
								$query .= " G2V.gemeente_id = G.id ";
								$query .= " AND G2V.vraag_id = '{$vragen[ $search_vraagnummer ]->id}'";
								$result = mysql_query( $query );
								while ( $row = mysql_fetch_object( $result ) ) {
									if ( in_array( $row->name, array( "s-Gravendeel", "s-Gravenhage", "s-Hertogenbosch" ) ) ):
										$row->name = "'" . $row->name;
									endif;
									$gemeentes[ $row->name ]->antwoord = "ja";
								}
							// put the scores in an array
								$scores = array();
								foreach ( $gemeentes as $gemeente_name => $gemeente ):
									$scores[ $gemeente->antwoord ] .= "\"[GEMNAAM]\"=\"" . $gemeente->name . "\" OR ";
								endforeach;
								krsort( $scores );
							// make the map; show the map
								$base_layer = "scoregemeente";
								include( "100_makemap.inc" );
								include( "110_vraag-showmap.inc" );
						else:
							// show list with each question is a link.
								$query  = "SELECT V.id AS vraag_id, ";
								$query .= " V.title_$lang AS vraag, V.score, ";
								$query .= " @vraagnummer:=@vraagnummer+1 vraagnummer ";
								$query .= " FROM ( SELECT @vraagnummer:=0 ) VNUM, vraag V ";
								$query .= " WHERE ";
								$query .= " V.active='Y' ";
								$query .= " AND V.indicator_id = '{$indicators[ $onderwerp ]->id}' ";
								$query .= " ORDER BY V.name ";
								$result = mysql_query( $query );
								
								$out .= "<DIV ID=\"vragenlijst\">\n";
								$out .= "<TABLE>\n";
								$out .= "<TR>\n";
								$out .= "<TD ID=\"vl-header-vraagnummer\">" . $strings[ "nr." ] . "</strong></TD>\n";
								$out .= "<TD ID=\"vl-header-vraag\">" . $strings[ "vraag" ] . "</TD>\n";
								$out .= "</TR>\n";
								while ( $row = mysql_fetch_object( $result ) ):
									$out .= "<TR>\n";
									$out .= "<TD ID=\"vl-vraagnummer\">$row->vraagnummer.</TD>\n";
									$out .= "<TD ID=\"vl-vraag\">";
									$out .= "<A HREF=\"{$settings->site_url}/$year/$lang/$onderwerp/$view/{$row->vraagnummer}";
									if ( $search ):
										$out .= "/$search";
									endif;
									$out .= "\">\n";
									$out .= $row->vraag;
									$out .= "</A>";
									$out .= "</TD>\n";
									$out .= "</TR>\n";
								endwhile;
								$out .= "</TABLE>\n";
								$out .= "</DIV>\n";
						endif;
			break;
			case $strings[ "ranglijst" ]:
				// sort on score_procent
					foreach( $gemeentes as $gemeente_name=>$gemeente ){
						if ( $gemeente->score_procent >= 0 ):
							$gemeentes_sorted[ ( 1000 + $gemeente->score_procent ) . $gemeente_name ] = array( "name" => $gemeente_name, "score_procent" => round( $gemeente->score_procent ) );
						endif;
					}
					krsort( $gemeentes_sorted );
				// ranglijst
				$out .= "<DIV ID=\"ranglijst\">";
				$out .= "<TABLE>";
				$santas_little_helper = 0;
				foreach( $gemeentes_sorted as $gemeente ){
					$out .= "<TR>";
					$out .= "<TD ID=\"rang-gem-nummer\">";
					$santas_little_helper++;
					$out .= $santas_little_helper;
					$out .= "</TD>\n";
					$out .= "<TD ID=\"rang-gem-naam\">";
					$out .= "<A HREF=\"{$settings->site_url}/$year/$lang/$onderwerp/{$strings[ "gemeente" ]}/{$gemeente[ "name" ]}\">";
					$out .= $gemeente[ "name" ];
					$out .= "</A></TD>\n";
					$out .= "<TD ID=\"rang-gem-balkje\">";
					$out .= "<IMG SRC=\"{$settings->site_url}/b.gif\" ";
					$out .= "ALT=\"{$gemeente[ "name" ]}: {$gemeente[ 'score_procent' ]}%\" ";
					$out .= "TITLE=\"{$gemeente[ "name" ]}: {$gemeente[ 'score_procent' ]}%\" ";
					$out .= "WIDTH=\"" . intval( $gemeente[ "score_procent" ] * 2 ) . "\" ";
					$out .= "HEIGHT=\"10\" BORDER=\"0\">";
					$out .= "</TD>\n";
					$out .= "<TD ID=\"rang-gem-score\">";
					$out .= $gemeente[ "score_procent" ];
					$out .= "%</TD>\n";
					$out .= "</TR>";
				}
				$out .= "</TABLE>";
				$out .= "</DIV>";
			break;
			case $strings[ "vragenlijst" ]:
				if ( $search ):
					include( "vragenlijst_verwerk.inc" );
				else:
					include( "vragenlijst.inc" );
				endif;
			break;
			default:
				echo "Programming error in switch staement in index.php";
			break;
		endswitch;
	endif;
// DIV credit-area
	switch ( $lang ):
		case "nl":
			$out .= "<DIV ID=\"credit-area\">\n";
			$out .= "<B>{$strings[ 'Duurzaamheidsmeter' ]} $year</B><BR>\n";
			$out .= "ontwikkeld door:  <A HREF=\"mailto:court@xs4all.nl\">Thijs de la Court</A><BR>\n";
			$out .= "inhoud:  <A HREF=\"mailto:r.vanleeuwen@cosnoordholland.nl\">Richard van Leeuwen</A> (COS Noord-Holland)<BR>\n";
			$out .= "graphic design: <A HREF=\"http://www.janveuger.com\" TARGET=\"_janveuger\">Jan Veuger</A><BR>\n";
			$out .= "database development &amp; hosting: <A HREF=\"http://www.suares.com\" TARGET=\"_suares\">Suares &amp; Co</A><BR>\n";
			$out .= "Aan deze website kunnen geen rechten worden ontleend.<BR>\n";
			$out .= "</DIV>\n";
		break;
		case "en":
			$out .= "<DIV ID=\"credit-area\">\n";
			$out .= "<B>{$strings[ 'Duurzaamheidsmeter' ]} $year</B><BR>\n";
			$out .= "developed by:  <A HREF=\"mailto:court@xs4all.nl\">Thijs de la Court</A><BR>\n";
			$out .= "content:  <A HREF=\"mailto:r.vanleeuwen@cosnoordholland.nl\">Richard van Leeuwen</A> (COS Noord-Holland)<BR>\n";
			$out .= "graphic design: <A HREF=\"http://www.janveuger.com\" TARGET=\"_janveuger\">Jan Veuger</A><BR>\n";
			$out .= "database development &amp; hosting: <A HREF=\"http://www.suares.com\" TARGET=\"_suares\">Suares &amp; Co</A><BR>\n";
			$out .= "From this Internet site no rights can be derived.<BR>\n";
			$out .= "</DIV>\n";
		break;
	endswitch;
// end HTML
	$out .= "</DIV>\n";
	// show tooltips for zoom_map
		if ( $settings->tooltips_on == "yes" ):
			$out .= $settings->tooltips;
		endif;
// Google Analytics
	$out .= "<script src=\"http://www.google-analytics.com/urchin.js\" type=\"text/javascript\"></script>\n";
	$out .= "<script type=\"text/javascript\">\n";
	$out .= "_uacct = \"UA-322327-6\";";
	$out .= "urchinTracker();\n";
	$out .= "</script>\n";
	$out .= "</BODY>\n";
	$out .= "</HTML>";

	echo $out;
?>