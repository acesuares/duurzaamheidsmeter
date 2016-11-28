<?php
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
	$out .= "<P ID=\"gemeente-gegevens-gemeente\">\n";
	$out .= "<strong>" . $strings[ "Gemeente" ] . ": {$gemeente->gemeente}</strong><BR>\n";
	$out .= $strings[ "provincie" ] . ": {$gemeente->provincie} (". number_format( $gemeente->inwoneraantal, 0, ",", "." ) . " " . $strings[ "inwoners" ] . ")<BR>\n";
	if ( $gemeente->adres . $gemeente->postocde . $gemeente->plaats != "" ):
		$out .= $strings[ "adres" ] . ": {$gemeente->adres}, {$gemeente->postcode} {$gemeente->plaats}<BR>\n";
	endif;
	if ( $gemeente->tel != "" ):
		$out .= $strings[ "telefoonnummer" ] . ": " . make_inter( $gemeente->tel ) . "<BR>\n";
	endif;
	if ( $gemeente->fax != "" ):
		$out .= $strings[ "faxnummer" ] . ": " . make_inter( $gemeente->fax ) . "<BR>\n";
	endif;
	if ( $gemeente->email != "" ):
		$out .= $strings[ "e-mail" ] . ": <A HREF=mailto:\"{$gemeente->email}\">{$gemeente->email}</A><BR>\n";
	endif;
	if ( $gemeente->website != "" ):
		$out .= $strings[ "website" ] . ": <A HREF=\"{$gemeente->website}\" TARGET=\"_web_$search\">{$gemeente->website}</A><BR>\n";
	endif;
	$out .= "</P>\n";
	$out .= "<P ID=\"gemeente-gegevens-contact\">\n";
// contactpersoon
	if ( $gemeente->contact_naam != " " ):   // the query delivers a space when no contact
		$gemeente->contact_naam = "{$gemeente->contact_aanspreektitel} {$gemeente->contact_naam}";
		$out .= "<strong>" . $strings[ "contactpersoon" ] . ":  {$gemeente->contact_naam}</strong><BR>\n";
		$out .= $strings[ "adres" ] . ": {$gemeente->contact_adres} {$gemeente->contact_pcplaats}<BR>\n";
		if ( $gemeente->contact_telefoonnummer ):
			$out .= $strings[ "telefoonnummer" ] . ": " . make_inter( $gemeente->contact_telefoonnummer ) . "<BR>\n";
		endif;
		if ( $gemeente->contact_email ):
			$out .= $strings[ "e-mail" ] . ": <A HREF=\"mailto:{$gemeente->contact_email}\">{$gemeente->contact_email}</A><BR>\n";
		endif;
	else:
		$gemeente->contact_naam = $strings[ "geen" ];
		$out .= "<strong>" . $strings[ "contactpersoon" ] . ":  {$gemeente->contact_naam}</strong><BR>\n";
	endif;
	$out .= "</P>\n";
// description
	if ( $gemeente->description ):
		$out .= "<P ID=\"gemeente-gegevens-description\">";
		$out .= nl2br( $gemeente->description );
		$out .= "</P>\n";
	endif;
?>