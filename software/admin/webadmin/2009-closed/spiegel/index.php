<?php
/* Copyright (C) 1999-2008 Suares & Co, Gravenstraat 4, Willemstad, Curacao (NA)
		http://www.suares.an/qwired, http://www.qwikzite.com, http://www.qwikzite.net
		
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
// initialize
$test="QwiRed 1.20";
	$qwired = new stdClass;
	include( "/etc/qwired/qwired.conf" );
	$qwired->db_host					= "localhost:/var/run/mysqld/mysqld.sock";
// get hostname and request_uri and query_string
		/* if this page is called as http://www.example.com/somefile.html
			then hostname = www.example.com and 
			request_uri = "/somefile.html"
			*/
	$qwired->hostname = "la21nl2009-nonrealhostname";
	$qwired->request_uri = $_SERVER[ "REQUEST_URI" ];
	$qwired->query_string = $_SERVER[ "QUERY_STRING" ];
// connect to the database
	$qwired->db_link = mysql_connect( $qwired->db_host,	$qwired->db_user,	$qwired->db_pass );
	mysql_select_db( $qwired->db );
// lookup the hostname in a mysql table.
	$query  = sprintf( "SELECT Q.*, T.name AS type 
											FROM {$qwired->db_table} Q, types T 
											WHERE Q.active = '1' 
											AND Q.type_id = T.id 
											AND Q.name = '%s' ",
											mysql_real_escape_string( $qwired->hostname )
										);
	$result = mysql_query( $query );
// if no results (or more then one) redirect to the unconfigured_url
	if ( mysql_num_rows( $result ) != 1 ):
		header( "Expires: Fri, 15 Oct 1965 22:43:11 GMT" );    										// Date in the past
		header( "Last-Modified: " . gmdate( "D, d M Y H:i:s" ) . " GMT" ); 				// always modified
		header( "Cache-Control: no-store, no-cache, must-revalidate" );  					// HTTP/1.1
		header( "Cache-Control: post-check=0, pre-check=0", false) ;
		header( "Pragma: no-cache" );                          										// HTTP/1.0
		header( "Location: {$qwired->unconfigured_url}" ); 												// Redirect browser
		exit;
	endif;
// retrieve the data
	$qwired_data = mysql_fetch_object( $result );
// switch based on qwired_data->type
	switch ( $qwired_data->type ):
			case "qwikzite":
			case "qwibace":
			case "redirect";
				$hostname								= $qwired->hostname;
				$QZAdmin_version 				= $qwired_data->qzadmin_version;
				$QwikZite_dbname 				= $qwired_data->qwikzite_dbname;
				$QZAdmin_user 					= $qwired_data->qzadmin_user;
				$QZAdmin_password 			= $qwired_data->qzadmin_password;
				$QZAdmin_include_path 	= $qwired->include_path . "qzadmin/" . $QZAdmin_version . "/qzadmin/";
				$QZAdmin_host 					= $qwired->db_host;
				$QZAdmin_dblink 				= mysql_connect( 	$QZAdmin_host,
																									$QZAdmin_user,
																									$QZAdmin_password );
				$QZAdmin_default_language_id = $qwired_data->qzadmin_default_language_id;
				$request_uri = explode( "/", $qwired->request_uri, 5 );
				$mode = explode( "?", $request_uri[ 2 ], 2 );
				switch( $mode[ 0 ] ):
					case "graphic.php":
						include( $QZAdmin_include_path . "graphic.inc");
					break;
					case "file.php":
						include( $QZAdmin_include_path . "file.inc");
					break;
					case "css":
						include( $QZAdmin_include_path . "css.inc");
					break;
					default:
						include( $QZAdmin_include_path . "index.inc");
					break;
				endswitch;
			break;
			default:
				echo "problems, sorry";
			break;
	endswitch;
?>
