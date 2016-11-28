<?php
// licensed under GPL v2. (C) 2008 Ace Suares, Willemstad
	/*
		Copyright (C) 2008 Ace Suares, Gravenstraat 4, Otrobanda, Willemstad, Curaçao, Nehterlands Antilles
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
	$out  = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//NL\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"$lang\" lang=\"$lang\">\n";
    $out .="<head>\n";
	$out .= "<title>{$strings[ 'Duurzaamheidsmeter' ]} $year</title>\n
           <link rel=\"stylesheet\" type=\"text/css\" href=\"{$settings->site_url}/$year/{$settings->stylesheet}\">\n
           </head>\n";
// HTML body
	$out .= "<body>\n";
	$out .= "<div class=\"page-area\">\n";
	include( "lang-switch.inc" );
	// title
	$out .= "<h1 class=\"title-area\">{$strings[ 'Duurzaamheidsmeter' ]} $year</H1>\n";
	include( "tabs.inc");
	if ( $onderwerp == $strings["introductie" ] ):
		$query  = "SELECT ";
		$query .= " I.introductie_$lang AS inleiding ";
		$query .= " FROM indicator I ";
		$query .= " WHERE ";
		$query .= " I.id = 8 ";
		$result = mysql_query( $query );
		$row = mysql_fetch_object( $result );
		$out .= "<div class=\"introductie-tekst\">\n";
		$out .= nl2br( stripslashes( $row->inleiding ) );
		$out .= "</div>\n";
	else:
		switch ( $view ):
			case $strings[ "uitleg" ]:
				// introductietekst
					$out .= "<div class=\"introductie-tekst\">\n";
					$out .= nl2br( stripslashes ( $indicators[ $onderwerp ]->introductie ) );
					$out .= "</div>\n";
			break;
			case $strings[ "gemeente" ]:
				// show dropdown menu
					$out .= "<form action=\"{$settings->site_url}/$year/$lang/$onderwerp/$view\" method=\"post\">\n";
					$out .= "<div class=\"dropdown\">\n";
					$out .= "<select name=\"search\" size=1 onchange=\"submit()\">\n";
					$out .= "<option>";
					$out .= $strings[ "kiesgemeente" ];
					$out .= "</option>\n";
					foreach( $gemeentes as $gemeente_name=>$gemeente ):
						if ( $search == $gemeente_name ):
							$selected = " selected";
						else:
							$selected = "";
						endif;
						if ( $gemeente->score_procent == -1 ):
							$score = "";
						else:
							$score = " (" . sprintf( "%.2f", $gemeente->score_procent ) . "%)";
						endif;
						$out .= "<option value=\"$gemeente_name\"$selected>$gemeente_name$score</option>\n";
					endforeach;
					$out .= "</select>\n";
					$out .= "<input type=\"submit\" value=\"&gt;&gt;\">\n";
					$out .= "</div>\n";
					$out .= "</form>\n";
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
						$out .= "<div class=\"map-area-topdivider\">";
						$out .= "</div>\n";
						$out .= "<div class=\"map-area\">";
						$out .= "<div class=\"map-large-provincie\">";
						$out .= "<img src=\"$image_url\" width=\"450\" height=\"500\" border=\"0\" alt=\"\" title=\"\">";
						$out .= "</div>\n";
					// show the list
						$out .= "<div class=\"lijstje-provincie\">";
						$out .= "<table>\n";
					//header
						$out .= "<tr>\n";
						$out .= "<td class=\"prov_naam\">{$strings[ "provincie" ]}</td>\n";
						$out .= "<td class=\"prov_deelname\">{$strings[ "deelname" ]}</td>\n";
						$out .= "<td class=\"prov_score\">{$strings[ "gemiddelde score" ]}</td>\n";
						$out .= "</tr>\n";
					// scores per provincie
						foreach ( $provincies as $provincie_name => $provincie ){
							$out .= "<tr>\n";
							$out .= "<td class=\"prov_naam\">$provincie_name</td>\n";
							if ( $provincie->score_procent  == -1 ):
								$out .= "<td colspan=\"2\" class=\"prov_onbekend\">" . $strings[ "onbekend" ] . "</td>\n";
							else:
								$out .= "<td class=\"prov_deelname\">" . number_format( $provincie->procent_deelname, 1, ",", "." ) . " %</td>\n";
								$out .= "<td class=\"prov_score\">" . number_format( $provincie->score_procent, 1, ",", "." ) . " %</td>\n";
							endif;
							$out .= "</tr>\n";
						}
						$out .= "</table>\n";
						$out .= "</div>\n";
						$out .= "</div>\n";
			break;
			case $strings[ "antwoorden" ]:
				// if there is no question selected, show a list; else a map
					// compose url in case a gemeent has already been chosen ($search)
						if ( $search ):
							$out .= "<form action=\"{$settings->site_url}/$year/$lang/$onderwerp/$view//$search\" method=\"post\">\n";
						else:
							$out .= "<form action=\"{$settings->site_url}/$year/$lang/$onderwerp/$view\" method=\"post\">\n";
						endif;
					// show dropdown menu
						$out .= "<div class=\"dropdown\">\n";
						$out .= "<select name=\"search_vraagnummer\" size=1 onchange=\"submit()\">\n";
						$out .= "<option>";
						$out .= $strings[ "kieskaart" ];
						$out .= "</option>\n";
						foreach( $vragen as $vraagnummer=>$vraag ):
							if ( $search_vraagnummer == $vraagnummer ):
								$selected = " selected";
							else:
								$selected = "";
							endif;
							$out .= "<option value=\"$vraagnummer\"$selected>";
							$out .= substr( $vraagnummer . ". " . $vraag->title, 0, 125 );
							if ( strlen( substr( $vraagnummer . ". " . $vraag->title, 0, 125 ) ) > 124 ):
								$out .= "...";
							endif;
							$out .= "</option>\n";
						endforeach;
						$out .= "</select>\n";
						$out .= "<input type=\"submit\" value=\"&gt;&gt;\">\n";
						$out .= "</div>\n";
						$out .= "</form>\n";
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
								
								$out .= "<div class=\"vragenlijst\">\n";
								$out .= "<table>\n";
								$out .= "<tr>\n";
								$out .= "<td class=\"vl-header-vraagnummer\">" . $strings[ "nr." ] . "</strong></td>\n";
								$out .= "<td class=\"vl-header-vraag\">" . $strings[ "vraag" ] . "</td>\n";
								$out .= "</tr>\n";
								while ( $row = mysql_fetch_object( $result ) ):
									$out .= "<tr>\n";
									$out .= "<td class=\"vl-vraagnummer\">$row->vraagnummer.</td>\n";
									$out .= "<td class=\"vl-vraag\">";
									$out .= "<a href=\"{$settings->site_url}/$year/$lang/$onderwerp/$view/{$row->vraagnummer}";
									if ( $search ):
										$out .= "/$search";
									endif;
									$out .= "\">\n";
									$out .= $row->vraag;
									$out .= "</a>";
									$out .= "</td>\n";
									$out .= "</tr>\n";
								endwhile;
								$out .= "</table>\n";
								$out .= "</div>\n";
						endif;
			break;
			case $strings[ "ranglijst" ]:
				// sort on score_procent
					foreach( $gemeentes as $gemeente_name=>$gemeente ){
						if ( $gemeente->score_procent >= 0 ):
							$gemeentes_sorted[ ( 1000 + $gemeente->score_procent ) . $gemeente_name ] = array( "name" => $gemeente_name, "score_procent" => round( $gemeente->score_procent ) );
						endif;
					}
                    if ( !is_array($gemeentes_sorted) ):
                        $gemeentes_sorted = array();
                    endif;
					krsort( $gemeentes_sorted );
				// ranglijst
				$out .= "<div class=\"ranglijst\">";
				$out .= "<table>";
				$santas_little_helper = 0;
				foreach( $gemeentes_sorted as $gemeente ){
					$out .= "<tr>";
					$out .= "<td class=\"rang-gem-nummer\">";
					$santas_little_helper++;
					$out .= $santas_little_helper;
					$out .= "</td>\n";
					$out .= "<td class=\"rang-gem-naam\">";
					$out .= "<a href=\"{$settings->site_url}/$year/$lang/$onderwerp/{$strings[ "gemeente" ]}/{$gemeente[ "name" ]}\">";
					$out .= $gemeente[ "name" ];
					$out .= "</a></td>\n";
					$out .= "<td class=\"rang-gem-balkje\">";
					$out .= "<img src=\"{$settings->site_url}/b.gif\" ";
					$out .= "alt=\"{$gemeente[ "name" ]}: {$gemeente[ 'score_procent' ]}%\" ";
					$out .= "title=\"{$gemeente[ "name" ]}: {$gemeente[ 'score_procent' ]}%\" ";
					$out .= "width=\"" . intval( $gemeente[ "score_procent" ] * 2 ) . "\" ";
					$out .= "height=\"10\" border=\"0\">";
					$out .= "</td>\n";
					$out .= "<td class=\"rang-gem-score\">";
					$out .= $gemeente[ "score_procent" ];
					$out .= "%</td>\n";
					$out .= "</tr>";
				}
				$out .= "</table>";
				$out .= "</div>";
			break;
			case $strings[ "vragenlijst" ]:
				if ( $search ):
					include( "verwerk_vragenlijst.inc" );
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
    $out .= "<div class=\"credit-area\">\n";
    $out .= "<b>{$strings[ 'Duurzaamheidsmeter' ]} $year</b><br />\n";
    $out .= "ontwikkeld door: Thijs de la Court<br />\n";
    $out .= "inhoud:  <a href=\"http://www.cosnoordholland.nl\" target=\"_cosnhn\">COS Noord-Holland</a><br />\n";
    $out .= "graphic design: <a href=\"http://www.kukiko.com\" target=\"_kukiko\">Kuki &amp; Ko</a><br />\n";
    $out .= "database development &amp; hosting: <a href=\"http://www.suares.com\" target=\"_suares\">Suares &amp; Co</a><br />\n";
    $out .= "gis map geleverd door <a href=\"http://www.geodan.nl\" target=\"_geodan\">Geodan</a><br />\n";
    $out .= "Aan deze website kunnen geen rechten worden ontleend.<br />\n";
    $out .= "</div>\n";
// end HTML
	$out .= "</div>\n";
	// show tooltips for zoom_map
		if ( $settings->tooltips_on == "yes" ):
			$out .= $settings->tooltips;
		endif;
	$out .= "</body>\n";
	$out .= "</html>";

	echo $out;
?>