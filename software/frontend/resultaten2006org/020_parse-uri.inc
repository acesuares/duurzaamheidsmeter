<?php
// parse uri
	$parm = explode( "/", $_SERVER['REQUEST_URI'], 9 );
//print_r($parm);
	// 0 = /, 1 = resultaten,  we don't need them
	$year = $parm[ 2 ];
	$lang = $parm[ 3 ];
	$onderwerp = $parm[ 4 ];
	$view = $parm[ 5 ]; // there are more parameters but they will be dealt with during 'sanitize view'.
// sanitize year
	if ( $year == "" ) { $year = $default_year; }
	if ( !in_array( $year, $years ) ):
		// garbage, so we relocate with a 404
		header( "HTTP/1.0 404 Not Found" );
		header( "Location: {$settings->site_url}/$default_year" );
		exit;
	endif;
// sanitize lang (language)
	if ( $lang == "" ) { $lang = $default_language; }
	if ( !in_array( $lang, $languages ) ):
		// garbage, so we relocate with a 404
		header( "HTTP/1.0 404 Not Found" );
		header( "Location: {$settings->site_url}/$year" );
		exit;
	endif;
// translations
	include( "020_02_translations.inc" );
	$strings = $translations[ $lang ];	
// get indicators (onderwerpen). We need to do that AFTER $lang is set...
	include( "020_01_onderwerp-get.inc" );
// sanitize onderwerp
	if ( $onderwerp == "" ) { $onderwerp = $default_onderwerp; }
	if ( !in_array( $onderwerp, $onderwerpen ) ):
		// garbage, so we relocate with a 404
		header( "HTTP/1.0 404 Not Found" );
		header( "Location: {$settings->site_url}/$year/$lang" );
		exit;
	endif;
