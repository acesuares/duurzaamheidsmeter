<?php
// licensed under GPL v2. (C) 2005 Ace Suares, Amsterdam/Willemstad
	/*
	Copyright (C) 2005 Ace Suares, Keizersgracht 132, 1015 CW Amsterdam
	http://www.suares.nl
	
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
// function click2map (to convert pixels to georef coordinates)
	function click2map ( $map, $click_x, $click_y ) {
		$x_pct = ( $click_x / $map->width );
		$y_pct = 1 - ( $click_y / $map->height );
		$x_map = $map->extent->minx + ( ( $map->extent->maxx - $map->extent->minx ) * $x_pct );
		$y_map = $map->extent->miny + ( ( $map->extent->maxy - $map->extent->miny ) * $y_pct );
		return array( $x_map, $y_map );
	}
// function map2click (to convert georef coordinates to pixels)
	function map2click ( $map, $bounds ) {
		$bounds_average_x = ( $bounds->maxx + $bounds->minx ) / 2;
		$bounds_average_y = ( $bounds->miny + $bounds->maxy ) / 2;
		$x_pct = ( ( $bounds_average_x - $map->extent->minx ) / ( $map->extent->maxx - $map->extent->minx ) );
		$y_pct = ( ( $bounds_average_y - $map->extent->miny )/ ( $map->extent->maxy - $map->extent->miny ) );
		$x_map = intval( $map->width * $x_pct );
		$y_map = intval( $map->height * (1 - $y_pct) );
		return array( $x_map, $y_map );
	}
// load php_mapscript
//	dl('php_mapscript.so');
// init some variables
	$map_file="dzhmeter_2005.map";
// colors
	$color[ 0 ] = "D0E7BD";
	$color[ 1 ] = "FFFFFE";
	$color[ 2 ] = "E5ECF5";
	$color[ 3 ] = "BECFE4";
	$color[ 4 ] = "7E9FCA";
	$color[ 5 ] = "396CAE";
// make a list of gemeentes, like this:
	$gemeentes = array();
	foreach ( $indicator_codes as $key => $indicator_code ){
		$indicator_id = $indicator_ids[ $key ];
		$indicator_max_score = $indicator_max_scores[ $key ];
		// get a list of gemeente-ids, gemeente-names, and scores for 2003
			$query  = " SELECT G.id, G.name, G.inwoneraantal, ${indicator_code}_procent_2003 AS score_procent ";
			$query .= " FROM gemeente G ";
			$result = mysql( $WebEngine_database, $query );
		// put the results in an array
			while ( $row = mysql_fetch_object( $result ) ) {
				$gemeentes[ $row->name ][ "id" ] = $row->id;
				$gemeentes[ $row->name ][ $indicator_code ][ "score_procent" ] = $row->score_procent;
				$gemeentes[ $row->name ][ $indicator_code ][ "datafromyear" ] = "2003";
				$gemeentes[ $row->name ][ "inwoneraantal" ] = $row->inwoneraantal;
			}
		// get a list of gemeente-ids, gemeente-names, and scores for 2005
			$query  = "SELECT ";
			$query .= " G.id, ";
			$query .= " G.name,  G.inwoneraantal,";
			$query .= " (((sum(V.score) + max(CF.correctie_factor))/$indicator_max_score)*100) AS score_procent ";
			
			$query .= "FROM ";
			$query .= " gemeente G, ";
			$query .= " gemeente2vraag G2V, ";
			$query .= " vraag V, ";
			$query .= " correctie CF ";
			
			$query .= " WHERE ";
			$query .= "  G2V.gemeente_id = G.id AND ";
			$query .= "  G2V.vraag_id = V.id AND ";
			
			$query .= "  CF.gemeente_id = G.id AND ";
			$query .= "  CF.indicator_id = '$indicator_id' AND ";
			
			$query .= "  V.active='Y' AND ";
			$query .= "  V.indicator_id = '$indicator_id' AND "; 
			$query .= "  G." . $indicator_code . "_datafromyear_id = '$current_datafromyear_id' ";
			$query .= " GROUP BY G.id ";
			
			$result = mysql( $WebEngine_database, $query );
		// put the results in an array
			while ( $row = mysql_fetch_object( $result ) ) {
				$gemeentes[ $row->name ][ "id" ] = $row->id;
				$gemeentes[ $row->name ][ $indicator_code ][ "score_procent" ] = $row->score_procent;
				$gemeentes[ $row->name ][ $indicator_code ][ "datafromyear" ] = "2005";
				$gemeentes[ $row->name ][ "inwoneraantal" ] = $row->inwoneraantal;
			}
	}
// put the scores in an array
	$scores = array();
	foreach ( $gemeentes as $gemeente_name => $gemeente ){
		$santaslittlehelper = 0;
		if ( substr( $gemeente->name, 0, 10) != "Amsterdam "):
			foreach ( $indicator_codes as $key => $indicator_code ){
				$santaslittlehelper += $gemeente[ $indicator_code ][ "score_procent" ];
			}
			$gemeentes[$gemeente_name][ "score_procent" ] = $santaslittlehelper / count( $indicator_codes );
			if ( $gemeentes[$gemeente_name][ "score_procent" ] > 0 ):
				for ( $i = 1 ; $i <= 5 ; $i++){
					if ( ( floatval( $gemeentes[$gemeente_name][ "score_procent" ] ) - ( 20 * $i ) ) < 0 ):
						$scores[ $i ] .= "'[GEMNAAM]'='" . $gemeente_name . "' OR ";
						break;
					endif;
				}
			endif;
		endif;
	}
	
// new map object              
	$map = ms_newMapObj( $map_file );
// assign scores to the layers
	foreach ( $scores as $key=>$value ){
		$value = "(" . substr( $value, 0 , -4 )  . ")";
		$s = $map->getlayerbyname("score" . $key );
		$s->setFilter( $value );
		$s->set( "status", MS_ON );
		$cl = $s->getClass(0);
		$sty = $cl->getStyle( 0 );
		$sty->color->setRGB( hexdec( substr( $color[ $key ], 0, 2) ), hexdec( substr( $color[ $key ], 2, 2) ), hexdec( substr( $color[ $key ], 4, 2) ) );
	}
// search by number
	if ( $spiegel == "gemeente" ):
		// find the name for this choice
			foreach ( $gemeentes as $naam => $gemeente ) {
				if ( $gemeente[ "id" ] == $choice ):
					$gemeente_naam = $naam;
					break;
				endif;
			}
	endif;
// search by name
	if ( isset( $gemeente_naam ) ) :
		// lookup this name in the shapefile
			// get the number of shapes.
				$shapeFile = ms_newShapefileObj( "/www/www.duurzaamheidsmeter.nl/maps2004/data/Nlge_r052"  , -1 );
				$numShapes = $shapeFile->numshapes;
				$shapeFile->free;
			// get the all layer
				$tmp_layer = $map->getlayerbyname("all") or die("couldnot open layer all");
				$tmp_layer->open();
				$shapeIndex = 0;
			// find the city
				unset( $bounds );
				while ( $shapeIndex < $numShapes ) {
					$shapeObject = $tmp_layer->getShape( -1, $shapeIndex );
					$shapeIndex++;
					if ( strtolower( $gemeente_naam ) == strtolower( $shapeObject->values["GEMNAAM"] ) ):
						$bounds = $shapeObject->bounds;
						break;
					endif;
				}
			// if we found bounds, then translate them in pixel coords
				if ( is_object( $bounds ) ):
					list( $kaart_x, $kaart_y ) = map2click( $map, $bounds );
				endif;
			// close the layer
				$tmp_layer->close();
				include( "strange_exceptions.inc" );
	endif;
// display map
	$display_zoom_map = "";
	if ( !isset( $kaart_x) ):
		// no choice has been made, we don't draw the small map, but display some text
		// draw the large map
			$map->selectOutputFormat("png");
			$image=$map->draw();
		// get the map URL
			$image_url=$image->saveWebImage();
	else:
		$display_zoom_map = "yes";
		list( $map_x, $map_y ) = click2map( $map, $kaart_x, $kaart_y );
		$p_geo = ms_newPointObj();
		$p_geo->setXY( $map_x, $map_y );
		// query the 'all' layer to see if we are in there.
			$tmp_layer = $map->getlayerbyname("all") or die("couldnot open layer all");
			$tmp_layer->set( "status", MS_ON );
			if ( @$tmp_layer->queryByPoint($p_geo, MS_SINGLE, -1) == MS_SUCCESS ):
				// we have a hit. There is always only one result.
					 $tmp_results = $tmp_layer->getResult(0);
					 $tmp_layer->open();
					 $shape_object = $tmp_layer->getShape( -1, $tmp_results->shapeindex );
					 $tmp_layer->close();
					 $bounds = $shape_object->bounds;
					 // calculate new pixel x,y from georef bounds.
						// convert to pixels
							list( $kaart_x, $kaart_y) = map2click( $map, $bounds );
				else:
					// outside a gemeente.
				endif;
		// highlight the found shape
			$tmp_filter = "('[GEMNAAM]'='" . $shape_object->values["GEMNAAM"] . "')";
			$tmp_layer = $map->getlayerbyname("selected" );
			$tmp_layer->setFilter( $tmp_filter );
			$tmp_layer->set( "status", MS_ON );
		// draw the large map
			$map->selectOutputFormat("png");
			$image=$map->draw();
		// get the map URL
			$image_url=$image->saveWebImage();
		// make the small map
			$p_pix = ms_newPointObj();
			$p_pix->setXY( $kaart_x, $kaart_y );
			$zoom_factor = 8;
			$map->set( "width", 324 );
			$map->set( "height", 324 );
			$map->zoompoint( $zoom_factor, $p_pix, 450, 500, $map->extent );
		// switch on gemeentenamen
			$tmp_layer = $map->getlayerbyname("gemeentenamen" );
			$tmp_layer->set( "status", MS_ON );
		// switch off legend
			$map->legend->set( "status", MS_OFF );
		// draw the zoomed map
			$image_zoom=$map->draw();
		// get the map URL
			$map->selectOutputFormat("png");
			$image_zoom_url=$image_zoom->saveWebImage();
		// make the imagemap
			$map->selectOutputFormat("imagemap");
			$image_zoom=$map->draw();
			$image_zoom_imagemap_url=$image_zoom->saveWebImage();
	endif;			

// start html
	$html_out .= "<TABLE BGCOLOR=\"#CDE6BD\" ";
	$html_out .= "WIDTH=\"780\" BORDER=\"0\" ";
	$html_out .= "CELLPADDING=\"5\" CELLSPACING=\"1\"";
	$html_out .= 	">";
// title
	$html_out .= "<TR>\n";
	$html_out .= "<TD WIDTH=\"100%\" BGCOLOR=\"#FFFFFF\" VALIGN=\"TOP\" ALIGN=\"LEFT\">";
	$html_out .= $sorteerbalk_font;
	$html_out .= "<FONT SIZE=\"5\" COLOR=\"#000000\">$kaart_title</FONT>";
	$html_out .= "</FONT>";
	$html_out .= "</TD>\n";
	$html_out .= "</TR>\n";
// the maps in html
	$map_html = "";
	$map_html .= "<TABLE WIDTH=\"100%\" BORDER=\"0\">\n";
		$map_html .= "<TR>\n";
			// main map
			$map_html .= "<TD>\n";
			$map_html .= "<IMG SRC=\"$image_url\" WIDTH=\"450\" HEIGHT=\"500\" ALT=\"Overzichtskaart\" TITLE=\"Overzichtskaart\" ONCLICK=\"form.submit()\">";
			$map_html .= "</TD>\n";
			// zoom map 
			$map_html .= "<TD VALIGN=\"TOP\">\n";
			if ( $display_zoom_map ):
				// inner table
				$map_html .= "<TABLE WIDTH=\"100%\" BORDER=\"0\">\n";
					// zoom map
					$map_html .= "<TR>\n";
						$map_html .= "<TD>\n";
							if ( $shape_object->values["GEMNAAM"] ):
								$zoom_map_title = "Gemeente " . $shape_object->values["GEMNAAM"] . " en Omgeving";
							else:
								$zoom_map_title = "";
							endif;
							$map_html .= "<IMG SRC=\"$image_zoom_url\" ALT=\"$kaart_title - $zoom_map_title\" TITLE=\"$kaart_title - $zoom_map_title\" BORDER=\"1\" WIDTH=\"300\">";
						$map_html .= "</TD>\n";
					$map_html .= "</TR>\n";
					// general info
					$map_html .= "<TR>\n";
						$map_html .= "<TD>\n";
							$show_table = "yes";
							if ( $shape_object->values["GEMNAAM"] ):
								$map_html .= "<FONT FACE=\"Verdana, Arial\" COLOR=\"#000000\" SIZE=\"2\">";
								$map_html .= "<BR><B>Gemeente: ";
								$map_html .= $shape_object->values["GEMNAAM"];
								$map_html .= "</B><BR>Provincie: ";
								$map_html .= $shape_object->values["PROVC_NM"];
								$map_html .= "<BR>Inwoners: ";
								$map_html .= number_format( $gemeentes[ $shape_object->values["GEMNAAM"] ][ "inwoneraantal" ] , 0 , "," , "." );
								if ( $gemeentes[ $shape_object->values["GEMNAAM"] ] ):
									$map_html .= "<BR><B>Score ";
									$map_html .= sprintf( "%.2f", $gemeentes[ $shape_object->values["GEMNAAM"] ][ "score_procent" ] );
									$map_html .= " %</B>\n";
								else:
									$map_html .= "<BR><B>Van deze gemeente zijn geen gegevens bekend.</B>";
									$show_table = "";
								endif;
							else:
								$map_html .= "<FONT FACE=\"Verdana, Arial\" COLOR=\"#000000\" SIZE=\"2\">";
								$map_html .= "Kies een gemeente door op de overzichtskaart te klikken.";
							endif;
						$map_html .= "</TD>\n";
					$map_html .= "</TR>\n";
				$map_html .= "</TABLE>\n";
			else:
				// text (we haven't choosen a gemeente yet
				$map_html .= "<TABLE WIDTH=\"100%\" CELLSPACING=\"0\" CELLPADDING=\"10\" BORDER=\"0\">\n";
					$map_html .= "<TR>\n";
					$map_html .= "<TD ALIGN=\"LEFT\" VALIGN=\"TOP\">\n";
					$map_html .= "<BR>\n";
					$map_html .= $kaart_text;
					$map_html .= "</TD>\n";
					$map_html .= "</TR>\n";
				$map_html .= "</TABLE>\n";
			endif;

			$map_html .= "</TD>\n";
		$map_html .= "</TR>\n";
	$map_html .= "</TABLE>\n";

	
	$html_out .= "<TR>\n";
	$html_out .= t_content( "100%", "CDE6BD", $t_font, $map_html, "CENTER" );
	$html_out .= "</TR>\n";
// spacer
	$html_out .= "<TR>\n";
	$html_out .= "<TD WIDTH=\"100%\" BGCOLOR=\"#FFFFFF\" VALIGN=\"TOP\" ALIGN=\"LEFT\">";
	$html_out .= "<BR>";
	$html_out .= "</TD>\n";
	$html_out .= "</TR>\n";
// end table
	$html_out .= "</TABLE>\n";
	$choice = $gemeentes[ $shape_object->values["GEMNAAM"] ][ "id" ];

	// Check if this choice exists...
	// Gemeente

			// get the scores
				$query  = "SELECT ";
				$query .= "G.name AS gemeente,";
				$query .= " sum(V.score) AS score ";
				
				$query .= " FROM gemeente2vraag G2V, ";
				$query .= " vraag V, ";
				$query .= " gemeente G ";
				
				$query .= " WHERE G.id='$choice' ";
				$query .= " AND V.active='Y' ";
				$query .= " AND G2V.vraag_id = V.id ";
				$query .= " AND G2V.gemeente_id = '$choice' ";
				$query .= " GROUP BY G.id ";

				$result=mysql($WebEngine_database,$query);

				// check for existence
				if ( mysql_num_rows($result) ):
					include( "gemeente.inc" );
				endif;
?>
