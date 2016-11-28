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
			$query  = " SELECT G.id, G.name, ${indicator_code}_procent_2003 AS score_procent ";
			$query .= " FROM gemeente G ";
			$result = mysql( $WebEngine_database, $query );
		// put the results in an array
			while ( $row = mysql_fetch_object( $result ) ) {
				$gemeentes[ $row->name ][ "id" ] = $row->id;
				$gemeentes[ $row->name ][ $indicator_code ][ "score_procent" ] = $row->score_procent;
				$gemeentes[ $row->name ][ $indicator_code ][ "datafromyear" ] = "2003";
			}
		// get a list of gemeente-ids, gemeente-names, and scores for 2005
			$query  = "SELECT ";
			$query .= " G.id, ";
			$query .= " G.name, ";
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
// draw the map
	$map->set( "width", 2250 );
	$map->set( "height", 2500 );
	$map->selectOutputFormat("png");
	$image=$map->draw();
// get the map URL
	$image_url=$image->saveWebImage();
	$html_out .= "<BR><BR><IMG SRC=\"$image_url\" WIDTH=\"2250\" HEIGHT=\"2500\" BORDER=\"0\"><BR><BR>";
?>
