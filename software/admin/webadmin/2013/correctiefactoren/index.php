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
/*	invoer 2008
		in the first step we select a gemeente.
		in the second step we show correctiefactoren
		in the third step we shove the whole stuff in the database.
		*/
// init
	include( "init.inc" );
// html header
	echo "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n
					<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n
					<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n
					<head>\n";
	echo "<title>{$settings->sitename}</title>\n
           <link rel=\"stylesheet\" type=\"text/css\" href=\"{$settings->stylesheet_url}\">\n
           </head>\n";
// body
	echo "<body>\n";
// header
	echo "<h1>STAP 1: Selecteer een gemeente.</h1>\n";
// form begin
	echo "<form action=\"step2.php\" method=\"post\">\n";
// select gemeente
	echo "Gemeente: <select name=\"gemeente_id\" size=\"1\" onchange=\"submit()\">\n";
	$query = "SELECT 	G.id, G.name AS gemeente,
										P.name AS provincie
						FROM 		gemeente G, provincie P 
						WHERE 	G.provincie_id = P.id 
						ORDER BY G.name ASC";
	$result = mysql_query( $query );
	while( $row = mysql_fetch_object( $result ) ):
		$id = $row->id;
		$gemeente = stripslashes( $row->gemeente );
		$provincie = stripslashes( $row->provincie );
		echo "<option value=\"{$id}\" >$gemeente ($provincie)\n";
	endwhile;
	echo " </select>\n";
// submit
	echo "<input type=\"submit\" value=\"Naar Stap 2 >>>\">\n";
// end form, body, html
	echo "</form>\n";
	echo "</body>\n";
	echo "</html>\n";
?>