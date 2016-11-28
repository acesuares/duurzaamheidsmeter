<?php
// display the large map
	$large_map_html = "<DIV ID=\"map-large\">
		<FORM ACTION=\"{$settings->site_url}/$year/$lang/$onderwerp/$view\" METHOD=\"POST\">
		<INPUT TYPE=\"image\" NAME=\"kaart\" SRC=\"$image_url\" WIDTH=\"450\" HEIGHT=\"500\" BORDER=\"0\" ALT=\"\" TITLE=\"\" ONCLICK=\"form.submit()\">
		</FORM>
		</DIV>\n";
// display the small map (zoom map) if needed		
	$zoom_map_html = "";
	if ( $display_zoom_map ):
		// zoom_map_html
		$zoom_map_html = "<DIV ID=\"map-zoom\">
			<IMG SRC=\"$image_zoom_url\" ALT=\"\" TITLE=\"\" ISMAP USEMAP=\"#map1\" WIDTH=\"306\" HEIGHT=\"500\" BORDER=\"0\">
			</DIV>\n";
		// prepare the imagemap
			$contents = file_get_contents( "/tmp/www.duurzaamheidsmeter.nl" . $image_zoom_imagemap_url );
			// replace 'empty' areas
			$contents = preg_replace ( "/<area href=\"javascript:Clicked\(''\);.*\n/", "", $contents );
			// replace gemeente-name with a link to the gemeente page
			$contents = preg_replace ( "/href=\"javascript:Clicked\('(.*)'\);\"/", "href=\"{$settings->site_url}/$year/$lang/$onderwerp/$view/$1\" onmouseover=\"return escape('$1')\" ", $contents );
			$contents = preg_replace ( "/''s/", "'\'s", $contents );
			// insert scores
			foreach ($gemeentes as $gemeente_name => $gemeente ){
				if ( $gemeente->score_procent == -1 ):
					$contents = str_replace ( "escape('" . addslashes( $gemeente->name ) . "')", "escape('" . addslashes( $gemeente->name ) . ": onbekend" . "')", $contents );
				else:
					$contents = str_replace ( "escape('" . addslashes( $gemeente->name ) . "')", "escape('" . addslashes( $gemeente->name ) . ": " . sprintf( "%.2f", $gemeente->score_procent ) . "%')", $contents );
				endif;
			}
			$out .= $contents;
			$settings->tooltips_on = "yes";
		// general info
	endif;
	$out .= "<DIV ID=\"map-area\">";
	$out .= $large_map_html . $zoom_map_html;
	$out .= "</DIV>\n";
?>