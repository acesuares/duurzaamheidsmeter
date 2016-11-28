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
	// kludge for 'kantine'.
	$onderwerp_code = $indicators[ $onderwerp ]->code;
	if ( $onderwerp_code == "ka" ):
		$onderwerp_code = "di";
	endif;
// show data for this gemeente_id and this indicator_id
// we have $indicators and $onderwerp
	$query  = "SELECT G.name AS gemeente,
              G.id,
              G.inwoneraantal,
              G.adres, G.postcode, G.plaats,
	           G.tel, G.fax, G.email, G.website,
	           G.description,
	           P.name AS provincie,
	           A.name AS contact_aanspreektitel,
	           CONCAT_WS(' ', G.${onderwerp_code}_voornaam, G.${onderwerp_code}_achternaam) AS contact_naam,
	           G.${onderwerp_code}_afdeling AS contact_afdeling,
	           IF( G.${onderwerp_code}_gemeenteadres='Y', G.${onderwerp_code}_adres, G.adres) AS contact_adres,
	           IF( G.${onderwerp_code}_gemeenteadres='Y', CONCAT_WS(' ', G.${onderwerp_code}_postcode, G.${onderwerp_code}_plaats), CONCAT_WS(' ', G.postcode, G.plaats) ) AS contact_pcplaats,
	           G.${onderwerp_code}_tel AS contact_telefoonnummer,
	           G.${onderwerp_code}_email AS contact_email
	           FROM gemeente G, aanspreektitel A, provincie P
	           WHERE G.provincie_id = P.id
	           AND G.name = '" . preg_replace( "/^'/", "", $search) . "'
	           AND G.${onderwerp_code}_aanspreektitel_id = A.id ";
	$result = mysql_query( $query );
	$gemeente = mysql_fetch_object( $result );
	if ( in_array( $gemeente->gemeente, array( "s-Gravendeel", "s-Gravenhage", "s-Hertogenbosch" ) ) ):
		$gemeente->gemeente = "'" . $gemeente->gemeente;
	endif;
			
// gemeente gegevens
	$out .= "<p class=\"gemeente-gegevens-gemeente\">\n";
	$out .= "<strong>" . $strings[ "Gemeente" ] . ": {$gemeente->gemeente}</strong><br />\n";
	$out .= $strings[ "provincie" ] . ": {$gemeente->provincie} (". number_format( $gemeente->inwoneraantal, 0, ",", "." ) . " " . $strings[ "inwoners" ] . ")<br />\n";
	if ( $gemeente->adres . $gemeente->postocde . $gemeente->plaats != "" ):
		$out .= $strings[ "adres" ] . ": {$gemeente->adres}, {$gemeente->postcode} {$gemeente->plaats}<br />\n";
	endif;
	if ( $gemeente->tel != "" ):
		$out .= $strings[ "telefoonnummer" ] . ": " . make_inter( $gemeente->tel ) . "<br />\n";
	endif;
	if ( $gemeente->fax != "" ):
		$out .= $strings[ "faxnummer" ] . ": " . make_inter( $gemeente->fax ) . "<br />\n";
	endif;
	if ( $gemeente->email != "" ):
		$out .= $strings[ "e-mail" ] . ": <A HREF=mailto:\"{$gemeente->email}\">{$gemeente->email}</A><br />\n";
	endif;
	if ( $gemeente->website != "" ):
		$out .= $strings[ "website" ] . ": <A HREF=\"{$gemeente->website}\" TARGET=\"_web_$search\">{$gemeente->website}</A><br />\n";
	endif;
	$out .= "</p>\n";
	$out .= "<p class=\"gemeente-gegevens-contact\">\n";
// contactpersoon
	if ( $gemeente->contact_naam != " " ):   // the query delivers a space when no contact
		$gemeente->contact_naam = "{$gemeente->contact_aanspreektitel} {$gemeente->contact_naam}";
		$out .= "<strong>" . $strings[ "contactpersoon" ] . ":  {$gemeente->contact_naam}</strong><br />\n";
		$out .= $strings[ "adres" ] . ": {$gemeente->contact_adres} {$gemeente->contact_pcplaats}<br />\n";
		if ( $gemeente->contact_telefoonnummer ):
			$out .= $strings[ "telefoonnummer" ] . ": " . make_inter( $gemeente->contact_telefoonnummer ) . "<br />\n";
		endif;
		if ( $gemeente->contact_email ):
			$out .= $strings[ "e-mail" ] . ": <a href=\"mailto:{$gemeente->contact_email}\">{$gemeente->contact_email}</a><br />\n";
		endif;
	else:
		$gemeente->contact_naam = $strings[ "geen" ];
		$out .= "<strong>" . $strings[ "contactpersoon" ] . ":  {$gemeente->contact_naam}</strong><br />\n";
	endif;
	$out .= "</p>\n";
// description
	if ( $gemeente->description ):
		$out .= "<p class=\"gemeente-gegevens-description\">";
		$out .= nl2br( $gemeente->description );
		$out .= "</p>\n";
	endif;
?>