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
// display the large map
	$large_map_html = "<div class=\"map-large\">
		<form action=\"{$settings->site_url}/$year/$lang/$onderwerp/$view\" method=\"post\">
		<input type=\"image\" name=\"kaart\" src=\"$image_url\" width=\"450\" height=\"500\" border=\"0\" alt=\"\" title=\"\" onclick=\"form.submit()\">
		</form>
		</div>\n";
         if ( $archive ):
           $large_map_html = "<div class=\"map-large\">
              <img src=\"$image_url\" width=\"450\" height=\"500\" border=\"0\" alt=\"\" title=\"\" />
             </div>\n";
	endif;
// display the small map (zoom map) if needed		
	$zoom_map_html = "";
	if ( $display_zoom_map ):
         if ( ! $archive ):
		// zoom_map_html
		$zoom_map_html = "<div class=\"map-zoom\">
			<img src=\"$image_zoom_url\" alt=\"\" title=\"\" ismap usemap=\"#map1\" width=\"370\" height=\"500\" border=\"0\">
			</div>\n";
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
	  else:

		$zoom_map_html = "<div class=\"map-zoom\">
			<img src=\"$image_zoom_url\" alt=\"\" title=\"\" width=\"370\" height=\"500\" border=\"0\">
			</div>\n";
			$settings->tooltips_on = "no";
	  endif;

	endif;
	$out .= "<div class=\"map-area\">";
	$out .= $large_map_html . $zoom_map_html;
	$out .= "</div>\n";
?>