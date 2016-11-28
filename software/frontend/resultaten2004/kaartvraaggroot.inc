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
	$map_file="dzhmeter_2005_vraag.map";
// colors
	$color[ 0 ] = "D0E7BD";
	$color[ 1 ] = "00FF00";
	$color[ 2 ] = "FF0000";
	$color[ 3 ] = "91e391";
	$color[ 4 ] = "dc6f6f";
// possible answers 
	$answers = array( "", "nee (2005)", "ja (2005)", "nee (2003)", "ja (2003)" );
// get the selected vraag
	$vraag_id = intval( $choice );
	$query  = " SELECT * ";
	$query .= " FROM vraag ";
	$query .= " WHERE id='$vraag_id' ";
	
	$result = mysql( $WebEngine_database, $query );
	if ( ! mysql_num_rows($result) ):
			header("Location: $PHP_SELF?spiegel=overzicht&choice=totaalkaart");
			exit;
	endif;
	
	$this_vraag = mysql_fetch_object( $result );
	$kaart_title = $this_vraag->title;
	$indicator_id = intval( $this_vraag->indicator_id );

// get the indicator
	$query  = " SELECT title, code ";
	$query .= " FROM indicator ";
	$query .= " WHERE id='$indicator_id' ";
	
	$result = mysql( $WebEngine_database, $query );
	$this_indicator = mysql_fetch_object( $result );
	$indicator_code = $this_indicator->code;

// make a list of all gemeentes
	$gemeentes = array();
	$query  = " SELECT G.id, G.name, ${indicator_code}_datafromyear_id AS datafromyear ";
	$query .= " FROM gemeente G ";
	$result = mysql( $WebEngine_database, $query );

	while ( $row = mysql_fetch_object( $result ) ) {
		$gemeentes[ $row->name ][ "id" ] = $row->id;
		$gemeentes[ $row->name ][ "datafromyear" ] = $row->datafromyear;
		$gemeentes[ $row->name ][ "score" ] = 0;
	}
	// get the gemeentes who answered yes
		$query  = "SELECT ";
		$query .= " G.id, ";
		$query .= " G.name, ";
		$query .= " COUNT( G.id ) as score ";
		
		$query .= "FROM ";
		$query .= " gemeente G, ";
		$query .= " gemeente2vraag G2V, ";
		$query .= " vraag V ";
		
		$query .= " WHERE ";
		$query .= "  G2V.gemeente_id = G.id ";
		$query .= " AND ( G2V.vraag_id = '" . $this_vraag->id . "'";
		$query .= " OR G2V.vraag_id = '" . $this_vraag->reference_question_2005 . "'";
		$query .= " OR G2V.vraag_id = '" . $this_vraag->reference_question_2003 . "'";
		$query .= " ) AND V.id = G2V.vraag_id ";
		$query .= " AND V.active='Y' ";
		$query .= "  GROUP BY G.id ";
		
		$result = mysql( $WebEngine_database, $query );
	// put the results in an array
		while ( $row = mysql_fetch_object( $result ) ) {
			$gemeentes[ $row->name ][ "id" ] = $row->id;
			if ( $gemeentes[ $row->name ][ "datafromyear" ] < $current_datafromyear_id ):
				if ( $this_vraag->questionfromyear_id < $current_datafromyear_id ):
					$gemeentes[ $row->name ][ "score" ] = $row->score + 2;
				endif;
			else:
				$gemeentes[ $row->name ][ "score" ] = $row->score;
			endif;
		}

// put the scores in an array
	$scores = array();
	foreach ( $gemeentes as $gemeente_name => $gemeente ){
		if ( substr( $gemeente_name, 0, 10) != "Amsterdam " AND $gemeente[ "score" ] != 0 ):
		$scores[ $gemeente[ "score" ] ] .= "'[GEMNAAM]'='" . $gemeente_name . "' OR ";
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
