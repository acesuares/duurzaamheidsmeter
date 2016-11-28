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
	$query  = " SELECT @vraagnummer:=@vraagnummer+1 vraagnummer,
							id, title_$lang AS title, score,
							IF( telt_mee_totaalscore = 'Y', score, 0 ) as score_voor_totaal,
							helptext_$lang AS helptext ";
	$query .= " FROM ( SELECT @vraagnummer:=0 ) VNUM, vraag V";
	$query .= " WHERE indicator_id = '" . $indicators[ $onderwerp ] ->id . "'";
	$query .= "  AND active = 'Y'";
	$query .= "  ORDER BY name";
	$result = mysql_query( $query );
	$vragen = array();
	while ( $row = mysql_fetch_object( $result ) ) {
		$vragen[ $row->vraagnummer ] = $row;
	}
/*	found the vraagnummer (vraagnummer) thingy here in the user comments,
	posted by Mark Malakanov on April 30 2006 3:42pm on this page:
	http://dev.mysql.com/doc/refman/5.0/en/user-variables.html
*/
?>