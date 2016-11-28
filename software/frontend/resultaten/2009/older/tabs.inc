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
// show onderwerp tabs
	$out .= "<div class=\"tab-area\">\n";
	$out .= "<ul>\n";
	// andere onderwerpen
	foreach( $indicators as $indicator ){
		if ( $onderwerp == $indicator->code_lang ):
			$selected = " class=\"tab-selected\"";
		else:
			$selected = "";
		endif;
		$out .= "<li>";
		$out .= "<span$selected>";
		if ( $onderwerp == $strings["introductie" ] ):
			$out .= "<a href=\"{$settings->site_url}/$year/$lang/" . $indicator->code_lang . "\">";
		else:
			switch ( $view ):
				case $strings[ "antwoorden" ]:
					$out .= "<a href=\"{$settings->site_url}/$year/$lang/" . $indicator->code_lang . "/$view\">";
				break;
				default:
                    if ( $search == 'verwerk' ):
        				$out .= "<a href=\"{$settings->site_url}/$year/$lang/" . $indicator->code_lang . "/$view\">";
                    else:
    					$out .= "<a href=\"{$settings->site_url}/$year/$lang/" . $indicator->code_lang . "/$view/$search\">";
                    endif;
				break;
			endswitch;
		endif;
		$out .= strtolower( stripslashes( $indicator->title ) );
		$out .= "</a>";
		$out .= "</span>";
		$out .= "</li>\n";
	}
	$out .= "</ul>\n";
// show second row of tabs (with tooltips)
	if ( $onderwerp != $strings[ "introductie" ] ):
		$out .= "</div>\n";
		$out .= "<div class=\"tab-area\">\n";
		$out .= "<ul>\n";
		foreach( $views as $my_view ){
			if ( $view == $my_view ):
				$selected = " class=\"tab-selected\"";
			else:
				$selected = "";
			endif;
			$out .= "<li>";
			$out .= "<span$selected>";
			if ( $view != $strings[ "gemeente" ] && $my_view == $strings[ "gemeente" ] && $search ):
				$out .= "<a href=\"{$settings->site_url}/$year/$lang/$onderwerp/$my_view/$search\">";
			else:
				$out .= "<a href=\"{$settings->site_url}/$year/$lang/$onderwerp/$my_view\">";
			endif;
			$out .= strtolower( $my_view );
			$out .= "<span>&nbsp;&nbsp;";
			$out .= $strings[ "tooltip" ] [ $my_view ];
			$out .= "</span>";
			$out .= "</a>";
			$out .= "</span>";
			$out .= "</li>\n";
		}
		$out .= "</ul>\n";
	endif;
	$out .= "</div>\n";
// nav help
		$out .= "<p class=\"nav-help\">";
		$out .= strtolower ( $indicators[ $onderwerp ]->title );
		if ( $view ):
			$out .= " &gt; $view";
		endif;
		if ( $search_vraagnummer ):
			$out .= " &gt; {$strings[ 'vraag' ]} $search_vraagnummer";
		endif;
		if ( $search ):
			switch ( $view ):
				case $strings[ 'gemeente' ]:
					$out .= " &gt; $search";
				break;
				case $strings[ "vragenlijst" ]:
					$out .= " &gt; Verwerkingsresultaat";
				break;
				default:
					$out .= " &gt; {$strings[ 'gemeente' ]} $search";
				break;
			endswitch;
		endif;
		$out .= "</p>\n";
?>