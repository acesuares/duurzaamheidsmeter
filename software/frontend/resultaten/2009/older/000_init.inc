<?php
// licensed under GPL v2. (C) 2008 Ace Suares, Willemstad
	/*
		Copyright (C) 2009 Ace Suares, Gravenstraat 4, Otrobanda, Willemstad, Curaçao, Nehterlands Antilles
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
// magic quotes test	
	if ( ! get_magic_quotes_gpc() ):
		echo "Magic quotes Get/Post/Cookie are off. Refusing to continue...";
		exit;
	endif;
	if ( ini_get( 'magic_quotes_sybase' ) ):
		echo "Magic quotes for sybase are on. Refusing to continue...";
		exit;
	endif;
// settings
	$settings = new stdClass; 
	$settings->parent_url = "http://www.duurzaamheidsmeter.nl";
	$settings->site_url = "http://www.duurzaamheidsmeter.nl/resultaten";
	$settings->stylesheet = "duurzaam.css";
	$settings->db_host = "127.0.0.1:3306";
	$settings->db_readonly_user = "RORub5io5u";
	$settings->db_readonly_password = "zu6Aeyiu";
	$settings->db_link = mysql_connect( $settings->db_host,
																			$settings->db_readonly_user,
																			$settings->db_readonly_password );
	if ( ! $settings->db_link ):
		die ( "Could not connect to database: " . mysql_error() );
	endif;
    mysql_query( "SET NAMES utf8" );
	$settings->db_name = "la21nl2009";
	$settings->db_select = mysql_select_db( $settings->db_name );
	if ( ! $settings->db_select ):
		die ( "Could not select database '{$settings->db_name}': " . mysql_error() );
	endif;
	$settings->map_file = "duurzaam2009.map";
	$settings->tooltips = "<script language=\"JavaScript\" type=\"text/javascript\" src=\"{$settings->site_url}/wz_tooltip.js\"></script>";
	$settings->tooltips_on = "no";
	$settings->years = array( "2009" );
	$settings->default_year = "2009";
	$settings->languages = array( "nl" );
	$settings->languages_long = array( "nl"=>"nederlands" );
	$settings->languages_longer = array( "nl"=>"Nederlands");
	$settings->default_language = "nl";
	$settings->color_unknown = "F9EBC7";
	$settings->color_gradient_start = "FFFFFE";
	$settings->color_gradient_end = "00B0EB";
	$settings->colors_vragen = array(	"onbekend" 	=> $settings->color_unknown,
																		"ja"			=> "23B9EB",
																		"nee"			=> "FCBB01" );
	/* $settings->colors_vragen = array(	"onbekend" 	=> $settings->color_unknown,
																		"ja"			=> "00CC99",
																		"nee"			=> "FF9999" );
     */
	$settings->invuller_formfields = array( "invuller_naam"=>"Naam (verplicht)",
											"invuller_organisatie"=>"Organisatie of afdeling",
											"invuller_adres"=>"Adres",
											"invuller_postcode"=>"Postcode",
											"invuller_plaats"=>"Plaats",
											"invuller_tel"=>"Telefoon (verplicht)",
											"invuller_fax"=>"Fax",
											"invuller_email"=>"E-mail",
											"invuller_website"=>"Website"
											);
?>