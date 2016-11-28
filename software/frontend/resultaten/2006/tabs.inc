<?php
// show onderwerp tabs
	$out .= "<DIV ID=\"tab-area\">\n";
	$out .= "<UL>\n";
	// andere onderwerpen
	foreach( $indicators as $indicator ){
		if ( $onderwerp == $indicator->code_lang ):
			$selected = " ID=\"tab-selected\"";
		else:
			$selected = "";
		endif;
		$out .= "<LI>";
		$out .= "<SPAN$selected>";
		if ( $onderwerp == $strings["introductie" ] ):
			$out .= "<A HREF=\"{$settings->site_url}/$year/$lang/" . $indicator->code_lang . "\">";
		else:
			switch ( $view ):
				case $strings[ "antwoorden" ]:
					$out .= "<A HREF=\"{$settings->site_url}/$year/$lang/" . $indicator->code_lang . "/$view\">";
				break;
				default:
					$out .= "<A HREF=\"{$settings->site_url}/$year/$lang/" . $indicator->code_lang . "/$view/$search\">";
				break;
			endswitch;
		endif;
		$out .= strtolower( stripslashes( $indicator->title ) );
		$out .= "</A>";
		$out .= "</SPAN>";
		$out .= "</LI>\n";
	}
	$out .= "</UL>\n";
// show second row of tabs (with tooltips)
	if ( $onderwerp != $strings[ "introductie" ] ):
		$out .= "</DIV>\n";
		$out .= "<DIV ID=\"tab-area\">\n";
		$out .= "<UL>\n";
		foreach( $views as $my_view ){
			if ( $view == $my_view ):
				$selected = " ID=\"tab-selected\"";
			else:
				$selected = "";
			endif;
			$out .= "<LI>";
			$out .= "<SPAN$selected>";
			if ( $view != $strings[ "gemeente" ] && $my_view == $strings[ "gemeente" ] && $search ):
				$out .= "<A HREF=\"{$settings->site_url}/$year/$lang/$onderwerp/$my_view/$search\">";
			else:
				$out .= "<A HREF=\"{$settings->site_url}/$year/$lang/$onderwerp/$my_view\">";
			endif;
			$out .= strtolower( $my_view );
			$out .= "<SPAN>&nbsp;&nbsp;";
			$out .= $strings[ "tooltip" ] [ $my_view ];
			$out .= "</SPAN>";
			$out .= "</A>";
			$out .= "</SPAN>";
			$out .= "</LI>\n";
		}
		$out .= "</UL>\n";
	endif;
	$out .= "</DIV>\n";
// nav help
		$out .= "<P ID=\"nav-help\">";
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
		$out .= "</P>\n";
?>