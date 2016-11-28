<?php
// display the large map
      if ( ! $archive ):
	$large_map_html  = "<DIV ID=\"map-large\">";
	if ( $search_vraagnummer ):
	  $large_map_html .= "<FORM ACTION=\"{$settings->site_url}/$year/$lang/$onderwerp/$view/$search_vraagnummer\" METHOD=\"POST\">
			<INPUT TYPE=\"image\" NAME=\"kaart\" SRC=\"$image_url\" WIDTH=\"450\" HEIGHT=\"500\" BORDER=\"0\" ALT=\"\" TITLE=\"\" ONCLICK=\"form.submit()\">
			</FORM>";
	else:
	  $large_map_html .= "<IMG NAME=\"kaart\" SRC=\"$image_url\" WIDTH=\"450\" HEIGHT=\"500\" BORDER=\"0\" ALT=\"Kies eerst een vraag!\" TITLE=\"Kies eerst een vraag!\">";
	endif;
	$large_map_html .= "</DIV>\n";
      else:
        $large_map_html  = "<div id=\"map-large\">";
        $large_map_html .= "<img name=\"kaart\" src=\"$image_url\" width=\"450\" height=\"500\" border=\"0\" alt=\"\" title=\"\" />";
        $large_map_html .= "</div>\n";
      endif;
// display the small map (zoom map) if needed		
	if ( $display_zoom_map ):
          if ( ! $archive ):
		// zoom_map_html
		$zoom_map_html = "<DIV ID=\"map-zoom\">
			<IMG SRC=\"$image_zoom_url\" ALT=\"\" TITLE=\"\" ISMAP USEMAP=\"#map1\" WIDTH=\"306\" HEIGHT=\"500\" BORDER=\"0\">
			</DIV>\n";
		// prepare the imagemap
			$contents = file_get_contents( "/tmp/www.duurzaamheidsmeter.nl" . $image_zoom_imagemap_url );
			// replace 'empty' areas
			$contents = preg_replace ( "/<area href=\"javascript:Clicked\(''\);.*\n/", "", $contents );
			$contents = preg_replace ( "/''s/", "'\'s", $contents );
			// replace gemeente-name with a link to the gemeente page
			if ( $search_vraagnummer ):
				$contents = preg_replace ( "/href=\"javascript:Clicked\('(.*)'\);\"/", "href=\"{$settings->site_url}/$year/$lang/$onderwerp/$view/$search_vraagnummer/$1\" onmouseover=\"return escape('$1')\" ", $contents );
			else:
				$contents = preg_replace ( "/href=\"javascript:Clicked\('(.*)'\);\"/", "href=\"{$settings->site_url}/$year/$lang/$onderwerp/$view/$1\" onmouseover=\"return escape('$1')\" ", $contents );
			endif;
			// insert scores
			foreach ($gemeentes as $gemeente_name => $gemeente ){
				$contents = str_replace ( "escape('" . addslashes( $gemeente->name ) . "')", "escape('" . addslashes( $gemeente->name ) . ": " . $gemeente->antwoord . "')", $contents );
			}
			$out .= $contents;
			$settings->tooltips_on = "yes";
          else:
                // zoom_map_html
                $zoom_map_html = "<div id=\"map-zoom\">
                        <img src=\"$image_zoom_url\" alt=\"\" title=\"\" width=\"370\" height=\"500\" border=\"0\">
                        </div>\n";
                $settings->tooltips_on = "yes";
          endif;
		// general info
	else:
	// text (we haven't choosen a gemeente yet
		$zoom_map_html  = "<DIV ID=\"map-text\">\n";
		$zoom_map_html .= nl2br( stripslashes( $indicators[ $onderwerp ]->tekst_kaartje ) );
		$zoom_map_html .= "</DIV>\n";
	endif;
	$out .= "<DIV ID=\"map-vraag\">";
	$out .= $vragen[ $search_vraagnummer ]->title;
	$out .= "</DIV>\n";
	$out .= "<DIV ID=\"map-area\">";
	$out .= $large_map_html . $zoom_map_html;
	$out .= "</DIV>\n";
?>