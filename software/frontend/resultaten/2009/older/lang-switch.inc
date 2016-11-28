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
// link naar hoofdstie en taalbuttons
	$out .= "<div class=\"link-en-taal\">";
	
	$out .= "|&nbsp;<a href=\"{$settings->parent_url}/{$languages_long[ $lang ]}\">{$strings[ 'home' ]}</a>&nbsp;| ";
/*	
	foreach ( $languages as $language ){
		$href = "{$settings->site_url}/$year/$language";
		if ( $onderwerp ):
			$href .= "/";
			$href .= $indicators[ $onderwerp ]->{"code_$language"};
		endif;
		if ( $view ):
			$href .= "/";
			$itchy = array_keys( $views , $view );
			$href .= $translations[ $language ][ "views" ][ $itchy[ 0 ] ];
		endif;
		if ( $view == $strings[ "antwoorden" ] ):
			if ( $search_vraagnummer ):
				$href .= "/" . $search_vraagnummer;
				if ( $search ):
					$href .= "/" . $search;
				endif;
			endif;
		else:
			if ( $search ):
				$href .= "/" . $search;
			endif;
		endif;
		$out .= "<A HREF=\"$href\">";
		$out .= "<IMG SRC=\"{$settings->site_url}/flag_$language.gif\" WIDTH=\"15\" HEIGHT=\"9\" ALT=\"{$languages_longer[ $language ]}\" TITLE=\"{$languages_longer[ $language ]}\" STYLE=\"border: solid 1px white;\">";
		$out .= "&nbsp;</A>";
	}
*/
	$out .= "</div>";
?>