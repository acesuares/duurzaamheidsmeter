<?php
// licensed under GPL v2. (C) 2008-2013 Ace Suares, Willemstad
	/*
		Copyright (C) 2008-2013 Ace Suares, Gravenstraat 4, Otrobanda, Willemstad, Curaçao, Nehterlands Antilles
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
		$query  = "SELECT V.id, ";
		$query .= " V.name, V.title_$lang AS vraag, V.score,
								IF( V.telt_mee_totaalscore = 'Y', V.score, 0 ) as score_voor_totaal,
								V.helptext_$lang AS helptext, ";
		$query .= " @vraagnummer:=@vraagnummer+1 vraagnummer ";
		$query .= " FROM ( SELECT @vraagnummer:=0 ) VNUM, vraag V ";
		$query .= " WHERE ";
		$query .= " V.active='Y' ";
		$query .= " AND V.indicator_id = '{$indicators[ $onderwerp ]->id}' ";
		$query .= " ORDER BY V.name ";

		$result = mysql_query( $query );
		
		$max_score = 0;
		
		$out .= "<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Download de vragenlijsten <a href='http://www.duurzaamheidsmeter.nl/index.php?page_id=7' target='_nsdnndndnqwndqnqweqw'>hier</a>!";

		$out .= "<form method=\"post\" action=\"{$settings->site_url}/$year/$lang/$onderwerp/$view/{$strings[ 'verwerk' ]}\">\n";
		$out .= "<div id=\"vragenlijst\">\n";
		$out .= "<table>\n";
		$out .= "<tr>\n";
		$out .= "<td class=\"vl-vraagnummer\"><strong>" . $strings[ "nr." ] . "</strong></td>\n";
		$out .= "<td class=\"vl-vraag\"><strong>" . $strings[ "vraag" ] . "</strong></td>\n";
		$out .= "<td class=\"vl-score\"><strong>" . $strings[ "score" ] . "</strong></td>\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "</tr>\n";
		while ( $row = mysql_fetch_object( $result ) ):
include('tussenkopjes.inc');
			$out .= "<tr>\n";
			$out .= "<td class=\"vl-vraagnummer\">$row->vraagnummer.</td>\n";
			$out .= "<td class=\"vl-vraag\"><a href=\"javascript:void(0)\">" . utf8_encode($row->vraag) . "<span>&nbsp;&nbsp;" . $row->helptext . "</span></a></td>\n";
			$out .= "<td class=\"vl-score\">";
			$out .= $row->score;
			$max_score += $row->score_voor_totaal;
			$out .= "</td>\n";
			$out .= "<td class=\"vl-ja\">";
			$out .= "<input type=\"radio\" name=\"vraag[{$row->id}]\" value=\"Y\">ja\n";
			$out .= "</td>\n";
			$out .= "<td class=\"vl-nee\">";
			$out .= "<input type=\"radio\" name=\"vraag[{$row->id}]\" checked value=\"N\">nee\n";
			$out .= "</td>\n";
			$out .= "</tr>\n";
		endwhile;
// max score 
	$out .= "<tr>\n";
	$out .= "<td>&nbsp;</td>\n";
	$out .= "<td class=\"vl-maxscore-tekst\">Maximum score</td>\n";
	$out .= "<td class=\"vl-maxscore-score\">$max_score</td>\n";
	$out .= "<td>&nbsp;</td>\n";
	$out .= "<td>&nbsp;</td>\n";
	$out .= "</tr>\n";
	$out .= "</table>\n";
	$out .= "</div>\n";
if ( ! $archive ):
// invulformulier gemeente
	$out .= "<div class=\"invul-area\">\n";
// invul helptekst
	$out .= "<p class=\"invul-help\">\n";
	$out .= "Als u de vragen hierboven hebt beantwoord, s.v.p ook de 
					onderstaande gegevens invullen.";
	$out .= "</p>\n";
// invul gemeente dropdown
	$out .= "<p class=\"invul-dropdown\">\n";
	$out .= "<select name=\"gemeente_id\" size=1>\n";
	$out .= "<option value=\"0\">";
	$out .= $strings[ "kiesgemeente-invul" ];
	$out .= "</option>\n";
	foreach( $gemeentes as $gemeente_name=>$gemeente ):
		$out .= "<option value=\"{$gemeente->id}\">$gemeente_name ({$gemeente->provincie})</option>\n";
	endforeach;
	$out .= "</select>\n";
	$out .= "</p>\n";
// invuller gegevens
	$out .= "<p class=\"invul-invuller-help\">\n";
	$out .= "Gegevens invuller:";
	$out .= "</p>\n";
	$out .= "<p class=\"invul-invuller\">\n";
	$out .= "<table>\n";
	$out .= "<tr>\n";
	$out .= "<td class=\"invul-header\">&nbsp;</td>\n";
	$out .= "<td class=\"invul-inputtext\">";
	$out .= "<select name=\"invuller_aanspreektitel\" size=1>\n";
	$out .= "<option value=\"Dhr/Mevr\" selected>Dhr/Mevr</option>\n";
	$out .= "<option value=\"Dhr\">Dhr</option>\n";
	$out .= "<option value=\"Mevr\">Mevr</option>\n";
	$out .= "</select>\n";
	$out .= "</tr>\n";
	foreach ( $settings->invuller_formfields as $formfield_name => $formfield_description ){
		$out .= "<tr>\n";
		$out .= "<td class=\"invul-header\">$formfield_description</td>\n";
		$out .= "<td class=\"invul-inputtext\">";
		$out .= "<input type=\"text\" size=\"60\" name=\"$formfield_name\"></td>\n";
		$out .= "</tr>\n";
	}
	$out .= "</table>\n";
	$out .= "</p>\n";
// contactambtenaar
	$out .= "<p class=\"invul-contactambtenaar-help\">\n";
	$out .= "Gegevens gemeentelijke contactpersoon (indien anders dan de invuller):";
	$out .= "</p>\n";
	$out .= "<p class=\"invul-contactambtenaar\">\n";
	$out .= "<table>\n";
	$out .= "<tr>\n";
	$out .= "<td class=\"invul-header\">Naam contactambtenaar</td>\n";
	$out .= "<td class=\"invul-inputtext\">";
	$out .= "<input type=\"text\" size=\"60\" name=\"contact_ambtenaar_naam\"></td>\n";
	$out .= "</tr>\n";
	$out .= "<tr>\n";
	$out .= "<td class=\"invul-header\">Telefoon contactambtenaar</td>\n";
	$out .= "<td class=\"invul-inputtext\">";
	$out .= "<input type=\"text\" size=\"60\" name=\"contact_ambtenaar_tel</td>\">";
	$out .= "</tr>\n";
	$out .= "</table>\n";
	$out .= "</p>\n";
// correctiegedoe
	$out .= "<p class=\"invul-correctie-help\">\n";
	$out .= "Toe te passen correctie (zie handleiding):";
	$out .= "</p>\n";
	$out .= "<p class=\"invul-correctiefactor\">\n";
	$out .= "<table>\n";
	$out .= "<tr>\n";
	$out .= "<td class=\"invul-header\">Correctiefactor</td>\n";
	$out .= "<td class=\"invul-inputtext\">";
	$out .= "<select name=\"correctiefactor\">\n";
	for ( $itchy = -( $indicators[ $onderwerp ]->maxcorr ); $itchy <= $indicators[ $onderwerp ]->maxcorr; $itchy++ ){
		$out .= "<option value=\"$itchy\" ";
		if ( 0 == $itchy) { $out .= " selected"; }
		$out .= ">$itchy</option>\n";
	}
	$out .= "</select> punten.</td>\n";
	$out .= "</tr>\n";
	$out .= "<tr>\n";
	$out .= "<td class=\"invul-header\">Als u de score corrigeert, vul hier de reden in.</td>\n";
	$out .= "<td class=\"invul-inputtext\">";
	$out .= "<textarea rows=20 cols=60 name=\"correctiereden\"></textarea></td>\n";
	$out .= "</tr>\n";
	$out .= "</table>\n";
	$out .= "</p>\n";
// submit
	$out .= "<p class=\"invul-submit-help\">\n";
	$out .= "Heeft u alles ingevuld?
		Wij adviseren u dit formulier te printen voor eigen gebruik.
		Klik vervolgens op 'verzenden' om uw gegevens te versturen.";
	$out .= "</p>\n";
	$out .= "<p class=\"invul-submit\">\n";
	$out .= "<input type=\"submit\" value=\"verzenden\"></td>\n";
	$out .= "</p>\n";
endif;
	
	$out .= "</div>\n";
	$out .= "</form>\n";

?>