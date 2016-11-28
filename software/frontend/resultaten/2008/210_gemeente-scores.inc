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
// blah
	$query = "SELECT sum(V.score) AS score,
       CF.correctie_factor AS correctiefactor,
       CF.correctie_reden AS correctiereden,
			 kl_datafromyear_id + 2001 AS kl_datafromyear, 
			 so_datafromyear_id + 2001 AS so_datafromyear,
			 rg_datafromyear_id + 2001 AS rg_datafromyear,
			 di_datafromyear_id + 2001 AS di_datafromyear,
			 di_datafromyear_id + 2001 AS ka_datafromyear,
       I.code,
       I.code_$lang AS code_lang
       FROM gemeente2vraag G2V,
       vraag V, indicator I,
       gemeente G,
       correctie CF
       WHERE G.id='$gemeente->id'
       AND V.active='Y'
       AND V.indicator_id = I.id
       AND V.indicator_id <> 8 
       AND G2V.vraag_id = V.id 
       AND G2V.gemeente_id = '$gemeente->id'
       AND CF.gemeente_id = '$gemeente->id'
       AND CF.indicator_id=I.id
       GROUP BY I.id
       ORDER BY I.name ";
  	$result = mysql_query( $query );
  	if ( mysql_num_rows( $result ) > 0 ):
		$out .= "<div class=\"score\">\n";
  		$out .= "<table>\n";
		$out .= "<tr class=\"score-header-row\">\n";
		$out .= "<td class=\"score-header-indicator\">" . $strings[ "indicator" ] . "</td>\n";
		$out .= "<td class=\"score-header-indicator\">" . $strings[ "datafromyear" ] . "</td>\n";
		$out .= "<td class=\"score-header-score\">" . $strings[ "score" ] . "</td>\n";
		$out .= "<td class=\"score-header-cfactor\">" . $strings[ "correctiefactor" ] . "</td>\n";
		$out .= "<td class=\"score-header-scoreprocent\">" . $strings[ "score" ] . " (%)</td>\n";
		$out .= "</tr>\n";
  		while( $row = mysql_fetch_object( $result ) ){
  			// save correctiefactor and _reden
  			if ( $row->code_lang == $onderwerp ):
  				$correctie = $row;
  			endif;
  			$out .= "<tr class=\"score-row\">\n";
  			$out .= "<td class=\"score-indicator\">";
  			$out .= "<a href=\"{$settings->site_url}/$year/$lang/$row->code_lang/$view/$search\">";
  			$out .= $indicators[ $row->code_lang ]->title;
  			$out .= "</a>";
  			$out .= "</td>\n";
  			$out .= "<td class=\"score-indicator\">";
				$itchy = $row->code . "_datafromyear";
  			$out .= $row->$itchy;
  			$out .= "</td>\n";
  			$out .= "<td class=\"score-score\">";
  			$out .= $row->score;
  			$out .= "</td>";
  			$out .= "<td class=\"score-cfactor\">";
  			$out .= $row->correctiefactor;
  			$out .= "</td>\n";
  			$out .= "<td class=\"score-scoreprocent\">";
  			$out .= round ( ( $row->score + $row->correctiefactor ) / $indicators[ $row->code_lang ]->max_score * 100 ) . " %";
  			$out .= "</td>\n";
  			$out .= "</tr>\n";
  		}
  		$out .= "</table>\n";
		$out .= "</div>\n";
	endif;
?>