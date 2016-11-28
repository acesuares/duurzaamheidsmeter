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
if ( $gemeentes[ $gemeente->gemeente ]->score_procent < 0 ):	
	$out .= "<div class=\"vl-title\">";
	$out .= "$gemeente->gemeente " . $strings[ "deed niet mee met de  vragenlijst" ] . " '" . $indicators[ $onderwerp ]->title . "'";
	$out .= "</div>\n";
else:
	$query  = "SELECT V.id AS vraag_id, ";
	$query .= " V.title_$lang AS vraag, V.score, G2V.id AS determinator, ";
	$query .= " @vraagnummer:=@vraagnummer+1 vraagnummer ";
	$query .= " FROM ( SELECT @vraagnummer:=0 ) VNUM, vraag V LEFT JOIN gemeente2vraag G2V ";
	$query .= " ON G2V.vraag_id=V.id AND G2V.gemeente_id='$gemeente->id' ";
	$query .= " WHERE ";
	$query .= " V.active='Y' ";
	$query .= " AND V.indicator_id = '{$indicators[ $onderwerp ]->id}' ";
	$query .= " ORDER BY V.name ";
	$result = mysql_query( $query );
	
	$totaal_score = 0;
	
	$out .= "<h2 class=\"vl-title\">";
	$out .= $indicators[ $onderwerp ]->title . " - " . $strings[ "vragenlijst gemeente" ] . " $gemeente->gemeente";
	$out .= "</h2>\n";
	
	$out .= "<div class=\"vragenlijst\">\n";
	$out .= "<table>\n";
	$out .= "<tr>\n";
	$out .= "<td class=\"vl-header-vraagnummer\">" . $strings[ "nr." ] . "</strong></td>\n";
	$out .= "<td class=\"vl-header-vraag\">" . $strings[ "vraag" ] . "</td>\n";
	$out .= "<td class=\"vl-header-jaofnee\">" . $strings[ "antwoord" ] . "</td>\n";
	$out .= "<td class=\"vl-header-score\">" . $strings[ "score" ] . "</td>\n";
	$out .= "<td>&nbsp;</td>\n";
	$out .= "</tr>\n";
	while ( $row = mysql_fetch_object( $result ) ):
		$out .= "<tr>\n";
		$out .= "<td class=\"vl-vraagnummer\">$row->vraagnummer.</td>\n";
		$out .= "<td class=\"vl-vraag\">";
			$out .= "<a href=\"{$settings->site_url}/$year/$lang/$onderwerp/{$strings[ 'antwoorden' ]}/{$row->vraagnummer}/$search\">";
		$out .= $row->vraag;
		$out .= "</a></td>\n";
		$out .= "<td class=\"vl-jaofnee\">";
		if ( $row->determinator ):
			$out .= "ja";
			$totaal_score += $row->score;
		else:
			$out .=  "nee";
		endif;
		$out .= "</td>\n";
		$out .= "<td class=\"vl-score\">";
		if ( $row->determinator ):
			$out .= $row->score;
		else:
			$out .=  "&nbsp;";
		endif;
		$out .= "</td>\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "</tr>\n";
	endwhile;
// totaalscore 
	$out .= "<tr class=\"vl-totaalscore-row\">\n";
	$out .= "<td>&nbsp;</td>\n";
	$out .= "<td class=\"vl-totaalscore-tekst\">Totaalscore</td>\n";
	$out .= "<td>&nbsp;</td>\n";
	$out .= "<td class=\"vl-totaalscore-score\">$totaal_score</td>\n";
	$out .= "<td>&nbsp;</td>\n";
	$out .= "</tr>\n";
// correctie
	if ( $correctie->correctiefactor <> 0 ):
		//factor
		$out .= "<tr class=\"vl-cfactor-row\">\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "<td class=\"vl-cfactor-tekst\">Correctiefactor</td>\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "<td class=\"vl-cfactor-score\">$correctie->correctiefactor</td>\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "</tr>\n";
		// gecorrigeerd
		$out .= "<tr class=\"vl-cfactor-totaal-row\">\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "<td class=\"vl-cfactor-totaal-tekst\">Gecorrigeerde Totaalscore</td>\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "<td class=\"vl-cfactor-totaal-score\">" . ( $totaal_score + $correctie->correctiefactor ) . "</td>\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "</tr>\n";
	endif;
	// gecorrigeerd in procenten
	$out .= "<tr class=\"vl-gecorr-totaal-row\">\n";
	$out .= "<td>&nbsp;</td>\n";
	$out .= "<td class=\"vl-gecorr-totaal-tekst\"><a href=\"javascript:void(0)\">Totaalscore (inclusief eventuele correcties) in procenten<span>&nbsp;&nbsp;In principe kan een gemeente op vrijwel alle vragen 'ja' antwoorden. 
		Voor elk positief antwoord zijn punten te behalen. De totaalscore (in 
		procenten) geeft aan hoeveel procent positief beantwoord is. Uiteraard 
		moet dit beeld gerelativeerd worden. We kunnen veel vragen niet stellen. 
		Maar het resultaat is een goede indicatie van de prestatie van de gemeente.
		</span></a></td>\n";
	$out .= "<td>&nbsp;</td>\n";
	$out .= "<td class=\"vl-gecorr-totaal-score\">";
	$out .= round ( ( $totaal_score + $correctie->correctiefactor ) / $indicators[ $onderwerp ]->max_score * 100 );
	$out .= "</td>\n";
	$out .= "<td class=\"vl-gecorr-totaal-procentteken\">&nbsp;%</td>\n";
	$out .= "</tr>\n";
	// reden
	if ( $correctie->correctiereden <> "" ):
		$out .= "<tr class=\"vl-creden-row\">\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "<td colspan=4 class=\"vl-creden-tekst\">" . $strings[ "Reden voor correctie" ] . ": $correctie->correctiereden</td>\n";
		$out .= "</tr>\n";
	endif;
	$out .= "</table>\n";
	$out .= "</div>\n";
endif;
?>