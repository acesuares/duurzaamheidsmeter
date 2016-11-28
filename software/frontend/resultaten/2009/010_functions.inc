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
// functions
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
		$y_pct = ( ( $bounds_average_y - $map->extent->miny ) / ( $map->extent->maxy - $map->extent->miny ) );
		$x_map = intval( $map->width * $x_pct );
		$y_map = intval( $map->height * (1 - $y_pct) );
		return array( $x_map, $y_map );
	}
// function gradient (from http://www.codingforums.com/showthread.php?t=79463 or http://www.reallyshiny.com/blog/php-colour-gradient-function/ )
	function gradient($hexstart, $hexend, $steps) { 
 
    $start['r'] = hexdec(substr($hexstart, 0, 2)); 
    $start['g'] = hexdec(substr($hexstart, 2, 2)); 
    $start['b'] = hexdec(substr($hexstart, 4, 2)); 
 
    $end['r'] = hexdec(substr($hexend, 0, 2)); 
    $end['g'] = hexdec(substr($hexend, 2, 2)); 
    $end['b'] = hexdec(substr($hexend, 4, 2)); 
     
    $step['r'] = ($start['r'] - $end['r']) / ($steps - 1); 
    $step['g'] = ($start['g'] - $end['g']) / ($steps - 1); 
    $step['b'] = ($start['b'] - $end['b']) / ($steps - 1); 
     
    $gradient = array(); 
     
    for($i = 0; $i <= $steps; $i++) { 
         
        $rgb['r'] = floor($start['r'] - ($step['r'] * $i)); 
        $rgb['g'] = floor($start['g'] - ($step['g'] * $i)); 
        $rgb['b'] = floor($start['b'] - ($step['b'] * $i)); 
         
        $hex['r'] = sprintf('%02x', ($rgb['r'])); 
        $hex['g'] = sprintf('%02x', ($rgb['g'])); 
        $hex['b'] = sprintf('%02x', ($rgb['b'])); 
         
        $gradient[] = implode(NULL, $hex); 
                         
    } 
     
    return $gradient; 
	}
?>