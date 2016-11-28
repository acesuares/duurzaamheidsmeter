<?php
// licensed under GPL v2. (C) 2008 Ace Suares, Willemstad
	/*
		Copyright (C) 2008 Ace Suares, Gravenstraat 4, Otrobanda, Willemstad, Curaçao, Nehterlands Antilles
		http://www.suares.an
		
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
$settings = new stdClass; 
$settings->sitename = "Duurzaamheidsspiegel Beheer 2009";
$settings->stylesheet_url = "";
$settings->db_host = "localhost:/var/run/mysqld/mysqld.sock";
$settings->db_readwrite_user = "RWThee9uPo";
$settings->db_readwrite_password = "xaiQp3ga";
$settings->db_link = mysql_connect( $settings->db_host,
																		$settings->db_readwrite_user,
																		$settings->db_readwrite_password );
if ( ! $settings->db_link ):
	die ( "Could not connect to database: " . mysql_error() );
endif;
$settings->db_name = "la21nl2009";
$settings->db_select = mysql_select_db( $settings->db_name );
if ( ! $settings->db_select ):
	die ( "Could not select database '{$settings->db_name}': " . mysql_error() );
endif;
?>
