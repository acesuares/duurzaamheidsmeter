<?php
// link naar hoofdstie en taalbuttons
	$out .= "<DIV ID=\"link-en-taal\">";
	
	$out .= "|&nbsp;<A HREF=\"{$settings->parent_url}/{$languages_long[ $lang ]}\">{$strings[ 'home' ]}</A>&nbsp;| ";
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
	$out .= "</DIV>";
?>