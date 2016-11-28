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
	$colors = gradient( $color_gradient_start, $color_gradient_end, $steps );
	array_unshift ( $colors, $color_unknown );
	ksort( $scores );
// assign scores to the layers
	foreach ( $scores as $key=>$value ){
		$value = "(" . substr( $value, 0 , -4 )  . ")";
		$layer = $map->getlayerbyname( $base_layer );
		$s = ms_newLayerObj( $map, $layer );
		$s->setFilter( $value );
		$s->set( "status", MS_ON );
		$cl = $s->getClass(0);
		switch ( $key ):
			case "0":
				$cl->set( "name", $strings[ "onbekend" ] );
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
		$layer = $map->getlayerbyname("provinciegrenzen") or die("couldnot open layer provinciegrenzen");
		$s = ms_newLayerObj( $map, $layer );
		$s->set( "status", MS_ON );
	// turn on 'provincienamen'
			$tmp_layer = $map->getlayerbyname("provincienamen" );
			$tmp_layer->set( "status", MS_ON );

// display map
		$map->selectOutputFormat("png");
		$image=$map->draw();
	// get the map URL
		$image_url=$image->saveWebImage();
?>
