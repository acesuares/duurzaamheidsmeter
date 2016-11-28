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
// new map object              
	$map = ms_newMapObj( $settings->map_file );
// make the gradient
	if ( $view == $strings[ "antwoorden" ] ):
		$colors = $colors_vragen;
	else:
		$colors = gradient( $color_gradient_start, $color_gradient_end, $steps );
		array_unshift ( $colors, $color_unknown );
		ksort( $scores );
	endif;
// assign scores to the layers
	foreach ( $scores as $key=>$value ){
		$layer = $map->getlayerbyname( $base_layer );
		$s = ms_newLayerObj( $map, $layer );
		$value = "(" . substr( $value, 0 , -4 )  . ")";
		$value = str_replace("'","\'",$value); // after upgrade ' is illegal. oct 2010
		$s->setFilter( $value );
		$s->set( "status", MS_ON );
		$cl = $s->getClass(0);
		switch ( $key ):
			case "0":
				$cl->set( "name", $strings[ "onbekend" ] );
			break;
			case "onbekend":
			case "ja":
			case "nee":
				$cl->set( "name", $strings[ $key ] );
			break;
			default:
				$cl->set( "name", strval( ( $key - 1 ) * $steps_factor ) . " - " . strval( $key * $steps_factor ) . "%" );
			break;
		endswitch;
		$sty = $cl->getStyle( 0 );
		$sty->color->setRGB( hexdec( substr( $colors[ $key ], 0, 2) ), hexdec( substr( $colors[ $key ], 2, 2) ), hexdec( substr( $colors[ $key ], 4, 2) ) );
	}
	// borders for gemeente
		$layer = $map->getlayerbyname("gemeentegrenzen") or die("couldnot open layer gemeentegrenzen");
		$s = ms_newLayerObj( $map, $layer );
		$s->set( "status", MS_ON );
	// borders for provincie
		if ( $view == $strings[ "provincie" ] ):
			$layer = $map->getlayerbyname("provinciegrenzen") or die("couldnot open layer provinciegrenzen");
			$s = ms_newLayerObj( $map, $layer );
			$s->set( "status", MS_ON );
		endif;

// search by name
	if ( $search ) :
		// lookup this name in the shapefile
			// get the number of shapes.
				$shapeFile = ms_newShapefileObj( "/www/geodan-data/Nlge_r06"  , -1 );
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
					if ( $search == $shapeObject->values["GEMNAAM"] ):
						$bounds = $shapeObject->bounds;
						break;
					endif;
				}
			// if we found bounds, then translate them in pixel coords
				if ( is_object( $bounds ) ):
					list( $kaart_x, $kaart_y ) = map2click( $map, $bounds );
				else: 
					// relocate
						header( "HTTP/1.0 404 Not Found" );
						header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
						exit;
				endif;
			// close the layer
				$tmp_layer->close();
				//include( "100_01_strange_exceptions.inc" );
	endif;
// display map
	$display_zoom_map = "";
	if ( ! $kaart_x ):
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
			$tmp_filter = "(\"[GEMNAAM]\"=\"" . $shape_object->values["GEMNAAM"] . "\")";
			$layer = $map->getlayerbyname( "selected" );
			$s = ms_newLayerObj( $map, $layer );
			$s->setFilter( $tmp_filter );
			$s->set( "status", MS_ON );
		// draw the large map
			$map->selectOutputFormat("png");
			$image=$map->draw();
		// get the map URL
			$image_url=$image->saveWebImage();
		// make the small map
			$p_pix = ms_newPointObj();
			$p_pix->setXY( $kaart_x, $kaart_y );
			$zoom_factor = 10;
			$map->set( "width", 306 );
			$map->set( "height", 500 );
			$map->zoompoint( $zoom_factor, $p_pix, 450, 500, $map->extent );
		// turn on 'gemeentenamen'
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
?>