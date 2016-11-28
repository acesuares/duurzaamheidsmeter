<?php
// licensed under GPL v2. (C) 2008-2013 Ace Suares, Willemstad
	/*
		Copyright (C) 2008-2013 Ace Suares, Gravenstraat 4, Otrobanda, Willemstad, Curaçao, Nehterlands Antilles
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

// sanitize gemeente_id
	$gemeente_id = abs( intval ( $_POST[ "gemeente_id" ] ) );
	if ( $gemeente_id != $_POST[ "gemeente_id" ] ):
		$gemeente_id = 0;
	endif;
	if ( ! in_array( $gemeente_id, array_keys( $gemeente_ids ) ) ):
		$gemeente_id = 0;
	endif;
// correctie
	$scratchy = "";
	if ( $_POST[ "correctiefactor" ] != 0 && ! $_POST[ "correctiereden" ] ):
		$scratchy = "correctiereden niet ingevuld";
	endif;
// gemeente_id, invuller_naam and invuller_tel are required		
	if ( 	$gemeente_id == 0 
			|| ! $_POST[ "invuller_naam" ] 
			|| ! $_POST[ "invuller_tel" ]
			|| $scratchy 
		):
		$out .= "<div class=\"invulfout\">";
		$out .= "<h1>Er is iets fout gegaan bij het invullen.</h1>";
		$out .= "<ul>";
		if ( $gemeente_id == 0 ):
			$out .= "<li>u hebt geen gemeente geselecteerd.";
		endif;
		if ( ! $_POST[ "invuller_naam" ] ):
			$out .= "<li>u hebt uw naam niet ingevuld.";
		endif;
		if ( ! $_POST[ "invuller_tel" ] ):
			$out .= "<li>u hebt uw telefoonnummer niet ingevuld.";
		endif;
		if ( ! $_POST[ "invuller_email" ] ):
			$out .= "<li>u hebt uw email adres niet ingevuld.";
		endif;
		if ( $scratchy ):
			$out .= "<li>u hebt een correctie factor ingevoerd maar geen reden opgegeven.";
		endif;
		$out .= "</ul>";
		$out .= "<h2>gebruik de 'back' of 'terug' knop van uw browser om terug te keren.</h2>";
		$out .= "</div>";
	else:
		$totaal_score = 0;
		$message_body_vragen = "";
	
		$out .= "<div class=\"vragenlijst\">\n";
		$out .= "<table>\n";
		$out .= "<tr>\n";
		$out .= "<td class=\"vl-header-vraagnummer\">" . $strings[ "nr." ] . "</strong></td>\n";
		$out .= "<td class=\"vl-header-vraag\">" . $strings[ "vraag" ] . "</td>\n";
		$out .= "<td class=\"vl-header-jaofnee\">" . $strings[ "antwoord" ] . "</td>\n";
		$out .= "<td class=\"vl-header-score\">" . $strings[ "score" ] . "</td>\n";
		$out .= "<td>&nbsp;</td>\n";
		$out .= "</tr>\n";
		foreach ( $vragen as $vraagnummer => $vraag ){
			$message_body_vragen .= "$vraagnummer. ";
			$out .= "<tr>\n";
			$out .= "<td class=\"vl-vraagnummer\">$vraagnummer.</td>\n";
			$out .= "<td class=\"vl-vraag\">";
			$out .= utf8_encode($vraag->title);
			$out .= "</td>\n";
			if ( $_POST[ "vraag" ][ $vraag->id ] == "Y" ):
				$out .= "<td class=\"vl-jaofnee\">";
				$out .= "ja";
				$out .= "</td>\n";
				$out .= "<td class=\"vl-score\">";
				$out .= $vraag->score;
				$out .= "</td>\n";
				$totaal_score += $vraag->score;
				$update_sql .= "{$vraag->id},Y;";
				$message_body_vragen .= "ja   ";
			else:
				$out .= "<td class=\"vl-jaofnee\">";
				$out .= "nee";
				$out .= "</td>\n";
				$out .= "<td class=\"vl-score\">";
				$out .=  "&nbsp;";
				$out .= "</td>\n";
				$update_sql .= "{$vraag->id},N;";
				$message_body_vragen .= "nee  ";
			endif;
			$out .= "<td>&nbsp;</td>\n";
			$out .= "</tr>\n";
			$message_body_vragen .= stripslashes( substr( $vraag->title, 0 , 60 ) ) . "\n";		
		}
		// TOTAALSCORE 
			$out .= "<tr class=\"vl-totaalscore-row\">\n";
			$out .= "<td>&nbsp;</td>\n";
			$out .= "<td class=\"vl-totaalscore-tekst\">totaalscore</td>\n";
			$out .= "<td>&nbsp;</td>\n";
			$out .= "<td class=\"vl-totaalscore-score\">$totaal_score</td>\n";
			$out .= "<td>&nbsp;</td>\n";
			$out .= "</tr>\n";
			$message_body_vragen .= "\n\nTotaalscore: $totaal_score\n";
		// Correctie
			$correctiefactor = intval( $_POST[ "correctiefactor" ] );
			$correctiereden = $_POST[ "correctiereden" ];
			if ( $correctiefactor <> 0 ):
				//factor
				$out .= "<tr class=\"vl-cfactor-row\">\n";
				$out .= "<td>&nbsp;</td>\n";
				$out .= "<td class=\"vl-cfactor-tekst\">Correctiefactor</td>\n";
				$out .= "<td>&nbsp;</td>\n";
				$out .= "<td class=\"vl-cfactor-score\">$correctiefactor</td>\n";
				$out .= "<td>&nbsp;</td>\n";
				$out .= "</tr>\n";
				// gecorrigeerd
				$out .= "<tr class=\"vl-cfactor-totaal-row\">\n";
				$out .= "<td>&nbsp;</td>\n";
				$out .= "<td class=\"vl-cfactor-totaal-tekst\">Gecorrigeerde Totaalscore</td>\n";
				$out .= "<td>&nbsp;</td>\n";
				$out .= "<td class=\"vl-cfactor-totaal-score\">" . ( $totaal_score + $correctiefactor ) . "</td>\n";
				$out .= "<td>&nbsp;</td>\n";
				$out .= "</tr>\n";
				$message_body_vragen .= "Correctiefactor: $correctiefactor\n";
				$message_body_vragen .= "Gecorrigeerde Totaalscore: " . ( $totaal_score + $correctiefactor ) . "\n";
			endif;
			// gecorrigeerd in procenten
			$out .= "<tr class=\"vl-gecorr-totaal-row\">\n";
			$out .= "<td>&nbsp;</td>\n";
			$out .= "<td class=\"vl-gecorr-totaal-tekst\"><a href=\"javascript:void(0)\">
				Totaalscore (inclusief eventuele correcties) in procenten<span>&nbsp;&nbsp;
				In principe kan een gemeente op vrijwel alle vragen 'ja' antwoorden. 
				Voor elk positief antwoord zijn punten te behalen. De totaalscore (in 
				procenten) geeft aan hoeveel procent positief beantwoord is. Uiteraard 
				moet dit beeld gerelativeerd worden. We kunnen veel vragen niet stellen. 
				Maar het resultaat is een goede indicatie van de prestatie van de gemeente.
				</span></a></td>\n";
			$out .= "<td>&nbsp;</td>\n";
			$out .= "<td class=\"vl-gecorr-totaal-score\">";
			$out .= round ( ( $totaal_score + $correctiefactor ) / $indicators[ $onderwerp ]->max_score * 100 );
			$out .= "</td>\n";
			$out .= "<td class=\"vl-gecorr-totaal-procentteken\">&nbsp;%</td>\n";
			$out .= "</tr>\n";
			$message_body_vragen .= "Totaalscore (inclusief eventuele correcties) in procenten: ";
			$message_body_vragen .= round ( ( $totaal_score + $correctiefactor ) / $indicators[ $onderwerp ]->max_score * 100 ) . "\n\n";
			// reden
			if ( $correctiereden <> "" ):
				$out .= "<tr class=\"vl-creden-row\">\n";
				$out .= "<td>&nbsp;</td>\n";
				$out .= "<td colspan=4 class=\"vl-creden-tekst\">" . $strings[ "reden voor correctie" ] . ": " . stripslashes( $correctiereden ) . "</td>\n";
				$out .= "</tr>\n";
				$message_body_vragen .= $strings[ "Reden voor correctie" ] . ": " . stripslashes( $correctiereden ) . "\n\n";
			endif;
			$out .= "</table>\n";
			$out .= "</div>\n";
		// invuller-gegevens
			$out .= "<p class=\"gemeente-gegevens-gemeente\">\n";
			$out .= "<strong>" . $strings[ "Gemeente" ] . ": {$gemeente_ids[ $gemeente_id ]}</strong><br />\n";
			$out .= "<br />\n";
			$out .= "Ingevuld door: " . $_POST[ "invuller_aanspreektitel" ] . " " . stripslashes( $_POST[ "invuller_naam" ] ) . "<br />\n";
			$out .= "Organisatie of Afdeling: " . stripslashes( $_POST[ "invuller_organisatie" ] ) . "<br />\n";
			$out .= "Adres: " . stripslashes( $_POST[ "invuller_adres" ] ) . "<br />\n";
			$out .= "Postcode en Plaats: " . $_POST[ "invuller_postcode" ] . " " . stripslashes( $_POST[ "invuller_plaats" ] ) . "<br />\n";
			$out .= "Telefoon: " . $_POST[ "invuller_tel" ]  . "<br />\n";
			$out .= "Fax: " . $_POST[ "invuller_fax" ]  . "<br />\n";
			$out .= "E-mail: " . $_POST[ "invuller_email" ]  . "<br />\n";
			$out .= "Website: " . $_POST[ "invuller_website" ]  . "<br />\n";
			$out .= "<br />\n";
			$out .= "Contactambtenaar: " . stripslashes( $_POST[ "contact_ambtenaar_naam" ] ) . "<br />\n";
			$out .= "Contactambtenaar telefoon: " . stripslashes ( $_POST[ "contact_ambtenaar_tel" ] ) . "<br />\n";
	// mail the results
		$message_body = "";
		$message_body .= $strings[ "Gemeente" ] . ": {$gemeente_ids[ $gemeente_id ]}\n\n";
		$message_body .= "Onderwerp: $onderwerp\n\n";
		$message_body .= "Ingevuld door: " . $_POST[ "invuller_aanspreektitel" ] . " " . stripslashes( $_POST[ "invuller_naam" ] ) . "\n";
		$message_body .= "Organisatie of Afdeling: " . stripslashes( $_POST[ "invuller_organisatie" ] ) . "\n";
		$message_body .= "Adres: " . stripslashes( $_POST[ "invuller_adres" ] ) . "\n";
		$message_body .= "Postcode en Plaats: " . $_POST[ "invuller_postcode" ] . " " . stripslashes( $_POST[ "invuller_plaats" ] ) . "\n";
		$message_body .= "Telefoon: " . $_POST[ "invuller_tel" ]  . "\n";
		$message_body .= "Fax: " . $_POST[ "invuller_fax" ]  . "\n";
		$message_body .= "E-mail: " . $_POST[ "invuller_email" ]  . "\n";
		$message_body .= "Website: " . $_POST[ "invuller_website" ]  . "\n";
		$message_body .= "IP Address: " . $_SERVER['REMOTE_ADDR']  . "\n";
		$message_body .= "Referer: " . $_SERVER['HTTP_REFERER']  . "\n";
		$message_body .= "\n";
		$message_body .= "Contactambtenaar: " . stripslashes( $_POST[ "contact_ambtenaar_naam" ] ) . "\n";
		$message_body .= "Contactambtenaar telefoon: " . stripslashes ( $_POST[ "contact_ambtenaar_tel" ] ) . "\n\n";
		$message_body .= "Correctiefactor: $correctiefactor\n";
		$message_body .= "Correctiereden: $correctiereden\n\n";
		$message_body .= "Databasecode:\n$update_sql\n\n\n";
		$message_body .= $message_body_vragen;
		$message_body .= "\n--einde bericht\n\n";
		$message_subject = "";
		$message_subject .= $gemeente_ids[ $gemeente_id ];
		$message_subject .= ", " . $onderwerp;
		$mail_to="acekiko@suares.com";
		$mail_headers="Return-Path: dzhm@duurzaamheidsmeter.nl\nFrom: dzhm@duurzaamheidsmeter.nl\nX-ASIC-application: Duurzaamheidsmeter 2006\nX-developer: Suares & Co (http://www.suares.com)";
		if ( ! ereg ("[0-9]", $_POST[ "invuller_tel" ] ) ):
			$message_subject = "SPAM: $message_subject";
		endif;
		if ( mail( $mail_to, $message_subject, $message_body, $mail_headers ) ):
			$out .= "<p class=\"mail-melding\">\n";
			$out .= "Uw antwoorden zijn verstuurd. Hartelijk dank voor uw bijdrage!<br />Over enige tijd zullen de resultaten zichtbaar zijn op deze site.<br /><br />";
			$out .= "Hierboven vindt u de gegevens die verwerkt zijn.<br />Mocht u deze willen corrigeren, neem dan contact op via info@duurzaamheidsmeter.nl of vul de lijst opnieuw in met de vermelding: 'gecorrigeerde lijst'";
			$out .= "</P>";
		else:
			$out .= "<P CLASS=\"mail-melding\">\n";
			$out .= "Uw aanvraag is NIET verstuurd door technische problemen. Neem contact op met <a href=\"mailto:webmaster@duurzaamheidsmeter.nl\">webmaster@duurzaamheidsmeter.nl</a>. Dit zijn de niet-verstuurde gegevens: ";
			$out .= "</p>";
			$out .= "<pre>\n";
			$out .= "$message_subject\n\n";
			$out .= "$message_body\n";
			$out .= "</pre>\n";
		endif;
	endif;
?>