// filter out introductie
	if ( $onderwerp == $strings[ "introductie" ] ):
		$junk = $view . $parm[ 6 ] . $parm[ 7 ] . $parm[ 8 ] ; // garbage.
		// sanitize junk. If they supplied junk, we wanna get rid of it. 
			if ( $junk || $_GET || $_POST || $_COOKIES ):
				header( "HTTP/1.0 404 Not Found" );
				header( "Location: {$settings->site_url}/$year/$lang/$onderwerp" );
				exit;
			endif;
	else:
		// sanitize view
			$views = $strings[ "views" ];	
			$default_view = $strings[ "default_view" ];	
			if ( $view == "" ) { $view = $default_view; }
			if ( !in_array( $view, $views ) ):
				// garbage, so we relocate with a 404
				header( "HTTP/1.0 404 Not Found" );
				header( "Location: {$settings->site_url}/$year/$lang/$onderwerp" );
				exit;
			endif;
			switch ( $view ):
				case $strings[ "antwoorden" ]:
					$search_vraagnummer = $parm[ 6 ]; 
					$search = $parm[ 7 ];
					$junk = $parm[ 8 ]; // garbage.
				break;
				case $strings[ "provincie" ]:
				case $strings[ "ranglijst" ]:
					$search = "";
					$junk = $parm[ 6 ] . $parm[ 7 ] . $parm[ 8 ] ; // garbage.
					// sanitize junk. If they supplied junk, we wanna get rid of it. 
						if ( $junk || $_GET || $_POST || $_COOKIES ):
							header( "HTTP/1.0 404 Not Found" );
							header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
							exit;
						endif;
				break;
				default:
					$search_vraagnummer = "";
					$search = $parm[ 6 ]; 
					$junk = $parm[ 7 ] . $parm[ 8 ] ; // garbage.
					// sanitize junk. If they supplied junk, we wanna get rid of it
					// but we will deal with POST junk later !!
						if ( $junk || $_GET || $_COOKIES ):
							header( "HTTP/1.0 404 Not Found" );
							header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
							exit;
						endif;
				break;
			endswitch;
		// we need to get some stuff from the database, now...
			include( "020_03_gemeente-get.inc" );
			include( "020_04_vraag-get.inc" );
			include( "020_05_provincie-get.inc" );
		// sanitize search_vraagnummer
			if ( $view == $strings[ "antwoorden" ] ):
				if ( 	$search_vraagnummer != "" ):
					// it might be from the URI, check if it's good.
					if ( 	$search_vraagnummer != abs( intval( $search_vraagnummer ) ) 
							|| ! isset( $vragen[ $search_vraagnummer ] ) ):
						header( "HTTP/1.0 404 Not Found" );
						header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
						exit;
					endif;
				else:
					if ( $_POST[ "search_vraagnummer" ] ):
						$search_vraagnummer = $_POST[ "search_vraagnummer" ];
						if ( 	count( $_POST ) != 1 
								|| $search_vraagnummer != abs( intval( $search_vraagnummer ) ) 
								|| ! isset( $vragen[ $search_vraagnummer ] ) ):
							header( "HTTP/1.0 404 Not Found" );
							header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
							exit;
						endif; 
						// we have a valid search_vraagnummer from POST, relocate
						if ( $search ):
							if ( in_array( $search, array( "\\'s-Gravendeel", "\\'s-Gravenhage", "\\'s-Hertogenbosch" ) ) ):
								$search = stripslashes( $search );
							endif;
							header( "HTTP/1.1 303 See Other" );
							header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view/$search_vraagnummer/$search" );
							exit;
						else:
							header( "HTTP/1.1 303 See Other" );
							header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view/$search_vraagnummer" );
							exit;
						endif;
					endif;
				endif; // we have a valid search_vraagnummer from the URI
			endif;
		// sanitize search
			$search = urldecode( $search );
			// handle 's (safely)
				if ( in_array( $search, array( "\\'s-Gravendeel", "\\'s-Gravenhage", "\\'s-Hertogenbosch" ) ) ):
					$search = stripslashes( $search );
				endif;
			// handle vragenlijsten - form processing
				if ( $view == $strings[ "vragenlijst" ] ):
					if ( $search ):
						if ( $search != $strings[ "verwerk" ] ):
							// some junk has been entered
								header( "HTTP/1.0 404 Not Found" );
								header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
								exit;
						endif;
					else:
						// if $search is empty, _POST should be empty
							if ( $_POST ):
								header( "HTTP/1.0 404 Not Found" );
								header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
								exit;
							endif;
					endif;
				endif;
				// search can come from POST or from the URI
					if ( $_POST[ "search" ] ):
						// search must be the only POST var; and search may not come via the URI
						if ( count( $_POST ) != 1 || $search != "" ):
							if ( $view == $strings[ "antwoorden" ] ):
								header( "HTTP/1.0 404 Not Found" );
								header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view/$search_vraagnummer" );
								exit;
							else:
								header( "HTTP/1.0 404 Not Found" );
								header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
								exit;
							endif;
						endif;
						$search = $_POST[ "search" ];
						// handle 's (safely)
						  if ( in_array( $search, array( "\\'s-Gravendeel", "\\'s-Gravenhage", "\\'s-Hertogenbosch" ) ) ):
						    $search = stripslashes( $search );
						  endif;
					endif;
					if ( $search && $view != $strings[ "vragenlijst" ] ):
						// now quickly relocate if $search is rubbish..
							if ( ! isset( $gemeentes[ $search ] ) ):
								if ( $view == $strings[ "antwoorden" ] ):
									header( "HTTP/1.0 404 Not Found" );
									header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view/$search_vraagnummer" );
									exit;
								else:
									header( "HTTP/1.0 404 Not Found" );
									header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
									exit;
								endif;
							endif;
						// but if POST was legitimate, we need to redirect!
							if ( $_POST[ "search" ] ):
								if ( $view == $strings[ "antwoorden" ] ):
									if ( $search_vraagnummer ):
										header( "HTTP/1.1 303 See Other" );
										header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view/$search_vraagnummer/$search" );
										exit;
									else:
										header( "HTTP/1.0 404 Not Found" );
										header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
										exit;
									endif;
								else:
									header( "HTTP/1.1 303 See Other" );
									header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view/$search" );
									exit;
								endif;
							endif;
					endif;
		// sanitize POST. if they supplied anything other then kaart_x and kaart_y, something's wrong. Thanks to mieg for the twoliner.
			if ( count( $_POST ) > 0 && $view != $strings[ "vragenlijst" ] ):
				// remember, we already dealt with POST[ search ] and with POST[ search_vraagnummer ]!
				$postkeys = sort( array_keys( $_POST ) );
				if ( $postkeys != array( "kaart_x", "kaart_y" ) ):
					header( "HTTP/1.0 404 Not Found" );
					header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view/$search" );
					exit;
				endif;
				// reroute if they clicked on the large map
					$kaart_x = abs( intval( $_POST[ "kaart_x" ] ) );
					$kaart_y = abs( intval( $_POST[ "kaart_y" ] ) );
					$map = ms_newMapObj( $settings->map_file );
					list( $map_x, $map_y ) = click2map( $map, $kaart_x, $kaart_y );
					$p_geo = ms_newPointObj();
					$p_geo->setXY( $map_x, $map_y );
					// query the 'all' layer to see if we are in there.
						$tmp_layer = $map->getlayerbyname( "all" ) or die( "couldnot open layer named 'all'");
						/*
						if ( $view == $strings[ "provincie" ] ):
							$tmp_layer->set( "data", "Nlpr_r06" );
						endif;
						*/
						$tmp_layer->set( "status", MS_ON );
						if ( @$tmp_layer->queryByPoint($p_geo, MS_SINGLE, -1) == MS_SUCCESS ):
							// we have a hit. There is always only one result.
								$tmp_results = $tmp_layer->getResult(0);
								$tmp_layer->open();
								$shape_object = $tmp_layer->getShape( -1, $tmp_results->shapeindex );
								$tmp_layer->close();
							// relocate
								// if ( $view == $strings[ "provincie" ] ):
								//	$search = $shape_object->values["PROVC_NM"];
								// else:
									$search = $shape_object->values["GEMNAAM"];
								// endif;
								if ( $view == $strings[ "antwoorden" ] ):
									if ( $search_vraagnummer ):
										header( "HTTP/1.1 303 See Other");
										header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view/$search_vraagnummer/$search" );
										exit;
									else:
										header( "HTTP/1.0 404 Not Found" );
										header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view" );
										exit;
									endif;
								else:
									header( "HTTP/1.1 303 See Other");
									header( "Location: {$settings->site_url}/$year/$lang/$onderwerp/$view/$search" );
									exit;
								endif;
						else:
							// clicked outside map
						endif;
			endif;
	endif;
?>